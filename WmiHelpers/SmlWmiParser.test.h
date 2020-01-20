#pragma once
#include "SmlWmiParser.h"
#include "MyFormatCode.h"
#include "../MTReadWrite/counted_ptr.h"

namespace SmartLib
{
	class SML_Win32_Process
	{
	public:
		const LPCTSTR CLASS_NAME{ L"Win32_Process" };
		SmartLib::counted_ptr<SmartLib::SmlWmi> _smlwmi;


		////////////////////////// Properties: Win32_Process //////////////////////////
		struct SML_Win32_Process_Props
		{
			CComVariant CSCreationClassName;
			CComVariant CSName;
			CComVariant Caption;
			CComVariant CommandLine;
			CComVariant CreationClassName;
			CComVariant CreationDate;
			CComVariant Description;
			CComVariant ExecutablePath;
			CComVariant ExecutionState;
			CComVariant Handle;
			CComVariant HandleCount;
			CComVariant InstallDate;
			CComVariant KernelModeTime;
			CComVariant MaximumWorkingSetSize;
			CComVariant MinimumWorkingSetSize;
			CComVariant Name;
			CComVariant OSCreationClassName;
			CComVariant OSName;
			CComVariant OtherOperationCount;
			CComVariant OtherTransferCount;
			CComVariant PageFaults;
			CComVariant PageFileUsage;
			CComVariant ParentProcessId;
			CComVariant PeakPageFileUsage;
			CComVariant PeakVirtualSize;
			CComVariant PeakWorkingSetSize;
			CComVariant Priority;
			CComVariant PrivatePageCount;
			CComVariant ProcessId;
			CComVariant QuotaNonPagedPoolUsage;
			CComVariant QuotaPagedPoolUsage;
			CComVariant QuotaPeakNonPagedPoolUsage;
			CComVariant QuotaPeakPagedPoolUsage;
			CComVariant ReadOperationCount;
			CComVariant ReadTransferCount;
			CComVariant SessionId;
			CComVariant Status;
			CComVariant TerminationDate;
			CComVariant ThreadCount;
			CComVariant UserModeTime;
			CComVariant VirtualSize;
			CComVariant WindowsVersion;
			CComVariant WorkingSetSize;
			CComVariant WriteOperationCount;
			CComVariant WriteTransferCount;
			CComVariant __CLASS;
			CComVariant __DERIVATION;
			CComVariant __DYNASTY;
			CComVariant __GENUS;
			CComVariant __NAMESPACE;
			CComVariant __PATH;
			CComVariant __PROPERTY_COUNT;
			CComVariant __RELPATH;
			CComVariant __SERVER;
			CComVariant __SUPERCLASS;
		};
		HRESULT Enum(LPCTSTR wherecond, std::vector<CComPtr<IWbemClassObject>>& objects, std::vector<SML_Win32_Process_Props>& props)
		{
			std::vector<SmlWmi::KVSet> veckvs;
			HRESULT hr = _smlwmi->EnumInstances(CLASS_NAME, wherecond, objects, veckvs);

			props.resize(veckvs.size());
			for (int ii = 0; ii < veckvs.size(); ++ii)
			{
				auto& outkvs = veckvs[ii];
				auto& outReturns = props[ii];
				if (outkvs[L"CSCreationClassName"].vt != VT_EMPTY) { outReturns.CSCreationClassName = outkvs[L"CSCreationClassName"]; }
				if (outkvs[L"CSName"].vt != VT_EMPTY) { outReturns.CSName = outkvs[L"CSName"]; }
				if (outkvs[L"Caption"].vt != VT_EMPTY) { outReturns.Caption = outkvs[L"Caption"]; }
				if (outkvs[L"CommandLine"].vt != VT_EMPTY) { outReturns.CommandLine = outkvs[L"CommandLine"]; }
				if (outkvs[L"CreationClassName"].vt != VT_EMPTY) { outReturns.CreationClassName = outkvs[L"CreationClassName"]; }
				if (outkvs[L"CreationDate"].vt != VT_EMPTY) { outReturns.CreationDate = outkvs[L"CreationDate"]; }
				if (outkvs[L"Description"].vt != VT_EMPTY) { outReturns.Description = outkvs[L"Description"]; }
				if (outkvs[L"ExecutablePath"].vt != VT_EMPTY) { outReturns.ExecutablePath = outkvs[L"ExecutablePath"]; }
				if (outkvs[L"ExecutionState"].vt != VT_EMPTY) { outReturns.ExecutionState = outkvs[L"ExecutionState"]; }
				if (outkvs[L"Handle"].vt != VT_EMPTY) { outReturns.Handle = outkvs[L"Handle"]; }
				if (outkvs[L"HandleCount"].vt != VT_EMPTY) { outReturns.HandleCount = outkvs[L"HandleCount"]; }
				if (outkvs[L"InstallDate"].vt != VT_EMPTY) { outReturns.InstallDate = outkvs[L"InstallDate"]; }
				if (outkvs[L"KernelModeTime"].vt != VT_EMPTY) { outReturns.KernelModeTime = outkvs[L"KernelModeTime"]; }
				if (outkvs[L"MaximumWorkingSetSize"].vt != VT_EMPTY) { outReturns.MaximumWorkingSetSize = outkvs[L"MaximumWorkingSetSize"]; }
				if (outkvs[L"MinimumWorkingSetSize"].vt != VT_EMPTY) { outReturns.MinimumWorkingSetSize = outkvs[L"MinimumWorkingSetSize"]; }
				if (outkvs[L"Name"].vt != VT_EMPTY) { outReturns.Name = outkvs[L"Name"]; }
				if (outkvs[L"OSCreationClassName"].vt != VT_EMPTY) { outReturns.OSCreationClassName = outkvs[L"OSCreationClassName"]; }
				if (outkvs[L"OSName"].vt != VT_EMPTY) { outReturns.OSName = outkvs[L"OSName"]; }
				if (outkvs[L"OtherOperationCount"].vt != VT_EMPTY) { outReturns.OtherOperationCount = outkvs[L"OtherOperationCount"]; }
				if (outkvs[L"OtherTransferCount"].vt != VT_EMPTY) { outReturns.OtherTransferCount = outkvs[L"OtherTransferCount"]; }
				if (outkvs[L"PageFaults"].vt != VT_EMPTY) { outReturns.PageFaults = outkvs[L"PageFaults"]; }
				if (outkvs[L"PageFileUsage"].vt != VT_EMPTY) { outReturns.PageFileUsage = outkvs[L"PageFileUsage"]; }
				if (outkvs[L"ParentProcessId"].vt != VT_EMPTY) { outReturns.ParentProcessId = outkvs[L"ParentProcessId"]; }
				if (outkvs[L"PeakPageFileUsage"].vt != VT_EMPTY) { outReturns.PeakPageFileUsage = outkvs[L"PeakPageFileUsage"]; }
				if (outkvs[L"PeakVirtualSize"].vt != VT_EMPTY) { outReturns.PeakVirtualSize = outkvs[L"PeakVirtualSize"]; }
				if (outkvs[L"PeakWorkingSetSize"].vt != VT_EMPTY) { outReturns.PeakWorkingSetSize = outkvs[L"PeakWorkingSetSize"]; }
				if (outkvs[L"Priority"].vt != VT_EMPTY) { outReturns.Priority = outkvs[L"Priority"]; }
				if (outkvs[L"PrivatePageCount"].vt != VT_EMPTY) { outReturns.PrivatePageCount = outkvs[L"PrivatePageCount"]; }
				if (outkvs[L"ProcessId"].vt != VT_EMPTY) { outReturns.ProcessId = outkvs[L"ProcessId"]; }
				if (outkvs[L"QuotaNonPagedPoolUsage"].vt != VT_EMPTY) { outReturns.QuotaNonPagedPoolUsage = outkvs[L"QuotaNonPagedPoolUsage"]; }
				if (outkvs[L"QuotaPagedPoolUsage"].vt != VT_EMPTY) { outReturns.QuotaPagedPoolUsage = outkvs[L"QuotaPagedPoolUsage"]; }
				if (outkvs[L"QuotaPeakNonPagedPoolUsage"].vt != VT_EMPTY) { outReturns.QuotaPeakNonPagedPoolUsage = outkvs[L"QuotaPeakNonPagedPoolUsage"]; }
				if (outkvs[L"QuotaPeakPagedPoolUsage"].vt != VT_EMPTY) { outReturns.QuotaPeakPagedPoolUsage = outkvs[L"QuotaPeakPagedPoolUsage"]; }
				if (outkvs[L"ReadOperationCount"].vt != VT_EMPTY) { outReturns.ReadOperationCount = outkvs[L"ReadOperationCount"]; }
				if (outkvs[L"ReadTransferCount"].vt != VT_EMPTY) { outReturns.ReadTransferCount = outkvs[L"ReadTransferCount"]; }
				if (outkvs[L"SessionId"].vt != VT_EMPTY) { outReturns.SessionId = outkvs[L"SessionId"]; }
				if (outkvs[L"Status"].vt != VT_EMPTY) { outReturns.Status = outkvs[L"Status"]; }
				if (outkvs[L"TerminationDate"].vt != VT_EMPTY) { outReturns.TerminationDate = outkvs[L"TerminationDate"]; }
				if (outkvs[L"ThreadCount"].vt != VT_EMPTY) { outReturns.ThreadCount = outkvs[L"ThreadCount"]; }
				if (outkvs[L"UserModeTime"].vt != VT_EMPTY) { outReturns.UserModeTime = outkvs[L"UserModeTime"]; }
				if (outkvs[L"VirtualSize"].vt != VT_EMPTY) { outReturns.VirtualSize = outkvs[L"VirtualSize"]; }
				if (outkvs[L"WindowsVersion"].vt != VT_EMPTY) { outReturns.WindowsVersion = outkvs[L"WindowsVersion"]; }
				if (outkvs[L"WorkingSetSize"].vt != VT_EMPTY) { outReturns.WorkingSetSize = outkvs[L"WorkingSetSize"]; }
				if (outkvs[L"WriteOperationCount"].vt != VT_EMPTY) { outReturns.WriteOperationCount = outkvs[L"WriteOperationCount"]; }
				if (outkvs[L"WriteTransferCount"].vt != VT_EMPTY) { outReturns.WriteTransferCount = outkvs[L"WriteTransferCount"]; }
				if (outkvs[L"__CLASS"].vt != VT_EMPTY) { outReturns.__CLASS = outkvs[L"__CLASS"]; }
				if (outkvs[L"__DERIVATION"].vt != VT_EMPTY) { outReturns.__DERIVATION = outkvs[L"__DERIVATION"]; }
				if (outkvs[L"__DYNASTY"].vt != VT_EMPTY) { outReturns.__DYNASTY = outkvs[L"__DYNASTY"]; }
				if (outkvs[L"__GENUS"].vt != VT_EMPTY) { outReturns.__GENUS = outkvs[L"__GENUS"]; }
				if (outkvs[L"__NAMESPACE"].vt != VT_EMPTY) { outReturns.__NAMESPACE = outkvs[L"__NAMESPACE"]; }
				if (outkvs[L"__PATH"].vt != VT_EMPTY) { outReturns.__PATH = outkvs[L"__PATH"]; }
				if (outkvs[L"__PROPERTY_COUNT"].vt != VT_EMPTY) { outReturns.__PROPERTY_COUNT = outkvs[L"__PROPERTY_COUNT"]; }
				if (outkvs[L"__RELPATH"].vt != VT_EMPTY) { outReturns.__RELPATH = outkvs[L"__RELPATH"]; }
				if (outkvs[L"__SERVER"].vt != VT_EMPTY) { outReturns.__SERVER = outkvs[L"__SERVER"]; }
				if (outkvs[L"__SUPERCLASS"].vt != VT_EMPTY) { outReturns.__SUPERCLASS = outkvs[L"__SUPERCLASS"]; }
			}
			return hr;
		}

