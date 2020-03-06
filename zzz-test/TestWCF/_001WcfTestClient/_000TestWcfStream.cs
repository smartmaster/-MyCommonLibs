using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _001WcfTestClient.ServiceStreamReference;
using System.ServiceModel;
using System.IO;
using System.ServiceModel.Channels;

namespace _001WcfTestClient
{
    public static class _000TestWcfStream
    {
        public static async Task Test()
        {
            try
            {

                Binding binding = null;
                EndpointAddress epa = null;

                int bindingNumer = 1;
                switch (bindingNumer)
                {
                    case 1:
                        {
                            binding = new BasicHttpBinding()
                            {
                                TransferMode = TransferMode.Streamed,
                                OpenTimeout = TimeSpan.FromMinutes(15),
                                ReceiveTimeout = TimeSpan.FromMinutes(15),
                                SendTimeout = TimeSpan.FromMinutes(15),
                                MaxReceivedMessageSize = 512 * 1024,
                                ReaderQuotas = new System.Xml.XmlDictionaryReaderQuotas
                                {
                                    MaxArrayLength = 512 * 1024,
                                    MaxBytesPerRead = 512 * 1024
                                }
                            };
                            epa = new EndpointAddress("http://localhost:8733/");
                        }
                        break;
                    case 0:
                        {
                            binding = new NetTcpBinding(SecurityMode.None, false)
                            {
                                TransferMode = TransferMode.Streamed,
                                OpenTimeout = TimeSpan.FromMinutes(15),
                                ReceiveTimeout = TimeSpan.FromMinutes(15),
                                SendTimeout = TimeSpan.FromMinutes(15),
                                MaxReceivedMessageSize = 512 * 1024
                            };
                            //net.tcp://localhost:8734/Design_Time_Addresses/WwwSSS/Service1/netTcp/
                            epa = new EndpointAddress("net.tcp://localhost:8734/tcp");
                        }
                        break;
                }


                using (var client = new ServiceStreamClient(binding, epa))
                {
                    const string FILE_NAME = @"e:\1.png";
                    using (Stream clientStream = new FileStream(FILE_NAME, FileMode.Open, FileAccess.Read, FileShare.Read))
                    {
                        using (Stream serverSteam = await client.InvokeAsync(clientStream))
                        {
                            bool testCloseServerStraem = false;
                            if (testCloseServerStraem)
                            {
                                serverSteam.Dispose();
                                for (int ii = 0; ii < 10; ++ ii)
                                {
                                    System.Threading.Thread.Sleep(TimeSpan.FromSeconds(1));
                                }
                            }
                            const string FILE_NAME3 = @"e:\3.png";
                            using (Stream clientStream3 = new FileStream(FILE_NAME3, FileMode.Create, FileAccess.Write, FileShare.None))
                            {
                                //await serverSteam.CopyToAsync(clientStream3);
                                await ZzzCommon.StreamHelper.CopyStreamAsync(serverSteam, clientStream3, new byte[64 * 1024]);
                            }
                        }
                    }
                }
            }
            catch(FaultException<ExceptionDetail> ex)
            {
                ZzzCommon.ExceptionHelper.PrintException(ex, Console.Out, true);
                throw;
            }
            catch (System.Exception ex)
            {
                ZzzCommon.ExceptionHelper.PrintException(ex, Console.Out, true);
                throw;
            }
        }
    }
}
