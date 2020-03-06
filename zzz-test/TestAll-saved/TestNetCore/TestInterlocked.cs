using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace TestNetCore
{
   static class TestInterlocked
    {
        public static void Case0()
        {
            int val = 0;

            int r1 = Interlocked.CompareExchange(ref val, 0, 0);
            Console.WriteLine($"{r1}, {val}");

            r1 = Interlocked.Add(ref val, 0);
            Console.WriteLine($"{r1}, {val}");

            val = 100;
            int r2 = Interlocked.CompareExchange(ref val, 0, 0);
            Console.WriteLine($"{r2}, {val}");
            r2 = Interlocked.Add(ref val, 0);
            Console.WriteLine($"{r2}, {val}");

        }
    }
}
