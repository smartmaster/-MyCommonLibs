using System;
using System.Threading.Tasks;

namespace testCS
{
    class Program
    {

        static async Task Main(string[] args)
        {
            int testCase = 0;
            if(args.Length > 0)
            {
                int.TryParse(args[0], out testCase);
            }
            else
            {
                Console.WriteLine($"Usage:Clean up sandbox folders:\r\n\t this.exe 4 <dir name>");
                Console.WriteLine($"Usage:Change owner and grant access:\r\n\t this.exe 11 <dir name>");
                return;
            }

            switch (testCase)
            {
                case 11:
                    {
                        ModifyFileSec.FileSystemSec.ChangeOwnerAndGrantAccess(args[1]);
                    }
                    break;
                case 10:
                    {
                        RefCountableTest.Case0();
                    }
                    break;
                case 9:
                    {
                        await testCS.DelegaeType.Case0();
                    }
                    break;
                case 8:
                    {
                        TestMem.TestSyncAsync2();
                        TestMem.TestSyncAsync();
                    }
                    break;
                case 7:
                    {
                        TestMem.TestSyncAsync();
                        await TestMem.TestAwaiter();
                    }
                    break;
                case 6:
                    {
                        await TestMem.ReadAsync(args[1]);
                    }
                    break;
                case 5:
                    {
                        Bin2Struct.Case0();
                    }
                    break;
                case 4:
                    {
                        EnumDir.ClearSanbox(args[1]);
                    }
                    break;
                case 3:
                    {
                        EnumDirTest.Case0();
                    }
                    break;
                case 2:
                    {
                        await Case2(args);
                    }
                    break;
                case 1:
                    {
                        await Case1(args);
                    }
                    break;
                case 0:
                    {
                        await Case0(args);
                    }
                    break;
            }

        }


        static async Task Case2(string[] args)
        {
            await StackAsync.DataOrCodeAsync();
        }

        static async Task Case1(string[] args)
        {
            await StackAsync.MethodAsync();
        }

        static async Task Case0(string[] args)
        {
            if (args.Length < 3)
            {
                Console.WriteLine("this.exe <file name> <offset> <length>");
                return;
            }
            string fileName = args[0];
            long offset = long.Parse(args[1]);
            long length = long.Parse(args[2]);
            await ReadWin32File.DumpAsync(fileName, offset, length);
            Console.WriteLine();
        }
    }
}
