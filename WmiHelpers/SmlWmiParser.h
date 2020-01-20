#pragma once


#include "..\PublicHeader\PublicEXPAND_MACRO.h"
#include "SmlWmi.h"

namespace SmartLib
{
	class SmlWmiParser
	{
		class Helper
		{
		public:
			static LPCTSTR NewLine()
			{
				return L"\r\n";
			}
			static VOID KVSet2PropsDesc(SmlWmi::KVSet & kvs, LPCTSTR typeName, LPCTSTR objName, CString & desc)
			{
				if (typeName && typeName[0])
				{
					desc.AppendFormat(TEXT("struct %s%s"), typeName, Helper::NewLine());
					desc.AppendFormat(TEXT("{%s"), Helper::NewLine());
				}

				//CComVariant Name;/r/n
				for (auto iter = kvs.begin(); iter != kvs.end(); ++ iter)
				{
					desc.AppendFormat(TEXT("CComVariant %s;%s"), iter->first.GetString(), NewLine());
				}

				if (typeName && typeName[0])
				{
					desc.AppendFormat(TEXT("} %s;%s"), objName, Helper::NewLine());
				}
			}
		};


	private:
		SmlWmi _wmi;
		HRESULT _hr{ E_FAIL };


	private:
		//struct MyStruct
		//{
		//	CComVariant prop1;
		//	CComVariant prop2;
		//} MyObject;
		HRESULT ParseProps(LPCTSTR className, LPCTSTR typeName, LPCTSTR objName, CString& desc)
		{

			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////

			SmlWmi::KVSet propset;
			CComPtr<IWbemClassObject> spclass;
			hr = _wmi.GetClass(className, &spclass, &propset);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("wmi.Connect"), hr, TEXT("%s"), className);
				LEAVE_BLOCK(0);
			}

			Helper::KVSet2PropsDesc(propset, typeName, objName, desc);


			//////////////////////////////////////////////////////////////////////////
			desc.AppendFormat(TEXT("HRESULT Enum(LPCTSTR wherecond, std::vector<CComPtr<IWbemClassObject>>& objects, std::vector<SML_%s_Props>& props)"), className);
			desc += Helper::NewLine();
			desc += TEXT("{"); desc += Helper::NewLine();
			desc += TEXT("std::vector<::SmartLib::SmlWmi::KVSet> veckvs;"); desc += Helper::NewLine();
			desc += TEXT("HRESULT hr = _smlwmi->EnumInstances(CLASS_NAME, wherecond, objects, veckvs);"); desc += Helper::NewLine();
			desc += Helper::NewLine();
			desc += TEXT("props.resize(veckvs.size());"); desc += Helper::NewLine();
			desc += TEXT("for (int ii = 0; ii< veckvs.size(); ++ ii)"); desc += Helper::NewLine();
			desc += TEXT("{"); desc += Helper::NewLine();
			desc += TEXT("auto& outkvs = veckvs[ii];"); desc += Helper::NewLine();
			desc += TEXT("auto& outReturns = props[ii];"); desc += Helper::NewLine();

			LPCTSTR outformat = SML_STRING_ANY_W(
				if (outkvs[L"%s"].vt != VT_EMPTY)
				{
					outReturns.%s = outkvs[L"%s"];
				});
			for (auto iter = propset.begin(); iter != propset.end(); ++ iter)
			{
				desc.AppendFormat(outformat, iter->first.GetString(), iter->first.GetString(), iter->first.GetString());
				desc += Helper::NewLine();
			}

			desc += TEXT("}"); desc += Helper::NewLine();
			desc += TEXT("return hr;"); desc += Helper::NewLine();
			desc += TEXT("}"); desc += Helper::NewLine();
			desc += Helper::NewLine();

