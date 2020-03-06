using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace AaaServerDllCs
{
    [ComVisible(true)]
    [Guid("8BC8EA7A-7499-4A1F-8517-DD8CC9A06FCF")]
    public interface IParam
    {

        long[] ArrayData
        {
            get;
            set;
        }
    

        string Message
        {
            get;
            set;
        }

        long Value
        {
            get;
            set;
        }
    }

    [ComVisible(true)]
    [Guid("1FDBE690-5940-48D7-A9B6-4883FBB4F5B5")]
    public class CParam : IParam
    {
        public CParam()
        {

        }

        public long[] ArrayData
        {
            get;
            set;
        }

        public string Message
        {
            get;
            set;
        }

        public long Value
        {
            get;
            set;
        }
    }

    [ComVisible(true)]
    [Guid("A3326C48-D804-4A1C-9486-073F800A3D1A")]
    public interface IHello
    {
        void Say(
            int val,
            out int oval,
            string str,
            out string ostr,
            long[] arr,
            out long[] oarr,
            IParam param,
            out IParam oparam
            );
    }

    [ComVisible(true)]
    [Guid("88995F7E-117B-4D8A-807F-0B1D3566B37E")]
    public class CHello : IHello
    {
        public CHello()
        {

        }

        public void Say(
            int val,
            out int oval,
            string str,
            out string ostr,
            long[] arr,
            out long[] oarr,
            IParam param,
            out IParam oparam
            )
        {
            oval = -val;
            Console.WriteLine("{0}, {1}", val, oval);

            ostr = str.ToUpper();
            Console.WriteLine("{0}, {1}", str, ostr);

            Console.WriteLine("{0}", arr);
            oarr = (long[])arr.Clone();
            for(int ii = 0; ii < oarr.Length; ++ ii)
            {
                oarr[ii] = -oarr[ii];
            }
            Console.WriteLine("{0}", oarr);

            oparam = new CParam
            {
                Value = -param.Value,
                Message = param.Message.ToUpper(),
                ArrayData = (long[])param.ArrayData.Clone()
            };

            for(int ii = 0; ii < oparam.ArrayData.Length; ++ ii)
            {
                oparam.ArrayData[ii] = -oparam.ArrayData[ii];
            }

            Console.WriteLine("{0}, {1}, {2}", param.Value, param.Message, param.ArrayData);
            Console.WriteLine("{0}, {1}, {2}", oparam.Value, oparam.Message, oparam.ArrayData);
        }
    }
}
