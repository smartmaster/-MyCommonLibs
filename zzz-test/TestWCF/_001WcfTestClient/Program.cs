using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _001WcfTestClient
{
    class Program
    {
        static void Main(string[] args)
        {
            int testCase = -1;
            int.TryParse(args[0], out testCase);
            switch(testCase)
            {
                case 1:
                    {
                        try
                        {
                            Task tt = _001WcfTestClient._001TestMessageStream.Test();
                            tt.Wait();
                        }
                        catch (System.Exception ex)
                        {
                            ZzzCommon.ExceptionHelper.PrintException(ex, Console.Out, true);
                        }

                    }
                  break;

                case 0:
                    {
                        try
                        {
                            Task tt = _001WcfTestClient._000TestWcfStream.Test();
                            tt.Wait();
                        }
                        catch (System.Exception ex)
                        {
                            ZzzCommon.ExceptionHelper.PrintException(ex, Console.Out);
                        }

                    }
                    break;

                default:
                    {
                        throw new InvalidOperationException("test case in invalid");
                    }
            }
        }
    }
}
