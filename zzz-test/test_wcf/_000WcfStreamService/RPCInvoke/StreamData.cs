using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace _000WcfStreamService.RPCInvoke
{
    [MessageContract(IsWrapped = false)]
   public class RpcData : IDisposable
    {
        [MessageHeader]
        public long EnumId { get; set; }
        [MessageHeader]
        public long Flags { get; set; }
        [MessageHeader]
        public string Data1 { get; set; }
        [MessageHeader]
        public string Data2 { get; set; }
        [MessageHeader]
        public long DataOffset { get; set; }
        [MessageHeader]
        public long DataLen { get; set; }
        [MessageBodyMember]
        public Stream StreamData { get; set; }

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    StreamData?.Dispose();
                    // TODO: dispose managed state (managed objects).
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~RpcData() {
        //   // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
        //   Dispose(false);
        // }

        // This code added to correctly implement the disposable pattern.
        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
            Dispose(true);
            // TODO: uncomment the following line if the finalizer is overridden above.
            // GC.SuppressFinalize(this);
        }
        #endregion
    }
}
