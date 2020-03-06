using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace SmartLib
{
    public class CompositeToRouteValues
    {
        private readonly Dictionary<string, object> _routeValues = new Dictionary<string, object>();
        private readonly List<string> _names = new List<string>();
        private readonly StringBuilder _sb = new StringBuilder();


        private IEnumerable<object> ConvertEnumerable(IEnumerable seq)
        {
            foreach (object ooo in seq)
            {
                yield return ooo;
            }
        }

        private string NamePrefix
        {
            get
            {
                _sb.Clear();
                foreach (var item in _names)
                {
                    _sb.Append(item);
                    _sb.Append('.');
                }
                return _sb.ToString();
            }
        }

        private bool IsSimpleType(Type oooType)
        {
            TypeInfo oooTypeInfo = oooType.GetTypeInfo();
            return (
                oooTypeInfo.IsPrimitive ||
                oooType == typeof(string) ||
                oooType == typeof(decimal) ||
                oooTypeInfo.IsEnum
                );
        }

        /************************************
         Composite
         {
            SimpleTypeX;
            ...

            List<SimpleTypeY>;
            ...

            CompositeX;
            ...

            List<CompositeY>;
            ...
         }
         ***********************************/

        private void Convert(object obj)
        {
            if (null == obj)
            {
                return;
            }

            if (obj is IDisposable)
            {
                return;
            }

            Type tt = obj.GetType();
            foreach (var propInfo in tt.GetProperties(BindingFlags.Instance | BindingFlags.Public))
            {
                object propVal = propInfo.GetValue(obj);
                if (null != propVal)
                {
                    if (IsSimpleType(propInfo.PropertyType)) //simple types
                    {
                        _routeValues[NamePrefix + propInfo.Name] = propVal;
                    }
                    else
                    {
                        IEnumerable enmb = propVal as IEnumerable;
                        if (enmb != null) //enumerable
                        {
                            var seq = ConvertEnumerable(enmb);
                            var itemValue = seq.FirstOrDefault();
                            if (null != itemValue)
                            {
                                if (IsSimpleType(itemValue.GetType())) //enumerable of simple
                                {
                                    _routeValues[NamePrefix + propInfo.Name] = propVal;
                                }
                                else //enumerable of composite
                                {
                                    int index = 0;
                                    foreach (var item in seq)
                                    {
                                        _names.Add($"{propInfo.Name}[{index}]");
                                        Convert(item);
                                        _names.RemoveAt(_names.Count - 1);
                                        ++index;
                                    }
                                }
                            }

                        }
                        else //composite
                        {
                            _names.Add(propInfo.Name);
                            Convert(propVal);
                            _names.RemoveAt(_names.Count - 1);
                        }
                    }

                }
            }

        }


        public static Dictionary<string, object> ConvertAPI(object obj)
        {
            CompositeToRouteValues converter = new CompositeToRouteValues();
            converter.Convert(obj);
            return converter._routeValues;
        }
    }

    #region TEST_CASES

    public static class SmlListExtensions
    {
        public static void Init<T>(this List<T> list, int size)
            where T : new()
        {
            for (int ii = 0; ii < size; ++ii)
            {
                list.Add(new T());
            }

        }


        public static void Init<T>(this List<T> list, params T[] values)
        {
            list.AddRange(values);
        }
    }


    public static class CompositeToRouteValuesTest
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





        public static void Case0()
        {
            Dictionary<string, object> dic1 = CompositeToRouteValues.ConvertAPI(new C1());
            Dictionary<string, object> dic2 = CompositeToRouteValues.ConvertAPI(new C2());
            Dictionary<string, object> dic3 = CompositeToRouteValues.ConvertAPI(new C3());

            //LLLC lllc = new LLLC();
            //lllc.Init();
            //Dictionary<string, object> dic4 = CompositeToRouteValues.ConvertAPI(lllc);

            Console.WriteLine(dic1);
            Console.WriteLine(dic2);
            Console.WriteLine(dic3);
            //Console.WriteLine(dic4);
        }
    }
    #endregion
}
