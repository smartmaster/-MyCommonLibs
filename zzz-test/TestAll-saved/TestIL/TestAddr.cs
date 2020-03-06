using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TestIL
{

    unsafe struct StringData
    {
        public int Length;
        public fixed char Data[4];
    }

    unsafe struct IntArrayData
    {
        public IntPtr Length;
        public fixed int Data[4];
    }


    struct MySs
    {
        public int first;
        public long second;
    }

    static class TestAddr
    {
        public unsafe static void Case0()
        {
            string str = "x64 Native Tools Command Prompt for VS 2017.lnk";
            StringData* ptr = (StringData*)TestAddrFx.AddrHelper.AddrOfObject(str);
            for(int ii = 0; ii < ptr->Length; ++ ii)
            {
                Console.Write(ptr->Data[ii]);
            }
            Console.WriteLine();
        }


        public unsafe static void Case1()
        {
            int[] arr = Enumerable.Range(1000, 20).ToArray();
            IntArrayData* ptr = (IntArrayData*)TestAddrFx.AddrHelper.AddrOfObject(arr);
            long len = ptr->Length.ToInt64();
            for(long ii = 0; ii < len; ++ ii)
            {
                Console.WriteLine(ptr->Data[ii]);
            }
            Console.WriteLine();
        }

        public unsafe static void Case2()
        {
            MySs ss = new MySs()
            {
                first = 100,
                second = 200,
            };


            MySs* ptr = (MySs*)TestAddrFx.AddrHelper.AddrOfValue(ref ss);
            Console.WriteLine($"{ptr->first}, {ptr->second}");


            ptr->first = -100;
            ptr->second = -200;
            Console.WriteLine($"{ss.first}, {ss.second}");
        }


        unsafe static void PrintFirstChar(string str)
        {
            fixed(char * pch = str)
            {
                Console.WriteLine(*pch);
            }
        }

        public unsafe static void CaseAll()
        {
            Case0();
            Case1();
            Case2();
            PrintFirstChar("Hello");
        }

    }
}
