using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace _000WcfStreamedLib
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "Service1" in both code and config file together.
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall, ConcurrencyMode = ConcurrencyMode.Multiple)]
    public class ServiceStream : IServiceStream
    {
        public async Task<Stream> Invoke(Stream clientStream)
        {
            try
            {
                const string FILE_NAME = @"e:\2.png";
                FileStream fs = new FileStream(FILE_NAME, FileMode.Create, FileAccess.ReadWrite, FileShare.None);
                //await clientStream.CopyToAsync(fs);
                await ZzzCommon.StreamHelper.CopyStreamAsync(clientStream, fs, new byte[64*1024]);
                await fs.FlushAsync();
                //fs.Position = 0;
                fs.Seek(0, SeekOrigin.Begin);
                return fs;
            }
            catch (FaultException ex)
            {
                throw;
            }
            catch(Exception ex)
            {
                throw new FaultException<ExceptionDetail>(new ExceptionDetail(ex), ex.Message);
            }
        }
    }
}
