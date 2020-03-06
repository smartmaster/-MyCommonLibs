using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace testCS
{
    class RefCountable<T> : IDisposable
        where T : class
    {
        private int _refCount;
        private T _obj;

        public T Obj => _obj;

        public static RefCountable<T>  operator+(RefCountable<T> left, RefCountable<T> right)
        {
            if (null != left)
            {
                left.ReleaseRef();
            }

            left = right;

            if (null != left)
            {
                left.AddRef();
            }

            return left;
        }


        public RefCountable(T obj)
        {
            _refCount = 1;
            _obj = obj;
        }


        int AddRef()
        {
            return Interlocked.Increment(ref _refCount);
        }

        void ReleaseRef()
        {
            if(0 == Interlocked.Decrement(ref _refCount))
            {
                (_obj as IDisposable)?.Dispose();
                _obj = null;
            }
        }
        
        public void Dispose()
        {
            ReleaseRef();
        }
        
    }


    static class RefCountableTest
    {
        public static void Case0()
        {
            RefCountable<SimpleDisposal> p1 = new RefCountable<SimpleDisposal>(new SimpleDisposal());
            RefCountable<SimpleDisposal> p2 = new RefCountable<SimpleDisposal>(new SimpleDisposal());

            p1 += p2;

            RefCountable<SimpleDisposal> p3 = null;
            p3 += p1;

            using (p1)
            {
            }

            using (p2)
            {

            }


            using (p3)
            {

            }

                Console.WriteLine("----------------------------------");
        }
        class SimpleDisposal : IDisposable
        {
            private static int s_index;

            private int _index;

            public SimpleDisposal()
            {
                _index = Interlocked.Increment(ref s_index);
                Console.WriteLine($"Object {_index} created");
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
                        Console.WriteLine($"Object {_index} destoyed");
                    }

                    // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                    // TODO: set large fields to null.

                    disposedValue = true;
                }
            }

            // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
            // ~SimpleDisposal() {
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
}
