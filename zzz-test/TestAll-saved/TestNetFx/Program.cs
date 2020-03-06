using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestNetFx
{
    class Program
    {
        static void Main(string[] args)
        {
            int testCase = 0;
            switch (testCase)
            {
                case 0:
                    {
                        TestNetFx.TestFinalizerCase.Case0();
                    }
                    break;
                default:
                    {
                        Console.WriteLine("default");
                    }
                    break;

            }
        }
    }
}
