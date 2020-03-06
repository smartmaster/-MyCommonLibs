using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace testCS
{
    public struct MySs
    {
        public int _data;
        public string _str;
        public double _dbl;
    }

   
   public static class StackAsync
    {
        public static void Print(in MySs ss)
        {
            Console.WriteLine($"{ss._data}, {ss._str}, {ss._dbl}");
        }

        public static void Print<T>(Span<T> span)
        {
            foreach(var item in span)
            {
                Console.WriteLine($"{item}");
            }
        }


        public static async Task DataOrCodeAsync()
        {
            Console.WriteLine($"starting tasks... {DateTime.Now}");
            var tasks = Enumerable.Range(0, Environment.ProcessorCount).Select(
                async ii => 
                {
                    await Task.Delay((ii + 1) * 2000);
                    Console.WriteLine($"Task {ii} {DateTime.Now}");
                }
                );
            Console.WriteLine($"started tasks... {DateTime.Now}");


            foreach(var task in tasks)
            {
                Console.WriteLine("..............................");
            }

            //Thread.Sleep(Environment.ProcessorCount * 2000);
            await Task.Delay(Environment.ProcessorCount * 2000);
            Console.WriteLine($"sleep finished {DateTime.Now}");
            await Task.WhenAll(tasks);
            Console.WriteLine($"all task finished {DateTime.Now}");
        }


        public static async Task MethodAsync2()
        {
            //int[] arr = new int[32];
            //Span<int> span = new Span<int>(arr, 1, 5);


            //Console.WriteLine($"{Thread.CurrentThread.ManagedThreadId}");
            //Print(span);
            //await Task.Delay(1000);


            //Console.WriteLine($"{Thread.CurrentThread.ManagedThreadId}");
            //Print(span);
            await Task.Yield();
            return;
        }

        public static async Task MethodAsync()
        {
            MySs ss = new MySs
            {
                _data = 100,
                _str = "hello world",
                _dbl = 123.456
            };

            Console.WriteLine($"{Thread.CurrentThread.ManagedThreadId}");
            Print(in ss);
            await Task.Delay(1000);

            ss._data += 100;
            ss._str += " oh!";
            ss._dbl += 789.123;
            Console.WriteLine($"{Thread.CurrentThread.ManagedThreadId}");
            Print(in ss);

            return;
        }
    }
}
