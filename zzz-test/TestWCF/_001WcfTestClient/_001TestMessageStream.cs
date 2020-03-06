using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _001WcfTestClient.ServiceReferenceMessageStream;
using System.IO;
using System.ServiceModel;

namespace _001WcfTestClient
{
    public static class _001TestMessageStream
    {
        public static async Task Test()
        {
            try
            {
                using (var client = new ServiceMessgeStreamClient("NetTcpBinding_IServiceMessgeStream", "net.tcp://localhost:8735/_002WcfMessgeStreamLib/ServiceMessgeStream/"))
                {
                    const string cliemtFileName = @"e:\1.png";
                    const string ServerFileName = @"e:\2.png";
                    const string clientFileNameDownloaded = @"e:\3.png";
                    using (var clientStream = new FileStream(cliemtFileName, FileMode.Open, FileAccess.Read, FileShare.None))
                    {
                        await client.UploadAsync(ServerFileName, clientStream);
                    }

                    using (Stream serverStream = await client.DownloadAsync(ServerFileName))
                    using(Stream clientSteam = new FileStream(clientFileNameDownloaded, FileMode.Create, FileAccess.Write, FileShare.None))
                    {
                        await ZzzCommon.StreamHelper.CopyStreamAsync(serverStream, clientSteam, new byte[32 * 1024]);
                    }
                }
            }
            catch (FaultException<ExceptionDetail> ex)
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
