using System;
using System.Collections.Generic;
using System.Text;

namespace TestCsAllInOne
{
    class PageTableAddr
    {
        static string Convert2Size(long value)
        {
            const long _1K = ((1L << 10) - 1);
            long _B = value & _1K;

            value >>= 10;
            long _KB = value & _1K;

            value >>= 10;
            long _MB = value & _1K;

            value >>= 10;
            long _GB = value & _1K;

            value >>= 10;
            long _TB = value /*& _1K*/;


            StringBuilder sb = new StringBuilder(128);
            if (_TB > 0)
            {
                sb.AppendFormat("{0}TB ", _TB);
            }
            if (_GB > 0)
            {
                sb.AppendFormat("{0}GB ", _GB);
            }
            if (_MB > 0)
            {
                sb.AppendFormat("{0}MB ", _MB);
            }

            if (_KB > 0)
            {
                sb.AppendFormat("{0}KB ", _KB);
            }

            if (_B > 0)
            {
                sb.AppendFormat("{0}B", _B);
            }

            return sb.ToString();
        }
        //9 9 9 9 12
        static long Va2PeAddr(long start, long va)
        {
            return start + (va >> 9);
        }

        public static void Case0()
        {
            long start = (3L << 46);
            long current = start;
            Console.WriteLine("{0,-30}|{1,-30}|{2,-40}|{3,-40}", "va", "pe", "offset-from-start", "offset-from-current");
            for(; ; )
            {
                long next = Va2PeAddr(start, current);
                // Console.WriteLine($"va:{current:X} pe:{next:X} offset-from-start:{(next - start):X}");
                Console.WriteLine($"{Convert2Size(current),-30}|{Convert2Size(next),-30}|{Convert2Size(next - start),-40}|{Convert2Size(next - current),-40}");
                if (next - current < 4096)
                {
                    break;
                }
                current = next;
            }
        }
    }
}
