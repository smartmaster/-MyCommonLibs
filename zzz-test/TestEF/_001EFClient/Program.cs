using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace _001EFClient
{
    class Program
    {
        static int Main(string[] args)
        {
            int testCase = -1;
            Int32.TryParse(args[0], out testCase);
            switch (testCase)
            {
                case 3:
                    {
                        Task ttt = GuidAndIntPK.Test.Case1(args);
                        ttt.Wait();
                    }
                    break;
                case 2:
                    {
                        Task ttt = _002TablePerTypeEF.Test.Case1(args);
                        ttt.Wait();
                    }
                    break;
                case 1:
                    {
                        Task ttt = _001SimpleMKEF.Test.Case1(args);
                        ttt.Wait();
                    }
                    break;
                case 0:
                    {
                        Task ttt =_000SimpleEF.Test.Case1(args);
                        ttt.Wait();
                    }
                    break;
                default:
                    {
                        throw new InvalidOperationException("the test case  number is not invalid");
                    }
            }

            return 0;
        }
    }
}
