using System;



using System.IO;
using System.Security.AccessControl;
using System.Security.Principal;

namespace ModifyFileSec
{
    class Program
    {
        static void Main(string[] args)
        {
            if(args.Length !=  3)
            {
                Console.WriteLine("usage: this.exe <file or dir> <owner name> <user name>");
                Console.WriteLine(@"Example: this.exe c:\ administrators john");
                Console.WriteLine(@"Example: this.exe c:\my.txt administrators john");
                return;
            }
            ModifyFileSec.FileSystemSec.API(args[0], args[1], args[2], FileSystemRights.FullControl, AccessControlType.Allow);
        }
    }
}
