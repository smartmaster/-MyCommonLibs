using System;
using System.Collections.Generic;
using System.Text;

namespace TestNetCore.AutoTimeoutCollection
{
    interface IAutoTimeoutCollection
    {
        string Add(object obj, int timeoutSec);
        object Get(string id);
        void Delete(string id);
    }

}
