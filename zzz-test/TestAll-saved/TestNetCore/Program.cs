using System;
using System.Threading.Tasks;

namespace TestNetCore
{
    class Program
    {
        static async Task<int> Main(string[] args)
        {
            int testCase = 16;
            switch(testCase)
            {
                case 16:
                    {
                        TestNetCore.TestRefStruct.Case0();
                    }
                    break;
                case 15:
                    {
                        TestNetCore.MyRsaCryptoTest.Case1();
                    }
                    break;
                case 14:
                    {
                        TestNetCore.TestRWLock.Case0();
                    }
                    break;
                case 13:
                    {
                        TestNetCore.TestTypeReflection.Case0();
                    }
                    break;
                case 12:
                    {
                        TestNetCore.MyTaskRun.Case2();
                        TestNetCore.MyTaskRun.Case0();
                    }
                    break;
                case 11:
                    {
                        TestNetCore.TestInterlocked.Case0();
                    }
                    break;
                case 10:
                    {
                        TestNetCore.TestCallTypes.Case0();
                    }
                    break;
                case 9:
                    {
                        TestNetCore.TestDynamic.Case0();
                    }
                    break;
                case 8:
                    {
                        TestNetCore.TestEnum.Case0();
                    }
                    break;
                case 7:
                    {
                        TestNetCore.TestCase.Case0();
                    }
                    break;
                case 6:
                    {
                        TestNetCore.CsInNutshell.TestHider.Case0();
                    }
                    break;
                case 5:
                    {
                        TestNetCore.Fib.TestFibSeq();
                    }
                    break;
                case 4:
                    {
                        TestNetCore.LinqTest.GroupJoin.Case0();
                    }
                    break;
                case 3:
                    {
                        TestNetCore.AutoTimeoutCollection.AutoTimeoutCollectionTest.Case1();
                    }
                    break;
                case 2:
                    {
                        TestNetCore.TestVirtualNew.Case1();
                    }
                    break;
                case 1:
                    {
                        TestNetCore.HanoiTest.Case1();
                    }
                    break;
                case 0:
                    {
                        TestNetCore.TestFinalizerCase.Case0();
                    }
                    break;
                default:
                    {
                        Console.WriteLine("default");
                    }
                    break;
                    
            }

            return await Task.FromResult<int>(0);
        }
    }
}