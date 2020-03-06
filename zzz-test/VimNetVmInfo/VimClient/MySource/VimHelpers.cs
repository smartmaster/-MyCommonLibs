using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using VimLib.VimServiceReference;
using ObjectDumper;

namespace VimClient
{
    static partial class StaticHelpers
    {
        public static void PrintVirtualDiskFlatVer2BackingInfoBriefInfo(StringBuilder sb, VirtualDiskFlatVer2BackingInfo info, IList<string> vmdkPath)
        {
            if (null == info) return;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.fileName), info.fileName); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.changeId), info.changeId); sb.AppendLine();
            sb.AppendLine();
            if(null != vmdkPath)
            {
                vmdkPath.Add(info.fileName);
            }
            PrintVirtualDiskFlatVer2BackingInfoBriefInfo(sb, info.parent, null);
        }

        public static void PrintVirtualDiskBriefInfo(StringBuilder sb, VirtualDisk info, IList<string> vmdkPath)
        {
            if (null == info) return;

            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.key), info.key); sb.AppendLine();

            VirtualDiskFlatVer2BackingInfo backing = (info.backing as VirtualDiskFlatVer2BackingInfo);
            if (null != backing)
            {
                PrintVirtualDiskFlatVer2BackingInfoBriefInfo(sb, backing, vmdkPath);
            }
            else
            {
                VirtualDeviceFileBackingInfo backing2 = (info.backing as VirtualDeviceFileBackingInfo);
                if (null != backing2)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(backing2.fileName), backing2.fileName); sb.AppendLine();
                    if(null != vmdkPath)
                    {
                        vmdkPath.Add(backing2.fileName);
                    }
                }
            }

        }
        public static void PrintVirtualDisksBriefInfo(StringBuilder sb, VirtualMachineConfigInfo info, IList<string> vmdkPath)
        {
            var devices = info?.hardware?.device;
            if (null == devices) return;

            foreach (var device in devices)
            {
                VirtualDisk virtualDisk = (device as VirtualDisk);
                if (null != virtualDisk)
                {
                    sb.Append('+', 80); sb.AppendLine();
                    PrintVirtualDiskBriefInfo(sb, virtualDisk, vmdkPath);
                    sb.Append('-', 80); sb.AppendLine();
                }
            }
        }
