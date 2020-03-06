using System;
using System.Collections.Generic;
using System.Text;

using System.IO;

namespace TestNetCore
{
  static  class TestTypeReflection
    {
        public static void  Case0()
        {
            Type t1 = typeof(IDisposable);
            Type t2 = typeof(Stream);

            bool b12 = t1.IsAssignableFrom(t2);
            bool b21 = t2.IsAssignableFrom(t1);

            Console.WriteLine($"{b12}, {b21}");



            Type tdic = typeof(Dictionary<int, string>);

            if(tdic.IsGenericType)
            {
                Type[] args = tdic.GetGenericArguments();
                foreach(var arg in args)
                {
                    Console.WriteLine($"{arg.Name}");
                }
            }



            Type unbound = typeof(Dictionary<,>);
            Type closed = unbound.MakeGenericType(typeof(int), typeof(string));

            var dic = (Dictionary<int, string>)Activator.CreateInstance(closed);
            dic.Add(1, "one");
            dic.Add(2, "two");
            dic.Add(3, "three");

            foreach(var kv in dic)
            {
                Console.WriteLine($"{kv.Key}, {kv.Value}");
            }

        }
    }
}
