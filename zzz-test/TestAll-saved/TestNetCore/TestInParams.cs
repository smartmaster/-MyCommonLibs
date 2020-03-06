using System;
using System.Collections.Generic;
using System.Text;



namespace TestNetCore
{
    class SomeData
    {
        public int Value { get; set; }
        public string Descripion { get; set; }

        void ConstMethod()
        {
            Console.WriteLine($"{Value}, {Descripion}");
        }


        void MutableMethod()
        {
            ++Value;
            Descripion = Descripion.ToUpper().ToLower();
            Console.WriteLine($"{Value}, {Descripion}");
        }
    }
    class TestInParams
    {
        public static void Call(ref SomeData sd)
        {
            sd = new SomeData()
            {
                Value = 100,
                Descripion = "MY NAME IS MIKE"
            };

        }
    }
}