		HRESULT NewObj(SML_Win32_Process_Props & inParams, CComVariant & var)
		{
			::SmartLib::SmlWmi::KVSet inkvs;
			if (inParams.Caption.vt != VT_EMPTY)
			{
				inkvs.insert({L"Caption", inParams.Caption});
			}

			HRESULT hr = _smlwmi->NewObject(CLASS_NAME, inkvs, var);
			return hr;
		}

		////////////////////////// Method: Create ///////////////////////////
		struct Create_InParams
		{
			CComVariant CommandLine;
			CComVariant CurrentDirectory;
			CComVariant ProcessStartupInformation;
			CComVariant __CLASS;
			CComVariant __DERIVATION;
			CComVariant __DYNASTY;
			CComVariant __GENUS;
			CComVariant __NAMESPACE;
			CComVariant __PATH;
			CComVariant __PROPERTY_COUNT;
			CComVariant __RELPATH;
			CComVariant __SERVER;
			CComVariant __SUPERCLASS;
		};
		struct Create_OutResturns
		{
			CComVariant ProcessId;
			CComVariant ReturnValue;
			CComVariant __CLASS;
			CComVariant __DERIVATION;
			CComVariant __DYNASTY;
			CComVariant __GENUS;
			CComVariant __NAMESPACE;
			CComVariant __PATH;
			CComVariant __PROPERTY_COUNT;
			CComVariant __RELPATH;
			CComVariant __SERVER;
			CComVariant __SUPERCLASS;
		};
		HRESULT Create(IWbemClassObject* obj, Create_InParams& inParams, Create_OutResturns& outReturns)
		{
			SmlWmi::KVSet inkvs;
			SmlWmi::KVSet outkvs;

			if (inParams.CommandLine.vt != VT_EMPTY) { inkvs.insert({ L"CommandLine", inParams.CommandLine }); }
			if (inParams.CurrentDirectory.vt != VT_EMPTY) { inkvs.insert({ L"CurrentDirectory", inParams.CurrentDirectory }); }
			if (inParams.ProcessStartupInformation.vt != VT_EMPTY) { inkvs.insert({ L"ProcessStartupInformation", inParams.ProcessStartupInformation }); }
			if (inParams.__CLASS.vt != VT_EMPTY) { inkvs.insert({ L"__CLASS", inParams.__CLASS }); }
			if (inParams.__DERIVATION.vt != VT_EMPTY) { inkvs.insert({ L"__DERIVATION", inParams.__DERIVATION }); }
			if (inParams.__DYNASTY.vt != VT_EMPTY) { inkvs.insert({ L"__DYNASTY", inParams.__DYNASTY }); }
			if (inParams.__GENUS.vt != VT_EMPTY) { inkvs.insert({ L"__GENUS", inParams.__GENUS }); }
			if (inParams.__NAMESPACE.vt != VT_EMPTY) { inkvs.insert({ L"__NAMESPACE", inParams.__NAMESPACE }); }
			if (inParams.__PATH.vt != VT_EMPTY) { inkvs.insert({ L"__PATH", inParams.__PATH }); }
			if (inParams.__PROPERTY_COUNT.vt != VT_EMPTY) { inkvs.insert({ L"__PROPERTY_COUNT", inParams.__PROPERTY_COUNT }); }
			if (inParams.__RELPATH.vt != VT_EMPTY) { inkvs.insert({ L"__RELPATH", inParams.__RELPATH }); }
			if (inParams.__SERVER.vt != VT_EMPTY) { inkvs.insert({ L"__SERVER", inParams.__SERVER }); }
			if (inParams.__SUPERCLASS.vt != VT_EMPTY) { inkvs.insert({ L"__SUPERCLASS", inParams.__SUPERCLASS }); }

			CComPtr<IWbemClassObject> spout;
			HRESULT hr = _smlwmi->InvokeMethod(obj, CLASS_NAME, L"Create", inkvs, outkvs, &spout);

			if (outkvs[L"ProcessId"].vt != VT_EMPTY) { outReturns.ProcessId = outkvs[L"ProcessId"]; }
			if (outkvs[L"ReturnValue"].vt != VT_EMPTY) { outReturns.ReturnValue = outkvs[L"ReturnValue"]; }
			if (outkvs[L"__CLASS"].vt != VT_EMPTY) { outReturns.__CLASS = outkvs[L"__CLASS"]; }
			if (outkvs[L"__DERIVATION"].vt != VT_EMPTY) { outReturns.__DERIVATION = outkvs[L"__DERIVATION"]; }
			if (outkvs[L"__DYNASTY"].vt != VT_EMPTY) { outReturns.__DYNASTY = outkvs[L"__DYNASTY"]; }
			if (outkvs[L"__GENUS"].vt != VT_EMPTY) { outReturns.__GENUS = outkvs[L"__GENUS"]; }
			if (outkvs[L"__NAMESPACE"].vt != VT_EMPTY) { outReturns.__NAMESPACE = outkvs[L"__NAMESPACE"]; }
			if (outkvs[L"__PATH"].vt != VT_EMPTY) { outReturns.__PATH = outkvs[L"__PATH"]; }
			if (outkvs[L"__PROPERTY_COUNT"].vt != VT_EMPTY) { outReturns.__PROPERTY_COUNT = outkvs[L"__PROPERTY_COUNT"]; }
			if (outkvs[L"__RELPATH"].vt != VT_EMPTY) { outReturns.__RELPATH = outkvs[L"__RELPATH"]; }
			if (outkvs[L"__SERVER"].vt != VT_EMPTY) { outReturns.__SERVER = outkvs[L"__SERVER"]; }
			if (outkvs[L"__SUPERCLASS"].vt != VT_EMPTY) { outReturns.__SUPERCLASS = outkvs[L"__SUPERCLASS"]; }


			return hr;
		}


