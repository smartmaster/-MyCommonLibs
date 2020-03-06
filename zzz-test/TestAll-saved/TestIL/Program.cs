using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestIL
{
    class Program
    {
        struct MyTestCase
        {
            public volatile int TestCase;
        }

        static void Main(string[] args)
        {
            MyTestCase tc = new MyTestCase
            {
                TestCase = 3,
            };

            switch(tc.TestCase)
            {
                case 3:
                    {
                        Console.WriteLine(TestIL.TestAsync.FuncAsync().Result);
                    }
                    break;
                case 2:
                    {
                        TestIL.TestPack.Case0();
                    }
                    break;
                case 1:
                    {
                        TestIL.TestAddr.CaseAll();
                    }
                    break;
                case 0:
                    {
                        TestIL.TestLabmda.Case0();
                    }
                    break;
                default:
                    {
                        throw new Exception("unexpected default");
                    }
                    //break;
            }
            
        }
    }
}
