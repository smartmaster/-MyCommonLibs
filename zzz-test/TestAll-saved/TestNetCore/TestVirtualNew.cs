using System;
using System.Collections.Generic;
using System.Text;

namespace TestNetCore
{

    
    
    interface I1
    {
        int a { get; set; }
        void a1();
        void a2();
    }

    interface I2
    {
        int b { get; set; }
        void b1();
        void b2();
    }

    interface I3
    {
        int c { get; set; }
        void c1();
        void c2();
    }


    class Ccc : I1, I2, I3
    {
        public int a { get; set; }
        public int b { get; set; }
        public int c { get; set; }
        public void a1()
        {
            //throw new NotImplementedException();
            Console.WriteLine($"a1");
        }

        public void a2()
        {
            //throw new NotImplementedException();
            Console.WriteLine($"a2");
        }

        public void b1()
        {
            //throw new NotImplementedException();
            Console.WriteLine($"b1");
        }

        public void b2()
        {
            //throw new NotImplementedException();
            Console.WriteLine($"b2");
        }

        public void c1()
        {
            //throw new NotImplementedException();
            Console.WriteLine($"c1");
        }

        public void c2()
        {
            //throw new NotImplementedException();
            Console.WriteLine($"c2");
        }
    }

    abstract class TestVirtualNew0
    {
      public  abstract void Print();


    }


    class TestVirtualNew1 : TestVirtualNew0
    {
        public override void Print()
        {
            Logger.Log2("hi");
        }
    }


    class TestVirtualNew2 : TestVirtualNew1
    {
        public override void Print()
        {
            Logger.Log2("hi");
        }
    }


    class TestVirtualNew3 : TestVirtualNew2
    {
        public new void Print()
        {
            Logger.Log2("hi");
        }
    }



    class TestVirtualNew4 : TestVirtualNew3
    {
        public virtual void Print()
        {
            Logger.Log2("hi");
        }
    }



    class TestVirtualNew5 : TestVirtualNew4
    {
        public override void Print()
        {
            Logger.Log2("hi");
        }
    }

    class TestVirtualNew6 : TestVirtualNew5
    {
        public override void Print()
        {
            Logger.Log2("hi");
        }
    }


    static class TestVirtualNew
    {
        public static void Case1()
        {
            TestVirtualNew6 p6 = new TestVirtualNew6();
            TestVirtualNew5 p5 = p6;
            TestVirtualNew4 p4 = p6;
            TestVirtualNew3 p3 = p6;
            TestVirtualNew2 p2 = p6;
            TestVirtualNew1 p1 = p6;
            TestVirtualNew0 p0 = p6;

            IntPtr ip0 = TestAddrFx.AddrHelper.AddrOfObject(p0);
            IntPtr ip1 = TestAddrFx.AddrHelper.AddrOfObject(p1);
            IntPtr ip2 = TestAddrFx.AddrHelper.AddrOfObject(p2);
            IntPtr ip3 = TestAddrFx.AddrHelper.AddrOfObject(p3);
            IntPtr ip4 = TestAddrFx.AddrHelper.AddrOfObject(p4);
            IntPtr ip5 = TestAddrFx.AddrHelper.AddrOfObject(p5);
            IntPtr ip6 = TestAddrFx.AddrHelper.AddrOfObject(p6);

            Console.WriteLine($"{ip0}");
            Console.WriteLine($"{ip1}");
            Console.WriteLine($"{ip2}");
            Console.WriteLine($"{ip3}");
            Console.WriteLine($"{ip4}");
            Console.WriteLine($"{ip5}");
            Console.WriteLine($"{ip6}");


            p0.Print();
            p1.Print();
            p2.Print();
            p3.Print();
            p4.Print();
            p5.Print();
            p6.Print();


            Console.WriteLine($"{typeof(TestVirtualNew0).FullName}");
            Console.WriteLine($"{p0.GetType().FullName}");


            Type tintq = typeof(int?);
            Console.WriteLine($"{tintq.FullName}, Is it struct? {tintq.IsValueType}");


            Ccc ccc = new Ccc()
            {
                a = 100,
                b = 200,
                c = 200
            };

            I1 i1 = ccc;
            I2 i2 = ccc;
            I3 i3 = ccc;

            ip0 = TestAddrFx.AddrHelper.AddrOfObject(ccc);
            ip1 = TestAddrFx.AddrHelper.AddrOfObject(i1);
            ip2 = TestAddrFx.AddrHelper.AddrOfObject(i2);
            ip3 = TestAddrFx.AddrHelper.AddrOfObject(i3);
            Console.WriteLine($"{ip0}");
            Console.WriteLine($"{ip1}");
            Console.WriteLine($"{ip2}");
            Console.WriteLine($"{ip3}");
        }
    }

}