		////////////////////////// Method: Terminate ///////////////////////////
		struct Terminate_InParams
		{
			CComVariant Reason;
			CComVariant __CLASS;
			CComVariant __DERIVATION;
			CComVariant __DYNASTY;
			CComVariant __GENUS;
			CComVariant __NAMESPACE;
			CComVariant __PATH;
			CComVariant __PROPERTY_COUNT;
			CComVariant __RELPATH;
			CComVariant __SERVER;
			CComVariant __SUPERCLASS;
		};
		struct Terminate_OutResturns
		{
			CComVariant ReturnValue;
			CComVariant __CLASS;
			CComVariant __DERIVATION;
			CComVariant __DYNASTY;
			CComVariant __GENUS;
			CComVariant __NAMESPACE;
			CComVariant __PATH;
			CComVariant __PROPERTY_COUNT;
			CComVariant __RELPATH;
			CComVariant __SERVER;
			CComVariant __SUPERCLASS;
		};
		HRESULT Terminate(IWbemClassObject* obj, Terminate_InParams& inParams, Terminate_OutResturns& outReturns)
		{
			SmlWmi::KVSet inkvs;
			SmlWmi::KVSet outkvs;

			if (inParams.Reason.vt != VT_EMPTY) { inkvs.insert({ L"Reason", inParams.Reason }); }
			if (inParams.__CLASS.vt != VT_EMPTY) { inkvs.insert({ L"__CLASS", inParams.__CLASS }); }
			if (inParams.__DERIVATION.vt != VT_EMPTY) { inkvs.insert({ L"__DERIVATION", inParams.__DERIVATION }); }
			if (inParams.__DYNASTY.vt != VT_EMPTY) { inkvs.insert({ L"__DYNASTY", inParams.__DYNASTY }); }
			if (inParams.__GENUS.vt != VT_EMPTY) { inkvs.insert({ L"__GENUS", inParams.__GENUS }); }
			if (inParams.__NAMESPACE.vt != VT_EMPTY) { inkvs.insert({ L"__NAMESPACE", inParams.__NAMESPACE }); }
			if (inParams.__PATH.vt != VT_EMPTY) { inkvs.insert({ L"__PATH", inParams.__PATH }); }
			if (inParams.__PROPERTY_COUNT.vt != VT_EMPTY) { inkvs.insert({ L"__PROPERTY_COUNT", inParams.__PROPERTY_COUNT }); }
			if (inParams.__RELPATH.vt != VT_EMPTY) { inkvs.insert({ L"__RELPATH", inParams.__RELPATH }); }
			if (inParams.__SERVER.vt != VT_EMPTY) { inkvs.insert({ L"__SERVER", inParams.__SERVER }); }
			if (inParams.__SUPERCLASS.vt != VT_EMPTY) { inkvs.insert({ L"__SUPERCLASS", inParams.__SUPERCLASS }); }

			CComPtr<IWbemClassObject> spout;
			HRESULT hr = _smlwmi->InvokeMethod(obj, CLASS_NAME, L"Terminate", inkvs, outkvs, &spout);

			if (outkvs[L"ReturnValue"].vt != VT_EMPTY) { outReturns.ReturnValue = outkvs[L"ReturnValue"]; }
			if (outkvs[L"__CLASS"].vt != VT_EMPTY) { outReturns.__CLASS = outkvs[L"__CLASS"]; }
			if (outkvs[L"__DERIVATION"].vt != VT_EMPTY) { outReturns.__DERIVATION = outkvs[L"__DERIVATION"]; }
			if (outkvs[L"__DYNASTY"].vt != VT_EMPTY) { outReturns.__DYNASTY = outkvs[L"__DYNASTY"]; }
			if (outkvs[L"__GENUS"].vt != VT_EMPTY) { outReturns.__GENUS = outkvs[L"__GENUS"]; }
			if (outkvs[L"__NAMESPACE"].vt != VT_EMPTY) { outReturns.__NAMESPACE = outkvs[L"__NAMESPACE"]; }
			if (outkvs[L"__PATH"].vt != VT_EMPTY) { outReturns.__PATH = outkvs[L"__PATH"]; }
			if (outkvs[L"__PROPERTY_COUNT"].vt != VT_EMPTY) { outReturns.__PROPERTY_COUNT = outkvs[L"__PROPERTY_COUNT"]; }
			if (outkvs[L"__RELPATH"].vt != VT_EMPTY) { outReturns.__RELPATH = outkvs[L"__RELPATH"]; }
			if (outkvs[L"__SERVER"].vt != VT_EMPTY) { outReturns.__SERVER = outkvs[L"__SERVER"]; }
			if (outkvs[L"__SUPERCLASS"].vt != VT_EMPTY) { outReturns.__SUPERCLASS = outkvs[L"__SUPERCLASS"]; }


			return hr;
		}


