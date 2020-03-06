using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

//http://ustc.openjudge.cn/practice/4/

namespace TestQuiz
{
    public class Calc18
    {
        private static int[][] p_numbers = new int[][]
        {
            new int[] {0, 1, 2},
            new int[] {0, 2, 1},
            new int[] {1, 0, 2},
            new int[] {1, 2, 0},
            new int[] {2, 0, 1},
            new int[] {2, 1, 0}
        };

        private static int[][] p_ops = new int[][]
        {
            new int[] {0, 1},
            new int[] {0, 2},
            new int[] {1, 0},
            new int[] {1, 2},
            new int[] {2, 0},
            new int[] {2, 1},
        };

        private static char[] ops = new char[] { '+', '-', '*' };

        private static int Calc(int aa, int bb, char op)
        {
            switch (op)
            {
                case '+':
                    return aa + bb;
                case '-':
                    return aa - bb;
                case '*':
                    return aa * bb;
            }
            return -1;
        }

        private static int Calc(int aa, int bb, int cc, char op0, char op1)
        {
            int rr = Calc(aa, bb, op0);
            return Calc(rr, cc, op1);
        }

        private static void Print(int aa, int bb, int cc, char op0, char op1, int result)
        {
            if (op0 != '*' && op1 == '*')
            {
                Console.WriteLine($"({aa} {op0} {bb}) {op1} {cc} == {result}");
            }
            else
            {
                Console.WriteLine($"{aa} {op0} {bb} {op1} {cc} == {result}");
            }
            
        }

        public static bool IsOkay(int[] numbers, int expected)
        {
            foreach (var numSeq in p_numbers)
            {
                foreach(char op in ops) //the 2 operators are same
                {
                    if (expected == Calc(numbers[numSeq[0]], numbers[numSeq[1]], numbers[numSeq[2]], op, op))
                    {
                        Print(numbers[numSeq[0]], numbers[numSeq[1]], numbers[numSeq[2]], op, op, expected);
                        return true;
                    }
                }

                foreach (var opSeq in p_ops) //the 2 operators are different
                {
                    if (expected == Calc(numbers[numSeq[0]], numbers[numSeq[1]], numbers[numSeq[2]], ops[opSeq[0]], ops[opSeq[1]]))
                    {
                        Print(numbers[numSeq[0]], numbers[numSeq[1]], numbers[numSeq[2]], ops[opSeq[0]], ops[opSeq[1]], expected);
                        return true;
                    }
                }
            }
            return false;
        }
    }


    public static class Calc18Test
    {

        public static void Case0()
        {
            int[][] testData = new int[][]
            {
                new int[] {7, 5, 5},
                new int[] {6, 6, 6},
                new int[] {3, 4, 5},
                new int[] {1000, 100, 256},
                new int[] {7, 8, 32},
            };

            foreach (var td in testData)
            {
                if (Calc18.IsOkay(td, 24))
                {
                }
                else
                {
                    Console.Write($"No solution for [ ");
                    foreach(int vv in td)
                    {
                        Console.Write($"{vv} ");
                    }
                    Console.WriteLine("]");
                }
            }
        }
    }
}
