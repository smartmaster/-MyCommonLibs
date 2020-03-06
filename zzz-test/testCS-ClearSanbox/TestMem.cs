using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace testCS
{
  static  class TestMem
    {
        public static /*unsafe*/ async Task TestStackAlloc()
        {
            unsafe
            {
                int* arr = stackalloc int[128];
            }
            
            await Task.Delay(10_1000);

        }


        public static void TestSyncAsync2()
        {
            int step = 0;
            ManualResetEventSlim evt = new ManualResetEventSlim();
            Console.WriteLine("{0}", step);
            Task t1 = Task.Delay(10_000);
            t1.ContinueWith(tt =>
            {
                ++step;
                Console.WriteLine("{0}", step);
                evt.Set();
            });


            evt.Wait();
            Thread.Sleep(1_000);
        }
        public static void TestSyncAsync()
        {
            int step = 0;
            ManualResetEventSlim evt = new ManualResetEventSlim();
            Console.WriteLine("{0}", step);
            Task t1 = Task.Delay(10_000);
            var awaiter = t1.GetAwaiter();
            awaiter.OnCompleted(() => 
            {
                ++step;
                Console.WriteLine("{0}", step);
                evt.Set();
            });


            evt.Wait();
            Thread.Sleep(1_000);
        }
        public static async Task TestAwaiter()
        {
            int step = 0;
            Console.WriteLine($"{step}");

            await Task.Delay(10_000);
            ++step;
            Console.WriteLine($"{step}");

            await Task.Delay(10_000);
            ++step;
            Console.WriteLine($"{step}");
        }
        public static async Task ReadAsync(string fileName)
        {
            byte[] arr = new byte[128];
            Memory<byte> mem = arr;
            using (FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.Read, 256*1024, true))
            {
               int len = await fs.ReadAsync(arr, 0, arr.Length);
                string str = Encoding.UTF8.GetString(arr, 0, len);
                Console.WriteLine(str);
            }
        }
    }
}
