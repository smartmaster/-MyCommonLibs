using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;

namespace SmartLib
{
    public class SessionObjectManager
    {
        private readonly ISession _session;
        private readonly IAutoTimeoutCollection _coll;
        public SessionObjectManager(ISession session, IAutoTimeoutCollection coll)
        {
            _session = session;
            _coll = coll;
        }

        public T Add<T>(string key, T obj, bool longLived = false, TimeSpan? life = null, Action<object> disposeAction = null)
        {
            lock(_coll)
            {
                Guid guid = _coll.Add(obj, longLived, life, disposeAction);
                _session.SetString(key, guid.ToString());

                return obj;
            }
        }

        public T Get<T>(string key)
        {
            lock(_coll)
            {
                T obj = default(T);
                string guidStr = _session.GetString(key);
                if (!string.IsNullOrEmpty(guidStr))
                {
                    Guid guid = Guid.Parse(guidStr);
                    obj = _coll.Get<T>(guid);
                }
                return obj;
            }
           
        }


        public T GetOrAdd<T>(string key, T obj, bool longLived = false, TimeSpan? life = null, Action<object> disposeAction = null)
        {
            lock(_coll)
            {
                T rr = default(T);

                string guidStr = _session.GetString(key);
                if (string.IsNullOrEmpty(guidStr))
                {
                    rr = Add<T>(key, obj, longLived, life, disposeAction);
                }
                else
                {
                    rr = Get<T>(key);
                }
              
                return rr;
            }
        }


        public T Delete<T>(string key)
        {
            lock(_coll)
            {
                T obj = default(T);
                string guidStr = _session.GetString(key);
                if (!string.IsNullOrEmpty(guidStr))
                {
                    Guid guid = Guid.Parse(guidStr);
                    obj = _coll.Delete<T>(guid);
                    _session.Remove(key);
                }

                return obj;
            }
          
        }
    }
}
