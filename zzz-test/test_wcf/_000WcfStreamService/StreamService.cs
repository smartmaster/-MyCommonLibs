using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace _000WcfStreamService
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "Service1" in both code and config file together.
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall, ConcurrencyMode = ConcurrencyMode.Multiple)]
    public class StreamService : IStreamService
    {
        public async Task<DownloadSteamInfo> Download(DownloadFileInfo dfi)
        {
            try
            {
                await Task.Yield();
                var fs = new FileStream(dfi.ServerFileName, FileMode.Open, FileAccess.Read, FileShare.Read);
                DownloadSteamInfo dsi = new DownloadSteamInfo()
                {
                    ServerStream = fs
                };
                return dsi;
            }
            catch(FaultException fex)
            {
                Common.CommonHelpers.PrintException(fex, Console.Out, true);
                throw;
            }
            catch (System.Exception ex)
            {
                Common.CommonHelpers.PrintException(ex, Console.Out, true);
                throw new FaultException<Exception>(ex);
            }
        }

        public async Task Upload(UploadStreamInfo usi)
        {
            try
            {
                byte[] buffer = new byte[512 * 1024];
                using (var fs = new FileStream(usi.ServerFileName, FileMode.Create, FileAccess.Write, FileShare.None))
                {
                    await Common.CommonHelpers.StreamCopyAsync(usi.ClientStream, fs, buffer);
                }
            }
            catch (FaultException fex)
            {
                Common.CommonHelpers.PrintException(fex, Console.Out, true);
                throw;
            }
            catch (System.Exception ex)
            {
                Common.CommonHelpers.PrintException(ex, Console.Out, true);
                throw new FaultException<Exception>(ex);
            }
        }
    }
}