		////////////////////////// Method: GetOwner ///////////////////////////
		struct GetOwner_InParams
		{
		};
		struct GetOwner_OutResturns
		{
			CComVariant Domain;
			CComVariant ReturnValue;
			CComVariant User;
			CComVariant __CLASS;
			CComVariant __DERIVATION;
			CComVariant __DYNASTY;
			CComVariant __GENUS;
			CComVariant __NAMESPACE;
			CComVariant __PATH;
			CComVariant __PROPERTY_COUNT;
			CComVariant __RELPATH;
			CComVariant __SERVER;
			CComVariant __SUPERCLASS;
		};
		HRESULT GetOwner(IWbemClassObject* obj, GetOwner_InParams& inParams, GetOwner_OutResturns& outReturns)
		{
			SmlWmi::KVSet inkvs;
			SmlWmi::KVSet outkvs;


			CComPtr<IWbemClassObject> spout;
			HRESULT hr = _smlwmi->InvokeMethod(obj, CLASS_NAME, L"GetOwner", inkvs, outkvs, &spout);

			if (outkvs[L"Domain"].vt != VT_EMPTY) { outReturns.Domain = outkvs[L"Domain"]; }
			if (outkvs[L"ReturnValue"].vt != VT_EMPTY) { outReturns.ReturnValue = outkvs[L"ReturnValue"]; }
			if (outkvs[L"User"].vt != VT_EMPTY) { outReturns.User = outkvs[L"User"]; }
			if (outkvs[L"__CLASS"].vt != VT_EMPTY) { outReturns.__CLASS = outkvs[L"__CLASS"]; }
			if (outkvs[L"__DERIVATION"].vt != VT_EMPTY) { outReturns.__DERIVATION = outkvs[L"__DERIVATION"]; }
			if (outkvs[L"__DYNASTY"].vt != VT_EMPTY) { outReturns.__DYNASTY = outkvs[L"__DYNASTY"]; }
			if (outkvs[L"__GENUS"].vt != VT_EMPTY) { outReturns.__GENUS = outkvs[L"__GENUS"]; }
			if (outkvs[L"__NAMESPACE"].vt != VT_EMPTY) { outReturns.__NAMESPACE = outkvs[L"__NAMESPACE"]; }
			if (outkvs[L"__PATH"].vt != VT_EMPTY) { outReturns.__PATH = outkvs[L"__PATH"]; }
			if (outkvs[L"__PROPERTY_COUNT"].vt != VT_EMPTY) { outReturns.__PROPERTY_COUNT = outkvs[L"__PROPERTY_COUNT"]; }
			if (outkvs[L"__RELPATH"].vt != VT_EMPTY) { outReturns.__RELPATH = outkvs[L"__RELPATH"]; }
			if (outkvs[L"__SERVER"].vt != VT_EMPTY) { outReturns.__SERVER = outkvs[L"__SERVER"]; }
			if (outkvs[L"__SUPERCLASS"].vt != VT_EMPTY) { outReturns.__SUPERCLASS = outkvs[L"__SUPERCLASS"]; }


			return hr;
		}


