using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Serialization;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

namespace WcfTransportSecurityCommon
{

    [DataContract]
    [KnownType(typeof(SimpleObjValue))]
    [KnownType(typeof(ComplexObjValue))]
    public class ObjValue
    {
    }

    [DataContract]
    public class ObjDesc
    {
        [DataMember(Order = 100)]
        public string ObjType { get; set; }
        [DataMember(Order = 200)]
        public string ObjName { get; set; }

        [DataMember(Order = 300)]
        public ObjValue ObjValue { get; set; }
    }


    [DataContract]
    public class SimpleObjValue : ObjValue
    {
        [DataMember(Order = 100)]
        public string SimpleValue { get; set; }
    }

    [DataContract]
    public class ComplexObjValue : ObjValue
    {
        [DataMember(Order = 100)]
        public List<ObjDesc> Filelds { get; set; } = new List<ObjDesc>();

        [DataMember(Order = 200)]
        public List<ObjDesc> Props { get; set; } = new List<ObjDesc>();
    }


    static class ObjViewerHelper
    {
        public static bool IsBasicType(Type tt)
        {
            return tt.IsPrimitive
                || tt.IsPointer
                || tt == typeof(string)
                || tt == typeof(decimal)
                || tt == typeof(void)
                || tt == typeof(object)
                || tt == typeof(Guid)
                || tt == typeof(DateTime)
                || tt.IsEnum
                ;
        }

        public static void WiteObjDesc(string fileName, ObjDesc od)
        {
            using (FileStream stream = new FileStream(fileName, FileMode.Create))
            using (XmlWriter xmlWrier = XmlWriter.Create(stream, new XmlWriterSettings { Indent = true, Encoding = Encoding.UTF8 }))
            using (XmlDictionaryWriter dictWriter = XmlDictionaryWriter.CreateDictionaryWriter(xmlWrier))
            {
                DataContractSerializer ser = new DataContractSerializer(typeof(ObjDesc));
                ser.WriteObject(dictWriter, od);
                dictWriter.Flush();
                xmlWrier.Flush();
                stream.Flush();
            }
        }
    }


    public class ObjectViewer
    {
        private readonly ObjectIDGenerator _oidGen = new ObjectIDGenerator();
        private bool _toProcessEnumerable;
        private bool _toProcessProps;





        public ObjectViewer(bool toProcessEnumerable, bool toProcessProps)
        {
            _toProcessEnumerable = toProcessEnumerable;
            _toProcessProps = toProcessProps;

        }
        public ObjDesc Parse(object obj, Type declaredType, string name)
        {
            if (obj == null)
            {
                return new ObjDesc
                {
                    ObjType = declaredType?.FullName,
                    ObjName = name,
                    ObjValue = null
                };
            }


            Type tt = obj.GetType();
            if (ObjViewerHelper.IsBasicType(tt))
            {
                return new ObjDesc
                {
                    ObjType = tt.FullName,
                    ObjName = name,
                    ObjValue = new SimpleObjValue { SimpleValue = obj.ToString() }
                };
            }


            Int64 oid = _oidGen.GetId(obj, out bool firstName);
            if (!firstName)
            {
                return new ObjDesc
                {
                    ObjType = tt.FullName,
                    ObjName = name,
                    ObjValue = new SimpleObjValue { SimpleValue = $"<reference to object with id [{oid}]>" }
                };
            }



            ComplexObjValue cov = new ComplexObjValue();
            ObjDesc od = new ObjDesc
            {
                ObjType = tt.FullName,
                ObjName = name + $"[oid={oid}]",
                ObjValue = cov
            };

            IEnumerable enumerable = obj as IEnumerable;
            if (enumerable != null && _toProcessEnumerable)
            {
                foreach (object oo in enumerable)
                {
                    cov.Filelds.Add(Parse(oo, null, null));
                }

                return od;
            }


            foreach (var fi in tt.GetFields(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic))
            {
                cov.Filelds.Add(Parse(fi.GetValue(obj), fi.FieldType, fi.Name));
            }

            if(_toProcessProps)
            {
                foreach (var pi in tt.GetProperties(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic))
                {
                    MethodInfo getter = pi.GetGetMethod(true);
                    try
                    {
                        if (getter == null)
                        {
                            continue;
                        }

                        if (getter.GetParameters().Any())
                        {
                            continue;
                        }

                        if (getter.Name == "get_DeclaringMethod")
                        {
                            continue;
                        }


                        var getter_fi = getter.GetType().GetField("m_invocationFlags", BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);

                        const uint MY_INVOCATION_FLAGS_NEED_SECURITY = 4;
                        var invocationFlags = (uint)getter_fi.GetValue(getter);
                        if ((invocationFlags & MY_INVOCATION_FLAGS_NEED_SECURITY) != 0)
                        {
                            invocationFlags &= (~MY_INVOCATION_FLAGS_NEED_SECURITY);
                            getter_fi.SetValue(getter, invocationFlags);
                        }

                        //cov.Props.Add(Parse(pi.GetValue(obj), pi.PropertyType, pi.Name));
                        cov.Props.Add(Parse(getter.Invoke(obj, new object[] { }), pi.PropertyType, pi.Name));


                    }
                    //catch(System.Reflection.TargetParameterCountException ex)
                    //{
                    //    Console.WriteLine($"{ex.Message}");
                    //}
                    //catch(System.ArgumentException ex)
                    //{
                    //    Console.WriteLine($"{ex.Message}");
                    //}
                    //catch(System.Reflection.TargetInvocationException ex)
                    //{
                    //    Console.WriteLine($"{ex.Message}");
                    //}
                    catch (Exception ex)
                    {
                        Console.WriteLine($"{ex.Message}");
                    }
                }

            }



            return od;
        }

        static public ObjDesc ParseObject(object obj, Type declaredType, string name, bool toProcessEnumerable = true, bool toProcessProps = true)
        {
            var ov = new ObjectViewer(toProcessEnumerable, toProcessProps);
            return ov.Parse(obj, declaredType, name);
        }
    }


    public static class ObjectViewerTest
    {
        public static void Case0()
        {
            //object obj = Thread.CurrentContext;

            //var obj = 
            //    from val in Enumerable.Range(100, 20)
            //    where (val & 1) == 0
            //    select new {data1 = val, data2 = -val };

            //var obj = new Dictionary<int, string>
            //{
            //    [1] = "one",
            //    [2] = "two",
            //    [3] = "three",
            //    [4] = "four",
            //};

            //var obj = new string[] {"111", "222", "333" };

            var obj = new X509Certificate2(@"f:\smart-wcf-server.p12", "sss");
            ObjDesc od = ObjectViewer.ParseObject(obj, null, "root", true, false);
            ObjViewerHelper.WiteObjDesc(@"f:\111.xml", od);
        }
    }
}
