using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace TestCsAllInOne
{
    
    unsafe class TestCsAndDll
    {
        [DllImport("NativeDll.dll")]
        //void DllFunc(int iCommand, const void* iBuffer, int iSize, int* oCommand, void* oBuffer, int oSize);
        public static extern void DllFunc(int iCommand, void* iBuffer, int iSize, out int oCommand, void* oBuffer, int oSize);


        public static void Case0()
        {
            //int loopCount = 300;
            //for(int ii = 0; ii < loopCount; ++ ii)
            //{
            //    Thread.Sleep(1000);
            //}
            IntPtr pBackup = IntPtr.Zero;
            
            DllFunc(0, (void*)222, 333, out int oCommand, (void*)(&pBackup), IntPtr.Size);

            object obackup = Marshal.GetObjectForIUnknown(pBackup);
            Type backupType = obackup.GetType();
            foreach(var method in backupType.GetMethods())
            {
                Console.WriteLine($"{method.Name}");
            }

            Guid guid = new Guid("665c1d5f-c218-414d-a05d-7fef5f9d5c86");
            Marshal.QueryInterface(pBackup, ref guid, out IntPtr vssbackup);
            object ovssbackup = Marshal.GetObjectForIUnknown(vssbackup);
            Type VssBackupType = ovssbackup.GetType();
            foreach (var method in VssBackupType.GetMethods())
            {
                Console.WriteLine($"{method.Name}");
            }

        }
    }
}
