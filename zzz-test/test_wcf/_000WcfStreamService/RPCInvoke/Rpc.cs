using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace _000WcfStreamService.RPCInvoke
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall, ConcurrencyMode = ConcurrencyMode.Multiple)]
    public class Rpc : IRpc
    {
        [OperationBehavior]
        public async Task<RpcData> Invoke(RpcData param)
        {
            try
            {
                await Task.Yield();
                RpcData ret = new RpcData()
                {
                    Data1 = "server",
                    StreamData = new MemoryStream()
            };
                
                await Common.CommonHelpers.StreamCopyAsync(param.StreamData, ret.StreamData, new byte[128 * 1024]);
                return ret;
            }
            catch(FaultException ex)
            {
                Common.CommonHelpers.PrintException(ex, Console.Out, true);
                throw;
            }
            catch(Exception ex)
            {
                Common.CommonHelpers.PrintException(ex, Console.Out, true);
                throw new FaultException<Exception>(ex);
            }
        }
    }
}
