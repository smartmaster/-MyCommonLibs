using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestNetCore.LinqTest
{
    static class RangeVarialbeTest
    {
        public static void Test()
        {
            int[] arr = { 10, 4, 6, 3, 7, 9, 2};
            var ee = from vv in arr
                     where (vv | 1) == 1
                     orderby vv
                     select vv * 100;

            foreach(int vv in ee)
            {
                Console.WriteLine($"{vv} ");
            }

            var ee2 = arr
                .Where(vv => (vv | 1) == 1)
                .OrderBy(aa => aa)
                .Select(cc => cc * 100);

            foreach (int vv in ee2)
            {
                Console.WriteLine($"{vv} ");
            }


        }
    }
}
