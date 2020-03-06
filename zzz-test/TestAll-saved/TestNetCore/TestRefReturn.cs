using System;
using System.Collections.Generic;
using System.Text;

namespace TestNetCore
{

    struct SsReadonly1
    {
        public int II;
        public string SS;
        public List<int> LL;


        public static void Init(ref SsReadonly1 oo, int seed)
        {
            oo.II = seed;
            oo.SS = seed.ToString();
            oo.LL = new List<int>() { seed };
        }
    }
    class CcReadonly1
    {
        public int II;
        public string SS;
        public List<int> LL{ get; set; }


        public static void Init(CcReadonly1 oo, int seed)
        {
            oo.II = seed;
            oo.SS = seed.ToString();
            oo.LL = new List<int>() { seed };
        }
    }


    struct SsReadonly2
    {
        public int II;
        public string SS;
        public List<int> LL;

        public SsReadonly1 SR;
        public CcReadonly1 CR;

        public static void Init(ref SsReadonly2 oo, int seed)
        {
            oo.II = seed;
            oo.SS = seed.ToString();
            oo.LL = new List<int>() { seed };

            SsReadonly1.Init(ref oo.SR, seed);

            oo.CR = new CcReadonly1();
            CcReadonly1.Init(oo.CR, seed);
        }
    }



    struct CcReadonly2
    {
        public int II;
        public string SS;
        public List<int> LL;

        public SsReadonly1 SR;
        public CcReadonly1 CR;


        public static void Init(CcReadonly2 oo, int seed)
        {
            oo.II = seed;
            oo.SS = seed.ToString();
            oo.LL = new List<int>() { seed };

            SsReadonly1.Init(ref oo.SR, seed);

            oo.CR = new CcReadonly1();
            CcReadonly1.Init(oo.CR, seed);
        }
    }


    class TestRefReturn
    {
        private SsReadonly2 _sr;
        private CcReadonly2 _cr;

        public TestRefReturn()
        {
            SsReadonly2.Init(ref _sr, 1000);
            _cr = new CcReadonly2();
            CcReadonly2.Init(_cr, 2000);
        }

        public ref readonly SsReadonly2 GetReadonlySs()
        {
            return ref _sr;
        }


        public ref readonly CcReadonly2 GetReadonlyCc()
        {
            return ref _cr;
        }
        
    }

    static class TestRefReturnCases
    {
        public static void Case0()
        {
            TestRefReturn oo = new TestRefReturn();
            ref readonly SsReadonly2 sr = ref oo.GetReadonlySs();
            ref readonly CcReadonly2 cr = ref oo.GetReadonlyCc();

            //sr.II = 100;
            //sr.SS = 100.ToString();
            sr.LL.Add(100);

            //sr.CR = new CcReadonly1();
            sr.CR.II = 100;
            sr.CR.SS = 100.ToString();
            sr.CR.LL.Add(100);


            //sr.SR = new SsReadonly1();
            //sr.SR.II = 100;
            //sr.SR.SS = 100.ToString();
            sr.SR.LL.Add(100);

            //cr.II = 100;
            //cr.SS = 100.ToString();
            cr.LL.Add(100);

            //cr.CR = new CcReadonly1();
            cr.CR.II = 100;
            cr.CR.SS = 100.ToString();
            cr.CR.LL.Add(100);


            //cr.SR = new SsReadonly1();
            //cr.SR.II = 100;
            //cr.SR.SS = 100.ToString();
            cr.SR.LL.Add(100);
        }
    }
}
