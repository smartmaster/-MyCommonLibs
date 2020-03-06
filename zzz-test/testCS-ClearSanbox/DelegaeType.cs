using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace testCS
{
   static class DelegaeType
    {
        public static async Task<string> Progress(string beginMessage, string endMessage, int loop)
        {
            Console.WriteLine($"{beginMessage}");
            for(int ii = 0; ii < loop; ++ ii)
            {
                await Task.Delay(1000);
                Console.WriteLine($"{ii}");
            }

            Console.WriteLine($"{endMessage}");


            return loop.ToString();
        }


        public static async Task Case0()
        {
            Func<string, string, int, Task<string>> func = Progress;
            string rv = await func("started...", "finished!", 15);

        }
    }
}
