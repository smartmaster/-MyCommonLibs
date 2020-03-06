using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Linq;
using System.Threading.Tasks;
using System.Threading;

namespace SmartLib
{

    public interface IAutoTimeoutCollection : IDisposable
    {
        Guid Add(object obj, bool longLived = false, TimeSpan? life = null, Action<object> disposeAction = null);
        T Get<T>(Guid id);
        T Delete<T>(Guid id);
    }
    internal class AutoTimeoutObject : IDisposable
    {
        public Guid Id;
        public object Object;
        public Action<object> DisposeAction;
        public TimeSpan Age;   //in sec
        public TimeSpan Life;  //in sec
        public bool LongLived;

        public void Dispose()
        {
            if (null != Object)
            {
                if (null != DisposeAction)
                {
                    DisposeAction(Object);
                }
                else
                {
                    (Object as IDisposable)?.Dispose();
                }

                Object = null;
            }
        }
    }
    public class AutoTimeoutCollection : IAutoTimeoutCollection
    {
        //private readonly CancellationTokenSource _cts = new CancellationTokenSource();
        private readonly ConcurrentDictionary<Guid, AutoTimeoutObject> _con = new ConcurrentDictionary<Guid, AutoTimeoutObject>();

        private readonly TimeSpan _defaultLife;
        private readonly TimeSpan _scanInterval;

        private ManualResetEvent _stopEvent = new ManualResetEvent(false);
        private readonly Thread _scanThread;

        public AutoTimeoutCollection(TimeSpan? defaultLife = null, TimeSpan? scanInterval = null)
        {
            _defaultLife = defaultLife ?? TimeSpan.FromMinutes(15);
            _scanInterval = scanInterval ?? TimeSpan.FromSeconds(15);
            _scanThread = new Thread(() => Scan());
            _scanThread.Start();
        }

        private void Scan()
        {
            //object state
            // - not expired, hence must be in container
            // - expired, but still in container
            // - expired, and removed from container
            for (;;)
            {
                bool stop =  _stopEvent.WaitOne(_scanInterval);
                if (stop)
                {
                    break;
                }

                List<AutoTimeoutObject> expired = new List<AutoTimeoutObject>();

                foreach (var aot in _con.Values)
                {
                    bool isExpired = false;
                    lock (aot)
                    {
                        aot.Age += _scanInterval;
                        isExpired = !aot.LongLived && (aot.Age >= aot.Life);
                    }

                    if (isExpired)
                    {
                        expired.Add(aot);
                    }
                }

                foreach (var ee in expired)
                {
                    AutoTimeoutObject aot = null;
                    _con.TryRemove(ee.Id, out aot);
                    ee.Dispose();
                }
            }
        }


        public Guid Add(object obj, bool longLived = false, TimeSpan? life = null, Action<object> disposeAction = null)
        {
            AutoTimeoutObject ato = new AutoTimeoutObject
            {
                Id = Guid.NewGuid(),
                Object = obj,
                DisposeAction = disposeAction,
                Age = TimeSpan.FromMilliseconds(0),   //in sec
                Life = life ?? _defaultLife, //in sec
                LongLived = longLived
            };
            _con.TryAdd(ato.Id, ato);
            return ato.Id;
        }

        public T Get<T>(Guid id)
        {
            AutoTimeoutObject res = null;
            AutoTimeoutObject aot = null;
            _con.TryGetValue(id, out aot);

            if (null != aot)
            {
                lock (aot)
                {
                    if (aot.LongLived || aot.Age < aot.Life)
                    {
                        aot.Age = TimeSpan.FromMilliseconds(0);
                        res = aot;
                    }
                }
            }
            return (T)(res?.Object);
        }

        

        public T Delete<T>(Guid id)
        {
            AutoTimeoutObject aot = null;
            AutoTimeoutObject res = null;
            _con.TryRemove(id, out aot);
            if (null != aot)
            {
                lock (aot)
                {
                   if(aot.LongLived || aot.Age < aot.Life)
                    {
                        aot.Age = TimeSpan.FromMilliseconds(0);
                        res = aot;
                    }
                }
            }

            return (T)(res?.Object);
        }

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    _stopEvent.Set();
                    _scanThread.Join((int)(_scanInterval.TotalMilliseconds * 100)); //x3 is enough... x100 is safe and of same effect as x3
                    _stopEvent.Dispose();

                    List<AutoTimeoutObject> expired = new List<AutoTimeoutObject>();

                    foreach (var aot in _con.Values)
                    {
                        expired.Add(aot);
                    }

                    foreach (var ee in expired)
                    {
                        AutoTimeoutObject aot = null;
                        _con.TryRemove(ee.Id, out aot);
                        ee.Dispose();
                    }
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
