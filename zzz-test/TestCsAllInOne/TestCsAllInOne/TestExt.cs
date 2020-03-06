using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCsAllInOne
{
    interface IMethod
    {
        void Method();
    }
    class Caa : IMethod
    {
        public virtual void Method()
        {
            Console.WriteLine("i am aa");
        }
    }

    class Cbb : Caa
    {
        public override void Method()
        {
            Console.WriteLine("i am bb");
        }
    }

    class Ccc : Cbb
    {
        public new virtual void Method()
        {
            Console.WriteLine("i am cc");
        }
    }


    class Cdd : Ccc
    {
        public new virtual void Method()
        {
            Console.WriteLine("i am dd");
        }
    }


    class Cee : Cdd
    {
        public override void Method()
        {
            Console.WriteLine("i am ee");
        }
    }


    interface Itf1
    {
        void Fun();
    }

    interface Itf2
    {
        void Fun();
    }


    class C1 : Itf1
    {
        public void Fun()
        {
            Console.WriteLine("i am c1");
        }
    }

    class C12 : Itf1, Itf2
    {
        public void Fun()
        {
            Console.WriteLine("i am c12");
        }
    }


    class Ca
    {
        public virtual void Func()
        {
            Console.WriteLine("i am ca");
        }
    }

    class Cb : Ca
    {
        public override void Func()
        {
            Console.WriteLine("i am cb");
        }
    }


    class Cbase
    {
        public void Func()
        {
            this.GetType();
            Console.WriteLine("i am Cbase");
        }
    }

    class CDerived : Cbase
    {
        public new void Func()
        {
            Console.WriteLine("i am CDerived");
        }
    }

   static class TestExt
    {
        public static void Case4()
        {
            //IQueryable<string> fadf = null;

            Cee ee = new Cee();
            (ee as IMethod).Method();
            (ee as Caa).Method();
            (ee as Cbb).Method();
            (ee as Ccc).Method();
            (ee as Cdd).Method();
            (ee as Cee).Method();
        }
        public static void Case3()
        {
            CDerived dd = new CDerived();
            dd.Func();
            (dd as Cbase).Func();
        }
        
        
        static void WhoAmI(this Ca aa)
        {
            Console.WriteLine("static void WhoAmI(this Ca aa)");
            aa.Func();
        }

        static void WhoAmI(this Cb bb)
        {
            Console.WriteLine("static void WhoAmI(this Cb bb)");
            bb.Func();
        }

        public static void Case2()
        {
            {
                Cb bb = new Cb();
                bb.WhoAmI();
                (bb as Ca).WhoAmI();
            }


            {
                Ca aa = new Ca();
                aa.WhoAmI();
                (aa as Cb)?.WhoAmI();
            }
            
        }


        static void CallMe(this Itf1 i1)
        {
            Console.WriteLine("CallMe(this Itf1 i1)");
            i1.Fun();
        }

        static void CallMe(this Itf2 i2)
        {
            Console.WriteLine("CallMe(this Itf2 i2)");
            i2.Fun();
        }


        public static void Case1()
        {
            {
                Itf1 i1 = new C1();
                i1.CallMe();
                (i1 as Itf2)?.CallMe();
            }
            {
                Itf1 i1 = new C12();
                i1.CallMe();
                (i1 as Itf2)?.CallMe();
            }
        }

        public static void Case0()
        {
            {
                Itf1 i1 = new C1();
                i1.Fun();
                (i1 as Itf2)?.Fun();
            }
            {
                Itf1 i1 = new C12();
                i1.Fun();
                (i1 as Itf2)?.Fun();
            }
        }
    }
}
