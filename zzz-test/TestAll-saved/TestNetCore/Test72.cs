using System;
using System.Collections.Generic;
using System.Text;

namespace TestNetCore
{

    struct CcIn1
    {
        public int ii { get; set; }
        public string ss { get; set; }
        public List<int> ll { get; set; }
    }


    struct SsIn1
    {
        public int ii { get; set; }
        public string ss { get; set; }
        public List<int> ll { get; set; }
    }
    struct SsOut1
    {
        public int ii { get; set; }
        public string ss { get; set; }
        public List<int> ll { get; set; }


        public SsIn1 innerSs;

        public CcIn1 innerCc;
    }

    static class Test72
    {
        static void Init(ref SsIn1 inner)
        {
            inner.ii = 100;
            inner.ss = nameof(inner);
            inner.ll = new List<int>() { 1, 2, 3 };
        }


        static void Init(CcIn1 inner)
        {
            inner.ii = 100;
            inner.ss = nameof(inner);
            inner.ll = new List<int>() { 1, 2, 3 };
        }


        static void Init(ref SsOut1 outer)
        {
            outer.ii = 100;
            outer.ss = nameof(outer);
            outer.ll = new List<int>() { 100, 200, 300 };
            Init(ref outer.innerSs);

            outer.innerCc = new CcIn1();
            Init(outer.innerCc);
        }

        static void NewAndInit(out SsOut1 oo)
        {
            oo = new SsOut1();
            Init(ref oo);
        }
        public static void Case0(in SsOut1 oo)
        {
            //oo.ii = 100;
            //oo.ss = "hello";
            oo.ll.Add(100);

            //oo.innerSs = new SsIn1();

            //oo.innerSs.ii = 100;
            //oo.innerSs.ss = "hello";
            oo.innerSs.ll.Add(1100);


            //oo.innerCc = new CcIn1();

            //oo.innerCc.ii = 100;
           // oo.innerCc.ss = "aaa";
            oo.innerCc.ll.Add(100);
            return;

        }


        public static void Case1()
        {
            //SsOut1 oo = new SsOut1();
            //Init(ref oo);


            NewAndInit(out SsOut1 oo);
            Case0(in oo);
            Case0(oo);
        }
    }
}
