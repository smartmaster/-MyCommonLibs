using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;


//http://ustc.openjudge.cn/ex5/4/

namespace TestQuiz.ClockGameV2
{
    public class ClockGame
    {
        private readonly int[] m_start;
        private readonly int[] m_result;

        private readonly Stack<int> m_currentSteps = new Stack<int>();
        private IList<int> m_minSteps = null;

        private readonly string[] m_moves = new string[]
        {
            "ABDE",
            "ABC",
            "BCEF",
            "ADG",
            "BDEFH",
            "CFI",
            "DEGH",
            "GHI",
            "EFHI"
        };


        public ClockGame(int[] start)
        {
            m_start = start;
            m_result = m_start.ToArray();
        }

        private void MoveForward(int step)
        {
            m_currentSteps.Push(step);
            foreach (char ch in m_moves[step])
            {
                ++m_result[ch - 'A'];
            }
        }

        private void MoveBack()
        {
            int step = m_currentSteps.Pop();
            foreach (char ch in m_moves[step])
            {
                --m_result[ch - 'A'];
            }

        }

        private bool IsOK()
        {
            bool ret = true;
            for (int ii = 0; ii < m_start.Length; ++ii)
            {
                if (m_result[ii] % 4 != 0)
                {
                    ret = false;
                    break;
                }
            }
            return ret;
        }

        private bool IsCircular()
        {
            bool ret = true;
            for (int ii = 0; ii < m_start.Length; ++ii)
            {
                if ((m_result[ii] - m_start[ii]) % 4 != 0)
                {
                    ret = false;
                    break;
                }
            }
            return ret;
        }

        private int LastStepCount(int step)
        {
            int count = 0;
            foreach(int ss in m_currentSteps)
            {
                if (ss == step)
                {
                    ++count;
                }
                else
                {
                    break;
                }
            }

            return count;
        }

        public void Move()
        {

            for (int ii = 0; ii < m_moves.Length; ++ii)
            {
                bool moveOn =
                    (m_currentSteps.Count == 0) ||
                    (ii > m_currentSteps.Peek()) ||
                    (ii == m_currentSteps.Peek() && LastStepCount(ii) < 3);

                if (moveOn)
                {
                    MoveForward(ii);
                    if (m_minSteps != null && m_minSteps.Count <= m_currentSteps.Count)
                    {
                        //do nothing
                    }
                    else if (IsOK())
                    {
                        m_minSteps = m_currentSteps.Reverse().ToList();
                    }
                    else if (IsCircular())
                    {
                        //do nothing
                    }
                    else
                    {
                        Move(); //recursively find
                    }
                    MoveBack();
                }
            }
        }



        bool VerifyResult()
        {
            int[] startTemp = m_start.ToArray();
            foreach (int step in m_minSteps)
            {
                foreach (char ch in m_moves[step])
                {
                    ++startTemp[ch - 'A'];
                }
            }

            bool bret = true;
            for (int ii = 0; ii < startTemp.Length; ++ii)
            {
                if (startTemp[ii] % 4 != 0)
                {
                    bret = false;
                    break;
                }
            }

            return bret;
        }

        static void PrintArray(string message, int[] arr)
        {
            Console.Out.Write($"{message}");
            foreach (int vv in arr)
            {
                Console.Out.Write($"{vv} ");
            }
            Console.Out.Write($"({arr.Length})");
            Console.Out.WriteLine();
        }

        public bool PrintResult()
        {
            bool verified = false;
            PrintArray("Clock:\t", m_start);
            Console.Out.Write("Move:\t");
            if (m_minSteps == null)
            {
                Console.Out.Write($"No solution!");
            }
            else
            {
                verified = VerifyResult();
                if (!verified)
                {
                    Console.Out.Write($"You have got a wrong answer!");
                }
                else
                {
                    foreach (int step in m_minSteps)
                    {
                        Console.Out.Write($"{step + 1} ");
                    }
                    Console.Out.Write($"({m_minSteps.Count})");
                }


            }
            Console.Out.WriteLine();
            return verified;
        }

    }


    public static class ClockTest
    {
        public static void Case0()
        {
            int[][] testData = new int[][]
            {
                new int[]{3, 3, 0, 2, 2, 2, 2, 1, 2 },
                new int[]{0, 1, 2, 1, 2, 3, 2, 3, 0 },
                new int[]{3, 3, 3, 3, 3, 3, 3, 3, 3 },
                new int[]{2, 2, 2, 2, 2, 2, 2, 2, 2 },
                new int[]{1, 1, 1, 1, 1, 1, 1, 1, 1 }, //4
                new int[]{1, 2, 3, 4, 5, 6, 7, 8, 9 },
                new int[]{9, 8, 7, 6, 5, 4, 3, 2, 1 },
                new int[]{0, 0, 0, 0, 0, 0, 0, 0, 0 },
            };

            //Console.Out.WriteLine("+++++++++++++Do not allow duplicates+++++++++++++");
            Console.Out.WriteLine();
            foreach (int[] td in testData)
            {
                Stopwatch sw = new Stopwatch();
                sw.Start();

                ClockGame cc = new ClockGame(td);
                cc.Move();
                cc.PrintResult();

                sw.Stop();
                Console.Out.WriteLine($"Elapsed {sw.ElapsedMilliseconds} ms");
                Console.Out.WriteLine();
            }
            Console.Out.WriteLine();
            Console.Out.WriteLine();
            Console.Out.WriteLine();

#if false
            Console.Out.WriteLine("+++++++++++++Do allow duplicates+++++++++++++");
            Console.Out.WriteLine();
            foreach (int[] td in testData)
            {
                Stopwatch sw = new Stopwatch();
                sw.Start();

                ClockGame cc = new ClockGame(td, true, 20);
                cc.Move();
                cc.PrintResult();

                sw.Stop();
                Console.Out.WriteLine($"Elapsed {sw.ElapsedMilliseconds} ms");
                Console.Out.WriteLine();
            }
            Console.Out.WriteLine();
            Console.Out.WriteLine();
            Console.Out.WriteLine();


            Console.Out.WriteLine("+++++++++++++try to find with more steps+++++++++++++");
            Console.Out.WriteLine();

            for (int index = 4; index < testData.GetLength(0); ++index)
            {
                for (int throttle = 30; throttle <= 100; throttle += 10)
                {
                    Stopwatch sw = new Stopwatch();
                    sw.Start();

                    int[] td = testData[index];
                    ClockGame cc = new ClockGame(td, true, throttle);
                    cc.Move();
                    bool verifed = cc.PrintResult();

                    sw.Stop();
                    Console.Out.WriteLine($"Elapsed {sw.ElapsedMilliseconds} ms");
                    Console.Out.WriteLine();

                    if (verifed)
                    {
                        break;
                    }
                }
            }
            Console.Out.WriteLine();
            Console.Out.WriteLine();
            Console.Out.WriteLine();
#endif

        }
    }

}

