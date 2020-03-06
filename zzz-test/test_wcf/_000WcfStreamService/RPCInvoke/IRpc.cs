using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace _000WcfStreamService.RPCInvoke
{
    [ServiceContract(SessionMode = SessionMode.NotAllowed)]
    public interface IRpc
    {
        [OperationContract]
        Task<RpcData> Invoke(RpcData param);
    }
}
