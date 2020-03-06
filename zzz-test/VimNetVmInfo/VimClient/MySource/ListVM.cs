using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using System.Text;
using System.IO;

using VimLib.VimServiceReference;
using ObjectDumper;

namespace VimClient
{
    public class CListVMs
    {
        private readonly VimPortTypeClient _vimService;
        private readonly ManagedObjectReference _morPropCollector;
        private readonly VimPropCollector _pc;
        private StringBuilder _sb;
        private StringWriter _sw;

        public CListVMs(VimPortTypeClient vimService, ManagedObjectReference morPropCollector)
        {
            _vimService = vimService;
            _morPropCollector = morPropCollector;
            _pc = new VimPropCollector(_vimService, morPropCollector);
            _sb = new StringBuilder();
            _sw = new StringWriter(_sb);
    }


        public void list_all_vms(ServiceContent sc)
        {
            list_folder(sc.rootFolder);
        }

        private void list_host(ManagedObjectReference morHost)
        {
            //vector<VirtualMachine> vms = hs.get_vm();
            var vms = (ManagedObjectReference[])_pc.Get(morHost, "vm");
            foreach (var vm in vms)
            {
                //print infos
                var vmsummary = (VirtualMachineSummary)_pc.Get(vm, "summary");
                var config = vmsummary.config;
                _sb.Append('~', 80);
                _sb.AppendLine();

                //StaticHelpers.PrintMor(sb, item.Datastore);
                config.Dump(nameof(config), _sw, false);

                VmSummaryInfo vmSummaryInfo = new VmSummaryInfo();
                vmSummaryInfo.VmHost.VmxSpec = "moref=" + vm.Value;
                VirtualMachineConfigInfo virtualMachineConfigInfo = (VirtualMachineConfigInfo)_pc.Get(vm, "config");
                StaticHelpers.PrintVirtualDisksBriefInfo(_sb, virtualMachineConfigInfo, vmSummaryInfo.VmHost.CurrentDiskPath);
                VimSnapshotInfo si = new VimSnapshotInfo(_vimService, _morPropCollector, vm, _sb, vmSummaryInfo.Snapshots);
                si.Traverse();
                _sb.Append('~', 80);


                Console.WriteLine("{0}", _sb.ToString());
                _sb.Clear();
            }
        }

        private void list_computerresource(ManagedObjectReference morComputeResource)
        {
            var hosts = (ManagedObjectReference[])_pc.Get(morComputeResource, "host");
            foreach (var host in hosts)
            {
                list_host(host);
            }
        }

        private void list_datastore(ManagedObjectReference mordatastore)
        {
            var dsinfo = (DatastoreInfo)_pc.Get(mordatastore, "info");
            return;
        }

        private void list_datacenter(ManagedObjectReference morDC)
        {
            var dcName = (string)_pc.Get(morDC, "name");
            var hostFolder = (ManagedObjectReference)_pc.Get(morDC, "hostFolder");
            var entities = (ManagedObjectReference[])_pc.Get(hostFolder, "childEntity");
            foreach (var entity in entities)
            {
                var ename = (string)_pc.Get(entity, "name");
                switch (entity.type)
                {
                    case "Folder":
                        list_folder(entity);
                        break;
                    case "ComputeResource":
                        list_computerresource(entity);
                        break;
                    case "Datastore":
                        list_datastore(entity);
                        break;
                    default:
                        break;
                }
            }
        }
        
        private void list_folder(ManagedObjectReference morFoler)
        {
            var Entities = (ManagedObjectReference[])_pc.Get(morFoler, "childEntity");
            foreach (var entity in Entities)
            {
                var ename = (string)_pc.Get(entity, "name");
                switch (entity.type)
                {
                    case "Datacenter":
                        list_datacenter(entity);
                        break;
                    case "Folder":
                        list_folder(entity);
                        break;
                    case "ComputeResource":
                        list_computerresource(entity);
                        break;
                    default:
                        break;
                }
            }
        }

    }
}