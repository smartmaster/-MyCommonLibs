#pragma once

#include <memory>
#include <vector>
using namespace std;

#include "SmlWmi.h"

namespace SmartLib
{
	class SMbContraint
	{
	public:
		struct NicIpConfig
		{
			UINT32 _InterfaceIndex;
			CString _NetConnectionID;
			CString _MACAddress;
			std::vector<CString> _IPAddress;
			std::vector<CString> _IPSubnet;
		};
	private:
		std::shared_ptr<SmlWmi> _wmi;
		std::shared_ptr<SmlWmi> _wmismb;

	private:
		static LPCTSTR Cimv2Namespace()
		{
			return L"root\\cimv2";
		}

		static LPCTSTR SmbNamespace()
		{
			return L"Root\\Microsoft\\Windows\\SMB";
		}
	public:

		SMbContraint(
			LPCTSTR Machine,
			LPCTSTR User,
			LPCTSTR Domain,
			LPCTSTR Password) 
		{
			_wmi = std::make_shared<SmlWmi>(Machine, User, Domain, Password, Cimv2Namespace());
			_wmismb = std::make_shared<SmlWmi>(Machine, User, Domain, Password, SmbNamespace());
		}


		HRESULT Connect()
		{
			HRESULT hr = _wmi->Connect();
			hr = _wmismb->Connect();
			return hr;
		}

		static int FindNic(const std::vector<NicIpConfig> & nics, LPCTSTR ipaddr)
		{
			
			for (int ii = 0; ii < nics.size(); ++ii)
			{
				for (int jj = 0; jj < nics[ii]._IPAddress.size(); ++jj)
				{
					if (0 == _tcsicmp(ipaddr, nics[ii]._IPAddress[jj].GetString()))
					{
						return ii;
					}
				}
			}

			return -1;
		}

		HRESULT CreateSmbConstraint(LPCTSTR localIP, LPCTSTR smbServer, SmlWmi::KVSetVec & smbconstraits)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			std::vector<NicIpConfig> nics;
			hr = GetNetworkAdapters(nics);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}

			int index = FindNic(nics, localIP);
			if (index < 0)
			{
				LEAVE_BLOCK(0);
			}
			
			CComVariant varInterfaceIndex;
			std::vector<int> vecInterfaceIndex{ (int)nics[index]._InterfaceIndex };
			SmlWmi::Helpers::Vector2Var<int, int>(vecInterfaceIndex, varInterfaceIndex);


			CComVariant varInterfaceAlias;
			std::vector<CString> vecInterfaceAlias{ nics[index]._NetConnectionID };
			SmlWmi::Helpers::Vector2Var<CString, BSTR>(vecInterfaceAlias, varInterfaceAlias);
			
