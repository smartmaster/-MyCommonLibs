#pragma once

#include "../MTReadWrite/counted_ptr.h"
#include "SmlWmi.h"


namespace SML_WMI_FROM_PARSER
{

#include "./generated/sml_wmi_parser.Win32_DiskDrive.hpp"
#include "./generated/sml_wmi_parser.Win32_NetworkAdapter.hpp"
#include "./generated/sml_wmi_parser.Win32_NetworkAdapterConfiguration.hpp"
#include "./generated/sml_wmi_parser.Win32_NetworkAdapterSetting.hpp"
#include "./generated/sml_wmi_parser.Win32_Process.hpp"
#include "./generated/sml_wmi_parser.Win32_ProcessStartup.hpp"

#include "./generated/sml_wmi_parser.MSFT_SmbShare.hpp"
#include "./generated/sml_wmi_parser.MSFT_SmbMultichannelConstraint.hpp"
#include "./generated/sml_wmi_parser.MSFT_SmbConnection.hpp"


#include "./generated/sml_wmi_parser.MSFT_Disk.hpp"
#include "./generated/sml_wmi_parser.MSFT_DiskImage.hpp"
#include "./generated/sml_wmi_parser.MSFT_Partition.hpp"
#include "./generated/sml_wmi_parser.MSFT_PhysicalDisk.hpp"
#include "./generated/sml_wmi_parser.MSFT_VirtualDisk.hpp"
#include "./generated/sml_wmi_parser.MSFT_Volume.hpp"
#include "./generated/sml_wmi_parser.MSFT_PartitionToVolume.hpp"

#include "./generated/sml_wmi_parser.Win32_ShadowCopy.hpp"
#include "./generated/sml_wmi_parser.Win32_ShadowProvider.hpp"



	class SmlWmiParserTest
	{
	public:
		static void CaseVSS()
		{
			LPCTSTR wmins = L"Root\\cimv2";
			SmartLib::counted_ptr<SmartLib::SmlWmi> wmi = SmartLib::counted_ptr<SmartLib::SmlWmi>::make(nullptr, nullptr, nullptr, nullptr, wmins);
			wmi->Connect();

			{
				SML_Win32_ShadowCopy wmiclass;
				wmiclass._smlwmi = wmi;
				

				SML_Win32_ShadowCopy::Create_InParams in;
				in.Volume = TEXT("F:\\");
				in.Context = TEXT("ClientAccessible");
				SML_Win32_ShadowCopy::Create_OutResturns out;
				wmiclass.Create(nullptr, in, out);

				std::vector<CComPtr<IWbemClassObject>> objvec;
				std::vector<SML_Win32_ShadowCopy::SML_Win32_ShadowCopy_Props> propsvec;
				wmiclass.Enum(nullptr, objvec, propsvec);
			}

			{
				SML_Win32_ShadowProvider wmiclass;
				wmiclass._smlwmi = wmi;

				std::vector<CComPtr<IWbemClassObject>> objvec;
				std::vector<SML_Win32_ShadowProvider::SML_Win32_ShadowProvider_Props> propsvec;
				wmiclass.Enum(nullptr, objvec, propsvec);
			}

		}

		static void CaseStorage()
		{
			LPCTSTR wmins = L"Root\\Microsoft\\Windows\\Storage";
			SmartLib::counted_ptr<SmartLib::SmlWmi> wmi = SmartLib::counted_ptr<SmartLib::SmlWmi>::make(nullptr, nullptr, nullptr, nullptr, wmins);
			wmi->Connect();

			{
				SML_MSFT_Disk wmiclass;
				wmiclass._smlwmi = wmi;

				std::vector<CComPtr<IWbemClassObject>> objvec;
				std::vector<SML_MSFT_Disk::SML_MSFT_Disk_Props> propsvec;
				wmiclass.Enum(nullptr, objvec, propsvec);
			}

			{
				SML_MSFT_DiskImage wmiclass;
				wmiclass._smlwmi = wmi;

				std::vector<CComPtr<IWbemClassObject>> objvec;
				std::vector<SML_MSFT_DiskImage::SML_MSFT_DiskImage_Props> propsvec;
				wmiclass.Enum(nullptr, objvec, propsvec);
			}


			{
				SML_MSFT_PhysicalDisk wmiclass;
				wmiclass._smlwmi = wmi;

				std::vector<CComPtr<IWbemClassObject>> objvec;
				std::vector<SML_MSFT_PhysicalDisk::SML_MSFT_PhysicalDisk_Props> propsvec;
				wmiclass.Enum(nullptr, objvec, propsvec);
			}

			{
				SML_MSFT_VirtualDisk wmiclass;
				wmiclass._smlwmi = wmi;

				std::vector<CComPtr<IWbemClassObject>> objvec;
				std::vector<SML_MSFT_VirtualDisk::SML_MSFT_VirtualDisk_Props> propsvec;
				wmiclass.Enum(nullptr, objvec, propsvec);
			}


			{
				SML_MSFT_Partition wmiclass;
				wmiclass._smlwmi = wmi;

				std::vector<CComPtr<IWbemClassObject>> objvec;
				std::vector<SML_MSFT_Partition::SML_MSFT_Partition_Props> propsvec;
				wmiclass.Enum(nullptr, objvec, propsvec);
			}

			{
				SML_MSFT_Volume wmiclass;
				wmiclass._smlwmi = wmi;

				std::vector<CComPtr<IWbemClassObject>> objvec;
				std::vector<SML_MSFT_Volume::SML_MSFT_Volume_Props> propsvec;
				wmiclass.Enum(nullptr, objvec, propsvec);
			}


			{
				SML_MSFT_PartitionToVolume wmiclass;
				wmiclass._smlwmi = wmi;

				std::vector<CComPtr<IWbemClassObject>> objvec;
				std::vector<SML_MSFT_PartitionToVolume::SML_MSFT_PartitionToVolume_Props> propsvec;
				wmiclass.Enum(nullptr, objvec, propsvec);

			}

			
		}

