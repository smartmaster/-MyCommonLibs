using System;
using System.Collections.Generic;
using System.Text;

namespace TestCsAllInOne
{
    class TestUrlEncoding
    {
        //const string orginal = @"AAA~!@#$%^&*()_+|}{"":?><,./;'\][=-` ZZZ";
        const string orginal = @"""";

        public static void Case0()
        {
            string encoded = System.Net.WebUtility.UrlEncode(orginal);
            string decoded = System.Net.WebUtility.UrlDecode(encoded);
            Console.WriteLine($"orginal: {orginal}");
            Console.WriteLine($"encoded: {encoded}");
            Console.WriteLine($"decoded: {decoded}");
            if (string.CompareOrdinal(orginal, decoded) == 0)
            {
                Console.WriteLine("Expected");
            }
            else
            {
                Console.WriteLine("Unexpected");
            }
        }
    }
}
