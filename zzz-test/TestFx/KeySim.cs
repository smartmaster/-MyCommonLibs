using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace TestFx
{
    class KeySim
    {

        public static void InputKey()
        {
            Random ran = new Random();
            string[] keys = { "{UP}", "{DOWN}", "{LEFT}", "{RIGHT}" };

            for (; ; )
            {
                SendKeys.SendWait(keys[ran.Next() % 4]);
                Thread.Sleep(3000);
            }

        }
    }
}
