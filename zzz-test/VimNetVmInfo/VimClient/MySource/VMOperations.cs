
using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using VimLib.VimServiceReference;


namespace VimClient
{
    interface IVMTask
    {
        //info TaskInfo Detailed information about this task.  
        TaskInfo GetInfo();
        TaskInfoState Wait(int interval, int loopCount, out TaskInfo info);
        TaskInfoState Cancle();
    }
    interface IVMOperation
    {

        //ManagedObjectReference CreateSnapshot_Task(ManagedObjectReference _this, string name, string description, bool memory, bool quiesce);
        TaskInfoState CreateSnapshot(string name, string description, bool memory, bool quiesce, out string snapshotOut);

        VirtualDisk[] QuerySnapshotVirtualDisks(string snapshot);

        //DiskChangeInfo QueryChangedDiskAreas(ManagedObjectReference _this, ManagedObjectReference snapshot, int deviceKey, long startOffset, string changeId);
        DiskChangeInfo QueryChangedDiskAreas(string snapshot,  VirtualDisk disk, long startOffset);

        //ManagedObjectReference RemoveSnapshot_Task(ManagedObjectReference _this, bool removeChildren, bool consolidate);
        TaskInfoState RemoveSnapshot(string snapshot, bool removeChildren, bool consolidate); //method of snapshot mor

        //ManagedObjectReference ConsolidateVMDisks_Task(ManagedObjectReference _this);
        TaskInfoState ConsolidateVMDisks();

        //ManagedObjectReference ReconfigVM_Task(ManagedObjectReference _this, VirtualMachineConfigSpec spec);
        TaskInfoState ReconfigVM(VirtualMachineConfigSpec spec);
    }

    class MyVMTask : IVMTask
    {
        private readonly VimPortTypeClient _vimService;
        private readonly VimPropCollector _pc;
        private readonly ManagedObjectReference _morThis;

        public MyVMTask(
            VimPortTypeClient vimService,
            VimPropCollector PropCollector,
            ManagedObjectReference morThis)
        {
            _vimService = vimService;
            _pc = PropCollector;
            _morThis = morThis;
        }

        public TaskInfo GetInfo()
        {
          return (TaskInfo)_pc.Get(_morThis, "info");
        }

        public TaskInfoState Cancle()
        {
            //void CancelTask(ManagedObjectReference _this);
            _vimService.CancelTask(_morThis);
            TaskInfo info = this.GetInfo();
            return info == null ? info.state : TaskInfoState.error;
        }

        public TaskInfoState Wait(int intervalMilsec, int loopCount, out TaskInfo info)
        {
            info = null;
            bool loopForver = (-1 == loopCount || 0 == loopCount);
            for (int ii = 0; ii < loopCount || loopForver; ++ ii)
            {
                Thread.Sleep(TimeSpan.FromMilliseconds(intervalMilsec));
                info = this.GetInfo();
                if (info.state == TaskInfoState.success || info.state == TaskInfoState.error)
                {
                    break;
                }
            }
            return info != null? info.state : TaskInfoState.error;
        }
    }

    class MyVMOperation : IVMOperation
    {
        private readonly VimPortTypeClient _vimService;
        private readonly VimPropCollector _pc;
        private readonly ManagedObjectReference _morThis;

        public MyVMOperation(
             VimPortTypeClient vimService,
            ManagedObjectReference morPropCollector,
            ManagedObjectReference morThis)
        {
            _vimService = vimService;
            _pc = new VimPropCollector(_vimService, morPropCollector);
            _morThis = morThis;
        }

        public TaskInfoState ConsolidateVMDisks()
        {
            ManagedObjectReference morTask = _vimService.ConsolidateVMDisks_Task(_morThis);
            MyVMTask myTask = new MyVMTask(_vimService, _pc, morTask);
            TaskInfo ti = null;
            return myTask.Wait(3000, -1, out ti);
        }

        public TaskInfoState CreateSnapshot(string name, string description, bool memory, bool quiesce, out string snapshotOut)
        {
            snapshotOut = string.Empty;
            ManagedObjectReference morTask = _vimService.CreateSnapshot_Task(_morThis, name, description, memory, quiesce);
            MyVMTask myTask = new MyVMTask(_vimService, _pc, morTask);
            TaskInfo ti = null;
            TaskInfoState state = myTask.Wait(3000, -1, out ti);
            if(state == TaskInfoState.success)
            {
                ManagedObjectReference morSnapshot = (ManagedObjectReference)(ti.result);
                snapshotOut = morSnapshot?.Value;
            }
            return state;
        }

        //DiskChangeInfo QueryChangedDiskAreas(ManagedObjectReference _this, ManagedObjectReference snapshot, int deviceKey, long startOffset, string changeId);
        public DiskChangeInfo QueryChangedDiskAreas(string snapshot, VirtualDisk disk, long startOffset)
        {
            ManagedObjectReference morSnapshot = new ManagedObjectReference
            {
                @type = "VirtualMachineSnapshot",
                Value = snapshot
            };
            return _vimService.QueryChangedDiskAreas(_morThis, morSnapshot, disk.key, startOffset, (disk.backing as VirtualDiskFlatVer2BackingInfo)?.changeId);
        }

        public VirtualDisk[] QuerySnapshotVirtualDisks(string snapshot)
        {
            ManagedObjectReference morSnapshot = new ManagedObjectReference
            {
                @type = "VirtualMachineSnapshot",
                Value = snapshot
            };
            VirtualMachineConfigInfo vmci = (VirtualMachineConfigInfo)_pc.Get(morSnapshot, "config");
            var devices = vmci?.hardware?.device;
            List<VirtualDisk> virtualDisks = new List<VirtualDisk>();
            if(null != devices)
            {
                foreach(var device in devices)
                {
                    VirtualDisk vdisk = (device as VirtualDisk);
                    if(null != vdisk)
                    {
                        virtualDisks.Add(vdisk);
                    }
                }
            }
            return virtualDisks.ToArray();
        }

        public TaskInfoState ReconfigVM(VirtualMachineConfigSpec spec)
        {
            ManagedObjectReference morTask = _vimService.ReconfigVM_Task(_morThis, spec);
            MyVMTask myTask = new MyVMTask(_vimService, _pc, morTask);
            TaskInfo ti = null;
            TaskInfoState state = myTask.Wait(3000, -1, out ti);
            return state;
        }

        public TaskInfoState RemoveSnapshot(string snapshot, bool removeChildren, bool consolidate)
        {
            ManagedObjectReference morSnapshot = new ManagedObjectReference
            {
                @type = "VirtualMachineSnapshot",
                Value = snapshot
            };
            ManagedObjectReference morTask = _vimService.RemoveSnapshot_Task(morSnapshot, removeChildren, consolidate);
            MyVMTask myTask = new MyVMTask(_vimService, _pc, morTask);
            TaskInfo ti = null;
            TaskInfoState state = myTask.Wait(3000, -1, out ti);
            return state;
        }
    }
}