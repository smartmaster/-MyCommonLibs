using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace WcfTransportSecurityLibraryHost
{
    class Program
    {
        static void Main(string[] args)
        {
            using (ServiceHost host = new ServiceHost(typeof(WcfTransportSecurityLibrary.MyDataService)))
            {
                host.Open();
                var uris = host.Description.Endpoints.Select(ep => ep.ListenUri.AbsoluteUri);
                foreach (string uri in uris)
                {
                    Console.WriteLine($"Service started at {uri} on {DateTime.Now}");
                }
                Console.WriteLine($"Press ENTER to end service...");
                Console.ReadLine();
            }
        }
    }
}
