using System;
using System.Collections.Generic;
using System.Text;

namespace TestNetCore
{

    class Person
    {
        public virtual void WhoAmI()
        {
            Console.WriteLine($"{nameof(Person)}");
        }
    }


    class Worker : Person
    {
        public override void WhoAmI()
        {
            Console.WriteLine($"{nameof(Worker)}"); ;
        }
    }


  static  class TestCallTypes
    {
        public static void F(Person p)
        {
            Console.WriteLine($"I am in F(Person p)");
            p.WhoAmI();
        }


        public static void F(Worker w)
        {
            Console.WriteLine($"I am in F(Worker w)");
            w.WhoAmI();
        }


        public static void Case0()
        {
            Worker w = new Worker();
            Person p = w;
            F(w);
            F(p);
        }
    }
}
