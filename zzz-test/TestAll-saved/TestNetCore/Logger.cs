using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Text;

namespace TestNetCore
{
   static class Logger
    {
        public static void Log(string message, 
            [CallerMemberName] string function = "",
            [CallerFilePath] string filePath = "",
            [CallerLineNumber] int line = 0
            )
        {
            Console.WriteLine($"{message}   ^^^[{function}, {filePath}, {line}]$$$");
        }


        public static void Log2(string message)
        {
            StackFrame frame = new StackFrame(1, true);
            var method = frame.GetMethod();
            var rtype = method.ReflectedType;
            Console.WriteLine($"{message}   ^^^[{method.Module.Name}!{rtype.Namespace}.{rtype.Name}::{method.Name}, {frame.GetFileName()}, {frame.GetFileLineNumber()}]$$$");
        }

    }
}
