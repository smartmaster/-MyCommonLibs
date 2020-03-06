using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;

namespace TestQuiz
{
    public static class Program
    {
        public static void Main(string[] args)
        {


            int testCase = 0;
            int.TryParse(args[0], out testCase);
            switch(testCase)
            {
                case 8:
                    {
                        Console.WriteLine(nameof(System));
                        Console.WriteLine(nameof(System.Collections));
                        Console.WriteLine(nameof(System.Collections.Generic));
                        Console.WriteLine(nameof(System.Collections.Generic.ICollection<object>));
                        Console.WriteLine(nameof(System.Collections.Generic.ICollection<object>.Add));
                        Console.WriteLine(nameof(System.Collections.Generic.ICollection<object>.Count));

                    }
                    break;

                case 7:
                    {
                        TestQuiz.AllCombinationOfSumTest.Case0();
                    }
                    break;
                case 6:
                    {
                        TestQuiz.Calc18Test.Case0();
                    }
                    break;
                case 5:
                    {
                        TestQuiz.BatchDisposableTest.Case0();
                    }
                    break;
                case 4:
                    {
                        TestQuiz.SmartRefCountTest.Case0();
                    }
                    break;
                case 3:
                    {
                        TestQuiz.PointerTest.Case0();
                    }
                    break;
                case 2:
                    {
                        TestQuiz.PermutationTest.Case0();
                    }
                    break;
                case 1:
                    {
                        TestQuiz.ClockGameV2.ClockTest.Case0();
                    }
                    break;
                case 0:
                    {
                        //TestQuiz.ClockGameV1.ClockTest.Case0();
                    }
                    break;
            }


        }
    }
}
