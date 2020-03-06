using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
   static class TestTaskRun
    {

        public static async Task TestTaskContinueWith()
        {
            int delay = 10_000;
            Task t1 = Task.Delay(delay);
            Console.WriteLine("Delay Task1 started");
            Task t2 = t1.ContinueWith( tt =>
            {
                Console.WriteLine("Delay task finished, run continuity");
                int sleep = 5_000;
                Thread.Sleep(sleep);
                Console.WriteLine("continuity finished");
            });

            Console.WriteLine("begin wait T2");
            await t2;
            Console.WriteLine("end wait T2");
        }
        public static async Task Case1()
        {
            string fileName = @"f:\111.txt";
            const int bufferSize = 64;
            using (FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.Read, bufferSize, FileOptions.Asynchronous))
            {
                byte[] buffer = new byte[bufferSize];
                Task<int> tt = fs.ReadAsync(buffer, 0, buffer.Length);
                int len = await tt;
                string readString = Encoding.UTF8.GetString(buffer, 0, len);
                Console.WriteLine($"read len is [{len}], string is [{readString}]");
            }
        }
        static int TaskProc(int val)
        {
            Thread.Sleep(TimeSpan.FromSeconds(val));
            Console.WriteLine($"Slept for {val} seconds");
            return val;
        }
        public static async Task<int> Case0()
        {
            Task<int> ti = Task.Factory.StartNew(() => TaskProc(5), TaskCreationOptions.None);
            var rr = await ti;
            Console.WriteLine($"Task finished and result is {rr}");
            return rr;
        }

        
    }
}
