using System;
using System.Collections.Generic;
using System.Text;

namespace TestCsAllInOne
{
    interface IA
    {
        void A1(int p1, int p2);
        void A2(int p1, int p2);
    }


    interface IB
    {
        void B1(int p1, int p2);
        void B2(int p1, int p2);
    }


    class CAB :
        IA,
        IB
    {
        public int _m1;
        public int _m2;

       public CAB(int m1, int m2)
        {
            _m1 = m1;
            _m2 = m2;
        }
        public virtual void A1(int p1, int p2)
        {
            int r = _m1 + _m2 + p1 + p2;
            Console.WriteLine("public void A1(int p1, int p2)");
            Console.WriteLine($"{r}");
        }

        public virtual void A2(int p1, int p2)
        {
            int r = _m1 + _m2 + p1 + p2;
            Console.WriteLine("public void A2(int p1, int p2)");
            Console.WriteLine($"{r}");
        }

        public virtual void B1(int p1, int p2)
        {
            int r = _m1 + _m2 + p1 + p2;
            Console.WriteLine("public void B1(int p1, int p2)");
            Console.WriteLine($"{r}");
        }

        public virtual void B2(int p1, int p2)
        {
            int r = _m1 + _m2 + p1 + p2;
            Console.WriteLine("public void B2(int p1, int p2)");
            Console.WriteLine($"{r}");
        }
    }


    static  class TestDasm
    {

        public static void Case1()
        {
            CAB ab = new CAB(0, 0);
            ab._m1 = 0x30303030;
            ab._m2 = 0x31313131;

            IA aa = ab;
            aa.A1(0x32323232, 0x33333333);
            aa.A2(0x34343434, 0x35353535);

            IB bb = ab;
            bb.B1(0x36363636, 0x37373737);
            bb.B2(0x38383838, 0x39393939);

            return;
        }
        public static void Case0()
        {
            System.UInt64 ii = 0XFEDCBA9876543210UL;
            object oo = ii;
            Type tt = oo.GetType();
            string nn = tt.FullName;
            Console.WriteLine(nn);

            return;
        }
    }
}

