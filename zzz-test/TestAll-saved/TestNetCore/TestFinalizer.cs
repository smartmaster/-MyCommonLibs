using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace TestNetCore
{

    class TestFinalizer
    {
        static int s_index = 0;
        public int _index;

        public TestFinalizer Friend { get; set; }

        public TestFinalizer()
        {
            _index = s_index++;
            Console.WriteLine($"Created {_index}");
        }

        ~TestFinalizer()
        {
            Console.WriteLine($"Destroyed {_index}, Friend {Friend?._index}");
        }
    }


    static class TestFinalizerCase
    {
        public unsafe static (IntPtr p0, IntPtr p1) Case0_0()
        {
            Console.WriteLine($"GC.GetTotalMemory {GC.GetTotalMemory(false)}");
            TestFinalizer v0 = new TestFinalizer();
            TestFinalizer v1 = new TestFinalizer();
            Console.WriteLine($"GC.GetTotalMemory {GC.GetTotalMemory(false)}");
            v0.Friend = v1;
            v1.Friend = v0;

            int* p0 = null;
            int* p1 = null;

            Console.WriteLine($"GC.GetTotalMemory {GC.GetTotalMemory(false)}");

            fixed (int* p00 = &v0._index)
            fixed (int* p01 = &v1._index)
            {
                p0 = p00;
                p1 = p01;
            }

            v0 = null;
            v1 = null;



            //TestFinalizer o2 = new TestFinalizer();

            Console.WriteLine(" GC.Collect()");
            GC.Collect();

            Console.WriteLine($"GC.GetTotalMemory {GC.GetTotalMemory(true)}");

            Console.WriteLine("sleeping start");
            Thread.Sleep(5_000);
            Console.WriteLine("sleeping end");

            Console.WriteLine($"GC.GetTotalMemory {GC.GetTotalMemory(false)}");

            GC.WaitForPendingFinalizers();
            Console.WriteLine(" GC.WaitForPendingFinalizers()");

            Console.WriteLine($"GC.GetTotalMemory {GC.GetTotalMemory(false)}");

            Console.WriteLine($"{*p0}, {*p1}");


            return (new IntPtr(p0), new IntPtr(p1));


        }


        public unsafe static void Case0()
        {
            var p = Case0_0();
            Console.WriteLine($"GC.GetTotalMemory {GC.GetTotalMemory(true)}");
            int* p0 = (int*)p.p0;
            int* p1 = (int*)p.p1;
            Console.WriteLine($"{*p0}, {*p1}");
        }
    }
}
