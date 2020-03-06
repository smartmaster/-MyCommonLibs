using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace WcfTransportSecurityCommon.TypeViewer
{

    [DataContract]
    public class MethodDesc
    {
        //[DataMember(Order = 100)]
        //public string Modifier { get; set; }

        [DataMember(Order = 200)]
        public string Name { get; set; }

        [DataMember(Order = 300)]
        public List<VarDesc> Parameters { get; set; } = new List<VarDesc>();

        [DataMember(Order = 400)]
        public VarDesc ReturnType { get; set; }
    }


    [DataContract]
    [KnownType(typeof(VarDetailDesc))]
    public class VarDesc
    {
        [DataMember(Order = 200)]
        public string VarType { get; set; }

        [DataMember(Order = 300)]
        public string VarName { get; set; }
    }

    [DataContract]
    public class VarDetailDesc : VarDesc
    {
        //[DataMember(Order = 100)]
        //public VarDesc ElementTypeDesc { get; set; }

        [DataMember(Order = 110)]
        public List<VarDesc> StaticFields { get; set; } = new List<VarDesc>();

        [DataMember(Order = 120)]
        public List<VarDesc> StaticProps { get; set; } = new List<VarDesc>();

        [DataMember(Order = 200)]
        public List<VarDesc> Fields { get; set; } = new List<VarDesc>();

        [DataMember(Order = 300)]
        public List<VarDesc> Props { get; set; } = new List<VarDesc>();

        [DataMember(Order = 650)]
        public List<MethodDesc> Methods { get; set; } = new List<MethodDesc>();

        [DataMember(Order = 700)]
        public List<MethodDesc> StaticMethods { get; set; } = new List<MethodDesc>();
    }

    static class Helpers
    {
        public static bool IsBasicType(Type tt)
        {
            return tt.IsPrimitive
                || tt.IsPointer
                || tt == typeof(string)
                || tt == typeof(decimal)
                || tt == typeof(void)
                || tt == typeof(object);
        }

        public static void WiteTypeDesc(string fileName, VarDesc td)
        {
            using (FileStream stream = new FileStream(fileName, FileMode.Create))
            using (XmlWriter xmlWrier = XmlWriter.Create(stream, new XmlWriterSettings { Indent = true, Encoding = Encoding.UTF8 }))
            using (XmlDictionaryWriter dictWriter = XmlDictionaryWriter.CreateDictionaryWriter(xmlWrier))
            {
                DataContractSerializer ser = new DataContractSerializer(typeof(VarDesc));
                ser.WriteObject(dictWriter, td);
                dictWriter.Flush();
            }
        }

        public static bool IsPropMethod(MethodInfo mi)
        {
            return mi.IsSpecialName && (mi.Name.IndexOf("get_") == 0 || mi.Name.IndexOf("set_") == 0);
        }


        public static string MethodName(MethodInfo mi)
        {
            StringBuilder sb = new StringBuilder();
            
            if(mi.IsPublic)
            {
                sb.Append("public ");
            }
            else
            {
                sb.Append("non-public ");
            }

            if (mi.IsVirtual)
            {
                sb.Append("virtual ");
            }
            else if (mi.IsStatic)
            {
                sb.Append("static ");
            }

            //sb.Append(mi.ReturnType.FullName);
            //sb.Append(' ');

            sb.Append(mi.Name);

            //sb.Append('(');

            //bool removeTrailing = false;
            //foreach(var pi in mi.GetParameters())
            //{
            //    removeTrailing = true;
            //    sb.Append($"{pi.ParameterType.FullName} {pi.Name}, ");
            //}

            //if(removeTrailing)
            //{
            //    sb.Remove(sb.Length - 2, 2);
            //}

            //sb.Append(')');

            return sb.ToString();
        }

        public static string PropName(PropertyInfo pi)
        {
            MethodInfo miGetter = pi.GetGetMethod();
            MethodInfo miSetter = pi.GetSetMethod();

            StringBuilder sb = new StringBuilder();


            if (miSetter?.IsVirtual ?? false)
            {
                sb.Append("virtual ");
            }
            else if (miGetter?.IsVirtual ?? false)
            {
                sb.Append("virtual ");
            }

            sb.Append(pi.Name);


            sb.Append(" { ");

            if (miGetter?.IsPublic ?? false)
            {
                sb.Append("get; ");
            }

            if (miSetter?.IsPublic ?? false)
            {
                sb.Append("set; ");
            }

            sb.Append("}");

            return sb.ToString();
        }
    }


    class TypeViewer
    {
        private ObjectIDGenerator _oidGen = new ObjectIDGenerator();
        private VarDesc Parse(Type tt, string varName)
        {
            _oidGen.GetId(tt, out bool fisrtTime);
            if (!fisrtTime)
            {
                return new VarDesc { VarType = tt.FullName, VarName = varName };
            }

            //complex type
            VarDetailDesc td = new VarDetailDesc();
            td.VarType = tt.FullName;
            td.VarName = varName;
            for (int ii = 0; ii < 2; ++ii)
            {
                BindingFlags bindingFlags;
                List<VarDesc> fields;

                if (ii == 0)
                {
                    bindingFlags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance;
                    fields = td.Fields;
                }
                else
                {
                    bindingFlags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static;
                    fields = td.StaticFields;
                }

                foreach (FieldInfo fi in tt.GetFields(bindingFlags))
                {
                    if (Helpers.IsBasicType(fi.FieldType))
                    {
                        fields.Add(new VarDesc { VarType = fi.FieldType.FullName, VarName = fi.Name });
                    }
                    else
                    {
                        fields.Add(Parse(fi.FieldType, fi.Name));
                    }
                }
            }



            for (int ii = 0; ii < 2; ++ii)
            {
                BindingFlags bindingFlags;
                List<VarDesc> props;

                if (ii == 0)
                {
                    bindingFlags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance;
                    props = td.Props;
                }
                else
                {
                    bindingFlags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static;
                    props = td.StaticProps;
                }

                foreach (PropertyInfo pi in tt.GetProperties(bindingFlags))
                {
                    if (Helpers.IsBasicType(pi.PropertyType))
                    {
                        props.Add(new VarDesc { VarType = pi.PropertyType.FullName, VarName = Helpers.PropName(pi) });
                    }
                    else
                    {
                        props.Add(Parse(pi.PropertyType, pi.Name));
                    }
                }
            }



            for (int ii = 0; ii < 2; ++ii)
            {
                BindingFlags bindingFlags;
                List<MethodDesc> mds;

                if (ii == 0)
                {
                    bindingFlags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance;
                    mds = td.Methods;
                }
                else
                {
                    bindingFlags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static;
                    mds = td.StaticMethods;
                }

                foreach (MethodInfo mi in tt.GetMethods(bindingFlags))
                {
                    if ((mi.DeclaringType == typeof(object) && tt != typeof(object))
                        || Helpers.IsPropMethod(mi))
                    {
                        continue;
                    }

                    List<VarDesc> parameters = new List<VarDesc>();
                    foreach (var param in mi.GetParameters())
                    {
                        parameters.Add(new VarDesc { VarType = param.ParameterType.FullName, VarName = param.Name });
                    }

                    MethodDesc md = new MethodDesc
                    {
                        //Modifier = mi.IsStatic ? "static" : mi.IsVirtual ? "virtual" : "",
                        Name = Helpers.MethodName(mi),
                        ReturnType = new VarDesc { VarType = mi.ReturnType.FullName, VarName = "return" },
                        Parameters = parameters
                    };

                    mds.Add(md);  
                }
            }

            return td;
        }


        static public VarDesc ParseType(Type tt, string varName)
        {
            var tv = new TypeViewer();
            return tv.Parse(tt, varName);
        }
    }

    #region TEST

    public static class TypeViewerTest
    {

        class myBBB
        {
            public static int staic_BBB;

            public int filed_BBB;
            public int prop_BBB { get; set; }

            public void func_BBB() { }

            public virtual void vvv() { }
        }

        class myAAA : myBBB
        {
            public static int staic_AAA;

            public int filed_AAA;
            public int prop_AAA { get; set; }

            public void func_AAA() { }

            public override void vvv() { }


        }


        class myCCC
        {
            public myCCC next;

            public virtual int prop
            {
                set
                {
                    Console.WriteLine(value);
                }
            }
            public int get_Data() { return 0; }
            public void set_Data(int value) { }
        }


        public static void Case0()
        {
            //Type tt = typeof(XmlDictionaryWriter);
            //Type tt = typeof(int);
            //Type tt = typeof(Guid);
            //Type tt = typeof(int[]);
            //Type tt = typeof(List<Dictionary<int, string>>);
            //Type tt = typeof(myAAA);
            //Type tt = typeof(Dictionary<string, int>);
            //Type tt = typeof(myCCC);
            //Type tt = typeof(System.Security.Cryptography.X509Certificates.X509Certificate2);
            //Type tt = typeof(object);
            Type tt = typeof(string);
            VarDesc td = TypeViewer.ParseType(tt, "root");
            Helpers.WiteTypeDesc($@"f:\111.xml", td);
        }
    }
    #endregion
}
