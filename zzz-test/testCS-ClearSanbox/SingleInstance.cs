using System;
using System.Collections.Generic;
using System.Text;

namespace testCS
{
    static class SingleInstance<T>
        where T : class, new()
    {
        private static T _instance;

        public static T Instance()
        {
            if(null == _instance)
            {
                lock(typeof(SingleInstance<T>))
                {
                    if(null == _instance)
                    {
                        _instance = new T();
                    }
                }
            }

            return _instance;
        }
    }
}
