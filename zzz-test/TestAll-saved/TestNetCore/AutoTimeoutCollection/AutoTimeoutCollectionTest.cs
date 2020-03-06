using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace TestNetCore.AutoTimeoutCollection
{

    class SomeObject : IDisposable
    {
        static int s_index = 0;
        private int _index;
     public   SomeObject()
        {
            _index = Interlocked.Increment(ref s_index);
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
                    Console.WriteLine($"object {_index} is being disposed");
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~SomeObject() {
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
  static  class AutoTimeoutCollectionTest
    {
        public static void Case0(int TIME_OUT = 10)
        {

            

            List<string> ids = new List<string>();
            using (var atc = new AutoTimeoutCollection(1))
            {
                for(int ii = 0; ii < 10; ++ ii)
                {
                    var id = atc.Add(new SomeObject(), TIME_OUT);
                    ids.Add(id);
                }

                Thread.Sleep(1000 * 3);

                foreach(var id in ids)
                {
                    var obj = (atc.Get(id) as SomeObject);
                }


                Thread.Sleep(1000 * 3);
                int count = ids.Count / 2;
                int cur = 0;
                foreach(var id in ids)
                {
                    atc.Delete(id);
                    ++cur;
                    if(cur >= count)
                    {
                        break;
                    }
                }



                Thread.Sleep(TIME_OUT * 1000 + 3 * 1000);
                Console.WriteLine();
                Console.WriteLine();
                Console.WriteLine();

            }
        }


        public static void Case1()
        {
            for(int to = 0; to <= 60; to += 3)
            {
                Case0(to);
            }
        }
    }
}
