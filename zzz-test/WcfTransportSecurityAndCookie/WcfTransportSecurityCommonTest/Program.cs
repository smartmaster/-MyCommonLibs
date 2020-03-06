using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WcfTransportSecurityCommonTest
{
    class Program
    {
        static void Main(string[] args)
        {
            int testCase = 1;
            switch (testCase)
            {
                case 1:
                    {
                        WcfTransportSecurityCommon.ObjectViewerTest.Case0();
                    }
                    break;

                case 0:
                    {
                        WcfTransportSecurityCommon.TypeViewer.TypeViewerTest.Case0();
                    }
                    break;
            }
        }
    }
}
