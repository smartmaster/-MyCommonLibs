using System;
using System.Collections.Generic;
using System.Text;

namespace TestNetCore
{
    interface IChange
    {
        void Set(int val);
        void Print();
    }

    struct TestBox : IChange
    {
        private int _data;
        public void Print()
        {
            Console.WriteLine(_data);
        }

        public void Set(int val)
        {
            _data = val;
        }
    }



    static class TestCase
    {
        public static void Case0()
        {
            TestBox tb = new TestBox();
            tb.Set(100);
            tb.Print();

            IChange ic = tb;
            ic.Print();

            ic.Set(200);
            ic.Print();

            tb.Print();

            tb.Set(300);
            ic.Print();
            tb.Print();
        }
    }
}
