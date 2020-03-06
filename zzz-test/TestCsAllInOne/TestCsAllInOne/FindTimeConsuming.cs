using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
    static class FindTimeConsumingHelper
    {
      public static (int start, int length) FindTimeString(string line)
      {
            int start = 0;
            while(start < line.Length)
            {
                if(char.IsNumber(line[start]))
                {
                    break;
                }
                ++start;
            }

            int end = start + 1;
            int colon = 0;
            while (end < line.Length)
            {
                if(line[end] == ':')
                {
                    ++colon;
                    if(colon == 3)
                    {
                        break;
                    }
                }

                ++end;
            }

            return (colon == 3) ? (start, end - start) : (0, 0);
      }
            
       
       public static async Task ParseFile(string fileName, int seconds)
        {
            TextReader reader = File.OpenText(fileName);
            string prev = null;
            DateTime dtPrev = default;
            for(; ; )
            {
                string line = await reader.ReadLineAsync();
                if(line == null)
                {
                    break;
                }
                (int start, int length) = FindTimeString(line);
                if(length != 0)
                {
                    DateTime.TryParse(line.AsSpan().Slice(start, length), out DateTime dt);

                    if (prev != null)
                    {
                        var timeSpan = dt - dtPrev;
                        if(timeSpan >= TimeSpan.FromSeconds(seconds))
                        {
                            Console.WriteLine($"Operation took {timeSpan.TotalSeconds} Seconds");
                            Console.WriteLine($"{prev}");
                            Console.WriteLine($"{line}");
                            Console.WriteLine();
                        }
                    }

                    prev = line;
                    dtPrev = dt;
                }
               
            }
        }
    }
   public static class FindTimeConsuming
    {
        public static async Task Case0(string fileName, int seconds)
        {
            await TestCsAllInOne.FindTimeConsumingHelper.ParseFile(fileName, seconds);
           
        }
    }
}
