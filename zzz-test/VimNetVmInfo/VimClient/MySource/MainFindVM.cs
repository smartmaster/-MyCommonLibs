using System;
using System.Net;
using System.Linq;
using System.Text;
using System.Collections.Generic;
using System.ServiceModel;
using VimLib.VimServiceReference;

using ObjectDumper;
using System.IO;

namespace VimClient
{
    public class MainSearchVM_In
    {
        public string Protocal { get; set; } //https or http
        public string ServerName { get; set; }
        public int Port { get; set; }
        public string User { get; set; }
        public string Password { get; set; }
        public string VmPath { get; set; } //"[datastore2-9T-raid0] sonmi01-004-rps2-veeam/sonmi01-004-rps2-veeam.vmx"
        public bool ToListAllVMs { get; set; }
    }

    public class MainSearchVM_Out
    {

    }

    static partial class MyTestCases
    {
        public static int MainFindVM(MainSearchVM_In inParam, MainSearchVM_Out outRet)
        {

            VmSummaryInfo vmSummaryInfo = new VmSummaryInfo();

            System.Net.ServicePointManager.Expect100Continue = false;

            MyCertVerify certVerify = new MyCertVerify();
            System.Net.ServicePointManager.ServerCertificateValidationCallback =
                certVerify.RemoteCertificateValidationCallback;

            BasicHttpBinding binding = null;

            if(string.Compare("https", inParam.Protocal, StringComparison.InvariantCultureIgnoreCase) == 0)
            {
                binding = new BasicHttpBinding(BasicHttpSecurityMode.Transport)
                {
                    AllowCookies = true,
                    OpenTimeout = TimeSpan.FromHours(1),
                    ReceiveTimeout = TimeSpan.FromHours(1),
                    SendTimeout = TimeSpan.FromHours(1),
                    CloseTimeout = TimeSpan.FromHours(1),
                    MaxBufferPoolSize = 1024 * 1024 * 2,
                    MaxReceivedMessageSize = 1024 * 512,
                    MaxBufferSize = 1024 * 512
                };
            }
            else
            {
                binding = new BasicHttpBinding()
                {
                    AllowCookies = true,
                    OpenTimeout = TimeSpan.FromHours(1),
                    ReceiveTimeout = TimeSpan.FromHours(1),
                    SendTimeout = TimeSpan.FromHours(1),
                    CloseTimeout = TimeSpan.FromHours(1),
                    MaxBufferPoolSize = 1024 * 1024 * 2,
                    MaxReceivedMessageSize = 1024 * 512,
                    MaxBufferSize = 1024 * 512
                };
            }

            string sdkUrl = string.Format("{0}://{1}{2}/sdk", inParam.Protocal, inParam.ServerName, 0 == inParam.Port ? "" : ":" + inParam.Port.ToString());
            EndpointAddress epa = new EndpointAddress(sdkUrl);

            vmSummaryInfo.VmHost.ServerName = inParam.ServerName;
            vmSummaryInfo.VmHost.UserName = inParam.User;
            vmSummaryInfo.VmHost.Password = inParam.Password;
            vmSummaryInfo.VmHost.Port = inParam.Port;


            StringBuilder sb = new StringBuilder();
            StringWriter sw = new StringWriter(sb);

            using (var vimClient = new VimPortTypeClient(binding, epa))
            {
                //var serviceCertificate = vimClient.ClientCredentials.ServiceCertificate;
                //serviceCertificate.Authentication.CertificateValidationMode = X509CertificateValidationMode.Custom;
                //serviceCertificate.Authentication.CustomCertificateValidator = new MyX509CertificateValidator(null);
                //serviceCertificate.Authentication.RevocationMode = X509RevocationMode.NoCheck;

                //var clientCertificate = vimClient.ClientCredentials.ClientCertificate;

                try
                {
                    ManagedObjectReference morServiceInstance = new ManagedObjectReference()
                    {
                        @type = "ServiceInstance",
                        Value = "ServiceInstance"
                    };
                    ServiceContent sc = vimClient.RetrieveServiceContent(morServiceInstance);

                    vmSummaryInfo.VmHost.ThumbPrint = StaticHelpers.FormatThumbPrint(certVerify.Thumbprint);

                    sb.AppendFormat("{0}, {1}, {2}, {3}, {4}", epa.Uri, inParam.User, inParam.Password, vmSummaryInfo.VmHost.ThumbPrint, certVerify.Thumbprint);
                    sb.AppendLine();
                    //StaticHelpers.PrintServerAboutInfo(sb, sc.about);
                    sc.about.Dump(nameof(sc.about), sw, false);

                    Console.WriteLine("{0}", sb.ToString());
                    sb.Clear();

                    
                    using (MyCollectionDisposable collectionDisposal = new MyCollectionDisposable())
                    {
                        UserSession userSession = vimClient.Login(sc.sessionManager, inParam.User, inParam.Password, null);
                        collectionDisposal.Add(() => vimClient.Logout(sc.sessionManager));

                        //bool bListAllVMs = false;
                        //if(bListAllVMs)
                        if (inParam.ToListAllVMs)
                        {
                            CListVMs listVMs = new CListVMs(vimClient, sc.propertyCollector);
                            listVMs.list_all_vms(sc);
                        }


                        VimPropCollector pc = new VimPropCollector(vimClient, sc.propertyCollector);

                        VimFinder vimFinder = new VimFinder(vimClient, sc.propertyCollector, sc.rootFolder);

                        List<ManagedObjectReference> morDatacenters = new List<ManagedObjectReference>();
                        vimFinder.FindDatacenters(morDatacenters);

                        foreach (var datacenter in morDatacenters)
                        {
                            try
                            {
                                ManagedObjectReference morVM = (ManagedObjectReference)vimClient.FindByDatastorePath(sc.searchIndex, datacenter, inParam.VmPath);
                                if (null != morVM)
                                {
                                    vmSummaryInfo.VmHost.VmxSpec = "moref=" + morVM.Value;

                                    VirtualMachineConfigInfo virtualMachineConfigInfo = (VirtualMachineConfigInfo)pc.Get(morVM, "config");
                                    //StaticHelpers.PrintMor(sb, morVM);
                                    morVM.Dump(nameof(morVM), sw, false);
                                    //StaticHelpers.PrintVMCI(sb, virtualMachineConfigInfo);
                                    StaticHelpers.PrintVirtualDisksBriefInfo(sb, virtualMachineConfigInfo, vmSummaryInfo.VmHost.CurrentDiskPath);
                                    virtualMachineConfigInfo.Dump(nameof(virtualMachineConfigInfo), sw, false);
                                    Console.WriteLine("{0}", sb.ToString());
                                    sb.Clear();

                                    VimSnapshotInfo si = new VimSnapshotInfo(vimClient, sc.propertyCollector, morVM, sb, vmSummaryInfo.Snapshots);
                                    si.Traverse();
                                    Console.WriteLine("{0}", sb.ToString());
                                    sb.Clear();

                                    //datastore ManagedObjectReference:Datastore[] datastore-983 (datastore_on_30_1T) 
                                    var datastore = (ManagedObjectReference[])pc.Get(morVM, "datastore");
                                    if (null != datastore)
                                    {
                                        var datastoreName = from item in datastore
                                                            select new { Datastore = item, DatastoreName = (string)pc.Get(item, "name") };
                                        foreach (var item in datastoreName)
                                        {
                                            sb.AppendLine();
                                            //StaticHelpers.PrintMor(sb, item.Datastore);
                                            item.Datastore.Dump(nameof(item.Datastore), sw, false);
                                            sb.AppendFormat("{0}\t\"{1}\"", nameof(item.DatastoreName), item.DatastoreName);
                                            sb.AppendLine();
                                        }
                                    }
                                    //guest GuestInfo guest 
                                    var guest = (GuestInfo)pc.Get(morVM, "guest");
                                    //StaticHelpers.PrintGuestInfo(sb, guest);
                                    guest.Dump(nameof(guest), sw, false);
                                    //layout VirtualMachineFileLayout layout 
                                    var layout = (VirtualMachineFileLayout)pc.Get(morVM, "layout");
                                    //StaticHelpers.PrintVirtualMachineFileLayout(sb, layout);
                                    layout.Dump(nameof(layout), sw, false);
                                    //layoutEx VirtualMachineFileLayoutEx layoutEx 
                                    var layoutEx = (VirtualMachineFileLayoutEx)pc.Get(morVM, "layoutEx");
                                    //StaticHelpers.PrintVirtualMachineFileLayoutEx(sb, layoutEx);
                                    layoutEx.Dump(nameof(layoutEx), sw, false);
                                    //name string "shuli02-vc60" 
                                    var name = (string)pc.Get(morVM, "name");
                                    sb.AppendFormat("{0}\t\"{1}\"", nameof(name), name);
                                    //network ManagedObjectReference:Network[] network-822(VM Network)
                                    var network = (ManagedObjectReference[])pc.Get(morVM, "network");
                                    if(null != network)
                                    {
                                        foreach(var Network in network)
                                        {
                                            //StaticHelpers.PrintMor(sb, Network);
                                            Network.Dump(nameof(Network), sw, false);
                                        }
                                    }
                                    
                                    //parent ManagedObjectReference:Folder group-v3 (vm) 
                                    var parent = (ManagedObjectReference)pc.Get(morVM, "parent");
                                    //StaticHelpers.PrintMor(sb, parent);
                                    parent.Dump(nameof(parent), sw, false);

                                    //resourceConfig ResourceConfigSpec resourceConfig 
                                    var resourceConfig = (ResourceConfigSpec)pc.Get(morVM, "resourceConfig");
                                    //StaticHelpers.PrintResourceConfigSpec(sb, resourceConfig);
                                    resourceConfig.Dump(nameof(resourceConfig), sw, false);

                                    //resourcePool ManagedObjectReference:ResourcePool resgroup-980 (VC) 
                                    var resourcePool = (ManagedObjectReference)pc.Get(morVM, "resourcePool");
                                    //StaticHelpers.PrintMor(sb, resourcePool);
                                    resourcePool.Dump(nameof(resourcePool), sw, false);

                                    //runtime VirtualMachineRuntimeInfo runtime 
                                    var runtime = (VirtualMachineRuntimeInfo)pc.Get(morVM, "runtime");
                                    //StaticHelpers.PrintVirtualMachineRuntimeInfo(sb, runtime);
                                    runtime.Dump(nameof(runtime), sw, false);

                                    string runtimeHostName = (string)pc.Get(runtime.host, "name");
                                    ManagedObjectReference morRuntimehostParent = (ManagedObjectReference)pc.Get(runtime.host, "parent");
                                    string runtimeHostParentName = (string)pc.Get(morRuntimehostParent, "name");
                                    //StaticHelpers.PrintMor(sb, runtime.host);
                                    runtime.host.Dump(nameof(runtime.host), sw, false);
                                    sb.AppendFormat("{0}\t\"{1}\"", nameof(runtimeHostName), runtimeHostName); sb.AppendLine();
                                    sb.AppendLine();
                                    //StaticHelpers.PrintMor(sb, morRuntimehostParent);
                                    morRuntimehostParent.Dump(nameof(morRuntimehostParent), sw, false);
                                    sb.AppendFormat("{0}\t\"{1}\"", nameof(runtimeHostParentName), runtimeHostParentName); sb.AppendLine();
                                    sb.AppendLine();

                                    //storage VirtualMachineStorageInfo storage 
                                    var storage = (VirtualMachineStorageInfo)pc.Get(morVM, "storage");
                                    //StaticHelpers.PrintVirtualMachineStorageInfo(sb, storage);
                                    storage.Dump(nameof(storage), sw, false);
                                    Console.WriteLine("{0}", sb.ToString());
                                    sb.Clear();

                                    //summary VirtualMachineSummary summary 
                                    var summary = (VirtualMachineSummary)pc.Get(morVM, "summary");
                                    //StaticHelpers.PrintVirtualMachineSummary(sb, summary);
                                    summary.Dump(nameof(summary), sw, false);


                                    Console.WriteLine("{0}", sb.ToString());
                                    sb.Clear();

                                    /////////////////////////////////////////////////////////////////////////////////////////////
                                    sb.Append('~', 80);sb.AppendLine();
                                    sb.Append('~', 80); sb.AppendLine();
                                    vmSummaryInfo.Dump(nameof(vmSummaryInfo), sw, false);
                                    sb.Append('~', 80); sb.AppendLine();
                                    sb.Append('~', 80); sb.AppendLine();
                                    Console.WriteLine("{0}", sb.ToString());
                                    sb.Clear();

                                    /////////////////////////////////////////////////////////////////////////////////////////////
                                    bool testVmOperation = false;
                                    if(testVmOperation)
                                    {
                                        try
                                        {
                                            MyVMOperation vmop = new MyVMOperation(vimClient, sc.propertyCollector, morVM);

                                            TaskInfoState state = vmop.ConsolidateVMDisks();

                                            string snapshotid1 = null;
                                            state = vmop.CreateSnapshot("test snapshot name 1", "test snapshot description1 ", false, true, out snapshotid1);
                                            VirtualDisk[] VirtualDisks1 = vmop.QuerySnapshotVirtualDisks(snapshotid1);


                                            string snapshotid2 = null;
                                            state = vmop.CreateSnapshot("test snapshot name 2", "test snapshot description2 ", false, true, out snapshotid2);
                                            VirtualDisk[] VirtualDisks2 = vmop.QuerySnapshotVirtualDisks(snapshotid2);



                                            try
                                            {
                                                foreach (var vdisk in VirtualDisks1)
                                                {
                                                    long startOffset = 0;
                                                    long diskSize = vdisk.capacityInBytes == 0 ? vdisk.capacityInKB * 1024 : vdisk.capacityInBytes;
                                                    while (startOffset < diskSize)
                                                    {
                                                        DiskChangeInfo diskChangeInfo = vmop.QueryChangedDiskAreas(snapshotid2, vdisk, startOffset);
                                                        if (null != diskChangeInfo.changedArea)
                                                        {
                                                            foreach (var changedArea in diskChangeInfo.changedArea)
                                                            {
                                                                changedArea.Dump(nameof(changedArea), sw, false);
                                                            }
                                                            Console.WriteLine("{0}", sb.ToString());
                                                            sb.Clear();
                                                        }
                                                        startOffset = diskChangeInfo.startOffset + diskChangeInfo.length;
                                                    }
                                                }
                                            }
                                            catch (Exception exQueryChangedDiskAreas)
                                            {
                                                StaticHelpers.PrintException(exQueryChangedDiskAreas, 0);
                                            }

                                            state = vmop.RemoveSnapshot(snapshotid2, false, true);
                                            state = vmop.RemoveSnapshot(snapshotid1, false, true);
                                        }
                                        catch(Exception exTestVmOperation)
                                        {
                                            StaticHelpers.PrintException(exTestVmOperation, 0);
                                        }
                                        
                                    }

                                    /////////////////////////////////////////////////////////////////////////////////////////////
                                    bool testGustOp = false;
                                    if(testGustOp)
                                    {
                                        try
                                        {
                                            string guestUser = "administrator";
                                            string guestPassword = "c@123456";
                                            VimGuestOperation vimGuestOp = new VimGuestOperation(vimClient, sc.propertyCollector, sc.guestOperationsManager, morVM, guestUser, guestPassword);

                                            string[] vars = vimGuestOp.ReadEnvironmentVariableInGuest(new string[] { "path" });

                                            GuestProgramSpec guestProgramSpec = new GuestProgramSpec()
                                            {
                                                programPath = @"c:\Windows\notepad.exe",
                                                arguments = "",
                                                workingDirectory = @"c:\",
                                                envVariables = null
                                            };
                                            long pid = vimGuestOp.StartProgramInGuest(guestProgramSpec);
                                            GuestProcessInfo[] gpi = vimGuestOp.ListProcessesInGuest(new long[] { pid });
                                            vimGuestOp.TerminateProcessInGuest(pid);


                                            string guestFilePath = @"E:\1.vhdx";
                                            string localFile = @"E:\~temp\1.vhdx";
                                            vimGuestOp.FileTransferFromGuest(guestFilePath, localFile);
                                            string guestFilePath2 = @"e:\111\1.vhdx";
                                            vimGuestOp.FileTransferToGuest(guestFilePath2, localFile, true);
                                        }
                                        catch (Exception exGust)
                                        {
                                            StaticHelpers.PrintException(exGust, 0);
                                        }
                                    }
                                }
                            }
                            catch (Exception exFind)
                            {
                                StaticHelpers.PrintException(exFind, 0);
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    StaticHelpers.PrintException(ex, 0);
                }
            }

            return 0;
        }
    }

}