using System;
using System.Collections.Generic;
using System.Reflection;
using System.Reflection.Emit;
using System.Runtime.InteropServices;
using System.Text;

namespace TestCsAllInOne
{
   
    static class TestDynamicMethodHelper
    {
        public delegate IntPtr GetStructAddr<T>(ref T ss);
        public static GetStructAddr<T> CreateMethodGetStructAddr<T>()
        {
            DynamicMethod dm = new DynamicMethod(
                "GetStructAddr_" + typeof(T).Name,
                typeof(IntPtr),
                new Type[] { typeof(T).MakeByRefType() });

            ILGenerator ilgen = dm.GetILGenerator();
            ilgen.Emit(OpCodes.Ldarg_0);
            ilgen.Emit(OpCodes.Conv_I);
            ilgen.Emit(OpCodes.Ret);
            return (GetStructAddr<T>)dm.CreateDelegate(typeof(GetStructAddr<T>));
        }

        [StructLayout(LayoutKind.Sequential/*, Pack = 1*/)]
        struct MyTestSS
        {
            public int ii;
            public byte bb;
            public char cc;
            public long ll;
        }

        public static unsafe void Case2()
        {
            MyTestSS myss = new MyTestSS
            {
                ii = 0x1122,
                bb = 0x88,
                cc = 'a',
                ll = 0x7788,
            };

            GetStructAddr<MyTestSS> getAddr = CreateMethodGetStructAddr<MyTestSS>();
            IntPtr addr = getAddr(ref myss);
            Span<MyTestSS> spanSS = new Span<MyTestSS>(addr.ToPointer(), 1);

            bool dataMatch = true;
            if (spanSS[0].ii != myss.ii)
            {
                dataMatch = false;
            }

            if (spanSS[0].bb != myss.bb)
            {
                dataMatch = false;
            }

            if (spanSS[0].cc != myss.cc)
            {
                dataMatch = false;
            }


            if (spanSS[0].ll != myss.ll)
            {
                dataMatch = false;
            }

            if(dataMatch)
            {
                Console.WriteLine("data match");
            }
            else
            {
                Console.WriteLine("data not match");
            }
        }

       

        public delegate IntPtr GetObjectAddr<T>(T obj);
        public static GetObjectAddr<T> CreateMethodGetObjectAddr<T>()
        {
            DynamicMethod dm = new DynamicMethod(
                "GetObjectAddr_" + typeof(T).Name,
                typeof(IntPtr),
                new Type[] { typeof(T) }
                );

            ILGenerator ilgen = dm.GetILGenerator();
            ilgen.Emit(OpCodes.Ldarg_0);
            ilgen.Emit(OpCodes.Conv_I);
            ilgen.Emit(OpCodes.Ret);
            return (GetObjectAddr<T>)dm.CreateDelegate(typeof(GetObjectAddr<T>));
        }


        [StructLayout(LayoutKind.Sequential)]
        public unsafe struct StringStruct
        {
            public IntPtr MT;
            public int Length;
            public fixed char Data[1];
        }
        public static unsafe void Case1()
        {
            string str = "hello world";
            GetObjectAddr<string> getAddr = CreateMethodGetObjectAddr<string>();
            IntPtr addr = getAddr(str);
            Span<StringStruct> spanSS = new Span<StringStruct>(addr.ToPointer(), 1);

            if(spanSS[0].Length == str.Length)
            {
                Console.WriteLine("Length match");
            }
            else
            {
                Console.WriteLine("Length not match");
            }

            bool dataMatch = true;
            for(int ii = 0; ii < str.Length; ++ ii)
            {
                if(spanSS[0].Data[ii] != str[ii])
                {
                    dataMatch = false;
                    break;
                    //Console.WriteLine("Wrong data memory location");
                }
            }

            if(dataMatch)
            {
                Console.WriteLine("data match");
            }
            else
            {
                Console.WriteLine("data not match");
            }
        }

        public delegate IntPtr GetFieldAddress<T>(ref T ss);
        public static GetFieldAddress<T> CreateMethod<T>(FieldInfo fi)
        {
            var dm = new DynamicMethod("GetFieldAddress_" + typeof(T).Name, typeof(IntPtr), new Type[] { typeof(T).MakeByRefType() });
            var ilgen = dm.GetILGenerator();
            ilgen.Emit(OpCodes.Ldarg_0);
            ilgen.Emit(OpCodes.Ldflda, fi);
            ilgen.Emit(OpCodes.Conv_I);
            ilgen.Emit(OpCodes.Ret);
            return (GetFieldAddress<T>)dm.CreateDelegate(typeof(GetFieldAddress<T>)); ;
        }


        public static void Case0()
        {
            MyTestSS myss = new MyTestSS
            {
                ii = 0x1122,
                bb = 0x88,
                cc = 'a',
                ll = 0x7788,
            };

            FieldInfo[] fis = typeof(MyTestSS).GetFields(BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
            foreach(var fi in fis)
            {
                var getaddr = CreateMethod<MyTestSS>(fi);
                IntPtr addr = getaddr(ref myss);
                Console.WriteLine($"{fi.Name} \t {addr}");
            }
        }


       
    }
}