			//////////////////////////////////////////////////////////////////////////
			desc.AppendFormat(TEXT("HRESULT EnumAssociators(LPCTSTR parentClass, LPCTSTR propName, LPCTSTR propValue, LPCTSTR assocClass, std::vector<CComPtr<IWbemClassObject> >& objects, std::vector<SML_%s_Props>& props)"), className);
			desc += Helper::NewLine();
			desc += TEXT("{"); desc += Helper::NewLine();
			desc += TEXT("std::vector<::SmartLib::SmlWmi::KVSet> veckvs;"); desc += Helper::NewLine();
			desc += TEXT("HRESULT hr = _smlwmi->EnumAssociators(parentClass, propName, propValue, assocClass, objects, veckvs);"); desc += Helper::NewLine();
			desc += Helper::NewLine();
			desc += TEXT("props.resize(veckvs.size());"); desc += Helper::NewLine();
			desc += TEXT("for (int ii = 0; ii< veckvs.size(); ++ ii)"); desc += Helper::NewLine();
			desc += TEXT("{"); desc += Helper::NewLine();
			desc += TEXT("auto& outkvs = veckvs[ii];"); desc += Helper::NewLine();
			desc += TEXT("auto& outReturns = props[ii];"); desc += Helper::NewLine();

			for (auto iter = propset.begin(); iter != propset.end(); ++iter)
			{
				desc.AppendFormat(outformat, iter->first.GetString(), iter->first.GetString(), iter->first.GetString());
				desc += Helper::NewLine();
			}

			desc += TEXT("}"); desc += Helper::NewLine();
			desc += TEXT("return hr;"); desc += Helper::NewLine();
			desc += TEXT("}"); desc += Helper::NewLine();
			desc += Helper::NewLine();

			//////////////////////////////////////////////////////////////////////////
			desc.AppendFormat(TEXT("HRESULT NewObj(SML_%s_Props & inParams, CComVariant & var)"), className);
			desc += Helper::NewLine();
			desc += Helper::NewLine();

			desc += TEXT("{"); desc += Helper::NewLine();
			desc += TEXT("::SmartLib::SmlWmi::KVSet inkvs;"); desc += Helper::NewLine();
			LPCTSTR informat = SML_STRING_ANY_W(
				if (inParams.%s.vt != VT_EMPTY)
				{
					inkvs.insert({ L"%s", inParams.%s });
				});

			for (auto iter = propset.begin(); iter != propset.end(); ++iter)
			{
				desc.AppendFormat(informat, iter->first.GetString(), iter->first.GetString(), iter->first.GetString());
				desc += Helper::NewLine();
			}

