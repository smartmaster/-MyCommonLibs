using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Threading.Tasks;

namespace SmartLib
{
    public static class DeepCloner
    {
        private static bool IncIndex(int[] index, int[] dimensions)
        {
            int carry = 1;
            for (int ii = index.Length - 1; ii >= 0; --ii)
            {
                if (carry != 0)
                {
                    index[ii] += carry;
                    carry = index[ii] / dimensions[ii];
                    index[ii] %= dimensions[ii];
                }
                else
                {
                    break;
                }
            }

            return carry == 0;
        }
        private static IEnumerable<int[]> EnumIndex(int[] dimensions)
        {
            int[] index = new int[dimensions.Length]; //start from [0][0][0]

            yield return index;

            for (;;)
            {
                bool ok = IncIndex(index, dimensions);
                if (ok)
                {
                    yield return index;
                }
                else
                {
                    yield break;
                }
            }
        }
        private static IEnumerable<object> ConvertEnumerable(IEnumerable seq)
        {
            foreach (object ooo in seq)
            {
                yield return ooo;
            }
        }

        private static bool IsSimpleType(Type oooType)
        {
            TypeInfo oooTypeInfo = oooType.GetTypeInfo();
            return (
                oooTypeInfo.IsPrimitive ||
                oooType == typeof(string) ||
                oooType == typeof(decimal) ||
                oooTypeInfo.IsEnum
                );
        }


        public static object DeepClone(object obj)
        {
            //////////////////////////////////////////////////////////////////////////
            if (null == obj)
            {
                return null;
            }


            //////////////////////////////////////////////////////////////////////////
            if (obj is IDisposable)
            {
                return null;
            }


            //////////////////////////////////////////////////////////////////////////
            Type tt = obj.GetType();
            if (tt == typeof(object))
            {
                return new object();
            }

            //////////////////////////////////////////////////////////////////////////
            if (IsSimpleType(tt)) //simple type
            {
                return obj;
            }
            else if (tt.IsArray) //array
            {
                Array arr = obj as Array;
                Array clonedArr = (Array)arr.Clone(); //shadow clone first

                if (arr.Length > 0)
                {
                    int rank = arr.Rank;
                    int[] dimension = new int[rank];
                    for (int ii = 0; ii < rank; ++ii)
                    {
                        dimension[ii] = arr.GetLength(ii);
                    }
                    foreach (var index in EnumIndex(dimension))
                    {
                        object item = arr.GetValue(index);
                        object clonedItem = DeepClone(item);
                        clonedArr.SetValue(clonedItem, index);
                    }
                }
                

                return clonedArr;
            }
            else //composite or collections
            {
                object cloned = Activator.CreateInstance(tt);

                IDictionary dic = obj as IDictionary;
                ICollection coll = obj as ICollection;
                if (dic != null) //Dictionary
                {
                    MethodInfo methodAdd = tt.GetMethod("Add");
                    foreach (var key in dic.Keys)
                    {
                        var value = dic[key];
                        if (key != null && value != null)
                        {
                            methodAdd = tt.GetMethod("Add", new Type[] {key.GetType(), value.GetType() });
                            break;
                        }
                    }

                    foreach (var key in dic.Keys)
                    {
                        var value = dic[key];

                        var clonedKey = DeepClone(key);
                        var clonedValue = DeepClone(value);

                        methodAdd.Invoke(cloned, new object[] { clonedKey, clonedValue });
                    }
                }
                else if (coll != null) //Collection
                {
                    var methodAdd = tt.GetMethod("Add");
                    foreach (object item in ConvertEnumerable(coll))
                    {
                        if (item != null)
                        {
                            methodAdd = tt.GetMethod("Add", new Type[] { item.GetType() });
                            break;
                        }
                    }

                    foreach(object item in ConvertEnumerable(coll))
                    {
                        object clonedItem = DeepClone(item);
                        methodAdd.Invoke(cloned, new object[] { clonedItem });
                    }
                }
                else
                {
                    FieldInfo[] fis = tt.GetFields(BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
                    foreach(var fi in fis)
                    {
                        object fieldValue = fi.GetValue(obj);
                        object clonedField = DeepClone(fieldValue);
                        fi.SetValue(cloned, clonedField);
                    }
                }

                return cloned;
            }
        }
    }

