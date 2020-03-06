


using System;
using System.IO;

namespace TestCsAllInOne
{
    class EnumFileSystem
    {
        private static void EnumWorker(string dir, Action<string> onDir, Action<string> onFile )
        {
            DirectoryInfo di = new DirectoryInfo(dir);
            foreach(var fsi in di.GetFileSystemInfos())
            {
                if(fsi.Attributes.HasFlag(FileAttributes.Directory))
                {
                    onDir(fsi.FullName);
                }
                else
                {
                    onFile(fsi.FullName);
                }
            }
        }

        private long totalDirCount = 0;
        private long totalFileCount = 0;

        public void EnumProc(string dir)
        {
            long dirCount = 0;
            long fileCount = 0;

            Action<string> onDir = (string subDir) =>
            {
                Console.WriteLine($"[D]{subDir}");
                if(dir != "." && dir != "..")
                {
                    ++totalDirCount;
                    ++dirCount;
                    try
                    {
                        EnumProc(subDir);
                    }
                    catch(Exception ex)
                    {
                        Console.WriteLine($"Cannot enum {subDir} with error {ex.Message}");
                    }
                    
                }
            };

            Action<string> onFile = (string fileName) =>
            {
                ++totalFileCount;
                ++fileCount;
                Console.WriteLine($"[F]{fileName}");
            };

            EnumWorker(dir, onDir, onFile);

            Console.WriteLine($"{nameof(dirCount)}={dirCount}, {nameof(fileCount)}={fileCount}");
            Console.WriteLine($"{nameof(totalDirCount)}={totalDirCount}, {nameof(totalFileCount)}={totalFileCount}");
        }


        public static void Enum(string dir)
        {
            EnumFileSystem efs = new EnumFileSystem();
            efs.EnumProc(dir);
        }
    }
}