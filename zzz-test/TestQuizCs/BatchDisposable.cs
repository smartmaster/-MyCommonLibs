using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace TestQuiz
{
    enum DisposeType
    {
        Unknown,
        Disposable,
        Action
    }
    public class BatchDisposable : IDisposable
    {
        private readonly List<DisposeType> m_type = new List<DisposeType>();
        private readonly List<object> m_dispose = new List<object>();

        public int Add(IDisposable disposable)
        {
            m_type.Add(DisposeType.Disposable);
            m_dispose.Add(disposable);

            return m_type.Count - 1;
        }


        public int Add(Action act)
        {
            m_type.Add(DisposeType.Action);
            m_dispose.Add(act);
            return m_type.Count - 1;
        }


        public void Remove(int index)
        {
            if (index >= 0 && index < m_type.Count)
            {
                m_type[index] = DisposeType.Unknown;
                m_dispose[index] = null;
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
                    for(int ii = m_type.Count - 1; ii >= 0; -- ii)
                    {
                        if (m_dispose[ii] != null)
                        {
                            switch(m_type[ii])
                            {
                                case DisposeType.Disposable:
                                    {
                                        (m_dispose[ii] as IDisposable).Dispose();
                                    }
                                    break;
                                case DisposeType.Action:
                                    {
                                        (m_dispose[ii] as Action).Invoke();
                                    }
                                    break;
                            }


                            m_type[ii] = DisposeType.Unknown;
                            m_dispose[ii] = null;
                        }
                    }
                }
                
                disposedValue = true;
            }
        }
        public void Dispose()
        {
            Dispose(true);
        }
        #endregion
    }


    public static class BatchDisposableTest
    {
        public static void Case0()
        {
            using (BatchDisposable bd = new BatchDisposable())
            {
                for (int ii = 0; ii < 10; ++ ii)
                {
                    int temp = ii;
                    bd.Add(() => Console.WriteLine($"{temp}"));
                    bd.Add(new Something($"{(char)('A' + ii)}"));

                    bd.Add(null as IDisposable);
                    bd.Add(null as Action);
                }

                bd.Remove(0);
                bd.Remove(1);
            }
        }
    }
}