		////////////////////////// Method: GetOwnerSid ///////////////////////////
		struct GetOwnerSid_InParams
		{
		};
		struct GetOwnerSid_OutResturns
		{
			CComVariant ReturnValue;
			CComVariant Sid;
			CComVariant __CLASS;
			CComVariant __DERIVATION;
			CComVariant __DYNASTY;
			CComVariant __GENUS;
			CComVariant __NAMESPACE;
			CComVariant __PATH;
			CComVariant __PROPERTY_COUNT;
			CComVariant __RELPATH;
			CComVariant __SERVER;
			CComVariant __SUPERCLASS;
		};
		HRESULT GetOwnerSid(IWbemClassObject* obj, GetOwnerSid_InParams& inParams, GetOwnerSid_OutResturns& outReturns)
		{
			SmlWmi::KVSet inkvs;
			SmlWmi::KVSet outkvs;


			CComPtr<IWbemClassObject> spout;
			HRESULT hr = _smlwmi->InvokeMethod(obj, CLASS_NAME, L"GetOwnerSid", inkvs, outkvs, &spout);

			if (outkvs[L"ReturnValue"].vt != VT_EMPTY) { outReturns.ReturnValue = outkvs[L"ReturnValue"]; }
			if (outkvs[L"Sid"].vt != VT_EMPTY) { outReturns.Sid = outkvs[L"Sid"]; }
			if (outkvs[L"__CLASS"].vt != VT_EMPTY) { outReturns.__CLASS = outkvs[L"__CLASS"]; }
			if (outkvs[L"__DERIVATION"].vt != VT_EMPTY) { outReturns.__DERIVATION = outkvs[L"__DERIVATION"]; }
			if (outkvs[L"__DYNASTY"].vt != VT_EMPTY) { outReturns.__DYNASTY = outkvs[L"__DYNASTY"]; }
			if (outkvs[L"__GENUS"].vt != VT_EMPTY) { outReturns.__GENUS = outkvs[L"__GENUS"]; }
			if (outkvs[L"__NAMESPACE"].vt != VT_EMPTY) { outReturns.__NAMESPACE = outkvs[L"__NAMESPACE"]; }
			if (outkvs[L"__PATH"].vt != VT_EMPTY) { outReturns.__PATH = outkvs[L"__PATH"]; }
			if (outkvs[L"__PROPERTY_COUNT"].vt != VT_EMPTY) { outReturns.__PROPERTY_COUNT = outkvs[L"__PROPERTY_COUNT"]; }
			if (outkvs[L"__RELPATH"].vt != VT_EMPTY) { outReturns.__RELPATH = outkvs[L"__RELPATH"]; }
			if (outkvs[L"__SERVER"].vt != VT_EMPTY) { outReturns.__SERVER = outkvs[L"__SERVER"]; }
			if (outkvs[L"__SUPERCLASS"].vt != VT_EMPTY) { outReturns.__SUPERCLASS = outkvs[L"__SUPERCLASS"]; }


			return hr;
		}


