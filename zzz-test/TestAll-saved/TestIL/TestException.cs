using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestIL
{
  static class TestException
    {
       static bool Filter(Exception ex, string message)
        {
            return ex.Message.Equals(message);
        }

        static string Ret(string str)
        {
            return str;
        }


    public    static void TCF()
        {
            try
            {
                throw new Exception("2");
            }
            catch(Exception ex1) when(Filter(ex1, "1"))
            {
                Console.WriteLine(ex1.Message);
                Console.WriteLine("filter 1");
            }
            catch(Exception ex2) when(Filter(ex2, "2"))
            {
                Console.WriteLine(ex2.Message);
                Console.WriteLine("filter 2");
            }
            catch(InvalidOperationException ioex)
            {
                Console.WriteLine(ioex.Message);
                Console.WriteLine("InvalidOperationException");
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.Message);
                Console.WriteLine("Exception");
            }
            finally
            {
                Console.WriteLine("finally");
            }
        }
    }
}
