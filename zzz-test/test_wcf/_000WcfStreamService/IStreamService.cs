using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace _000WcfStreamService
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IService1" in both code and config file together.
    [ServiceContract]
    public interface IStreamService
    {
        [OperationContract]
        Task Upload(UploadStreamInfo usi);
        [OperationContract]
        Task<DownloadSteamInfo> Download(DownloadFileInfo dfi);
    }
}
