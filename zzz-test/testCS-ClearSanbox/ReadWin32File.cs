using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading.Tasks;

namespace testCS
{
   public static class ReadWin32File
    {
        static string Bin2Str(byte[] buffer, int len)
        {
            StringBuilder sb = new StringBuilder();
            for(int ii = 0; ii < len; ++ ii)
            {
                sb.AppendFormat("{0:X2} ", buffer[ii]);
            }

            sb.Append('\t');
            for (int ii = 0; ii < len; ++ii)
            {
                char ch = (char)buffer[ii];
                if(char.IsLetterOrDigit(ch))
                {
                    sb.Append(ch);
                }
                else
                {
                    sb.Append(' ');
                }
                
            }
            //sb.AppendLine();
            return sb.ToString();
        }
        public static async Task DumpAsync(string fileName, long offset, long length)
        {
            byte[] buffer = new byte[16];
            using (FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
            {
                try
                {
                    fs.Position = offset;
                    while (length > 0)
                    {
                        int currentLen = (int)length;
                        if (currentLen > buffer.Length)
                        {
                            currentLen = buffer.Length;
                        }
                        int len = await fs.ReadAsync(buffer, 0, currentLen);
                        Console.WriteLine(Bin2Str(buffer, len));
                        length -= currentLen;
                    }

                }
                catch(Exception ex)
                {
                    Console.WriteLine(ex);
                }
                
            }
        }
    }
}
