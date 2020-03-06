using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace TestNetCore
{
    class TestRWLock : IDisposable
    {
        private readonly Random _random = new Random();
        private readonly ReaderWriterLockSlim _rwlock = new ReaderWriterLockSlim();
        private readonly ManualResetEventSlim _event = new ManualResetEventSlim();

        const int _threadCount = 5;
        private readonly CountdownEvent _count = new CountdownEvent(_threadCount);

        private void ThreadProc()
        {
            Console.WriteLine($"{Thread.CurrentThread.ManagedThreadId} created");
            while (!_event.Wait(0))
            {
                _rwlock.EnterUpgradeableReadLock();
                
                int sleep = _random.Next(1, 5) * 1000;
                Console.Write($"<---Thread {Thread.CurrentThread.ManagedThreadId} ");
                Thread.Sleep(sleep);
                Console.WriteLine($"slept for {sleep} ms--->");
                _rwlock.ExitUpgradeableReadLock();
            }

            Console.WriteLine($"{Thread.CurrentThread.ManagedThreadId} exited");
            _count.Signal();
        }


        public void Test()
        {
            for(int ii = 0; ii < _threadCount; ++ ii)
            {
                new Thread(() => ThreadProc()).Start();
            }

            Thread.Sleep(30 * 1000);
            _event.Set();
            _count.Wait();
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
                    _count?.Dispose();
                    _event?.Dispose();
                    _rwlock?.Dispose();
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~TestRWLock() {
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



        #region test cases
        public static void Case0()
        {
            using (TestRWLock test = new TestRWLock())
            {
                test.Test();
            }
        }
        #endregion //test cases

    }
}
