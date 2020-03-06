using System;
using System.Collections.Generic;
using System.Text;

namespace TestNetCore
{
    class TestEnum
    {
        static bool IsFlagDefined(Enum e)
        {
            decimal d;
            return !decimal.TryParse(e.ToString(), out d);
        }
        [Flags]
        public enum BorderSides { Left = 1, Right = 2, Top = 4, Bottom = 8 }
       public static void Case0()
        {
            for (int i = 0; i <= 32; i++)
            {
                BorderSides side = (BorderSides)i;
                Console.WriteLine(IsFlagDefined(side) + " [" + side + "], " + side.ToString());
            }
        }
    }
}
