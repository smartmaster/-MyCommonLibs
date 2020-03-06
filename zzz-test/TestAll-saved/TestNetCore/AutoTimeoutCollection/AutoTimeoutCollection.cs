using System;
using System.Collections.Generic;
using System.Text;

using System.Threading;

namespace TestNetCore.AutoTimeoutCollection
{
    class AutoTimeoutObject
    {
        public int TimeoutSec;
        public int CurrentTimeoutSec;
        public object Object;
    }

    class AutoTimeoutCollection : IAutoTimeoutCollection, IDisposable
    {
        private readonly Dictionary<string, AutoTimeoutObject> _dict = new Dictionary<string, AutoTimeoutObject>();
        private readonly ReaderWriterLockSlim _lock = new ReaderWriterLockSlim();
        private readonly ManualResetEventSlim _stop = new ManualResetEventSlim();
        private readonly int _scanIntervalSec = 1000 * 60 * 3; //3 min
        private readonly Thread _thread;

        public AutoTimeoutCollection(int scanIntervalSec)
        {
            _scanIntervalSec = scanIntervalSec;
            _thread = new Thread(()=>Scan());
            _thread.Start();
        }

        public string Add(object obj, int timeoutSec)
        {
            AutoTimeoutObject ato = new AutoTimeoutObject
            {
                TimeoutSec = timeoutSec,
                Object = obj
            };
            string id = Guid.NewGuid().ToString();
            _lock.EnterWriteLock();
            try
            {
                _dict.Add(id, ato);
            }
            finally
            {
                _lock.ExitWriteLock();
            }

            return id;
        }

        public void Delete(string id)
        {
            AutoTimeoutObject ato = null;
            _lock.EnterWriteLock();
            try
            {
                _dict.Remove(id, out ato);
            }
            finally
            {
                _lock.ExitWriteLock();
            }

            (ato?.Object as IDisposable)?.Dispose();
        }

        public object Get(string id)
        {
            AutoTimeoutObject ato = null;
            _lock.EnterReadLock();
            try
            {
                bool ok = _dict.TryGetValue(id, out ato);
                if (ok && ato != null)
                {
                    Interlocked.Exchange(ref ato.CurrentTimeoutSec, 0);
                }
            }
            finally
            {
                _lock.ExitReadLock();
            }

            return ato?.Object;
        }


        private void Scan()
        {
            for (; ; )
            {

                bool waitOk = _stop.Wait(_scanIntervalSec * 1000);
                if (waitOk)
                {
                    break;
                }


                Dictionary<string, AutoTimeoutObject> dictLocal = new Dictionary<string, AutoTimeoutObject>();
                _lock.EnterReadLock();
                try
                {
                    foreach (var kv in _dict)
                    {
                        int cto = Interlocked.Add(ref kv.Value.CurrentTimeoutSec, _scanIntervalSec);
                        if (cto >= kv.Value.TimeoutSec) //first test in reader lock
                        {
                            dictLocal.Add(kv.Key, kv.Value);
                        }
                    }
                }
                finally
                {
                    _lock.ExitReadLock();
                }


                List<object> listLocal = new List<object>();
                _lock.EnterWriteLock();
                try
                {
                    foreach(var kv in dictLocal) 
                    {
                        if(kv.Value.CurrentTimeoutSec >= kv.Value.TimeoutSec) //second test in writer lock
                        {
                            listLocal.Add(kv.Value.Object);
                            _dict.Remove(kv.Key);
                        }
                    }
                }
                finally
                {
                    _lock.ExitWriteLock();
                }


                foreach(var obj in listLocal)
                {
                    (obj as IDisposable)?.Dispose();
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
                    _stop.Set();
                    _thread.Join();

                    List<object> listLocal = new List<object>();
                  
                    _lock.EnterWriteLock();
                    try
                    {
                        Dictionary<string, AutoTimeoutObject> dictLocal = new Dictionary<string, AutoTimeoutObject>(_dict);
                        foreach (var kv in dictLocal)
                        {
                            listLocal.Add(kv.Value.Object);
                            _dict.Remove(kv.Key);
                        }
                    }
                    finally
                    {
                        _lock.ExitWriteLock();
                    }


                    foreach(var obj in listLocal)
                    {
                        (obj as IDisposable)?.Dispose();
                    }


                    _lock.Dispose();
                    _stop.Dispose();
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~AutoTimeoutCollection() {
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
