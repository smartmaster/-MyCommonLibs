using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WcfTransportSecurityLibraryClient
{
    class Program
    {
        static async Task Main(string[] args)
        {

            System.Net.ServicePointManager.ServerCertificateValidationCallback =
            (sender, certificate, chain, sslPolicyErrors) =>
            {
                return true;
            };


            using (MyDataServiceReference.MyDataServiceClient client = new MyDataServiceReference.MyDataServiceClient())
            {

                await client.BeginAsync();

                MyDataServiceReference.MyData myData = new MyDataServiceReference.MyData()
                {
                    Offset = 100,
                    Length = 200,
                    Name = "Captain Jack",
                    Buffer = UTF8Encoding.UTF8.GetBytes("hello world")
                };


                const int LOOP_COUNT = 5;
                for (int ii = 0; ii < LOOP_COUNT; ++ii)
                {
                    MyDataServiceReference.MyData rv = await client.GetDataAsync(myData);
                    Console.WriteLine($"{nameof(rv.Offset)}={rv.Offset}, {nameof(rv.Length)}={rv.Length}, {nameof(rv.Name)}={rv.Name}, {nameof(rv.Buffer)}={UTF8Encoding.UTF8.GetString(rv.Buffer)}");
                }

                await client.EndAsync();
            }

        }
    }
}
