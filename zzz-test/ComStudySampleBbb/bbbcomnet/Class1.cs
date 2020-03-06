using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.InteropServices;

namespace bbbcomnet
{
    [ComVisible(true)]
    [Guid("ED81A651-0ABF-429E-A7EA-7EA606384335")]
    public interface IComplexData
    {
        long lval { get; set; }
        string sval { get; set; }
        long[] larr { get; set; }
        string[] sarr { get; set; }
        void Init();
    }

    [ComVisible(true)]
    [Guid("2D96E033-D76B-4952-8A7D-E01C621F4C79")]
    public class CComplexData : IComplexData
    {
        public CComplexData()
        {
        }

        public CComplexData(int data, int len)
        {
            Init(data, len);
        }

        public long lval { get; set; }
        public string sval { get; set; }
        public long[] larr { get; set; }
        public string[] sarr { get; set; }

        public void Init()
        {
            Init(3, 5);
        }

        private void Init(int data, int len)
        {
            lval = data;
            sval = data.ToString();

            larr = new long[len];
            for (int ii = 0; ii < larr.Length; ++ii)
            {
                larr[ii] = data;
            }

            sarr = new string[len];
            for (int ii = 0; ii < sarr.Length; ++ii)
            {
                sarr[ii] = data.ToString();
            }
        }
    }

    [ComVisible(true)]
    [Guid("2BFB1406-DCD9-4AAB-B178-6320D7E30647")]
    public interface IMoreComplexData
    {
        long lval { get; set; }
        string sval { get; set; }
        long[] larr { get; set; }
        string[] sarr { get; set; }

        IComplexData complex { get; set; }
        IComplexData[] arrcomplex { get; set; }

        void Init();
    }

    [ComVisible(true)]
    [Guid("D6494F1F-EACD-4BAC-92F1-5E9AF85BD3B4")]
    public class CMoreComplexData : IMoreComplexData
    {
        public CMoreComplexData()
        {
        }


        public CMoreComplexData(int data, int len)
        {
            Init(data, len);
        }


        private void Init(int data, int len)
        {

            lval = data;
            sval = data.ToString();

            larr = new long[len];
            for (int ii = 0; ii < larr.Length; ++ii)
            {
                larr[ii] = data;
            }

            sarr = new string[len];
            for (int ii = 0; ii < sarr.Length; ++ii)
            {
                sarr[ii] = data.ToString();
            }

            complex = new CComplexData(data, len);

            arrcomplex = new IComplexData[len];
            for (int ii = 0; ii < arrcomplex.Length; ++ii)
            {
                arrcomplex[ii] = new CComplexData(data, len);
            }
        }

        public long lval { get; set; }
        public string sval { get; set; }
        public long[] larr { get; set; }
        public string[] sarr { get; set; }
        public IComplexData complex { get; set; }
        public IComplexData[] arrcomplex { get; set; }

        public void Init()
        {
            Init(3, 5);
        }
    }



    [ComVisible(true)]
    [Guid("7F05D9F1-ABD3-47DC-9ADC-E357C0907B47")]
    public interface ItestList
    {
        void Mmm(string[] hi, out string[] hello);
        //void Mmm2(IList<string> hi, out IList<string> hello);

        void Mmm3(IMoreComplexData[] inparam, out IMoreComplexData[] outparam);
    }

    [ComVisible(true)]
    [Guid("C7A97B62-7576-445B-8A73-FE38A91AAF58")]
    public class CtestList : ItestList
    {
        public CtestList()
        {
        }

        public void Mmm(string[] hi, out string[] hello)
        {
            foreach(var h in hi)
            {
                Console.WriteLine("{0}", h);
            }

            hello = new string[hi.Length];
            for(int ii = 0; ii < hello.Length; ++ ii)
            {
                hello[ii] = hi[ii].ToUpper();
            }
        }


        //public void Mmm2(IList<string> hi, out IList<string> hello)
        //{
        //    foreach (var h in hi)
        //    {
        //        Console.WriteLine("{0}", h);
        //    }

        //    hello = new string[hi.Count];
        //    for (int ii = 0; ii < hello.Count; ++ii)
        //    {
        //        hello[ii] = hi[ii].ToUpper();
        //    }
        //}


        public void Mmm3(IMoreComplexData[] inparam, out IMoreComplexData[] outparam)
        {
            foreach(var inp in inparam)
            {
                Console.WriteLine("{0}", inp);
            }

            outparam = new IMoreComplexData[inparam.Length];
            for(int ii = 0; ii < outparam.Length; ++ ii)
            {
                outparam[ii] = new CMoreComplexData(999, 3);
            }
        }
    }
}
