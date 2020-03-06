using System;
using System.Threading.Tasks;

namespace TestCs
{
    class Program
    {
        static async Task Main(string[] args)
        {
            int testCase = 0;
            if (args.Length > 0)
            {
                int.TryParse(args[0], out testCase);
            }

            switch (testCase)
            {
                case 5:
                    {
                        TestCs.UriEscapeChar.Case0();
                    }
                    break;
                case 4:
                    {
                        TestCs.MaxSeq.TestCase1();
                        TestCs.MaxSeq.TestCase0();
                    }
                    break;
                case 3:
                    {
                        TestCs.MyEntry.Case0();
                    }
                    break;
                case 2:
                    {
                        TestCs.MyNumTest.Case();
                    }
                    break;
                case 1:
                    {
                        Console.WriteLine($"using async method...");
                        await TestCs.ManualAsync.LoopAsync(10);
                        Console.WriteLine($"using manually composed async method...");
                        await TestCs.ManualAsync.MyLoopAsync(10);
                    }
                    break;
                case 0:
                    {
                        int res = await TestCs.ManualAsync.MyFooAsync(2, 3, 4);
                        Console.WriteLine($"{res}");

                    }
                    break;
            }
        }
    }
}
