using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace testCS
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    unsafe struct MyStruct
    {
        public byte bb;
        public int ii;
        public long ll;
        public fixed char str[6];
    }
    static class Bin2Struct
    {
        public static unsafe void Case0()
        {
            byte[] arr = new byte[] {(byte)'a', 1,2,3,4, 1,2,3,4,5,6,7,8, (byte)'a', 0, (byte)'a'+1, 0, (byte)'a' + 2, 0, (byte)'a' + 3, 0, (byte)'a'+4, 0, (byte)'a' + 5, 0, 0, 0, 0, 0 , 0};
            Span<byte> buffer = arr;
            Span<MyStruct> ss = MemoryMarshal.Cast<byte, MyStruct>(buffer);
            var bb = ss[0].bb;
            var ii = ss[0].ii;
            var ll = ss[0].ll;
            StringBuilder sb = new StringBuilder(100);
            int kk = 0;
            for (; ;)
            {
                char ch = ss[0].str[kk];
                if(ch == 0)
                {
                    break;
                }
                sb.Append(ch);
                ++kk;
            }

            Span<byte> buffer2 = MemoryMarshal.Cast<MyStruct, byte>(ss);
            foreach(byte bb2 in buffer2)
            {
                Console.Write("{0:x2} ", bb2);
            }
            Console.WriteLine();

            foreach (byte bb2 in buffer)
            {
                Console.Write("{0:x2} ", bb2);
            }
            Console.WriteLine();
        }
    }
}