		////////////////////////// Method: SetPriority ///////////////////////////
		struct SetPriority_InParams
		{
			CComVariant Priority;
			CComVariant __CLASS;
			CComVariant __DERIVATION;
			CComVariant __DYNASTY;
			CComVariant __GENUS;
			CComVariant __NAMESPACE;
			CComVariant __PATH;
			CComVariant __PROPERTY_COUNT;
			CComVariant __RELPATH;
			CComVariant __SERVER;
			CComVariant __SUPERCLASS;
		};
		struct SetPriority_OutResturns
		{
			CComVariant ReturnValue;
			CComVariant __CLASS;
			CComVariant __DERIVATION;
			CComVariant __DYNASTY;
			CComVariant __GENUS;
			CComVariant __NAMESPACE;
			CComVariant __PATH;
			CComVariant __PROPERTY_COUNT;
			CComVariant __RELPATH;
			CComVariant __SERVER;
			CComVariant __SUPERCLASS;
		};
		HRESULT SetPriority(IWbemClassObject* obj, SetPriority_InParams& inParams, SetPriority_OutResturns& outReturns)
		{
			SmlWmi::KVSet inkvs;
			SmlWmi::KVSet outkvs;

			if (inParams.Priority.vt != VT_EMPTY) { inkvs.insert({ L"Priority", inParams.Priority }); }
			if (inParams.__CLASS.vt != VT_EMPTY) { inkvs.insert({ L"__CLASS", inParams.__CLASS }); }
			if (inParams.__DERIVATION.vt != VT_EMPTY) { inkvs.insert({ L"__DERIVATION", inParams.__DERIVATION }); }
			if (inParams.__DYNASTY.vt != VT_EMPTY) { inkvs.insert({ L"__DYNASTY", inParams.__DYNASTY }); }
			if (inParams.__GENUS.vt != VT_EMPTY) { inkvs.insert({ L"__GENUS", inParams.__GENUS }); }
			if (inParams.__NAMESPACE.vt != VT_EMPTY) { inkvs.insert({ L"__NAMESPACE", inParams.__NAMESPACE }); }
			if (inParams.__PATH.vt != VT_EMPTY) { inkvs.insert({ L"__PATH", inParams.__PATH }); }
			if (inParams.__PROPERTY_COUNT.vt != VT_EMPTY) { inkvs.insert({ L"__PROPERTY_COUNT", inParams.__PROPERTY_COUNT }); }
			if (inParams.__RELPATH.vt != VT_EMPTY) { inkvs.insert({ L"__RELPATH", inParams.__RELPATH }); }
			if (inParams.__SERVER.vt != VT_EMPTY) { inkvs.insert({ L"__SERVER", inParams.__SERVER }); }
			if (inParams.__SUPERCLASS.vt != VT_EMPTY) { inkvs.insert({ L"__SUPERCLASS", inParams.__SUPERCLASS }); }

			CComPtr<IWbemClassObject> spout;
			HRESULT hr = _smlwmi->InvokeMethod(obj, CLASS_NAME, L"SetPriority", inkvs, outkvs, &spout);

			if (outkvs[L"ReturnValue"].vt != VT_EMPTY) { outReturns.ReturnValue = outkvs[L"ReturnValue"]; }
			if (outkvs[L"__CLASS"].vt != VT_EMPTY) { outReturns.__CLASS = outkvs[L"__CLASS"]; }
			if (outkvs[L"__DERIVATION"].vt != VT_EMPTY) { outReturns.__DERIVATION = outkvs[L"__DERIVATION"]; }
			if (outkvs[L"__DYNASTY"].vt != VT_EMPTY) { outReturns.__DYNASTY = outkvs[L"__DYNASTY"]; }
			if (outkvs[L"__GENUS"].vt != VT_EMPTY) { outReturns.__GENUS = outkvs[L"__GENUS"]; }
			if (outkvs[L"__NAMESPACE"].vt != VT_EMPTY) { outReturns.__NAMESPACE = outkvs[L"__NAMESPACE"]; }
			if (outkvs[L"__PATH"].vt != VT_EMPTY) { outReturns.__PATH = outkvs[L"__PATH"]; }
			if (outkvs[L"__PROPERTY_COUNT"].vt != VT_EMPTY) { outReturns.__PROPERTY_COUNT = outkvs[L"__PROPERTY_COUNT"]; }
			if (outkvs[L"__RELPATH"].vt != VT_EMPTY) { outReturns.__RELPATH = outkvs[L"__RELPATH"]; }
			if (outkvs[L"__SERVER"].vt != VT_EMPTY) { outReturns.__SERVER = outkvs[L"__SERVER"]; }
			if (outkvs[L"__SUPERCLASS"].vt != VT_EMPTY) { outReturns.__SUPERCLASS = outkvs[L"__SUPERCLASS"]; }


			return hr;
		}


