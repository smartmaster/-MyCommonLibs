

using System.Collections.Generic;

namespace VimClient
{
   public class VmHostInfo
    {
        public string VmxSpec { get; set; }
        public string ServerName { get; set; }
        public string UserName { get; set; }
        public string Password { get; set; }
        public int Port { get; set; }
        public string ThumbPrint { get; set; }

        public IList<string> CurrentDiskPath { get; set; }

        public VmHostInfo()
        {
            CurrentDiskPath = new List<string>();
        }
    }

    public class SnapshotInfo
    {
        public string Name { get; set; }
        public string Desc { get; set; }
        public  string SnapshotRef { get; set; }
        public IList<string> DiskPath { get; set; }

        public SnapshotInfo()
        {
            DiskPath = new List<string>();
        }
    }

    public class VmSummaryInfo
    {
        public VmHostInfo VmHost { get; set; }
        public IList<SnapshotInfo> Snapshots { get; set; }

        public VmSummaryInfo()
        {
            VmHost = new VmHostInfo();
            Snapshots = new List<SnapshotInfo>();
        }
    }

}