using System;
using System.Collections.Generic;
using System.Text;

namespace TestNetCore
{
 static   class Fib
    {
        public static T TestClass<T>(T obj)
            where T: class
        {
            return default(T);
        }

        public static void TestConstaints()
        {
            string ss = "";
            var str = TestClass(ss);

            //int? ii = null;
            //var iq = TestClass(ii);

        }

        public static IEnumerable<int> FibSeq(int count)
        {
            yield return 1;
            yield return 1;

            int first = 1;
            int second = 1;
            for (int ii = 0; ii < count; ++ ii)
            {
                int cur = first + second;
                first = second;
                second = cur;
                yield return cur;
            }
        }


        public static void TestFibSeq()
        {
            //foreach(int ff in FibSeq(20))
            //{
            //    Console.WriteLine(ff);
            //}


            using (IEnumerator<int> ee = FibSeq(10).GetEnumerator())
            {
                while(ee.MoveNext())
                {
                    Console.WriteLine(ee.Current);
                }
            }
        }

    }
}
