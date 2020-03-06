using System;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
   class MyDebug
    {
        private const string _name = "hello";
        public void Print()
        {
            Console.WriteLine($"{_name}");
        }
    }

    
   static class Program
    {

        static void CmpStr()
        {
            object obj = "String";
            string str = typeof(string).Name;

            bool b1 = object.ReferenceEquals(obj, str);

            bool b2 = (obj == str);


            string str1 = (string)obj;

            bool b3 = (str1 == str);

            Console.WriteLine("");
        }


        static async Task Main(string[] args)
        {
            //MyDebug mydebug = new MyDebug();
            //Console.WriteLine("Enter to continue...");
            //Console.ReadLine();
            //mydebug.Print();

            CmpStr();

            int testCase = -1;
            if (args.Length > 0)
            {
                int.TryParse(args[0], out testCase);
            }
            switch (testCase)
            {
                case 32:
                    {
                       Task tt = TestCs.TestIoAsync.IoAsyncMachine.IoAsync(args[1]);
                       tt.Wait();
                    }
                    break;
                case 31:
                    {
                        TestCsAllInOne.TestTaskContinueWith.Case0(args);
                    }
                    break;
                case 30:
                    {
                       await TestValueTask.Case0();

                    }
                    break;
                case 29:
                    {
                        TestCsAllInOne.EnumFileSystem.Enum(args[1]);
                    }
                    break;
                case 28:
                    {
                        TestCsAllInOne.PageTableAddr.Case0();
                    }
                    break;
                case 27:
                    {
                        TestCsAllInOne.TestUrlEncoding.Case0();
                    }
                    break;
                case 26:
                    {
                        if (args.Length > 2)
                        {
                            bool todelete = false;
                            if (args.Length > 4)
                            {
                                todelete = (0 == string.Compare(args[4], "yes", true));
                            }

                            bool ignoreCase = false;
                            if (args.Length > 5)
                            {
                                ignoreCase = (0 == string.Compare(args[5], "yes", true));
                            }
                            DeleteKeyByValueTest.DeleteKeyByValueAPI(args[1], args[2], args[3], todelete, ignoreCase);
                        }
                        else
                        {
                            Console.WriteLine("Usage: this <topkey {LocalMachine|ClassesRoot|CurrentConfig|CurrentUser|PerformanceData}> <root_path> <value_text> [yes to delete key, else not] [yes to ignore case, else not]");
                            Console.WriteLine(@"Sample: this LocalMachine ""SOFTWARE\Microsoft\Windows\CurrentVersion\Installer\UserData\S-1-5-18\Components"" ""agent"" no yes");
                        }
                    }
                    break;
                case 25:
                    {
                        await TestCsAllInOne.TestAsyncIOTask.Case1();
                        //await TestCsAllInOne.TestAsyncIOTask.Case0();
                    }
                    break;
                case 24:
                    {
                        Console.WriteLine("auto async method");
                        await TestCsAllInOne.TestAsyncMethodBuilder.MethodAutoAsync();
                        Console.WriteLine("auto async method ended");

                    }
                    break;
                case 23:
                    {
                        Console.WriteLine("manual async method");
                        var tt = TestCsAllInOne.TestAsyncMethodBuilder.MethodManualAsync();
                        int result = await tt.ConfigureAwait(false);
                        Console.WriteLine("manual async method ended");
                    }
                    break;
                case 22:
                    {
                        await TestStartTask.Case0(true);
                        await TestStartTask.Case0(false);

                        await TestStartTask.Case1(true);
                        await TestStartTask.Case1(false);

                        await TestStartTask.Case2(true);
                        await TestStartTask.Case2(false);



                    }
                    break;
                case 21:
                    {
                        TestCsAllInOne.TestPerm.Case0();
                    }
                    break;
                case 20:
                    {
                        TestCsAllInOne.TestTaskWait.Case0();
                    }
                    break;
                case 19:
                    {
                        await TestCsAllInOne.TestMonitor.Test.Case0();
                    }
                    break;
                case 18:
                    {
                        TestCsAllInOne.TestClrMethodTable.Case0();
                    }
                    break;
                case 17:
                    {
                        TestCsAllInOne.TestDelagate.Case0();
                    }
                    break;
                case 16:
                    {
                        int rr = await TestCsAllInOne.TestAwaitAsync.RunByStateMachine();
                        Console.WriteLine($"async method returned {rr}");
                        //await TestCsAllInOne.TestAwaitAsync.RunByAsyncAwait();

                    }
                    break;
                case 15:
                    {
                        if (args.Length > 1)
                        {
                            await TestCsAllInOne.DownloadZettaJason.DownloadAll(args[1]);
                        }
                        else
                        {
                            Console.WriteLine("Usage: this 15 <http list file>");
                        }

                    }
                    break;
                case 14:
                    {
                        TestCsAllInOne.TestILCallVirtual.Case0();
                    }
                    break;
                case 13:
                    {
                        TestCsAllInOne.TestDynamicMethodHelper.Case2();
                        TestCsAllInOne.TestDynamicMethodHelper.Case1();
                        TestCsAllInOne.TestDynamicMethodHelper.Case0();
                    }
                    break;
                case 12:
                    {
                        await TestCsAllInOne.TestTaskRun.TestTaskContinueWith();
                        await TestCsAllInOne.TestTaskRun.Case1();
                        await TestCsAllInOne.TestTaskRun.Case0();
                    }
                    break;
                case 11:
                    {
                        var t2 = TestCs.ManualAsync.MyFooAsync(5, 10, 15);
                        t2.Wait();
                        var rr = t2.Result;
                        Console.WriteLine($"result is {rr}");
                    }
                    break;
                case 10:
                    {

                        if (args.Length > 1 && int.TryParse(args[1], out int timeout))
                        {

                        }
                        else
                        {
                            timeout = 100;
                        }

                        var t1 = TestCs.ManualAsync.MyBarAsync(timeout);
                        bool usingAwait = true;
                        if (usingAwait)
                        {
                            var rr = await t1;
                            Console.WriteLine($"result is {rr}");
                        }
                        else
                        {
                            t1.Wait();
                            var rr = t1.Result;
                            Console.WriteLine($"result is {rr}");
                        }

                    }
                    break;
                case 9:
                    {

                        if (args.Length < 3)
                        {
                            Console.WriteLine("Usage: this.exe 9 <fileName> <seconds>");
                        }
                        else
                        {
                            if (!int.TryParse(args[2], out int seconds))
                            {
                                seconds = 60;
                            }
                            await TestCsAllInOne.FindTimeConsuming.Case0(args[1], seconds);
                        }

                    }
                    break;
                case 8:
                    {
                        string line = "[2019/01/30 15:51:25:594   02   16700   34932             ] ================ SUMMARY LOG =======================";
                        TestCsAllInOne.FindTimeConsumingHelper.FindTimeString(line);
                    }
                    break;
                case 7:
                    {
                        TestCsAllInOne.TestMonitor.Case0();
                    }
                    break;
                case 6:
                    {
                        await TestCsAllInOne.TestBufferCast.Case1();
                    }
                    break;
                case 5:
                    {
                        TestCsAllInOne.TestSpanAsyncAwait.Case2();
                        TestCsAllInOne.TestSpanAsyncAwait.Case1();
                    }
                    break;
                case 4:
                    {

                        await TestCsAllInOne.TestSpanAsyncAwait.DelaySomeTime2();
                        TestCsAllInOne.TestSpanAsyncAwait.Case0();
                    }
                    break;
                case 3:
                    {
                        TestCsAllInOne.TestCsAndDll.Case0();
                    }
                    break;
                case 2:
                    {
                        TestCsAllInOne.TestBufferCast.Case0();
                    }
                    break;
                case 1:
                    {
                        TestCsAllInOne.TestDasm.Case1();
                        TestCsAllInOne.TestDasm.Case0();
                    }
                    break;
                case 0:
                    {
                        TestCsAllInOne.TestExt.Case4();
                        TestCsAllInOne.TestExt.Case3();
                        TestCsAllInOne.TestExt.Case2();
                        TestCsAllInOne.TestExt.Case1();
                        TestCsAllInOne.TestExt.Case0();
                    }
                    break;
                case -1:
                default:
                    {
                        Console.WriteLine("Please input a valid case number!, the command line is:");
                        for (int ii = 0; ii < args.Length; ++ii)
                        {
                            Console.WriteLine($"arg[{ii}]={args[ii]}");
                        }

                    }

                    break;
            }

        }
    }
}