			desc += TEXT("HRESULT hr = _smlwmi->NewObject(CLASS_NAME, inkvs, var);"); desc += Helper::NewLine();
			desc += TEXT("return hr;"); desc += Helper::NewLine();
			desc += TEXT("}"); desc += Helper::NewLine();
			desc += Helper::NewLine();
			desc += Helper::NewLine();
			

			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}


		HRESULT ParseMethod(SmlWmi::MethodInfo & mi, CString & desc)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CString methodName = mi._name;
			static const LPCTSTR InParamsName = L"_InParams";
			Helper::KVSet2PropsDesc(mi._inParams, methodName + InParamsName, L"", desc);

			static const LPCTSTR OutResturnsName = L"_OutResturns";
			Helper::KVSet2PropsDesc(mi._outReturns, methodName + OutResturnsName, L"", desc);

			desc.AppendFormat(TEXT("HRESULT %s(IWbemClassObject* obj, %s%s& inParams, %s%s& outReturns)%s"),
				methodName.GetString(),
				methodName.GetString(),
				InParamsName,
				methodName.GetString(),
				OutResturnsName,
				Helper::NewLine());
			desc.AppendFormat(TEXT("{%s"), Helper::NewLine());

			//parse function body
			desc += TEXT("::SmartLib::SmlWmi::KVSet inkvs;");
			desc += Helper::NewLine();
			desc += TEXT("::SmartLib::SmlWmi::KVSet outkvs;");
			desc += Helper::NewLine();
			desc += Helper::NewLine();

			LPCTSTR informat = SML_STRING_ANY_W(
			if (inParams.%s.vt != VT_EMPTY)
			{
				inkvs.insert({ L"%s", inParams.%s });
			});

			for (auto & initem : mi._inParams)
			{
				desc.AppendFormat(informat, initem.first.GetString(), initem.first.GetString(), initem.first.GetString());
				desc += Helper::NewLine();
			}

			desc += Helper::NewLine();
			desc += TEXT("CComPtr<IWbemClassObject> spout;"); desc += Helper::NewLine();
			LPCTSTR methodformat = SML_STRING_ANY_W(HRESULT hr = _smlwmi->InvokeMethod(obj, CLASS_NAME, L"%s", inkvs, outkvs, &spout););
			desc.AppendFormat(methodformat, (LPCTSTR)(mi._name));
			desc += Helper::NewLine();
			desc += Helper::NewLine();

			desc += TEXT("if (SUCCEEDED(hr))"); desc += Helper::NewLine();
			desc += TEXT("{"); desc += Helper::NewLine();

			LPCTSTR outformat = SML_STRING_ANY_W(
				if (outkvs[L"%s"].vt != VT_EMPTY)
				{
					outReturns.%s = outkvs[L"%s"];
				});

			for (auto& outitem : mi._outReturns)
			{
				desc.AppendFormat(outformat, outitem.first.GetString(), outitem.first.GetString(), outitem.first.GetString());
				desc += Helper::NewLine();
			}

			desc += TEXT("}"); desc += Helper::NewLine();

			desc += Helper::NewLine();
			desc += Helper::NewLine();

			desc += TEXT("return hr;");
			desc += Helper::NewLine();

			desc.AppendFormat(TEXT("}%s"), Helper::NewLine());
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

	public:
		SmlWmiParser(LPCTSTR wmins):
			_wmi{ nullptr, nullptr, nullptr, nullptr, wmins}
		{
			_hr = _wmi.Connect();
			if (FAILED(_hr))
			{
				D_API_ERR(0, TEXT("wmi.Connect"), _hr, TEXT("%s"), wmins);
				//LEAVE_BLOCK(0);
			}
		}
		HRESULT Parse(/*LPCTSTR wmiNamespace,*/ LPCTSTR className, CString & desc)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			if (FAILED(_hr))
			{
				D_API_ERR(0, TEXT("WMI service not connected"), _hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			LPCTSTR classHeader = SML_STRING_ANY_W(
			class %s %s
			{ %s
			public: %s
			const LPCTSTR CLASS_NAME{ L"%s" }; %s
			SmartLib::counted_ptr<SmartLib::SmlWmi> _smlwmi; %s
			);

			desc.AppendFormat(classHeader, (CString(L"SML_") + className).GetString(), Helper::NewLine(), Helper::NewLine(), Helper::NewLine(), className, Helper::NewLine(), Helper::NewLine());
			desc += Helper::NewLine();
			desc += Helper::NewLine();

			//////////////////////////////////////////////////////////////////////////
			CString propTypeName = L"SML_";
			propTypeName += className;
			propTypeName += L"_Props";
			desc.AppendFormat(TEXT("////////////////////////// Properties: %s //////////////////////////%s"), className, Helper::NewLine());
			hr = ParseProps(className, propTypeName, L"", desc);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ParseProps"), hr, TEXT("%s"), className);
				LEAVE_BLOCK(0);
			}
			desc += Helper::NewLine();
			desc += Helper::NewLine();

			


			//////////////////////////////////////////////////////////////////////////
			std::vector<SmlWmi::MethodInfo> mis;
			hr = _wmi.EnumMethods(className, mis);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("EnumMethods"), hr, TEXT("%s"), className);
				LEAVE_BLOCK(0);
			}

			for (int ii = 0; ii < mis.size(); ++ ii)
			{
				desc.AppendFormat(TEXT("////////////////////////// Method: %s ///////////////////////////%s"), (LPCTSTR)(mis[ii]._name), Helper::NewLine());
				ParseMethod(mis[ii], desc);
				desc += Helper::NewLine();
				desc += Helper::NewLine();
			}


			desc += TEXT("};");
			desc += Helper::NewLine();
			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}
	};
}