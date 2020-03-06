using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _000WcfStreamService.Common
{
    public static class CommonHelpers
    {
        public static void PrintException(Exception ex, TextWriter tw, bool recursively)
        {
            if (null != ex)
            {
                tw.WriteLine("{0}", ex.GetType().FullName);
                tw.WriteLine("{0}", ex.Message);
                tw.WriteLine("{0}", ex);
                tw.WriteLine();
                tw.WriteLine();
                if (recursively)
                {
                    PrintException(ex.InnerException, tw, recursively);
                }
            }
        }
        public static async Task StreamCopyAsync(Stream source, Stream dest, byte[] buffer)
        {
            try
            {
                for (;;)
                {
                    int readSize = await source.ReadAsync(buffer, 0, buffer.Length);
                    if (0 == readSize)
                    {
                        break;
                    }
                    await dest.WriteAsync(buffer, 0, readSize);
                }
                await dest.FlushAsync();
            }
            catch (System.Exception ex)
            {
                PrintException(ex, Console.Out, true);
                throw;
            }
        } 
    }
}
