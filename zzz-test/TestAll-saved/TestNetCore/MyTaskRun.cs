using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestNetCore
{
    public static class MyTaskRun
    {
        public static Task<Result> MyRun<Result>(Func<Result> func)
        {
            TaskCompletionSource<Result> tcs = new TaskCompletionSource<Result>();
            Thread thread = new Thread(() =>
            {
                try
                {
                    tcs.SetResult(func());
                }
                catch (Exception ex)
                {
                    tcs.SetException(ex);
                }
            });

            thread.Start();

            return tcs.Task;
        }

        struct mySs
        {
            public int x;
            public int y;
        }


        public static void Case0()
        {
            Task<int> tt = MyRun<int>(()=>
            {
                Thread.Sleep(5000);
                return 100;
            });

            Console.WriteLine($"{DateTime.Now}");
            Console.WriteLine($"{tt.Result}");
            Console.WriteLine($"{DateTime.Now}");

            mySs ss = new mySs
            {
                x = 100,
                y = 200
            };

            ref mySs rr = ref ss;
            Console.WriteLine($"{rr.x}, {rr.y}");

            rr.x = -1000;
            rr.y = -2000;
            Console.WriteLine($"{ss.x}, {ss.y}");

        }


        public static async Task<int> Case1()
        {
            Console.WriteLine($"I am in Case1 function {DateTime.Now}");
            await Task.Delay(5000);
            Console.WriteLine($"I am in Case1 function {DateTime.Now}");
            return 888;
        }

        public static void Case2()
        {
            int? v1 = 100;
            int? v2 = v1;
            Console.WriteLine($"{v1}, {v2}");
            v1 = 200;
            Console.WriteLine($"{v1}, {v2}");
            v2 = 2000;
            Console.WriteLine($"{v1}, {v2}");
            Console.WriteLine($"I am in Case2 function {DateTime.Now}");
            Task<int> tt = Case1();
            Console.WriteLine($"I am in Case2 function {DateTime.Now}");
            Console.WriteLine($"I am in Case2 function {tt.Result}");
            Console.WriteLine($"I am in Case2 function {DateTime.Now}");
        }
    }
}