		////////////////////////// Method: AttachDebugger ///////////////////////////
		struct AttachDebugger_InParams
		{
		};
		struct AttachDebugger_OutResturns
		{
			CComVariant ReturnValue;
			CComVariant __CLASS;
			CComVariant __DERIVATION;
			CComVariant __DYNASTY;
			CComVariant __GENUS;
			CComVariant __NAMESPACE;
			CComVariant __PATH;
			CComVariant __PROPERTY_COUNT;
			CComVariant __RELPATH;
			CComVariant __SERVER;
			CComVariant __SUPERCLASS;
		};
		HRESULT AttachDebugger(IWbemClassObject* obj, AttachDebugger_InParams& inParams, AttachDebugger_OutResturns& outReturns)
		{
			SmlWmi::KVSet inkvs;
			SmlWmi::KVSet outkvs;


			CComPtr<IWbemClassObject> spout;
			HRESULT hr = _smlwmi->InvokeMethod(obj, CLASS_NAME, L"AttachDebugger", inkvs, outkvs, &spout);

			if (outkvs[L"ReturnValue"].vt != VT_EMPTY) { outReturns.ReturnValue = outkvs[L"ReturnValue"]; }
			if (outkvs[L"__CLASS"].vt != VT_EMPTY) { outReturns.__CLASS = outkvs[L"__CLASS"]; }
			if (outkvs[L"__DERIVATION"].vt != VT_EMPTY) { outReturns.__DERIVATION = outkvs[L"__DERIVATION"]; }
			if (outkvs[L"__DYNASTY"].vt != VT_EMPTY) { outReturns.__DYNASTY = outkvs[L"__DYNASTY"]; }
			if (outkvs[L"__GENUS"].vt != VT_EMPTY) { outReturns.__GENUS = outkvs[L"__GENUS"]; }
			if (outkvs[L"__NAMESPACE"].vt != VT_EMPTY) { outReturns.__NAMESPACE = outkvs[L"__NAMESPACE"]; }
			if (outkvs[L"__PATH"].vt != VT_EMPTY) { outReturns.__PATH = outkvs[L"__PATH"]; }
			if (outkvs[L"__PROPERTY_COUNT"].vt != VT_EMPTY) { outReturns.__PROPERTY_COUNT = outkvs[L"__PROPERTY_COUNT"]; }
			if (outkvs[L"__RELPATH"].vt != VT_EMPTY) { outReturns.__RELPATH = outkvs[L"__RELPATH"]; }
			if (outkvs[L"__SERVER"].vt != VT_EMPTY) { outReturns.__SERVER = outkvs[L"__SERVER"]; }
			if (outkvs[L"__SUPERCLASS"].vt != VT_EMPTY) { outReturns.__SUPERCLASS = outkvs[L"__SUPERCLASS"]; }


			return hr;
		}


