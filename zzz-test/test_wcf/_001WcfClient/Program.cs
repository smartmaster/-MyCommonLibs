using _000WcfStreamService.Common;
using _001WcfClient.WcfRpcServiceReference;
using _001WcfClient.WcfStreamedServiceReference;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _001WcfClient
{
    class Program
    {


        static async Task Test000Client(string[] args)
        {
            try
            {
                using (var bd = new BatchDisposable())
                {
                    var client = new StreamServiceClient("StreamedNetTcpEndpoint", "net.tcp://localhost:8734/_000WcfStreamService.StreamService");
                    bd.Add(() => (client as IDisposable)?.Dispose());

                    const string serverFileName = @"e:\1s.rar";
                    DownloadSteamInfo dsi = await client.DownloadAsync(serverFileName, new DownloadFileInfo());
                    bd.Add(() => dsi?.ServerStream?.Dispose());

                    const string clientFileName = @"e:\2c.rar";
                    var clientStream = new FileStream(clientFileName, FileMode.Create, FileAccess.ReadWrite, FileShare.None);
                    bd.Add(() => clientStream?.Dispose());


                    byte[] buffer = new byte[1024 * 1024];
                    await CommonHelpers.StreamCopyAsync(dsi.ServerStream, clientStream, buffer);

                    clientStream.Seek(0L, SeekOrigin.Begin);
                    const string serverFileNameUpload = @"e:\3s.rar";
                    await client.UploadAsync(serverFileNameUpload, clientStream);

                }
            }
            catch (System.Exception ex)
            {
                CommonHelpers.PrintException(ex, Console.Out, true);
            }
            
        }


        static async Task Test001Client(string[] args)
        {
            try
            {
                using (var bd = new BatchDisposable())
                {
                    var client = new RpcClient("StreamedNetTcpRpcEndpoint", "net.tcp://localhost:8834/_000WcfStreamService.RPCInvoke.Rpc");
                    bd.Add(() => (client as IDisposable)?.Dispose());

                    RpcData param = new RpcData()
                    {
                        Data1 = "client",
                        StreamData = new MemoryStream()
                    };
                    bd.Add(() => param.StreamData?.Dispose());

                    RpcData ret = await client.InvokeAsync(param.Data1, param.Data2, param.DataLen, param.DataOffset, param.EnumId, param.Flags, param.StreamData);
                    bd.Add(() => ret.StreamData?.Dispose());

                    Console.WriteLine();
                }
            }
            catch (System.Exception ex)
            {
                CommonHelpers.PrintException(ex, Console.Out, true);
            }

        }

        static void Main(string[] args)
        {
            int testCase = 0;
            int.TryParse(args[0], out testCase);
            switch(testCase)
            {
                case 1:
                    {
                        var ttt = Test001Client(args);
                        ttt.Wait();
                    }
                    break;
                case 0:
                    {
                        var ttt = Test000Client(args);
                        ttt.Wait();
                    }
                    break;
                default:
                    throw new InvalidOperationException($"Invalid test case {testCase}");
            }
            
        }
    }
}