		static void CaseTestGenerated()
		{
			//////////////////////////////////////////////////////////////////////////
			LPCTSTR wmins = L"root\\cimv2";
			SmartLib::counted_ptr<SmartLib::SmlWmi> wmi = SmartLib::counted_ptr<SmartLib::SmlWmi>::make(nullptr, nullptr, nullptr, nullptr, wmins);
			wmi->Connect();

			//////////////////////////////////////////////////////////////////////////
			SML_Win32_Process wmiprocess;
			wmiprocess._smlwmi = wmi;

			//////////////////////////////////////////////////////////////////////////
			std::vector<CComPtr<IWbemClassObject>> objects;
			std::vector<SML_Win32_Process::SML_Win32_Process_Props> props;
			wmiprocess.Enum(nullptr, objects, props);

			//////////////////////////////////////////////////////////////////////////
			SML_Win32_Process::Create_InParams create_InParams;
			create_InParams.CommandLine = TEXT("notepad.exe");

			SML_Win32_ProcessStartup wmips;
			wmips._smlwmi = wmi;
			SML_Win32_ProcessStartup::SML_Win32_ProcessStartup_Props procstartup;
			procstartup.X = 100;
			procstartup.XSize = 101;
			procstartup.Y = 200;
			procstartup.YSize = 201;
			wmips.NewObj(procstartup, create_InParams.ProcessStartupInformation);

			SML_Win32_Process::Create_OutResturns create_OutResturns;
			wmiprocess.Create(nullptr, create_InParams, create_OutResturns);

			//////////////////////////////////////////////////////////////////////////
			CString strwhere;
			strwhere.Format(TEXT("ProcessId = %d"), create_OutResturns.ProcessId.intVal);
			std::vector<CComPtr<IWbemClassObject>> foundobjs;
			std::vector<SML_Win32_Process::SML_Win32_Process_Props> foundprops;
			wmiprocess.Enum(strwhere.GetString(), foundobjs, foundprops);
			if (foundprops.size())
			{
				SML_Win32_Process::Terminate_InParams terminate_InParams;
				SML_Win32_Process::Terminate_OutResturns terminate_OutResturns;
				terminate_InParams.Reason = 1234;
				wmiprocess.Terminate(foundobjs[0], terminate_InParams, terminate_OutResturns);
			}

			//////////////////////////////////////////////////////////////////////////
			{
				SML_Win32_NetworkAdapter wmiNic;
				wmiNic._smlwmi = wmi;
				std::vector<CComPtr<IWbemClassObject>> nicobjects;
				std::vector<SML_Win32_NetworkAdapter::SML_Win32_NetworkAdapter_Props> nicprops;
				wmiNic.Enum(L"NetEnabled = True", nicobjects, nicprops);

				SML_Win32_NetworkAdapterConfiguration wminicconf;
				wminicconf._smlwmi = wmi;
				for (int ii = 0; ii < nicprops.size(); ++ii)
				{
					auto& nic = nicprops[ii];

					std::vector<CComPtr<IWbemClassObject>> nicConfObj;
					std::vector<SML_Win32_NetworkAdapterConfiguration::SML_Win32_NetworkAdapterConfiguration_Props> nicConfs;
					wminicconf.EnumAssociators(wmiNic.CLASS_NAME, L"DeviceID", nic.DeviceID.bstrVal, L"Win32_NetworkAdapterSetting", nicConfObj, nicConfs);
				}
			}
			



		}
	};
}
