using System;
using System.Collections.Generic;
using System.Text;

namespace TestNetCore
{

    class HanoiNR
    {
        public int Count { get; set; }
        public int S { get; set; }
        public int D { get; set; }
        public int X { get; set; }


        public HanoiNR(int count, int s, int d, int x)
        {
            Count = count;
            S = s;
            D = d;
            X = x;
        }

        (HanoiNR, HanoiNR, HanoiNR) Split()
        {
            return (
                new HanoiNR(Count - 1, S, X, D),
                new HanoiNR(1, S, D, X),
                new HanoiNR(Count - 1, X, D, S)
                );
        }


        public static void Move(int count, int s, int d, int x)
        {
            Stack<HanoiNR> stack = new Stack<HanoiNR>();
            stack.Push(new HanoiNR(count, s, d, x));
            while (stack.Count != 0)
            {
                HanoiNR top = stack.Pop();
                if (top.Count == 1)
                {
                    Console.WriteLine($"{top.S} --> {top.D}");
                }
                else
                {
                    var subs = top.Split();
                    stack.Push(subs.Item3);
                    stack.Push(subs.Item2);
                    stack.Push(subs.Item1);
                }
            }
        }
    }
    class Hanoi
    {
        public static void Move(int count, int s, int d, int x)
        {

            if (1 == count)
            {
                Console.WriteLine($"{s} --> {d}");
                return;
            }

            Move(count - 1, s, x, d);
            Move(1, s, d, x);
            Move(count - 1, x, d, s);
        }

    }


    static class HanoiTest
    {
        public static void Case1()
        {
            int count = 4;
            int s = 1;
            int x = 2;
            int d = 3;

            Console.WriteLine("recursive version:");

            Hanoi.Move(count, s, d, x);

            Console.WriteLine();
            Console.WriteLine();


            Console.WriteLine("non-recursive version:");
            HanoiNR.Move(count, s, d, x);
        }
    }
}
