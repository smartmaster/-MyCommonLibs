using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestFx
{
    class Program
    {
        static void Main(string[] args)
        {
            int testCase = -1;
            if(args.Length > 0)
            {
                int.TryParse(args[0], out testCase);
            }

            switch(testCase)
            {
                case 0:
                    {
                        TestFx.KeySim.InputKey();
                    }
                    break;
                case -1:
                default:
                    {
                        Console.WriteLine("Invalid test case");
                    }
                    break;
            }
        }
    }
}
