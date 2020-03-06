using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading.Tasks;


namespace TestCsAllInOne
{
   static class TestSpanAsyncAwait
    {

        public static void Case2()
        {
            const string fileName = @"f:\sss.dat";
            const int BUFFER_SIZE = 4 * 1024;
            using (FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.Read, BUFFER_SIZE, FileOptions.Asynchronous))
            {
                byte[] buffer = new byte[BUFFER_SIZE];
                for(; ; )
                {
                    var task = fs.ReadAsync(buffer, 0, buffer.Length);
                    var awaiter = task.GetAwaiter();
                    int readLen = awaiter.GetResult();
                    if(readLen < BUFFER_SIZE)
                    {
                        break;
                    }
                }
            }
        }
        public static void Case1()
        {
            var task = Task.Delay(3_000);
            var awaiter = task.GetAwaiter();
            awaiter.GetResult();
        }

        public static async Task<int> DelaySomeTime2()
        {
            int sum = 1000;
            int delta = 1000;
            for(int ii = 0; ii < 5; ++ ii)
            {
                await Task.Delay(sum);
                sum += delta;
                char[] arr = (sum.ToString() + ".00000").ToCharArray();
                UseSpanInAsync();

                void UseSpanInAsync()
                {
                    Span<char> span = arr.AsSpan();
                    double.TryParse(span, out double rr);
                    Console.WriteLine($"Delay {rr} milSec");
                }
            }

            Console.WriteLine($"sum is {sum}");

            return sum;
        }
        public static Task<int> SpanWithAsync()
        {
            int sum = 1000;
            int delta = 1000;
            char[] arr = "-123.456".ToCharArray();
            Span<char> span = arr.AsSpan();
            double.TryParse(span, out double result);
            return DelaySomeTime();

            async Task<int> DelaySomeTime()
            {
                Console.WriteLine("in async local method");
                for(int ii = 0; ii < 3; ++ ii)
                {
                    await Task.Delay(sum);
                    sum += delta;
                    Console.WriteLine($"delayed {sum} ms");
                }
                return sum;
            }
        }

        public static void Case0()
        {
            var task = SpanWithAsync();
            var awaiter = task.GetAwaiter();
            int rr = awaiter.GetResult();
            Console.WriteLine($"resullt is {rr}");
        }

  

       
    }
}
