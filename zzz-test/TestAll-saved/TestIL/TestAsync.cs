using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestIL
{
  static  class TestAsync
    {

        public static async Task<int> FuncAsync()
        {
            for(int ii = 0; ii < 5; ++ ii)
            {
                Console.WriteLine(".........................");
                await Task.Delay(100);
            }

            return 100;
        }
    }
}
