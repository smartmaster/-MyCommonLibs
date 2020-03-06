using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ZzzCommon
{
    public static class StreamHelper
    {
        public static async Task CopyStreamAsync(Stream source, Stream dest, byte[] buffer)
        {
            try
            {
                for (;;)
                {
                    int byteRead = await source.ReadAsync(buffer, 0, buffer.Length);
                    if (byteRead == 0)
                    {
                        break;
                    }
                    await dest.WriteAsync(buffer, 0, byteRead);
                }
                await dest.FlushAsync();
            }
            catch (System.Exception ex)
            {
                ZzzCommon.ExceptionHelper.PrintException(ex, Console.Out);
                throw;
            }
        }
    }
}
