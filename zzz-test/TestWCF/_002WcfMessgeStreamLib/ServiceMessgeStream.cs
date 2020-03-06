using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace _002WcfMessgeStreamLib
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "Service1" in both code and config file together.
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall, ConcurrencyMode = ConcurrencyMode.Multiple)]
    public class ServiceMessgeStream : IServiceMessgeStream
    {
        [OperationBehavior(TransactionScopeRequired = false)]
        public async Task<Stream> Download(string serverFileName)
        {
            try
            {
                await Task.Yield();
                FileStream fs = new FileStream(serverFileName, FileMode.Open, FileAccess.Read, FileShare.Read);
                return fs;
            }
            catch(FaultException ex)
            {
                throw;
            }
            catch (System.Exception ex)
            {
                throw new FaultException<ExceptionDetail>(new ExceptionDetail(ex), ex.Message);
            }
        }

        [OperationBehavior(TransactionScopeRequired = false)]
        public async Task Upload(UploadFileInfo ufi)
        {
            try
            {
                using (FileStream fs = new FileStream(ufi.ServerFileName, FileMode.Create, FileAccess.Write, FileShare.None))
                {
                    await ZzzCommon.StreamHelper.CopyStreamAsync(ufi.ClientStream, fs, new byte[32 * 1024]);
                }
            }
            catch (FaultException ex)
            {
                throw;
            }
            catch (System.Exception ex)
            {
                throw new FaultException<ExceptionDetail>(new ExceptionDetail(ex), ex.Message);
            }
        }
    }
}
