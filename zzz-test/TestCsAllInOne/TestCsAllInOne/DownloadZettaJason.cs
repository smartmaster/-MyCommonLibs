using System;
using System.Collections.Generic;
using System.IO;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
    class DownloadZettaJason
    {
        static async Task Download(string uri, string fileName)
        {
            using (HttpClient client = new HttpClient())
            {
                string response = await client.GetStringAsync(uri);
                await File.WriteAllTextAsync(fileName, response, Encoding.UTF8);
            }
        }

        public static async Task DownloadAll(string manifestFile)
        {
            int index = 0;
            const string HTTPS_PREFIX = "https://";
            const string HTTP_PREFIX = "http://";
            foreach (string uri in File.ReadAllLines(manifestFile))
            {
                if(uri.Length > HTTPS_PREFIX.Length &&
                    (string.Compare(uri.Substring(0, HTTPS_PREFIX.Length), HTTPS_PREFIX, true) == 0 ||
                     string.Compare(uri.Substring(0, HTTP_PREFIX.Length), HTTP_PREFIX, true) == 0))
                    
                {
                    await Download(uri, index.ToString() + ".json");
                    ++index;
                }
            }
        }

    }
}
