using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TestIL
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 0)]
    class TestPack
    {
        public int ii;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string ss;
        public long ll;

        public static void Case0()
        {
            Console.WriteLine(Marshal.SizeOf<TestPack>());
        }
    }
}
