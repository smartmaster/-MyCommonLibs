using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestAddrFx
{


        public static class AddrHelper
        {
            public static IntPtr AddrOfObject<T>(T oo)
                where T : class
            {
                return IntPtr.Zero + IntPtr.Size;
            }


            public static IntPtr AddrOfValue<T>(ref T ss)
                where T : struct
            {
                return IntPtr.Zero + IntPtr.Size;
            }
        }

}
