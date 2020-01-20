
WmiHelpers.exe -wmiparse root\cimv2 Win32_Process  Win32_ProcessStartup Win32_DiskDrive Win32_NetworkAdapter Win32_NetworkAdapterConfiguration Win32_NetworkAdapterSetting

WmiHelpers.exe -wmiparse Root\Microsoft\Windows\Smb MSFT_SmbMultichannelConstraint MSFT_SmbShare MSFT_SmbConnection

WmiHelpers.exe -wmiparse Root\Microsoft\Windows\Storage MSFT_Disk MSFT_DiskImage MSFT_Partition MSFT_PhysicalDisk MSFT_VirtualDisk MSFT_Volume MSFT_PartitionToVolume


WmiHelpers.exe -wmiparse root\cimv2 Win32_ShadowCopy  Win32_ShadowProvider


WmiHelpers.exe -wmiparse Root\Virtualization\v2 Msvm_CollectionSnapshotService Msvm_SnapshotCollection