			SmlWmi::KVSet params =
			{
				{TEXT("ServerName"), smbServer},
				{TEXT("InterfaceIndex"), varInterfaceIndex},
				//{TEXT("InterfaceAlias"), varInterfaceAlias},
			};
			SmlWmi::KVSet outs;
			hr = _wmismb->InvokeMethod(nullptr, TEXT("MSFT_SmbMultichannelConstraint"), TEXT("CreateConstraint"), params, outs, nullptr);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}

			//CComSafeArray<IUnknown*> sa = outs[L"Output"].parray;
			//for (int ii = 0; ii < sa.GetCount(); ++ ii)
			//{
			//	smbconstraits.resize(smbconstraits.size() + 1);
			//	SmlWmi::Helpers::GetProp((IWbemClassObject*)(sa[ii].p), smbconstraits.back());
			//}
			std::vector<CComPtr<IWbemClassObject>> vecout;
			SmlWmi::Helpers::Var2Vector<CComPtr<IWbemClassObject>, IUnknown*>(outs[L"Output"], vecout);
			SmlWmi::Helpers::Objects2Props(vecout, smbconstraits);
			

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
			
		}


		HRESULT DeleteConstrait(LPCTSTR path)
		{
			return _wmismb->DeleteObject(path);
		}

		
		//select * from Win32_NetworkAdapter
		HRESULT GetNetworkAdapters(vector<NicIpConfig> & nicipconfigs)
		{
			vector<CComPtr<IWbemClassObject>> vecInstances;
			SmlWmi::KVSetVec nicsprop;
			HRESULT hr = _wmi->EnumInstances(L"Win32_NetworkAdapter", L"NetEnabled = True", vecInstances, nicsprop);

			for (int ii = 0; ii < vecInstances.size(); ++ ii)
			{
				NicIpConfig nicconf;

				nicconf._InterfaceIndex = nicsprop[ii][L"InterfaceIndex"].uintVal;
				nicconf._NetConnectionID = nicsprop[ii][L"NetConnectionID"].bstrVal;
				nicconf._MACAddress = nicsprop[ii][L"MACAddress"].bstrVal;

				vector<CComPtr<IWbemClassObject> > vecNetworkAdapterSetting;
				SmlWmi::KVSetVec confprops;
				hr = _wmi->EnumAssociators(L"Win32_NetworkAdapter", L"DeviceID", nicsprop[ii][L"DeviceID"].bstrVal, L"Win32_NetworkAdapterSetting", vecNetworkAdapterSetting, confprops);
				for (int jj = 0; jj < vecNetworkAdapterSetting.size(); ++ jj)
				{
					if (!confprops[jj][L"IPEnabled"].boolVal)
					{
						continue;
					}

					if (0 == nicconf._IPAddress.size())
					{
						SmlWmi::Helpers::Var2Vector<CString, BSTR>(confprops[jj][L"IPAddress"], nicconf._IPAddress);
						SmlWmi::Helpers::Var2Vector<CString, BSTR>(confprops[jj][L"IPSubnet"], nicconf._IPSubnet);
					}
					else
					{
						vector<CString> IPAddress;
						vector<CString> IPSubnet;

						SmlWmi::Helpers::Var2Vector<CString, BSTR>(confprops[jj][L"IPAddress"], IPAddress);
						SmlWmi::Helpers::Var2Vector<CString, BSTR>(confprops[jj][L"IPSubnet"], IPSubnet);

						for (auto& ip : IPAddress)
						{
							nicconf._IPAddress.push_back(ip);
						}

						for (auto& subnet : IPSubnet)
						{
							nicconf._IPSubnet.push_back(subnet);
						}
					}
					
				}
			
				if (nicconf._IPAddress.size())
				{
					nicipconfigs.push_back(nicconf);
				}
			
			}

			return hr;
		}

		HRESULT CreateShare(LPCTSTR name, LPCTSTR path, const std::vector<CString>& users, SmlWmi::KVSet& sharePros)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CComVariant varUsers;
			SmlWmi::Helpers::Vector2Var<CString, BSTR>(users, varUsers);

			SmlWmi::KVSet params =
			{
				{L"Name", name},
				{L"Path", path},
				{L"FullAccess", varUsers},
				//{L"FolderEnumerationMode", 1},

			};

			SmlWmi::KVSet outs;
			hr = _wmismb->InvokeMethod(nullptr, TEXT("MSFT_SmbShare"), TEXT("CreateShare"), params, outs, nullptr);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}
			
			//////////////////////////////////////////////////////////////////////////
			hr = SmlWmi::Helpers::GetProp((IWbemClassObject*)outs[L"CreatedShare"].punkVal, sharePros);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}
	};

	class SMbContraintTest
	{
	public:

		static void Casse1()
		{
			SMbContraint smbcons(
				nullptr,//LPCTSTR Machine,
				nullptr,//LPCTSTR User,
				nullptr,//LPCTSTR Domain,
				nullptr//LPCTSTR Password
			);
			smbcons.Connect();

			//vector< SMbContraint::NicIpConfig> nics;
			//smbcons.GetNetworkAdapters(nics);
			SmlWmi::KVSetVec smbconstraits;
			smbcons.CreateSmbConstraint(L"10.57.10.25", L"10.57.18.2", smbconstraits);
			for (auto& cons : smbconstraits)
			{
				smbcons.DeleteConstrait(cons[L"__PATH"].bstrVal);
			}
		}


		static void Casse0()
		{
			HRESULT hr = S_OK;
			
			SMbContraint smbcons(
				L"10.57.18.2",//LPCTSTR Machine,
				L"administrator",//LPCTSTR User,
				L"10.57.18.2",//LPCTSTR Domain,
				L"Master16516336"//LPCTSTR Password
			);
			smbcons.Connect();

			vector< SMbContraint::NicIpConfig> nics;
			smbcons.GetNetworkAdapters(nics);
			SmlWmi::KVSetVec smbconstraits;
			smbcons.CreateSmbConstraint(nics[0]._IPAddress[0], L"www.baidu.com", smbconstraits);
			for (auto & cons : smbconstraits)
			{
				smbcons.DeleteConstrait(cons[L"__PATH"].bstrVal);
			}

			SmlWmi::KVSet shareprops;
			hr = smbcons.CreateShare(L"shareCreatedByWMI", L"D:\\zzz-temp", std::vector<CString>{L"everyone", L"administrators", L"administrator"}, shareprops);
			
			hr = smbcons.DeleteConstrait(shareprops[L"__PATH"].bstrVal);

		}


		
	};
}