#if false
        public static void PrintMor(StringBuilder sb, ManagedObjectReference info)
        {
            if (null == info) return;

            sb.AppendFormat("{0}=\"{1}\"\t{2}=\"{3}\"", nameof(info.type), info.type, nameof(info.Value), info.Value); sb.AppendLine();
        }


        public static void PrintVirtualDiskFlatVer2BackingInfoParent(StringBuilder sb, VirtualDiskFlatVer2BackingInfo info)
        {
            if (null != info)
            {
                sb.AppendFormat("{0}\t\"{1}\"", nameof(info.fileName), info.fileName); sb.AppendLine();
                sb.AppendFormat("{0}\t\"{1}\"", nameof(info.changeId), info.changeId); sb.AppendLine();
                sb.AppendFormat("{0}\t\"{1}\"", nameof(info.backingObjectId), info.backingObjectId); sb.AppendLine();
                //PrintMor(sb, info.datastore); sb.AppendLine();

                PrintVirtualDiskFlatVer2BackingInfoParent(sb, info.parent);
            }
        }

        public static void PrintVirtualDiskFlatVer2BackingInfo(StringBuilder sb, VirtualDiskFlatVer2BackingInfo info)
        {
            if (null == info) return;

            //private string changeId;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.fileName), info.fileName); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.changeId), info.changeId); sb.AppendLine();
            PrintMor(sb, info.datastore); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.backingObjectId), info.backingObjectId); sb.AppendLine();

            //private string contentId;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.contentId), info.contentId); sb.AppendLine();


            //    private string diskMode;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.diskMode), info.diskMode); sb.AppendLine();
            //private bool split;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.split), info.split); sb.AppendLine();
            //private bool splitSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.splitSpecified), info.splitSpecified); sb.AppendLine();
            //private bool writeThrough;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.writeThrough), info.writeThrough); sb.AppendLine();
            //private bool writeThroughSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.writeThroughSpecified), info.writeThroughSpecified); sb.AppendLine();
            //private bool thinProvisioned;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.thinProvisioned), info.thinProvisioned); sb.AppendLine();
            //private bool thinProvisionedSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.thinProvisionedSpecified), info.thinProvisionedSpecified); sb.AppendLine();
            //private bool eagerlyScrub;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.eagerlyScrub), info.eagerlyScrub); sb.AppendLine();
            //private bool eagerlyScrubSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.eagerlyScrubSpecified), info.eagerlyScrubSpecified); sb.AppendLine();
            //private string uuid;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.uuid), info.uuid); sb.AppendLine();

            //private string deltaDiskFormat;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.deltaDiskFormat), info.deltaDiskFormat); sb.AppendLine();
            //private bool digestEnabled;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.digestEnabled), info.digestEnabled); sb.AppendLine();
            //private bool digestEnabledSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.digestEnabledSpecified), info.digestEnabledSpecified); sb.AppendLine();
            //private int deltaGrainSize;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.deltaGrainSize), info.deltaGrainSize); sb.AppendLine();
            //private bool deltaGrainSizeSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.deltaGrainSizeSpecified), info.deltaGrainSizeSpecified); sb.AppendLine();
            //private string deltaDiskFormatVariant;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.deltaDiskFormatVariant), info.deltaDiskFormatVariant); sb.AppendLine();
            //private string sharing;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.sharing), info.sharing); sb.AppendLine();

            //private VirtualDiskFlatVer2BackingInfo parent;
            //sb.AppendFormat("{0}\t\"{1}\"", nameof(info.xxx), info.xx); sb.AppendLine();
            sb.AppendFormat("VMDK chain:"); sb.AppendLine();
            PrintVirtualDiskFlatVer2BackingInfoParent(sb, info);

            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualDeviceBackingInfo(StringBuilder sb, VirtualDeviceBackingInfo info)
        {
            if (null == info) return;

            VirtualDiskFlatVer2BackingInfo virtualDiskFlatVer2BackingInfo = (info as VirtualDiskFlatVer2BackingInfo);
            if (null != virtualDiskFlatVer2BackingInfo)
            {
                PrintVirtualDiskFlatVer2BackingInfo(sb, virtualDiskFlatVer2BackingInfo);
            }
            else
            {
                VirtualDeviceFileBackingInfo virtualDiskFlatVerBackingInfo = (info as VirtualDeviceFileBackingInfo);
                if (null != virtualDiskFlatVerBackingInfo)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(virtualDiskFlatVerBackingInfo.fileName), virtualDiskFlatVerBackingInfo.fileName); sb.AppendLine();
                }
            }
        }


        public static void PrintDescription(StringBuilder sb, Description info)
        {
            if (null == info) return;
            //            private string labelField;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.label), info.label); sb.AppendLine();
            //private string summaryField;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.summary), info.summary); sb.AppendLine();
        }


        public static void PrintVirtualDeviceConnectInfo(StringBuilder sb, VirtualDeviceConnectInfo info)
        {
            if (null == info) return;

            //    private bool startConnected;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.startConnected), info.startConnected); sb.AppendLine();
            //private bool allowGuestControl;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.allowGuestControl), info.allowGuestControl); sb.AppendLine();
            //private bool connected;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.connected), info.connected); sb.AppendLine();
            //private string status;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.status), info.status); sb.AppendLine();
        }

        public static void PrintVirtualDeviceBusSlotInfo(StringBuilder sb, VirtualDeviceBusSlotInfo info)
        {
            if (null == info) return;
        }

        public static void PrintStorageIOAllocationInfo(StringBuilder sb, StorageIOAllocationInfo info)
        {
            if (null == info) return;
            //            private long limit;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.limit), info.limit); sb.AppendLine();
            //private bool limitSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.limitSpecified), info.limitSpecified); sb.AppendLine();
            //private SharesInfo shares;
            PrintSharesInfo(sb, info.shares);
            //private int reservation;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.reservation), info.reservation); sb.AppendLine();
            //private bool reservationSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.reservationSpecified), info.reservationSpecified); sb.AppendLine();
        }

        public static void  PrintVirtualDiskVFlashCacheConfigInfo(StringBuilder sb, VirtualDiskVFlashCacheConfigInfo info)
        {
            if (null == info) return;

            //           private string vFlashModule;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.vFlashModule), info.vFlashModule); sb.AppendLine();
            //private long reservationInMB;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.reservationInMB), info.reservationInMB); sb.AppendLine();
            //private bool blockSizeInKB;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.blockSizeInKB), info.blockSizeInKB); sb.AppendLine();
            //private string cacheConsistencyType;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.blockSizeInKB), info.blockSizeInKB); sb.AppendLine();
            //private string blockSizeInKB;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.blockSizeInKB), info.blockSizeInKB); sb.AppendLine();
            //private long blockSizeInKB;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.blockSizeInKB), info.blockSizeInKB); sb.AppendLine();
            //private bool blockSizeInKBSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.blockSizeInKBSpecified), info.blockSizeInKBSpecified); sb.AppendLine();
        }

        public static void PrintVirtualDisk(StringBuilder sb, VirtualDisk info)
        {
            if (null == info) return;

            //            private int key;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.key), info.key); sb.AppendLine();
            //private Description deviceInfo;
            PrintDescription(sb, info.deviceInfo);
            //private VirtualDeviceBackingInfo backing;
            PrintVirtualDeviceBackingInfo(sb, info.backing);
            //private VirtualDeviceConnectInfo connectable;
            PrintVirtualDeviceConnectInfo(sb, info.connectable);
            //private VirtualDeviceBusSlotInfo slotInfo;
            PrintVirtualDeviceBusSlotInfo(sb, info.slotInfo);
            //private int controllerKey;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.controllerKey), info.controllerKey); sb.AppendLine();
            //private bool controllerKeySpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.controllerKeySpecified), info.controllerKeySpecified); sb.AppendLine();
            //private int unitNumber;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.unitNumber), info.unitNumber); sb.AppendLine();
            //private bool unitNumberSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.unitNumberSpecified), info.unitNumberSpecified); sb.AppendLine();
            //private long capacityInKB;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.capacityInKB), info.capacityInKB); sb.AppendLine();
            //private long capacityInBytes;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.capacityInBytes), info.capacityInBytes); sb.AppendLine();
            //private bool capacityInBytesSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.capacityInBytesSpecified), info.capacityInBytesSpecified); sb.AppendLine();
            //private SharesInfo shares;
            PrintSharesInfo(sb, info.shares);
            //private StorageIOAllocationInfo storageIOAllocation;
            PrintStorageIOAllocationInfo(sb, info.storageIOAllocation);
            //private string diskObjectId;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.diskObjectId), info.diskObjectId); sb.AppendLine();
            //private VirtualDiskVFlashCacheConfigInfo vFlashCacheConfigInfo;
            PrintVirtualDiskVFlashCacheConfigInfo(sb, info.vFlashCacheConfigInfo);
            //private string[] iofilter;
            if (null != info.iofilter)
            {
                foreach(var iofilter in info.iofilter)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(iofilter), iofilter); sb.AppendLine();
                }
            }
        }

        public static void PrintVMCI(StringBuilder sb, VirtualMachineConfigInfo info)
        {
            if (null == info) return;

            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.changeTrackingEnabledSpecified), info.changeTrackingEnabledSpecified); sb.AppendLine();
            if(null != info?.datastoreUrl)
            {
                foreach (var datastoreUrl in info.datastoreUrl)
                {
                    sb.AppendLine();
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(datastoreUrl) + nameof(datastoreUrl.name), datastoreUrl.name); sb.AppendLine();
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(datastoreUrl) + nameof(datastoreUrl.url), datastoreUrl.url); sb.AppendLine();
                    sb.AppendLine();
                }
            }

            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.files.vmPathName), info.files.vmPathName); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.firmware), info.firmware); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.guestFullName), info.guestFullName); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.guestId), info.guestId); sb.AppendLine();
            if(null != info?.hardware?.device)
            {
                sb.AppendLine(); sb.AppendFormat("VMDKS begin+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"); sb.AppendLine();
                foreach (var device in info.hardware.device)
                {
                    VirtualDisk virtualDisk = (device as VirtualDisk);
                    if(null != virtualDisk)
                    {
                        sb.AppendFormat("VMDK begin=================================================================================="); sb.AppendLine();
                        PrintVirtualDisk(sb, virtualDisk);
                        sb.AppendFormat("VMDK end=================================================================================="); sb.AppendLine();
                    }
                }
                sb.AppendFormat("VMDKS end+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"); sb.AppendLine(); sb.AppendLine();
            }
            

            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.hardware.memoryMB), info.hardware.memoryMB); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.hardware.numCPU), info.hardware.numCPU); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.hardware.numCoresPerSocket), info.hardware.numCoresPerSocket); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.instanceUuid), info.instanceUuid); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.name), info.name); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.tools.toolsVersion), info.tools.toolsVersion); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.tools.toolsVersionSpecified), info.tools.toolsVersionSpecified); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.uuid), info.uuid); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.version), info.version); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVMST(StringBuilder sb, VirtualMachineSnapshotTree info)
        {
            if (null == info) return;

            PrintMor(sb, info.snapshot);
            PrintMor(sb, info.vm);

            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.name), info.name); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.description), info.description); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.id), info.id); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.createTime), info.createTime); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.state), info.state); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.quiesced), info.quiesced); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.backupManifest), info.backupManifest); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.replaySupported), info.replaySupported); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.replaySupportedSpecified), info.replaySupportedSpecified); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintServerAboutInfo(StringBuilder sb, AboutInfo info)
        {
            if (null == info) return;

            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.name), info.name); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.fullName), info.fullName); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.vendor), info.vendor); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.version), info.version); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.build), info.build); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.localeVersion), info.localeVersion); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.localeBuild), info.localeBuild); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.osType), info.osType); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.productLineId), info.productLineId); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.apiType), info.apiType); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.apiVersion), info.apiVersion); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.instanceUuid), info.instanceUuid); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.licenseProductName), info.licenseProductName); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.licenseProductVersion), info.licenseProductVersion); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintKeyValue(StringBuilder sb, KeyValue info)
        {
            if (null == info) return;

            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.key), info.key); sb.AppendLine();
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.value), info.value); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintNetDhcpConfigInfoDhcpOptions(StringBuilder sb, NetDhcpConfigInfoDhcpOptions info)
        {
            if (null == info) return;
            //               private bool enable;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.enable), info.enable); sb.AppendLine();
            //private KeyValue[] config;
            if(null != info.config)
            {
                foreach (var item in info.config)
                {
                    PrintKeyValue(sb, item);
                }
            }
            
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintNetDhcpConfigInfo(StringBuilder sb, NetDhcpConfigInfo info)
        {
            if (null == info) return;
            //            private NetDhcpConfigInfoDhcpOptions ipv6Field;
            PrintNetDhcpConfigInfoDhcpOptions(sb, info.ipv6);

            //private NetDhcpConfigInfoDhcpOptions ipv4Field;
            PrintNetDhcpConfigInfoDhcpOptions(sb, info.ipv4);
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintNetIpConfigInfoIpAddress(StringBuilder sb, NetIpConfigInfoIpAddress info)
        {
            if (null == info) return;

            //           private string ipAddress;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.ipAddress), info.ipAddress); sb.AppendLine();
            //private int prefixLength;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.prefixLength), info.prefixLength); sb.AppendLine();
            //private string origin;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.origin), info.origin); sb.AppendLine();
            //private string state;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.state), info.state); sb.AppendLine();
            //private System.DateTime lifetime;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.lifetime), info.lifetime); sb.AppendLine();
            //private bool lifetimeSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.lifetimeSpecified), info.lifetimeSpecified); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintNetIpConfigInfo(StringBuilder sb, NetIpConfigInfo info)
        {
            if (null == info) return;

            if (null != info?.ipAddress)
            {
                //            private NetIpConfigInfoIpAddress[] ipAddress;
                foreach (var item in info.ipAddress)
                {
                    PrintNetIpConfigInfoIpAddress(sb, item);
                }
            }
            
            //private NetDhcpConfigInfo dhcp;
            PrintNetDhcpConfigInfo(sb, info.dhcp);
            //private bool autoConfigurationEnabled;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.autoConfigurationEnabled), info.autoConfigurationEnabled); sb.AppendLine();
            //private bool autoConfigurationEnabledSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.autoConfigurationEnabledSpecified), info.autoConfigurationEnabledSpecified); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintNetBIOSConfigInfo(StringBuilder sb, NetBIOSConfigInfo info)
        {
            if (null == info) return;

            //private string modeField;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.mode), info.mode); sb.AppendLine();
        }

        public static void PrintNetDnsConfigInfo(StringBuilder sb, NetDnsConfigInfo info)
        {
            if (null == info) return;

            //            private bool dhcp;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.dhcp), info.dhcp); sb.AppendLine();
            //private string hostName;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.hostName), info.hostName); sb.AppendLine();
            //private string domainName;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.domainName), info.domainName); sb.AppendLine();

            if (null != info?.ipAddress)
            {
                //private string[] ipAddress;
                foreach (var ipAddress in info.ipAddress)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(ipAddress), ipAddress); sb.AppendLine();
                }

            }

            if (null != info?.searchDomain)
            {
                //private string[] searchDomain;
                foreach (var searchDomain in info.searchDomain)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(searchDomain), searchDomain); sb.AppendLine();
                }
            }
           
            sb.AppendLine();
            sb.AppendLine();
        }


        public static void PrintNetIpRouteConfigInfoGateway(StringBuilder sb, NetIpRouteConfigInfoGateway info)
        {
            if (null == info) return;

            // private string ipAddressField;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.ipAddress), info.ipAddress); sb.AppendLine();
            //private string deviceField;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.device), info.device); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintNetIpRouteConfigInfoIpRoute(StringBuilder sb, NetIpRouteConfigInfoIpRoute info)
        {
            if (null == info) return;

            //         private string network;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.network), info.network); sb.AppendLine();
            //private int prefixLength;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.prefixLength), info.prefixLength); sb.AppendLine();
            //private NetIpRouteConfigInfoGateway gateway;
            PrintNetIpRouteConfigInfoGateway(sb, info.gateway);
        }


        public static void PrintNetIpRouteConfigInfo(StringBuilder sb, NetIpRouteConfigInfo info)
        {
            if (null == info) return;

            if (null != info?.ipRoute)
            {
                //NetIpRouteConfigInfoIpRoute[] ipRouteField;
                foreach (var ipRoute in info.ipRoute)
                {
                    PrintNetIpRouteConfigInfoIpRoute(sb, ipRoute);
                }
            }
            
        }

        public static void PrintGuestStackInfo(StringBuilder sb, GuestStackInfo info)
        {
            if (null == info) return;

            //    private NetDnsConfigInfo dnsConfig;
            PrintNetDnsConfigInfo(sb, info.dnsConfig);
            //private NetIpRouteConfigInfo ipRouteConfig;
            PrintNetIpRouteConfigInfo(sb, info.ipRouteConfig);
            if (null != info?.ipStackConfig)
            {
                //private KeyValue[] ipStackConfig;
                foreach (var item in info.ipStackConfig)
                {
                    PrintKeyValue(sb, item);
                }
            }
            
            //private NetDhcpConfigInfo dhcpConfig;
            PrintNetDhcpConfigInfo(sb, info.dhcpConfig);
            sb.AppendLine();
        }

        public static  void PrintGuestDiskInfo(StringBuilder sb, GuestDiskInfo info)
        {
            if (null == info) return;

            //            private string diskPath;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.diskPath), info.diskPath); sb.AppendLine();
            //private long capacity;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.capacity), info.capacity); sb.AppendLine();
            //private bool capacitySpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.capacitySpecified), info.capacitySpecified); sb.AppendLine();
            //private long freeSpace;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.freeSpace), info.freeSpace); sb.AppendLine();
            //private bool freeSpaceSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.freeSpaceSpecified), info.freeSpaceSpecified); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintGuestScreenInfo(StringBuilder sb, GuestScreenInfo info)
        {
            if (null == info) return;

            //    private int width;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.width), info.width); sb.AppendLine();
            //private int height;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.height), info.height); sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintGuestInfoNamespaceGenerationInfo(StringBuilder sb, GuestInfoNamespaceGenerationInfo info)
        {
            if (null == info) return;

            //            private string key;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.key), info.key); sb.AppendLine();
            //private int generationNo;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.generationNo), info.generationNo); sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintGuestInfo(StringBuilder sb, GuestInfo info)
        {
            if (null == info) return;

            //    private VirtualMachineToolsStatus toolsStatus;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.toolsStatus), info.toolsStatus); sb.AppendLine();
            //private bool toolsStatusSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.toolsStatusSpecified), info.toolsStatusSpecified); sb.AppendLine();
            //private string toolsVersionStatus;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.toolsVersionStatus), info.toolsVersionStatus); sb.AppendLine();
            //private string toolsVersionStatus2;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.toolsVersionStatus2), info.toolsVersionStatus2); sb.AppendLine();
            //private string toolsRunningStatus;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.toolsRunningStatus), info.toolsRunningStatus); sb.AppendLine();
            //private string toolsVersion;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.toolsVersion), info.toolsVersion); sb.AppendLine();
            //private string guestId;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.guestId), info.guestId); sb.AppendLine();
            //private string guestFamily;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.guestFamily), info.guestFamily); sb.AppendLine();
            //private string guestFullName;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.guestFullName), info.guestFullName); sb.AppendLine();
            //private string hostName;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.hostName), info.hostName); sb.AppendLine();
            //private string ipAddress;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.ipAddress), info.ipAddress); sb.AppendLine();
            //private GuestNicInfo[] net;
            if(null != info.net)
            {
                foreach (GuestNicInfo item in info?.net)
                {
                    //              private string network;
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(item.network), item.network); sb.AppendLine();
                    //private string[] ipAddress;
                    if(null != item?.ipAddress)
                    {
                        foreach (string ipaddr in item.ipAddress)
                        {
                            sb.AppendFormat("{0}\t\"{1}\"", nameof(ipaddr), ipaddr); sb.AppendLine();
                        }
                    }
                    

                    //private string macAddress;
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(item.macAddress), item.macAddress); sb.AppendLine();
                    //private bool connected;
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(item.connected), item.connected); sb.AppendLine();
                    //private int deviceConfigId;
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(item.deviceConfigId), item.deviceConfigId); sb.AppendLine();
                    
                    if(null != item.dnsConfig)
                    {
                        //private NetDnsConfigInfo dnsConfig;
                        //                private bool dhcp;
                        sb.AppendFormat("{0}\t\"{1}\"", nameof(item.dnsConfig.dhcp), item.dnsConfig.dhcp); sb.AppendLine();
                        //private string hostName;
                        sb.AppendFormat("{0}\t\"{1}\"", nameof(item.dnsConfig.hostName), item.dnsConfig.hostName); sb.AppendLine();
                        //private string domainName;
                        sb.AppendFormat("{0}\t\"{1}\"", nameof(item.dnsConfig.domainName), item.dnsConfig.domainName); sb.AppendLine();
                        //private string[] ipAddress;
                        if (null != item?.dnsConfig?.ipAddress)
                        {
                            foreach (string ipAddress in item.dnsConfig.ipAddress)
                            {
                                sb.AppendFormat("{0}\t\"{1}\"", nameof(ipAddress), ipAddress); sb.AppendLine();
                            }
                        }

                        if(null != item?.dnsConfig?.searchDomain)
                        {
                            //private string[] searchDomain;
                            foreach (string searchDomain in item.dnsConfig.searchDomain)
                            {
                                sb.AppendFormat("{0}\t\"{1}\"", nameof(searchDomain), searchDomain); sb.AppendLine();
                            }
                        }
                        
                    }
                    
                    //private NetIpConfigInfo info;
                    if(null != item.ipConfig) PrintNetIpConfigInfo(sb, item.ipConfig);

                    //private NetBIOSConfigInfo netBIOSConfig;
                    if(null != item.netBIOSConfig) PrintNetBIOSConfigInfo(sb, item.netBIOSConfig);
                }
            }
            
            //private GuestStackInfo[] ipStack;
            if(null != info.ipStack)
            {
                foreach (var item in info.ipStack)
                {
                    PrintGuestStackInfo(sb, item);
                }
            }
            

            //private GuestDiskInfo[] disk;
            if(null != info.disk)
            {
                foreach (var item in info.disk)
                {
                    PrintGuestDiskInfo(sb, item);
                }
            }
            

            //private GuestScreenInfo screen;
            if(null != info.screen) PrintGuestScreenInfo(sb, info.screen);

            //private string guestState;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.guestState), info.guestState); sb.AppendLine();

            //private string appHeartbeatStatus;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.appHeartbeatStatus), info.appHeartbeatStatus); sb.AppendLine();
            //private bool guestKernelCrashed;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.guestKernelCrashed), info.guestKernelCrashed); sb.AppendLine();
            //private bool guestKernelCrashedSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.guestKernelCrashedSpecified), info.guestKernelCrashedSpecified); sb.AppendLine();
            //private string appState;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.appState), info.appState); sb.AppendLine();
            //private bool guestOperationsReady;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.guestOperationsReady), info.guestOperationsReady); sb.AppendLine();
            //private bool guestOperationsReadySpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.guestOperationsReadySpecified), info.guestOperationsReadySpecified); sb.AppendLine();
            //private bool interactiveGuestOperationsReady;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.interactiveGuestOperationsReady), info.interactiveGuestOperationsReady); sb.AppendLine();
            //private bool interactiveGuestOperationsReadySpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.interactiveGuestOperationsReadySpecified), info.interactiveGuestOperationsReadySpecified); sb.AppendLine();
            //private bool guestStateChangeSupported;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.guestStateChangeSupported), info.guestStateChangeSupported); sb.AppendLine();
            //private bool guestStateChangeSupportedSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.guestStateChangeSupportedSpecified), info.guestStateChangeSupportedSpecified); sb.AppendLine();
            //private GuestInfoNamespaceGenerationInfo[] generationInfo;
            if(null != info.generationInfo)
            {
                foreach (var item in info.generationInfo)
                {
                    PrintGuestInfoNamespaceGenerationInfo(sb, item);
                }
            }
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineFileLayoutDiskLayout(StringBuilder sb, VirtualMachineFileLayoutDiskLayout info)
        {
            if (null == info) return;

            //     private int key;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.key), info.key); sb.AppendLine();
            //private string[] diskFile;
            if(null != info.diskFile)
            {
                foreach(var diskFile in info.diskFile)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(diskFile), diskFile); sb.AppendLine();
                }
            }
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineFileLayoutSnapshotLayout(StringBuilder sb, VirtualMachineFileLayoutSnapshotLayout info)
        {
            if (null == info) return;

            //            private ManagedObjectReference key;
            PrintMor(sb, info.key);
            //private string[] snapshotFile;
            if (null != info.snapshotFile)
            {
                foreach (var snapshotFile in info.snapshotFile)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(snapshotFile), snapshotFile); sb.AppendLine();
                }
            }
            sb.AppendLine();
            sb.AppendLine();

        }

        public static void PrintVirtualMachineFileLayout(StringBuilder sb, VirtualMachineFileLayout info)
        {
            if (null == info) return;

        //            private string[] configFile;
            if(null != info.configFile)
            {
                foreach(var configFile in info.configFile)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(configFile), configFile); sb.AppendLine();
                }
            }

            //private string[] logFile;
            if (null != info.logFile)
            {
                foreach (var logFile in info.logFile)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(logFile), logFile); sb.AppendLine();
                }
            }

            //private VirtualMachineFileLayoutDiskLayout[] disk;
            if (null != info.disk)
            {
                foreach (var disk in info.disk)
                {
                    PrintVirtualMachineFileLayoutDiskLayout(sb, disk);
                }
            }

            //private VirtualMachineFileLayoutSnapshotLayout[] snapshot;
            if (null != info.snapshot)
            {
                foreach (var snapshot in info.snapshot)
                {
                    PrintVirtualMachineFileLayoutSnapshotLayout(sb, snapshot);
                }
            }

            //private string swapFile;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.swapFile), info.swapFile); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineFileLayoutExFileInfo(StringBuilder sb, VirtualMachineFileLayoutExFileInfo info)
        {
            if (null == info) return;

            //     private int key;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.key), info.key); sb.AppendLine();
            //private string name;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.name), info.name); sb.AppendLine();
            //private string type;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.type), info.type); sb.AppendLine();
            //private long size;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.size), info.size); sb.AppendLine();
            //private long uniqueSize;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.uniqueSize), info.uniqueSize); sb.AppendLine();
            //private bool uniqueSizeSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.uniqueSizeSpecified), info.uniqueSizeSpecified); sb.AppendLine();
            //private string backingObjectId;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.backingObjectId), info.backingObjectId); sb.AppendLine();
            //private bool accessible;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.accessible), info.accessible); sb.AppendLine();
            //private bool accessibleSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.accessibleSpecified), info.accessibleSpecified); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineFileLayoutExDiskUnit(StringBuilder sb, VirtualMachineFileLayoutExDiskUnit info)
        {
            if (null == info) return;

            //  private int[] fileKey
            if(null != info.fileKey)
            {
                foreach(var fileKey in info.fileKey)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(fileKey), fileKey); sb.AppendLine();
                }
            }

            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineFileLayoutExDiskLayout(StringBuilder sb, VirtualMachineFileLayoutExDiskLayout info)
        {
            if (null == info) return;

            //     private int keyField;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.key), info.key); sb.AppendLine();
            //private VirtualMachineFileLayoutExDiskUnit[] chainField;
            if(null != info.chain)
            {
                foreach(var chain in info.chain)
                {
                    PrintVirtualMachineFileLayoutExDiskUnit(sb, chain);
                }
            }
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineFileLayoutExSnapshotLayout(StringBuilder sb, VirtualMachineFileLayoutExSnapshotLayout info)
        {
            if (null == info) return;

            //          private ManagedObjectReference key;
            PrintMor(sb, info.key);
            //private int dataKey;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.dataKey), info.dataKey); sb.AppendLine();
            //private int memoryKey;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.memoryKey), info.memoryKey); sb.AppendLine();
            //private bool memoryKeySpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.memoryKeySpecified), info.memoryKeySpecified); sb.AppendLine();
            //private VirtualMachineFileLayoutExDiskLayout[] disk;
            if(null != info.disk)
            {
                foreach(var disk in info.disk)
                {
                    PrintVirtualMachineFileLayoutExDiskLayout(sb, disk);
                }
            }
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineFileLayoutEx(StringBuilder sb, VirtualMachineFileLayoutEx info)
        {
            if (null == info) return;

            //            private VirtualMachineFileLayoutExFileInfo[] file;
            if (null != info.file)
            {
                foreach (var file in info.file)
                {
                    PrintVirtualMachineFileLayoutExFileInfo(sb, file);
                }
            }

            //private VirtualMachineFileLayoutExDiskLayout[] disk;
            if (null != info.disk)
            {
                foreach (var disk in info.disk)
                {
                    PrintVirtualMachineFileLayoutExDiskLayout(sb, disk);
                }
            }

            //private VirtualMachineFileLayoutExSnapshotLayout[] snapshot;
            if (null != info.snapshot)
            {
                foreach (var snapshot in info.snapshot)
                {
                    PrintVirtualMachineFileLayoutExSnapshotLayout(sb, snapshot);
                }
            }

            //private System.DateTime timestamp;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.timestamp), info.timestamp); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintSharesInfo(StringBuilder sb, SharesInfo info)
        {
            if (null == info) return;

            //    private int shares;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.shares), info.shares); sb.AppendLine();
            //private SharesLevel level;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.level), info.level); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintResourceAllocationInfo(StringBuilder sb, ResourceAllocationInfo info)
        {
            if (null == info) return;

            //       private long reservation;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.reservation), info.reservation); sb.AppendLine();
            //private bool reservationSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.reservationSpecified), info.reservationSpecified); sb.AppendLine();
            //private bool expandableReservation;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.expandableReservation), info.expandableReservation); sb.AppendLine();
            //private bool expandableReservationSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.expandableReservationSpecified), info.expandableReservationSpecified); sb.AppendLine();
            //private long limit;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.limit), info.limit); sb.AppendLine();
            //private bool limitSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.limitSpecified), info.limitSpecified); sb.AppendLine();
            //private SharesInfo shares;
            PrintSharesInfo(sb, info.shares);
            //private long overheadLimit;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.overheadLimit), info.overheadLimit); sb.AppendLine();
            //private bool overheadLimitSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.overheadLimitSpecified), info.overheadLimitSpecified); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintResourceConfigSpec(StringBuilder sb, ResourceConfigSpec info)
        {
            if (null == info) return;

            //private ManagedObjectReference entity;
            PrintMor(sb, info.entity);
            //private string changeVersion;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.changeVersion), info.changeVersion); sb.AppendLine();
            //private System.DateTime lastModified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.lastModified), info.lastModified); sb.AppendLine();
            //private bool lastModifiedSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.lastModifiedSpecified), info.lastModifiedSpecified); sb.AppendLine();
            //private ResourceAllocationInfo cpuAllocation;
            PrintResourceAllocationInfo(sb, info.cpuAllocation);
            //private ResourceAllocationInfo memoryAllocation;
            PrintResourceAllocationInfo(sb, info.memoryAllocation);
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineDeviceRuntimeInfoVirtualEthernetCardRuntimeState(StringBuilder sb, VirtualMachineDeviceRuntimeInfoVirtualEthernetCardRuntimeState info)
        {
            if (null == info) return;

            //    private bool vmDirectPathGen2Active;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.vmDirectPathGen2Active), info.vmDirectPathGen2Active); sb.AppendLine();
            //private string[] vmDirectPathGen2InactiveReasonVm;
            if(null != info.vmDirectPathGen2InactiveReasonVm)
            {
                foreach(var vmDirectPathGen2InactiveReasonVm in info.vmDirectPathGen2InactiveReasonVm)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(vmDirectPathGen2InactiveReasonVm), vmDirectPathGen2InactiveReasonVm); sb.AppendLine();
                }
            }
            //private string[] vmDirectPathGen2InactiveReasonOther;
            if (null != info.vmDirectPathGen2InactiveReasonOther)
            {
                foreach (var vmDirectPathGen2InactiveReasonOther in info.vmDirectPathGen2InactiveReasonOther)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(vmDirectPathGen2InactiveReasonOther), vmDirectPathGen2InactiveReasonOther); sb.AppendLine();
                }
            }

            //private string vmDirectPathGen2InactiveReasonExtended;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.vmDirectPathGen2InactiveReasonExtended), info.vmDirectPathGen2InactiveReasonExtended); sb.AppendLine();
            //private string reservationStatus;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.reservationStatus), info.reservationStatus); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineDeviceRuntimeInfo(StringBuilder sb, VirtualMachineDeviceRuntimeInfo info)
        {
            if (null == info) return;
            //            private VirtualMachineDeviceRuntimeInfoDeviceRuntimeState runtimeStateField;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.runtimeState), info.runtimeState); sb.AppendLine();
            PrintVirtualMachineDeviceRuntimeInfoVirtualEthernetCardRuntimeState(sb, info.runtimeState as VirtualMachineDeviceRuntimeInfoVirtualEthernetCardRuntimeState);
            //private int keyField;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.key), info.key); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineMessage(StringBuilder sb, VirtualMachineMessage info)
        {
            if (null == info) return;

            //          private string id;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.id), info.id); sb.AppendLine();
            //private object[] argument;
            if (null != info.argument)
            {
                foreach (var argument in info.argument)
                {
                    sb.AppendFormat("{0}\t\"{1}\"", nameof(argument), argument); sb.AppendLine();
                }
            }
            //private string text;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.text), info.text); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }
        public static void PrintVirtualMachineQuestionInfo(StringBuilder sb, VirtualMachineQuestionInfo info)
        {
            if (null == info) return;


            //private string id;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.id), info.id); sb.AppendLine();
            //private string text;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.text), info.text); sb.AppendLine();
            //private ChoiceOption choice;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.choice), info.choice); sb.AppendLine();
            //private VirtualMachineMessage[] message;
            if(null != info.message)
            {
                foreach(var message in info.message)
                {
                    PrintVirtualMachineMessage(sb, message);
                }
            }
        }

        public static void PrintVirtualMachineFeatureRequirement(StringBuilder sb, VirtualMachineFeatureRequirement info)
        {
            if (null == info) return;
            //            private string key;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.key), info.key); sb.AppendLine();
            //private string featureName;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.featureName), info.featureName); sb.AppendLine();
            //private string value;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.value), info.value); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }
        public static void PrintHostFeatureMask(StringBuilder sb, HostFeatureMask info)
        {
            if (null == info) return;

            //    private string key;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.key), info.key); sb.AppendLine();
            //private string featureName;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.featureName), info.featureName); sb.AppendLine();
            //private string value;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.value), info.value); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineRuntimeInfo(StringBuilder sb, VirtualMachineRuntimeInfo info)
        {
            if (null == info) return;

            //    private VirtualMachineDeviceRuntimeInfo[] device;
            if (null != info.device)
            {
                foreach (var device in info.device)
                {
                    PrintVirtualMachineDeviceRuntimeInfo(sb, device);
                }
            }
            //private ManagedObjectReference host;
            PrintMor(sb, info.host); ;
            //private VirtualMachineConnectionState connectionState;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.connectionState), info.connectionState); sb.AppendLine();
            //private VirtualMachinePowerState powerState;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.powerState), info.powerState); sb.AppendLine();
            //private VirtualMachineFaultToleranceState faultToleranceState;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.faultToleranceState), info.faultToleranceState); sb.AppendLine();
            //private bool faultToleranceStateSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.faultToleranceStateSpecified), info.faultToleranceStateSpecified); sb.AppendLine();
            //private VirtualMachineRuntimeInfoDasProtectionState dasVmProtection;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.dasVmProtection), info.dasVmProtection); sb.AppendLine();
            //private bool toolsInstallerMounted;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.toolsInstallerMounted), info.toolsInstallerMounted); sb.AppendLine();
            //private System.DateTime suspendTime;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.suspendTime), info.suspendTime); sb.AppendLine();
            //private bool suspendTimeSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.suspendTimeSpecified), info.suspendTimeSpecified); sb.AppendLine();
            //private System.DateTime bootTime;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.bootTime), info.bootTime); sb.AppendLine();
            //private bool bootTimeSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.bootTimeSpecified), info.bootTimeSpecified); sb.AppendLine();
            //private long suspendInterval;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.suspendInterval), info.suspendInterval); sb.AppendLine();
            //private bool suspendIntervalSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.suspendIntervalSpecified), info.suspendIntervalSpecified); sb.AppendLine();
            //private VirtualMachineQuestionInfo question;
            PrintVirtualMachineQuestionInfo(sb, info.question);
            //private long memoryOverhead;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.memoryOverhead), info.memoryOverhead); sb.AppendLine();
            //private bool memoryOverheadSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.memoryOverheadSpecified), info.memoryOverheadSpecified); sb.AppendLine();
            //private int maxCpuUsage;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.maxCpuUsage), info.maxCpuUsage); sb.AppendLine();
            //private bool maxCpuUsageSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.maxCpuUsageSpecified), info.maxCpuUsageSpecified); sb.AppendLine();
            //private int maxMemoryUsage;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.maxMemoryUsage), info.maxMemoryUsage); sb.AppendLine();
            //private bool maxMemoryUsageSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.maxMemoryUsageSpecified), info.maxMemoryUsageSpecified); sb.AppendLine();
            //private int numMksConnections;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.numMksConnections), info.numMksConnections); sb.AppendLine();
            //private VirtualMachineRecordReplayState recordReplayState;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.recordReplayState), info.recordReplayState); sb.AppendLine();
            //private bool recordReplayStateSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.recordReplayStateSpecified), info.recordReplayStateSpecified); sb.AppendLine();
            //private bool cleanPowerOff;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.cleanPowerOff), info.cleanPowerOff); sb.AppendLine();
            //private bool cleanPowerOffSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.cleanPowerOffSpecified), info.cleanPowerOffSpecified); sb.AppendLine();
            //private string needSecondaryReason;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.needSecondaryReason), info.needSecondaryReason); sb.AppendLine();
            //private bool onlineStandby;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.onlineStandby), info.onlineStandby); sb.AppendLine();
            //private bool onlineStandbySpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.onlineStandbySpecified), info.onlineStandbySpecified); sb.AppendLine();
            //private string minRequiredEVCModeKey;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.minRequiredEVCModeKey), info.minRequiredEVCModeKey); sb.AppendLine();
            //private bool consolidationNeeded;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.consolidationNeeded), info.consolidationNeeded); sb.AppendLine();
            //private bool consolidationNeededSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.consolidationNeededSpecified), info.consolidationNeededSpecified); sb.AppendLine();
            //private VirtualMachineFeatureRequirement[] offlineFeatureRequirement;
            if (null != info.offlineFeatureRequirement)
            {
                foreach (var offlineFeatureRequirement in info.offlineFeatureRequirement)
                {
                    PrintVirtualMachineFeatureRequirement(sb, offlineFeatureRequirement);
                }
            }
            //private VirtualMachineFeatureRequirement[] featureRequirement;
            if (null != info.featureRequirement)
            {
                foreach (var featureRequirement in info.featureRequirement)
                {
                    PrintVirtualMachineFeatureRequirement(sb, featureRequirement);
                }
            }
            //private HostFeatureMask[] featureMask;
            if (null != info.featureMask)
            {
                foreach (var featureMask in info.featureMask)
                {
                    PrintHostFeatureMask(sb, featureMask);
                }
            }
            //private long vFlashCacheAllocation;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.vFlashCacheAllocation), info.vFlashCacheAllocation); sb.AppendLine();
            //private bool vFlashCacheAllocationSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.vFlashCacheAllocationSpecified), info.vFlashCacheAllocationSpecified); sb.AppendLine();
            //private bool paused;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.paused), info.paused); sb.AppendLine();
            //private bool pausedSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.pausedSpecified), info.pausedSpecified); sb.AppendLine();
            //private bool snapshotInBackground;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.snapshotInBackground), info.snapshotInBackground); sb.AppendLine();
            //private bool snapshotInBackgroundSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.snapshotInBackgroundSpecified), info.snapshotInBackgroundSpecified); sb.AppendLine();
            //private bool quiescedForkParent;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.quiescedForkParent), info.quiescedForkParent); sb.AppendLine();
            //private bool quiescedForkParentSpecified;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.quiescedForkParentSpecified), info.quiescedForkParentSpecified); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineUsageOnDatastore(StringBuilder sb, VirtualMachineUsageOnDatastore info)
        {
            if (null == info) return;


            //private ManagedObjectReference datastore;
            PrintMor(sb, info.datastore);
            //private long committed;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.committed), info.committed); sb.AppendLine();
            //private long uncommitted;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.uncommitted), info.uncommitted); sb.AppendLine();
            //private long unshared;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.unshared), info.unshared); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }

        public static void PrintVirtualMachineStorageInfo(StringBuilder sb, VirtualMachineStorageInfo info)
        {
            if (null == info) return;
        //        private VirtualMachineUsageOnDatastore[] perDatastoreUsage;
        if(null != info.perDatastoreUsage)
            {
                foreach(var perDatastoreUsage in info.perDatastoreUsage)
                {
                    PrintVirtualMachineUsageOnDatastore(sb, perDatastoreUsage);
                }
            }

            //private System.DateTime timestamp;
            sb.AppendFormat("{0}\t\"{1}\"", nameof(info.timestamp), info.timestamp); sb.AppendLine();
            sb.AppendLine();
            sb.AppendLine();
        }
#endif
    }



    class VimFinder
    {
        private readonly VimPortTypeClient m_vimService;
        //private readonly ManagedObjectReference m_morPropCollector;
        private readonly ManagedObjectReference m_morRootFolder;
        private readonly VimPropCollector m_propCollecor;

        public VimFinder(VimPortTypeClient vimService, ManagedObjectReference morPropCollector, ManagedObjectReference morRootFolder)
        {
            m_vimService = vimService;
            //m_morPropCollector = morPropCollector;
            m_morRootFolder = morRootFolder;
            m_propCollecor = new VimPropCollector(m_vimService, morPropCollector);
        }

        public void FindDatacenters(List<ManagedObjectReference> morDatacenters)
        {
            SearchFolder(m_morRootFolder, morDatacenters);
        }

        private void SearchFolder(ManagedObjectReference morFolder, List<ManagedObjectReference> morDatacenters)
        {
            ManagedObjectReference[] morChildren = (ManagedObjectReference[])m_propCollecor.Get(morFolder, "childEntity");
            if (null != morChildren)
            {
                foreach (var child in morChildren)
                {
                    if (0 == string.Compare("Datacenter", child.type, StringComparison.InvariantCultureIgnoreCase))
                    {
                        morDatacenters.Add(child);
                        SearchDatacenter(child, morDatacenters); //no need to call this because datacenter will not contain data centers. leave it here because no harm
                    }
                    else if (0 == string.Compare("Folder", child.type, StringComparison.InvariantCultureIgnoreCase))
                    {
                        SearchFolder(child, morDatacenters);
                    }
                }
            }

        }

        private void SearchDatacenter(ManagedObjectReference morDatacenter, List<ManagedObjectReference> morDatacenters)
        {
            ManagedObjectReference morhostFolder = (ManagedObjectReference)m_propCollecor.Get(morDatacenter, "hostFolder");
            if (null != morhostFolder)
            {
                ManagedObjectReference[] morChildren = (ManagedObjectReference[])m_propCollecor.Get(morhostFolder, "childEntity");
                if (null != morChildren)
                {
                    foreach (var child in morChildren)
                    {
                        if (0 == string.Compare("Datacenter", child.type, StringComparison.InvariantCultureIgnoreCase))
                        {
                            morDatacenters.Add(child);
                            SearchDatacenter(child, morDatacenters);
                        }
                        else if (0 == string.Compare("Folder", child.type, StringComparison.InvariantCultureIgnoreCase))
                        {
                            SearchFolder(child, morDatacenters);
                        }
                    }
                }

            }
        }
    }
    class VimPropCollector
    {
        private readonly VimPortTypeClient m_vimService;
        private readonly ManagedObjectReference m_morPropCollector;
        //private ManagedObjectReference m_morObject;

        public VimPropCollector(VimPortTypeClient vimService, ManagedObjectReference morPropCollector)
        {
            this.m_vimService = vimService;
            this.m_morPropCollector = morPropCollector;
        }

        public object Get(ManagedObjectReference morObject, string propName)
        {
            return GetDynamicProperty(morObject, propName);
        }

        private object GetDynamicProperty(ManagedObjectReference morObject, string propertyName)
        {
            ObjectContent[] objContent = GetObjectProperties(morObject, new string[] { propertyName });

            object propertyValue = null;
            if (objContent != null)
            {
                DynamicProperty[] dynamicProperty = objContent[0].propSet;
                if (dynamicProperty != null)
                {
                    object dynamicPropertyVal = dynamicProperty[0].val;
                    string dynamicPropertyName = dynamicPropertyVal.GetType().FullName;
                    propertyValue = dynamicPropertyVal;
                }
            }
            return propertyValue;
        }


        private ObjectContent[] GetObjectProperties(ManagedObjectReference morObject, string[] properties)
        {
            PropertyFilterSpec spec = new PropertyFilterSpec();
            spec.propSet = new PropertySpec[] { new PropertySpec() };
            spec.propSet[0].all = properties == null || properties.Length == 0;
            spec.propSet[0].allSpecified = spec.propSet[0].all;
            spec.propSet[0].type = morObject.type;
            spec.propSet[0].pathSet = properties;

            spec.objectSet = new ObjectSpec[] { new ObjectSpec() };
            spec.objectSet[0].obj = morObject;
            spec.objectSet[0].skip = false;

            return retrievePropertiesEx(new PropertyFilterSpec[] { spec });
        }

        private ObjectContent[] retrievePropertiesEx(PropertyFilterSpec[] specs)
        {
            return retrievePropertiesEx(specs, null);
        }

        private ObjectContent[] retrievePropertiesEx(PropertyFilterSpec[] specs, int? maxObjects)
        {
            List<ObjectContent> objectList = new List<ObjectContent>();
            var result = m_vimService.RetrievePropertiesEx(m_morPropCollector, specs, new RetrieveOptions()
            {
                maxObjects = maxObjects.GetValueOrDefault(),
                maxObjectsSpecified = (maxObjects != null)
            });
            if (result != null)
            {
                string token = result.token;
                objectList.AddRange(result.objects.AsEnumerable());
                while (token != null && !string.Empty.Equals(token))
                {
                    result = m_vimService.ContinueRetrievePropertiesEx(m_morPropCollector, token);
                    if (result != null)
                    {
                        token = result.token;
                        objectList.AddRange(result.objects.AsEnumerable());
                    }
                }
            }
            return objectList.ToArray();
        }
    }


    class VimSnapshotInfo
    {
        private VimPortTypeClient _vimClient;
        private ManagedObjectReference _morePropCollector;
        private ManagedObjectReference _morVM;
        private VimPropCollector _pc;

        private StringBuilder _sb;
        private StringWriter _sw;
        private IList<SnapshotInfo> _snapshots;

        public VimSnapshotInfo(
            VimPortTypeClient vimClient,
            ManagedObjectReference morePropCollector,
            ManagedObjectReference morVM,
            StringBuilder sb,
            IList<SnapshotInfo> snapshots
            )
        {
            _vimClient = vimClient;
            _morePropCollector = morePropCollector;
            _morVM = morVM;
            _pc = new VimPropCollector(_vimClient, _morePropCollector);
            _sb = sb ?? new StringBuilder();
            _sw = new StringWriter(_sb);
            _snapshots = snapshots;
        }

        public void Traverse()
        {
            VirtualMachineSnapshotInfo vmsi = (VirtualMachineSnapshotInfo)_pc.Get(_morVM, "snapshot");

            if (null != vmsi?.currentSnapshot)
            {
                _sb.Append("currentSnapshot:"); _sb.AppendLine();
                //StaticHelpers.PrintMor(_sb, vmsi.currentSnapshot);
                vmsi.currentSnapshot.Dump(nameof(vmsi.currentSnapshot), _sw, false);
                VirtualMachineConfigInfo vmci = (VirtualMachineConfigInfo)_pc.Get(vmsi.currentSnapshot, "config");
                //StaticHelpers.PrintVMCI(_sb, vmci);
                StaticHelpers.PrintVirtualDisksBriefInfo(_sb, vmci, null);
                vmci.Dump(nameof(vmci), _sw, false);
            }


            if (null != vmsi?.rootSnapshotList)
            {
                _sb.Append("rootSnapshotList:"); _sb.AppendLine();
                foreach (var childTree in vmsi.rootSnapshotList)
                {
                    TraverseTreeRecursive(childTree);
                }
            }
        }


        private void TraverseTreeRecursive(VirtualMachineSnapshotTree snapshotTree)
        {
            SnapshotInfo snapshotInfo = new SnapshotInfo();

            //StaticHelpers.PrintVMST(_sb, snapshotTree);
            snapshotTree.Dump(nameof(snapshotTree), _sw, false);
            VirtualMachineConfigInfo vmci = (VirtualMachineConfigInfo)_pc.Get(snapshotTree.snapshot, "config");
            //StaticHelpers.PrintVMCI(_sb, vmci);
            StaticHelpers.PrintVirtualDisksBriefInfo(_sb, vmci, snapshotInfo.DiskPath);
            vmci.Dump(nameof(vmci), _sw, false);
                        
            snapshotInfo.Name = snapshotTree.name;
            snapshotInfo.Desc = snapshotTree.description;
            snapshotInfo.SnapshotRef = snapshotTree.snapshot.Value;

            _snapshots.Add(snapshotInfo);

            if (null != snapshotTree.childSnapshotList)
            {
                foreach (var childTree in snapshotTree.childSnapshotList)
                {
                    _sb.Append("ParentSnapshot:"); _sb.AppendLine();
                    //StaticHelpers.PrintMor(_sb, snapshotTree.snapshot);
                    snapshotTree.snapshot.Dump(nameof(snapshotTree.snapshot), _sw, false);
                    TraverseTreeRecursive(childTree);
                }
            }
        }
    }
}