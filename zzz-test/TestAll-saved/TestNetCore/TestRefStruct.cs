using System;
using System.Collections.Generic;
using System.Text;

namespace TestNetCore
{
    struct MySss
    {
        public int II;
        public string SS;

        public void Change(int kk)
        {
            II = kk;
            SS = kk.ToString();
        }
    }
    static class TestRefStruct
    {
        public static void RefIn(ref MySss sss)
        {
            //sss.II = 999;
            //sss.SS = "good morning";
            Console.WriteLine($"{sss.II}, {sss.SS}");

            ref /*readonly*/ MySss sss1 = ref sss;
            sss.Change(1);
            Console.WriteLine($"{sss.II}, {sss.SS}");
            sss1.Change(2);
            Console.WriteLine($"{sss.II}, {sss.SS}");
        }


        public static void Case0()
        {
            MySss sss = new MySss
            {
                II = -1,
                SS = "-1"
            };
            RefIn(ref sss);
        }
    }
}
