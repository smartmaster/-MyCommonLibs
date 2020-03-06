using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_sc_azure_sdk
{
    class Program
    {
        static void Main(string[] args)
        {
            int testCase = int.Parse(args[0]);
            _001AzureResourceTest.TestCases(testCase);
        }
    }
}
