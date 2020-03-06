using ProcessPrivileges;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;

namespace testCS
{
    public static class EnumDir
    {
        const string _SandBox = "\\Sandbox";

        public static bool Pred(string parent)
        {
            try
            {
                int count = 0;
                string dirName = "";
                foreach (var fi in Directory.EnumerateFileSystemEntries(parent, "*", SearchOption.TopDirectoryOnly))
                {
                    ++count;
                    if (1 == count)
                    {
                        dirName = fi;
                    }
                    else
                    {
                        break;
                    }
                }

                return count == 1 && dirName.EndsWith(_SandBox, StringComparison.InvariantCultureIgnoreCase) && Directory.Exists(dirName);
            }
            catch(Exception ex)
            {
                Console.WriteLine($"Cannot enumerate {parent}, {ex.Message}");
                return false;
            }
        }


        public static void DeleteSandbox(string path)
        {
            try
            {
                ModifyFileSec.FileSystemSec.ChangeOwnerAndGrantAccess(path);
                Console.WriteLine($"Deleting {path}");
                Directory.Delete(path +  _SandBox);
                Directory.Delete(path);
            }
            catch(Exception ex)
            {
                Console.WriteLine($"Failed to delete directory or sub directory, {ex.Message}");
            }
        }


        public static void ClearSanbox(string path)
        {
            using (new PrivilegeEnabler(Process.GetCurrentProcess(), Privilege.TrustedComputerBase, Privilege.TakeOwnership))
            {
                Run(path, Pred, DeleteSandbox);
            }
                
        }


        public static void Run(string path, Func<string, bool> pred, Action<string> act)
        {
            foreach(string subDir in Directory.EnumerateDirectories(path, "*", SearchOption.TopDirectoryOnly))
            {
                if(pred(subDir))
                {
                    act(subDir);
                }
            }
        }
    }


    public static class EnumDirTest
    {
        public static void Case0()
        {
            Func<string, bool> pred = (dir => true);
            Action<string> act = dir => Console.WriteLine($"{dir}");
            EnumDir.Run(@"c:\", pred, act);
        }
    }
}
