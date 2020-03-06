using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
   static class TestTaskContinueWith
    {

        public static void Case0(string[] args)
        {
            int upperBound = args.Length >= 1 ? Int32.Parse(args[0]) : 200;


            Func<bool[]> ComputePrimes = () =>
            { // True = composite.
              // False = prime.
                bool[] values = new bool[upperBound + 1];
                for (int ctr = 2; ctr <= (int)Math.Sqrt(upperBound); ctr++)
                {
                    Thread.Sleep(1_000);
                    if (values[ctr] == false)
                    {
                        for (int product = ctr * ctr; product <= upperBound;
                                                      product = product + ctr)
                            values[product] = true;
                    }
                }
                return values;
            };

            Func<Task<bool[]>, int> printResult = (Task<bool[]>  antecedent) =>
            { // Create a list of prime numbers.
                var primes = new List<int>();
                bool[] numbers = antecedent.Result;
                string output = String.Empty;

                for (int ctr = 1; ctr <= numbers.GetUpperBound(0); ctr++)
                    if (numbers[ctr] == false)
                        primes.Add(ctr);

                // Create the output string.
                for (int ctr = 0; ctr < primes.Count; ctr++)
                {
                    output += primes[ctr].ToString("N0");
                    if (ctr < primes.Count - 1)
                        output += ",  ";
                    if ((ctr + 1) % 8 == 0)
                        output += Environment.NewLine;
                }
                //Display the result.
                Console.WriteLine("Prime numbers from 1 to {0}:\n",
                                  upperBound);
                Console.WriteLine(output);
                return primes.Count;
            };

            var t1 = Task.Run(ComputePrimes);


            var t2 = t1.ContinueWith(printResult);

            try
            {
                int count = t2.Result;
                Console.WriteLine($"the count of prime number is {count}");
                //t2.Wait();
            }
            catch (AggregateException ae)
            {
                foreach (var e in ae.InnerExceptions)
                    Console.WriteLine("{0}: {1}", e.GetType().Name, e.Message);
            }
        }
    }

}

