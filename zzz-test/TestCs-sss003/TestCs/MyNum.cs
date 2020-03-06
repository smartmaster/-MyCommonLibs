using System;
using System.Collections.Generic;
using System.Text;

namespace TestCs
{
    class MyNum
    {
        public int Num { get; set; }

        public static bool operator true(MyNum num)
            {
            Console.WriteLine($"operator true on {num.Num}");
            return num.Num > 0;
            }


        public static bool operator false(MyNum num)
        {
            Console.WriteLine($"operator false on {num.Num}");
            return num.Num < 0;
        }

        public static MyNum operator &(MyNum n1, MyNum n2)
        {
            Console.WriteLine($"operator & on {n1.Num}, {n2.Num}");
            bool b1 = false;
            bool b2 = false;
            if (n1)
            {
                b1 = true;
            }


            if(n2)
            {
                b2 = true;
            }

            if(b1 & b2)
            {
                return new MyNum { Num = 1};
            }
            else
            {
                return new MyNum { Num = 0 };
            }
        }

        public static MyNum operator |(MyNum n1, MyNum n2)
        {
            Console.WriteLine($"operator | on {n1.Num}, {n2.Num}");
            bool b1 = false;
            bool b2 = false;
            if (n1)
            {
                b1 = true;
            }

            if (n2)
            {
                b2 = true;
            }

            if (b1 | b2)
            {
                return new MyNum { Num = 1 };
            }
            else
            {
                return new MyNum { Num = 0 };
            }
        }

        public static MyNum operator !(MyNum num)
        {
            Console.WriteLine($"operator ! on {num.Num}");
            bool b = false;
            if(num)
            {
                b = true;
            }

            return !b? new MyNum { Num = 1 } : new MyNum { Num = 0 };
        }

    }


    static class MyNumTest
    {
        public static void Case()
        {
            MyNum num1 = new MyNum { Num = 0 };
            num1.Num = 0;
            if (num1)
            {
                Console.WriteLine("num is non-zero");
            }
            else
            {
                Console.WriteLine("num is zero");
            }


            MyNum num = new MyNum {Num = 100 };
            if(num)
            {
                Console.WriteLine("num is non-zero");
            }
            else
            {
                Console.WriteLine("num is zero");
            }

            if(num && num1)
            {
                
            }
            Console.WriteLine();

            if (num1 && num)
            {

            }
            Console.WriteLine();


            if (num || num1)
            {

            }
            Console.WriteLine();

            if (num1 || num)
            {

            }
            Console.WriteLine();

            if (!num)
            {

            }
            Console.WriteLine();


            if (!num1)
            {

            }
            Console.WriteLine();
        }
    }
}
