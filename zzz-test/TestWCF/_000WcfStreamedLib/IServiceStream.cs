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
    [ServiceContract]
    public interface IServiceStream
    {
        [OperationContract]
        Task<Stream> Invoke(Stream clientStream);
    }
}
