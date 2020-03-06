using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ZzzCommon
{
    public static class ExceptionHelper
    {
        public static void PrintException(Exception ex, TextWriter tw, bool recur = true)
        {
            if (ex != null)
            {
                tw.WriteLine();
                tw.WriteLine($"{ex.GetType().FullName}");
                tw.WriteLine($"{ex.Message}");
                tw.WriteLine($"{ex}");
                if(recur)
                {
                    PrintException(ex.InnerException, tw, recur);
                }
            }
        }
    }
}
