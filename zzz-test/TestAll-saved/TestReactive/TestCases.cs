using System;
using System.Collections.Generic;
using System.Reactive.Linq;
using System.Text;

namespace TestReactive
{
  public static  class TestCases
    {
        public static void Case0()
        {
            IObservable<int> source = Observable.Range(1, 20);
            IDisposable subcription = source.Subscribe(
                x => Console.WriteLine($"on next value {x}"),
                ex => Console.WriteLine($"on exception {ex}"),
                () => Console.WriteLine("On completed"));

            Console.WriteLine("Press ENTER to continue...");

            Console.WriteLine();

            subcription.Dispose();
        }

    }
}