    #region TEST_CASES
    public static class DeepClonerTest
    {
     
            public class C1
            {
                public int ii { get; set; } = 111;
                public string sss { get; set; } = "aaaa";
                public decimal ddd { get; set; } = 2222.222M;


                public List<int> list_ii { get; set; } = new List<int> { 111, 222, 333 };
                public List<string> list_sss { get; set; } = new List<string> { "aaaa", "bbbbb", "cccc" };
                public List<decimal> list_ddd { get; set; } = new List<decimal> { 2222.222M, 333.333M, 5555.555M };
            }


            public class C2
            {
                public int ii { get; set; } = 111;
                public string sss { get; set; } = "aaaa";
                public decimal ddd { get; set; } = 2222.222M;


                public List<int> list_ii { get; set; } = new List<int> { 111, 222, 333 };
                public List<string> list_sss { get; set; } = new List<string> { "aaaa", "bbbbb", "cccc" };
                public List<decimal> list_ddd { get; set; } = new List<decimal> { 2222.222M, 333.333M, 5555.555M };

                public C1 c1 { get; set; } = new C1();
                public List<C1> list_c1 { get; set; } = new List<C1> { new C1(), new C1() };
            }


            public class C3
            {
                public int ii { get; set; } = 111;
                public string sss { get; set; } = "aaaa";
                public decimal ddd { get; set; } = 2222.222M;


                public List<int> list_ii { get; set; } = new List<int> { 111, 222, 333 };
                public List<string> list_sss { get; set; } = new List<string> { "aaaa", "bbbbb", "cccc" };
                public List<decimal> list_ddd { get; set; } = new List<decimal> { 2222.222M, 333.333M, 5555.555M };

                public C1 c1 { get; set; } = new C1();
                public List<C1> list_c1 { get; set; } = new List<C1> { new C1(), new C1() };


                public C2 c2 { get; set; } = new C2();
                public List<C2> list_c2 { get; set; } = new List<C2> { new C2(), new C2() };
            }


            public class LLLC
            {
                public List<List<List<C1>>> lll { get; set; } = new List<List<List<C1>>>();

                private Dictionary<string, C3> dict = new Dictionary<string, C3>
                {
                    ["hello"] = new C3(),
                    ["world"] = new C3(),
                };

                public void Init()
                {
                    lll.Init(2);
                    foreach (var ll in lll)
                    {
                        ll.Init(2);
                        foreach (var l in ll)
                        {
                            l.Init(2);
                        }
                    }
                }
            }



        public class ArrayData
        {
            C3[] c3arr = new C3[0];
            C3[] c3arr2 = new C3[] { new C3(), new C3(), new C3(), new C3(), new C3(), new C3(), new C3() };

            string[] arr1 = new string[] { "000", "111", "222" };
            string[][] arrJungle = new string[2][]
                {
                    new string[] {"01", "02" },
                    new string[] { "10", "11", "12" }
                };

            string[,] arryblock = new string[2, 3]
            {
                {"00", "01", "02" },
                {"10", "11", "12" }
            };

            public void MakeChange()
            {
                arr1[0] = "zzz";
                arrJungle[0][0] = "zzz";
                arrJungle[1][0] = "zzz";

                arryblock[0, 0] = "zzz";
                arryblock[1, 0] = "zzz";
            }
        }


        public static void Case0()
            {
            var ooo1 = (C1)DeepCloner.DeepClone(new C1());
            var ooo2 = (C2)DeepCloner.DeepClone(new C2());
            var ooo3 = (C3)DeepCloner.DeepClone(new C3());

            LLLC lllc = new LLLC();
            lllc.Init();
            LLLC ooo4 = (LLLC)DeepCloner.DeepClone(lllc);

            var arrData = new ArrayData();
            ArrayData arrDataCloned = (ArrayData)DeepCloner.DeepClone(arrData);

            arrDataCloned.MakeChange();

            Console.WriteLine(ooo1);
            Console.WriteLine(ooo2);
            Console.WriteLine(ooo3);
            Console.WriteLine(ooo4);
        }
        }
        #endregion
    
}
