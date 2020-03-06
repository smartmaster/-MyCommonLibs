using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace WcfTransportSecurityCommon
{
    public class AutoTimeoutObject<T> where T : class
    {
        public T Object { get; set; }
        public long MaxAgeMillSec { get; set; }
        public long CurrentAgeMillSec { get; set; }
        public long LastAccessTime { get; set; }

    }


    public interface IWcfObjectRepository<T>
        where T : class
    {
        T Get(string id);
        string Add(T obj, long timeToliveMS);

        void Update(string id, T newObj);

        void Delete(string id);

    }
    public class WcfObjectRepository<T> : IWcfObjectRepository<T>, IDisposable
        where T : class
    {
        private Dictionary<string, AutoTimeoutObject<T>> _dict = new Dictionary<string, AutoTimeoutObject<T>>();
        private long _scanIntervalMillSec = (long)TimeSpan.FromMinutes(30).TotalMilliseconds + 1;
        private ManualResetEventSlim _event = new ManualResetEventSlim();
        private Thread _thread;

        private void ScanProc()
        {
            
            for (; ; )
            {
                List<AutoTimeoutObject<T>> localObjs = new List<AutoTimeoutObject<T>>();
                List<string> localIds = new List<string>();

                bool stopped = _event.Wait((int)_scanIntervalMillSec);

                lock (_dict)
                {
                    foreach (var kv in _dict)
                    {
                        kv.Value.CurrentAgeMillSec += _scanIntervalMillSec;
                        if (stopped || kv.Value.CurrentAgeMillSec > kv.Value.MaxAgeMillSec)
                        {
                            localIds.Add(kv.Key);
                            localObjs.Add(kv.Value);
                        }
                    }

                    foreach (string id in localIds)
                    {
                        _dict.Remove(id);
                    }
                }

                foreach(var ato in localObjs)
                {
                    if(ato?.Object != null)
                    {
                        lock(ato.Object)
                        {
                            (ato.Object as IDisposable)?.Dispose();
                        }
                    }
                }

                if(stopped)
                {
                    break;
                }
            }
        }

        public WcfObjectRepository(long scanIntervalMillSec)
        {
            _scanIntervalMillSec = scanIntervalMillSec;
            _thread = new Thread(ScanProc);
            _thread.Start();
        }
        public string Add(T obj, long maxAgeMillSec)
        {
            string id = Guid.NewGuid().ToString();
            AutoTimeoutObject<T> ato = new AutoTimeoutObject<T>()
            {
                Object = obj,
                MaxAgeMillSec = maxAgeMillSec,
                CurrentAgeMillSec = 0,
                LastAccessTime = DateTime.UtcNow.ToFileTimeUtc()
            };

            lock (_dict)
            {
                _dict.Add(id, ato);
            }

            return id;
        }

        public void Delete(string id)
        {
            AutoTimeoutObject<T> ato = null;
            lock (_dict)
            {
                _dict.TryGetValue(id, out ato);
                _dict.Remove(id);
            }

            if (ato?.Object != null)
            {
                lock (ato.Object)
                {
                    (ato.Object as IDisposable)?.Dispose();
                }
            }

        }

        public T Get(string id)
        {
            AutoTimeoutObject<T> ato = null;
            long lastAccessedTime = DateTime.UtcNow.ToFileTimeUtc();

            lock (_dict)
            {
                _dict.TryGetValue(id, out ato);
                if (ato != null)
                {
                    ato.CurrentAgeMillSec = 0;
                    ato.LastAccessTime = lastAccessedTime;
                }
            }

            return ato?.Object;
        }

        public void Update(string id, T newObj)
        {
            AutoTimeoutObject<T> ato = null;
            long lastAccessedTime = DateTime.UtcNow.ToFileTimeUtc();

            lock (_dict)
            {
                _dict.TryGetValue(id, out ato);
                if (ato != null)
                {
                    ato.Object = newObj;
                    ato.CurrentAgeMillSec = 0;
                    ato.LastAccessTime = lastAccessedTime;
                }
            }
        }

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    _event.Set();
                    _thread.Join();
                    _event.Dispose();
                }
                
                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~WcfObjectRepository() {
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
