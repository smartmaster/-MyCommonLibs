using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
    static class TestStartTask
    {
        static int FuncSync()
        {
            Thread.Sleep(4_000);
            Thread.Sleep(6_000);
            return 0;
        }

        public static async Task Case0(bool toAwait)
        {
            const int taskCount = 3;

            CountdownEvent cde = new CountdownEvent(taskCount);
            Stopwatch watch = Stopwatch.StartNew();
            //watch.Start();
            for (int ii = 0; ii < taskCount; ++ii)
            {
                var tt = Task.Factory.StartNew(() =>
                 {
                     FuncSync();
                     cde.Signal();
                 }, TaskCreationOptions.LongRunning);
                if (toAwait)
                {
                    await tt;
                }
            }
            watch.Stop();
            Console.WriteLine($"For loops to create tasks elapsed {watch.ElapsedMilliseconds} milsecs.");
            if (toAwait)
            {
                //Debug.Assert(watch.ElapsedMilliseconds / 1000 == 30);
            }
            else
            {
                //Debug.Assert(watch.ElapsedMilliseconds / 1000 == 0);
            }
            watch.Start();
            cde.Wait();
            watch.Stop();
            Console.WriteLine($"All tasks ended with {watch.ElapsedMilliseconds} milsecs.");
            if (toAwait)
            {
                //Debug.Assert(watch.ElapsedMilliseconds / 1000 == 30);
            }
            else
            {
                //Debug.Assert(watch.ElapsedMilliseconds / 1000 == 10);
            }
            Console.WriteLine();
        }

        static async Task<int> FuncAsync()
        {
            Thread.Sleep(4_000);
            await Task.Delay(6_000);
            return 0;
        }

        public static async Task Case1(bool toAwait)
        {
            const int taskCount = 3;

            CountdownEvent cde = new CountdownEvent(taskCount);
            Stopwatch watch = Stopwatch.StartNew();
            //watch.Start();
            for (int ii = 0; ii < taskCount; ++ii)
            {
                var tt = Task.Factory.StartNew(async () =>
                {
                    await FuncAsync();
                    cde.Signal();
                }, TaskCreationOptions.LongRunning);
                if (toAwait)
                {
                    await tt;
                }
            }
            watch.Stop();
            Console.WriteLine($"For loops to create tasks elapsed {watch.ElapsedMilliseconds} milsecs.");
            if (toAwait)
            {
                //Debug.Assert(watch.ElapsedMilliseconds / 1000 == 12);
            }
            else
            {
                //Debug.Assert(watch.ElapsedMilliseconds / 1000 == 0);
            }
            watch.Start();
            cde.Wait();
            watch.Stop();
            Console.WriteLine($"All tasks ended with {watch.ElapsedMilliseconds} milsecs.");
            if (toAwait)
            {
                //Debug.Assert(watch.ElapsedMilliseconds / 1000 == 18);
            }
            else
            {
                //Debug.Assert(watch.ElapsedMilliseconds / 1000 == 10);
            }
            Console.WriteLine();
        }


        static async Task<int> FuncAsync2()
        {
            await Task.Delay(4_000);
            Thread.Sleep(6_000);
            return 0;
        }

        public static async Task Case2(bool toAwait)
        {
            const int taskCount = 3;

            CountdownEvent cde = new CountdownEvent(taskCount);
            Stopwatch watch = Stopwatch.StartNew();
            //watch.Start();
            for (int ii = 0; ii < taskCount; ++ii)
            {
                var tt = Task.Factory.StartNew(async () =>
                {
                    await FuncAsync2();
                    cde.Signal();
                }, TaskCreationOptions.LongRunning);
                if (toAwait)
                {
                    await tt;
                }
            }
            watch.Stop();
            Console.WriteLine($"For loops to create tasks elapsed {watch.ElapsedMilliseconds} milsecs.");
            if (toAwait)
            {
                //Debug.Assert(watch.ElapsedMilliseconds / 1000 == 0);
            }
            else
            {
                //Debug.Assert(watch.ElapsedMilliseconds / 1000 == 0);
            }
            watch.Start();
            cde.Wait();
            watch.Stop();
            Console.WriteLine($"All tasks ended with {watch.ElapsedMilliseconds} milsecs.");
            if (toAwait)
            {
                //Debug.Assert(watch.ElapsedMilliseconds / 1000 == 10);
            }
            else
            {
                //Debug.Assert(watch.ElapsedMilliseconds / 1000 == 10);
            }
            Console.WriteLine();
        }

    }
}
