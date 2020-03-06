using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace TestCsAllInOne
{
   static class TestDelagate
    {
        delegate int SumFunc(int aa, int bb);

        class MyClass
        {
            static int s_index = 0;

            private int _index;
          public  MyClass()
            {
                _index = Interlocked.Increment(ref s_index);
            }
            public int InstanceSum(int aa, int bb)
            {
                return aa + bb + _index;
            }
        }

        static int StaticSum(int aa, int bb)
        {
            return aa + bb;
        }

      public  static void Case0()
        {
            int aa = 1;
            int bb = 2;
            SumFunc f1 = new SumFunc(StaticSum);
            int ss = f1.Invoke(aa, bb);
            Console.WriteLine($"{ss}={aa} + {bb}");


            MyClass obj = new MyClass();
            SumFunc f2 = new SumFunc(obj.InstanceSum);
            aa = 10;
            bb = 20;
            ss = f2.Invoke(aa, bb);
            Console.WriteLine($"{ss}={aa} + {bb}");
        }
    }
}