		////////////////////////// Method: GetAvailableVirtualSize ///////////////////////////
		struct GetAvailableVirtualSize_InParams
		{
		};
		struct GetAvailableVirtualSize_OutResturns
		{
			CComVariant AvailableVirtualSize;
			CComVariant ReturnValue;
			CComVariant __CLASS;
			CComVariant __DERIVATION;
			CComVariant __DYNASTY;
			CComVariant __GENUS;
			CComVariant __NAMESPACE;
			CComVariant __PATH;
			CComVariant __PROPERTY_COUNT;
			CComVariant __RELPATH;
			CComVariant __SERVER;
			CComVariant __SUPERCLASS;
		};
		HRESULT GetAvailableVirtualSize(IWbemClassObject* obj, GetAvailableVirtualSize_InParams& inParams, GetAvailableVirtualSize_OutResturns& outReturns)
		{
			SmlWmi::KVSet inkvs;
			SmlWmi::KVSet outkvs;


			CComPtr<IWbemClassObject> spout;
			HRESULT hr = _smlwmi->InvokeMethod(obj, CLASS_NAME, L"GetAvailableVirtualSize", inkvs, outkvs, &spout);

			if (outkvs[L"AvailableVirtualSize"].vt != VT_EMPTY) { outReturns.AvailableVirtualSize = outkvs[L"AvailableVirtualSize"]; }
			if (outkvs[L"ReturnValue"].vt != VT_EMPTY) { outReturns.ReturnValue = outkvs[L"ReturnValue"]; }
			if (outkvs[L"__CLASS"].vt != VT_EMPTY) { outReturns.__CLASS = outkvs[L"__CLASS"]; }
			if (outkvs[L"__DERIVATION"].vt != VT_EMPTY) { outReturns.__DERIVATION = outkvs[L"__DERIVATION"]; }
			if (outkvs[L"__DYNASTY"].vt != VT_EMPTY) { outReturns.__DYNASTY = outkvs[L"__DYNASTY"]; }
			if (outkvs[L"__GENUS"].vt != VT_EMPTY) { outReturns.__GENUS = outkvs[L"__GENUS"]; }
			if (outkvs[L"__NAMESPACE"].vt != VT_EMPTY) { outReturns.__NAMESPACE = outkvs[L"__NAMESPACE"]; }
			if (outkvs[L"__PATH"].vt != VT_EMPTY) { outReturns.__PATH = outkvs[L"__PATH"]; }
			if (outkvs[L"__PROPERTY_COUNT"].vt != VT_EMPTY) { outReturns.__PROPERTY_COUNT = outkvs[L"__PROPERTY_COUNT"]; }
			if (outkvs[L"__RELPATH"].vt != VT_EMPTY) { outReturns.__RELPATH = outkvs[L"__RELPATH"]; }
			if (outkvs[L"__SERVER"].vt != VT_EMPTY) { outReturns.__SERVER = outkvs[L"__SERVER"]; }
			if (outkvs[L"__SUPERCLASS"].vt != VT_EMPTY) { outReturns.__SUPERCLASS = outkvs[L"__SUPERCLASS"]; }


			return hr;
		}


	};

	class SmlWmiParserTest
	{
	public:
		static void CaseTestGenerated()
		{
			//////////////////////////////////////////////////////////////////////////
			LPCTSTR wmins = L"root\\cimv2";
			SmartLib::counted_ptr<SmlWmi> wmi = SmartLib::counted_ptr<SmlWmi>::make(nullptr, nullptr, nullptr, nullptr, wmins);
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
			
		}
		static void Case0()
		{
			LPCTSTR wmins = L"root\\cimv2";
			SmlWmiParser parser(wmins);
			

			LPCTSTR className = L"Win32_Process";
			
			CString desc;
			parser.Parse(/*wmins, */className, desc);
		}
	};

	

}