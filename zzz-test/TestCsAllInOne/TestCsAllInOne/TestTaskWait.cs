using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
    class TestTaskWait
    {
        public static long MySleep(int loop, int milSec)
        {
            for(int ii = 0; ii < loop; ++ ii)
            {
                Thread.Sleep(milSec);
            }
            return (long)(loop) * (long)(milSec);
        }

        public static Task<long> MyDelayTask(int loop, int milSec)
        {
            long TaskProc()
            {
                long rv = MySleep(loop, milSec);
                return rv;
            }

            Task<long> tt = Task.Factory.StartNew(TaskProc, TaskCreationOptions.LongRunning);

            return tt;
        }

        public static void Case0()
        {
            var tt = MyDelayTask(3600, 1000);
            tt.Wait();
            Console.WriteLine($"Task result is {tt.Result}");
        }
    }
}
