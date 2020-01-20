#pragma once
#include "SmlWmi.h"


namespace SmartLib
{
	class SmlWmiTest
	{
	public:
		static void Case0()
		{
			SmlWmi smlwmi(
				L"10.57.18.2",//LPCTSTR Machine,
				L"administrator",//LPCTSTR User,
				L"10.57.18.2",//LPCTSTR Domain,
				L"Master16516336",//LPCTSTR Password
				L"root\\cimv2"
			);
			smlwmi.Connect();

			{
				CComPtr<IWbemClassObject> spnic;
				SmlWmi::KVSet nicprops;
				smlwmi.GetClass(L"Win32_NetworkAdapter", &spnic, &nicprops);

				std::vector<SmlWmi::MethodInfo> mis;
				smlwmi.EnumMethods(L"Win32_NetworkAdapter", mis);
			}
			
			std::vector<CComPtr<IWbemClassObject>> objects;
			SmlWmi::KVSetVec vecProps;
			smlwmi.EnumInstances(L"Win32_NetworkAdapter", L"NetEnabled = True", objects, vecProps);

			for (auto& props : vecProps)
			{
				std::vector<CComPtr<IWbemClassObject>> assocs;
				SmlWmi::KVSetVec vecAssocProps;
				smlwmi.EnumAssociators(
					L"Win32_NetworkAdapter",//IN LPCTSTR parentClass,
					L"DeviceID",//IN LPCTSTR propName,
					props[L"DeviceID"].bstrVal,//IN LPCTSTR propValue,
					L"Win32_NetworkAdapterSetting",//IN LPCTSTR AssocClass,
					assocs,
					vecAssocProps
				);
			}

			//////////////////////////////////////////////////////////////////////////
			SmlWmi::KVSet props =
			{
				{L"CreateFlags", 8},
				{L"X", 100},
				{L"XSize", 101},
				{L"Y", 200},
				{L"YSize", 201},
			};

			{
				CComVariant newObj;
				smlwmi.NewObject(L"Win32_ProcessStartup", props, newObj);
				SmlWmi::KVSet newObjProps;
				SmlWmi::Helpers::GetProp((IWbemClassObject*)newObj.punkVal, newObjProps);
			}


			//////////////////////////////////////////////////////////////////////////
			std::vector<SmlWmi::MethodInfo> mis;
			smlwmi.EnumMethods(L"Win32_Process", mis);

			SmlWmi::KVSet psprop =
			{
				{L"WinstationDesktop", L""},
				{L"ShowWindow", (BYTE)(SW_SHOWMAXIMIZED)},
				{L"Title", TEXT("test wmi")},
			};
			CComVariant psvar;
			smlwmi.NewObject(L"Win32_ProcessStartup", psprop, psvar);

			SmlWmi::KVSet inparams =
			{
				{L"CommandLine", L"c:\\windows\\system32\\notepad.exe"},
				{L"CurrentDirectory", L"c:\\windows\\system32\\"},
				{L"ProcessStartupInformation", psvar},
			};

			SmlWmi::KVSet outs;
			//CComPtr<IWbemClassObject> spOutRets;
			smlwmi.InvokeMethod(nullptr, TEXT("Win32_Process"), TEXT("Create"), inparams, outs, nullptr);

			{
				CString where;
				where.Format(TEXT("ProcessId = %d"), outs[L"ProcessId"].intVal);
				std::vector<CComPtr<IWbemClassObject>> vecobjs;
				SmlWmi::KVSetVec ps;
				smlwmi.EnumInstances(TEXT("Win32_Process"), where.GetString(), vecobjs, ps);
				SmlWmi::KVSet inparams =
				{
					{TEXT("Reason"), -1},
				};
				SmlWmi::KVSet outs;
				smlwmi.InvokeMethod(vecobjs[0], nullptr, TEXT("Terminate"), inparams, outs, nullptr);

			}

			return;
		}
	};
}

