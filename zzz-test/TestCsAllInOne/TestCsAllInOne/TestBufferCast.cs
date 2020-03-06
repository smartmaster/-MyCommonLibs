using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Unicode)]
    unsafe struct MyBin
    {
        public short major;
        public short minor;
        public int size;
        public fixed char name[8];
    }

    static class MemConv
    {
        public static ref T Buffer2Struct<T>(ref byte buffer, int bufferLen, out int length)
            where T: struct
        {
            Span<byte> spanBuffer = MemoryMarshal.CreateSpan(ref buffer, bufferLen);
            Span<T> ss = MemoryMarshal.Cast<byte, T>(spanBuffer);
            length = ss.Length;
            return ref ss[0];
        }

        public static ref byte Struct2Buffer<T>(ref T ss, int length, out int bufferLen)
            where T: struct
        {
            Span<T> spanSS = MemoryMarshal.CreateSpan(ref ss, length);
            Span<byte> spanBuffer = MemoryMarshal.Cast<T, byte>(spanSS);
            bufferLen = spanBuffer.Length;
            return ref spanBuffer[0];
        }
    }
    static class TestBufferCast
    {
        public static async Task Case1()
        {
            byte[] buffer = new byte[Marshal.SizeOf<MyBin>()];

            Step100();
            void Step100() //local function
            {
                ref MyBin ss = ref MemConv.Buffer2Struct<MyBin>(ref buffer[0], buffer.Length, out int length);
                ss.major = 1;
                ss.minor = 2;
                ss.size = Marshal.SizeOf<MyBin>();
                string refs = "ReFS";
                unsafe
                {
                    Span<char> name = MemoryMarshal.CreateSpan(ref ss.name[0], 8);
                    refs.AsSpan().CopyTo(name);
                }

                ref byte temp = ref MemConv.Struct2Buffer<MyBin>(ref ss, 1, out int tempLen);
                Span<byte> tempSpan = MemoryMarshal.CreateSpan(ref temp, tempLen);

            }

            string fileName = @"sss.bin";
            const int BUFFER_SIZE = 64 * 1024;

            using (FileStream fs = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.None, BUFFER_SIZE, FileOptions.Asynchronous))
            {
                await fs.WriteAsync(buffer, 0, buffer.Length);
                await fs.FlushAsync();
            }


            buffer.AsSpan().Clear();
            using (FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.None, BUFFER_SIZE, FileOptions.Asynchronous))
            {
                int readLen = await fs.ReadAsync(buffer, 0, buffer.Length);

                Step200();
                void Step200() //local function
                {
                    ref MyBin ss = ref MemConv.Buffer2Struct<MyBin>(ref buffer[0], buffer.Length, out int length);
                                       
                    ref byte temp = ref MemConv.Struct2Buffer<MyBin>(ref ss, 1, out int tempLen);
                    Span<byte> tempSpan = MemoryMarshal.CreateSpan(ref temp, tempLen);

                }
            }
            
            
            return;
        }

        public unsafe static void Case0()
        {
            Span<MyBin> ss = new MyBin[] 
            {
              new MyBin
              {
                major = 1,
                minor = 2,
                size = Marshal.SizeOf<MyBin>()
              }
            };

            string refs = "ReFS";
            Span<char> name = MemoryMarshal.CreateSpan(ref ss[0].name[0], 8);
            refs.AsSpan().CopyTo(name);
            //for(int ii = 0; ii < name.Length && ii < refs.Length; ++ ii)
            //{
            //    name[ii] = refs[ii];
            //}

            Span<byte> bytes = MemoryMarshal.Cast<MyBin, byte>(ss);

            string fileName = @"sss.bin";
            const int BUFFER_SIZE = 64 * 1024;
            using (FileStream fs = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.None, BUFFER_SIZE, FileOptions.Asynchronous))
            {
                fs.Write(bytes);
                var taskFlush = fs.FlushAsync();
                taskFlush.Wait();
                //fs.Close();
            }

            using (FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.None, BUFFER_SIZE, FileOptions.Asynchronous))
            {
                Span<MyBin> ss1 = new MyBin[1];
                Span<byte> buffer = MemoryMarshal.Cast<MyBin, byte>(ss1);
                fs.Read(buffer);

                Span<byte> spanByte = MemoryMarshal.Cast<MyBin, byte>(ss);
                Span<byte> spanByte1 = MemoryMarshal.Cast<MyBin, byte>(ss1);
            }

#if true
            using (FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.None, BUFFER_SIZE, FileOptions.Asynchronous))
            {
                byte[] buffer = new byte[fs.Length];
                Task<int> tr = fs.ReadAsync(buffer, 0, buffer.Length);
                int readlen = tr.Result;
                fs.Dispose();

                Span<MyBin> ss1 = MemoryMarshal.Cast<byte, MyBin>(buffer.AsSpan());
                Span<byte> spanByte = MemoryMarshal.Cast<MyBin, byte>(ss);
                Span<byte> spanByte1 = MemoryMarshal.Cast<MyBin, byte>(ss1);
            }
#endif
        }
    }
}
