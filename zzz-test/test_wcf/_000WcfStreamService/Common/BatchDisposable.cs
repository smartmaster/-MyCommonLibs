using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _000WcfStreamService.Common
{
    public class BatchDisposable : IDisposable
    {
        private List<Action> m_disposes = new List<Action>();

        public int Add(Action action)
        {
            m_disposes.Add(action);
            return m_disposes.Count - 1;
        }

        public void Remove(int index)
        {
            m_disposes[index] = null;
        }


        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects).
                    for (int ii = m_disposes.Count - 1; ii >= 0; -- ii)
                    {
                        m_disposes[ii]?.Invoke();
                        m_disposes[ii] = null;
                    }
                    m_disposes.Clear();
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~BatchDisposable() {
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
