using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestIL
{
   static class TestLabmda
    {
      public  static void Case0()
        {
            int times = 0;
            Func<int, int> func = (int x) =>
            {
                return x * times;
            };


            times = 10;
            Console.WriteLine(func(9));

            times = 100;
            Console.WriteLine(func(9));

        }
    }
}
