#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlsafe.h>

#include <Wbemidl.h>

#include <map>
#include <vector>

//////////////////////////////////////////////////////////////////////////
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"


namespace SmartLib
{
	class SmlWmi
	{
	public:
		typedef std::map<CString, CComVariant> KVSet;
		typedef std::vector<std::map<CString, CComVariant>> KVSetVec;

		class Helpers
		{
		public:
			static VOID ComposeAuthId(COAUTHIDENTITY& authIdent, CString& strUserTmp, CString& strDomainTmp, CString& strPasswordTmp)
			{
				memset(&authIdent, 0, sizeof(COAUTHIDENTITY));

				authIdent.PasswordLength = strPasswordTmp.GetLength();
				authIdent.Password = (USHORT*)strPasswordTmp.GetString();

				authIdent.User = (USHORT*)strUserTmp.GetString();
				authIdent.UserLength = strUserTmp.GetLength();

				authIdent.Domain = (USHORT*)strDomainTmp.GetString();;
				authIdent.DomainLength = strDomainTmp.GetLength();

				authIdent.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
			}


			static VOID CombineDomainUser(CONST CString& Domain, CONST CString& User, CString& strDomainUser)
			{
				if (User.GetLength())
				{
					if (Domain.GetLength())
					{
						strDomainUser = Domain;
						strDomainUser += TEXT("\\");
						strDomainUser += User;
					}
					else
					{
						strDomainUser = TEXT(".");
						strDomainUser += TEXT("\\");
						strDomainUser += User;
					}
				}
			}


			static VOID SplitDomainUser(CONST CString& strDomainUser, CString& Domain, CString& User)
			{
				int index = strDomainUser.Find(TEXT('\\'));
				if (-1 != index)
				{
					Domain = strDomainUser.Mid(0, index);
					User = strDomainUser.Mid(index + 1);
				}
				else
				{
					index = index = strDomainUser.Find(TEXT('@'));
					if (-1 != index)
					{
						Domain = strDomainUser.Mid(index + 1);
						User = strDomainUser.Mid(0, index);
					}
					else
					{
						Domain = TEXT(".");
						User = strDomainUser;
					}
				}
			}


			static LPCTSTR ObjPath(IWbemClassObject* pInstance, CString & path)
			{
				CComVariant var;
				GetNamedProp(pInstance, L"__PATH", &var);
				path = var.bstrVal;
				return path.GetString();
			}

			static LPCTSTR ObjClass(IWbemClassObject* pInstance, CString& objclass)
			{
				CComVariant var;
				GetNamedProp(pInstance, L"__CLASS", &var);
				objclass = var.bstrVal;
				return objclass.GetString();
			}

			static HRESULT GetNamedProp(IWbemClassObject* pInstance, LPCTSTR name, VARIANT* pvar)
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;

				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				hr = pInstance->Get(name, 0, pvar, NULL, NULL);
				if (FAILED(hr))
				{
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("Get"), hr, TEXT("%s"), name);
					LEAVE_BLOCK(0);
				}

				//////////////////////////////////////////////////////////////////////////
				END_BLOCK(0);

				return hr;
			}

			static HRESULT PutProp(IWbemClassObject* spObj, KVSet& props)
			{
				HRESULT hr = S_OK;
				
				for (auto& item : props)
				{
					hr = spObj->Put(item.first.GetString(), 0, &item.second, 0);
					if (FAILED(hr))
					{
						//LastError = WSAGetLastError();
						//LastError = GetLastError();
						//hr = HRESULT_FROM_WIN32(LastError);
						//D_SET_FISRT_STATUS(hr, LastError);
						//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
						//D_SET_LAST_STATUS(hr, LastError);
						//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
						D_API_ERR(0, TEXT("Put"), hr, TEXT("%s"), item.first.GetString());
						break;
					}
				}
				return hr;
			}
			static HRESULT GetProp(IWbemClassObject* spObj, KVSet & props) //names and vars are allocated by the function
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;

				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				hr = spObj->BeginEnumeration(
					0///* [in] */ long lEnumFlags
				);
				if (FAILED(hr))
				{
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("BeginEnumeration"), hr, TEXT(""));
					LEAVE_BLOCK(0);
				}

				//vars.reserve(256);
				for (;;)
				{
					CComBSTR name;
					CComVariant var;
					hr = spObj->Next(
						NULL,///* [in] */ long lFlags,
						&name,///* [unique][in][out] */ BSTR *strName,
						&var,///* [unique][in][out] */ VARIANT *pVal,
						NULL,///* [unique][in][out] */ CIMTYPE *pType,
						NULL///* [unique][in][out] */ long *plFlavor
					);
					if (FAILED(hr))
					{
						//LastError = WSAGetLastError();
						//LastError = GetLastError();
						//hr = HRESULT_FROM_WIN32(LastError);
						//D_SET_FISRT_STATUS(hr, LastError);
						//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
						//D_SET_LAST_STATUS(hr, LastError);
						//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
						D_API_ERR(0, TEXT("Next"), hr, TEXT(""));
						LEAVE_BLOCK(0);
						break;
					}
					else if (S_FALSE == hr)
					{
						break;
					}
					else if (NULL == (LPCTSTR)(name))
					{
						break;
					}

					props.insert({(LPCTSTR)(name), var});
				}

				//////////////////////////////////////////////////////////////////////////
				END_BLOCK(0);

				HRESULT hrTemp = spObj->EndEnumeration();
				if (FAILED(hrTemp))
				{
					hr = hrTemp;
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("EndEnumeration"), hr, TEXT(""));
					//LEAVE_BLOCK(0);
				}

				return hr;
			}
			static HRESULT Objects2Props(const std::vector<CComPtr<IWbemClassObject>>& objs, KVSetVec & vecprops)
			{
				HRESULT hr = S_OK;
				HRESULT hrfinal = S_OK;
				vecprops.resize(objs.size());
				for (int ii = 0; ii < objs.size(); ++ii)
				{
					hr = GetProp(objs[ii], vecprops[ii]);
					if (FAILED(hr))
					{
						hrfinal = hr;
					}
				}
				return hrfinal;
			}

			template<typename TCPP, typename TCOM>
			static HRESULT Vector2SafeArray(CONST std::vector<TCPP>& vec, CComSafeArray<TCOM>& sa)
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;

				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				hr = sa.Create(vec.size());
				if (FAILED(hr))
				{
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("Create"), hr, TEXT(""));
					LEAVE_BLOCK(0);
				}

				for (size_t ii = 0; ii < vec.size(); ++ii)
				{
					sa.SetAt(ii, vec[ii], TRUE);
				}

				//////////////////////////////////////////////////////////////////////////
				END_BLOCK(0);

				return hr;
			}

			template<>
			static HRESULT Vector2SafeArray<CString, BSTR>(CONST std::vector<CString>& vec, CComSafeArray<BSTR>& sa)
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;

				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				hr = sa.Create(vec.size());
				if (FAILED(hr))
				{
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("Create"), hr, TEXT(""));
					LEAVE_BLOCK(0);
				}

				for (size_t ii = 0; ii < vec.size(); ++ii)
				{
					sa.SetAt(ii, vec[ii].AllocSysString(), FALSE);
				}

				//////////////////////////////////////////////////////////////////////////
				END_BLOCK(0);

				return hr;
			}

			template<typename TCPP, typename TCOM = TCPP>
			static HRESULT SafeArray2Vector(CONST CComSafeArray<TCOM> & sa, std::vector<TCPP> & vec)
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;

				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				vec.resize(sa.GetCount());

				for (size_t ii = 0; ii < vec.size(); ++ii)
				{
					vec[ii] = sa.GetAt(ii);
				}

				//////////////////////////////////////////////////////////////////////////
				END_BLOCK(0);

				return hr;
			}

			template<typename TCPP, typename TCOM>
			static HRESULT Vector2Var(CONST std::vector<TCPP>& vec, CComVariant& var)
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;

				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				CComSafeArray<TCOM> sa;
				hr = Vector2SafeArray<TCPP, TCOM>(vec, sa);
				if (FAILED(hr))
				{
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("Vector2SafeArray"), hr, TEXT(""));
					LEAVE_BLOCK(0);
				}

				var = sa;

				//////////////////////////////////////////////////////////////////////////
				END_BLOCK(0);

				return hr;
			}

			template<typename TCPP, typename TCOM>
			static HRESULT Var2Vector(CComVariant& var, std::vector<TCPP>& vec)
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;

				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				CComSafeArray<TCOM> sa;
				sa = var.parray;

				hr = SafeArray2Vector<TCPP, TCOM>(sa, vec);
				if (FAILED(hr))
				{
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("SafeArray2Vector"), hr, TEXT(""));
					LEAVE_BLOCK(0);
				}

				//////////////////////////////////////////////////////////////////////////
				END_BLOCK(0);

				return hr;
			}
		};

		struct MethodInfo
		{
			CComBSTR _name;
			CComPtr<IWbemClassObject> _InSignature;
			CComPtr<IWbemClassObject> _OutSignature;
			KVSet _inParams;
			KVSet _outReturns;
		};

	private:
		CString _Machine;		/***INCOMMING***/
		CString _User;			/***INCOMMING***/
		CString _Domain;		/***INCOMMING***/
		CString _Password;		/***INCOMMING***/
		CString _WmiNamespace;	/***INCOMMING***/

		CString _FullNamespace;				/***INTERNAL***/
		COAUTHIDENTITY _authid{ 0 };		/***INTERNAL***/
		CComPtr<IWbemServices> _Service; /***INTERNAL***//***CLEANUP***/

	private:
		HRESULT Query(LPCTSTR query, std::vector<CComPtr<IWbemClassObject>> & objects)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CComPtr<IEnumWbemClassObject> spEnumerator;
			hr = _Service->ExecQuery(
				CComBSTR(L"WQL"),
				CComBSTR(query),
				WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
				NULL,
				&spEnumerator
			);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("ExecQuery"), hr, TEXT("%s"), query);
				LEAVE_BLOCK(0);
			}

			// Secure the enumerator proxy
			COAUTHIDENTITY* pAuthId = (_Machine.GetLength() == 0 ? nullptr : &_authid);
			if (pAuthId)
			{

				hr = CoSetProxyBlanket(
					spEnumerator,                    // Indicates the proxy to set
					RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
					RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
					COLE_DEFAULT_PRINCIPAL,         // Server principal name
					RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx
					RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
					pAuthId,                       // client identity
					EOAC_NONE                       // proxy capabilities
				);
				if (FAILED(hr))
				{
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("CoSetProxyBlanket"), hr, TEXT(""));
					LEAVE_BLOCK(0);
				}
			}

			while (spEnumerator.p)
			{
				CComPtr<IWbemClassObject> spInstance;
				ULONG uReturn = 0;
				hr = spEnumerator->Next(WBEM_INFINITE, 1, &spInstance, &uReturn);

				if (FAILED(hr))
				{
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("Next"), hr, TEXT(""));
					break;
				}

				if (0 == uReturn || S_FALSE == hr)
				{
					break;
				}

				objects.push_back(spInstance);
			}
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}
			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

		

		HRESULT MakeInParamsObject(LPCTSTR className, LPCTSTR methodName, KVSet & params, IWbemClassObject ** inParamsObj)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////////////////////////
			CComPtr<IWbemClassObject> spClass;
			hr = GetClass(className, &spClass, nullptr);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("ServiceGetClass"), hr, TEXT("%s"), className);
				LEAVE_BLOCK(0);
			}
			//////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			CComPtr<IWbemClassObject> spClassMethodInParams;
			hr = spClass->GetMethod(methodName, 0, &spClassMethodInParams, NULL);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("GetMethod"), hr, TEXT("%s"), methodName);
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			CComPtr<IWbemClassObject> spMethodInParam;
			hr = spClassMethodInParams->SpawnInstance(0, &spMethodInParam);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("SpawnInstance"), hr, TEXT("%s"), methodName);
				LEAVE_BLOCK(0);
			}

			hr = Helpers::PutProp(spMethodInParam, params);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("PutProp"), hr, TEXT("%s"), methodName);
				LEAVE_BLOCK(0);
			}


			/////////////////////////////////////////////////////////////////////////
			*inParamsObj = spMethodInParam.Detach();
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}
		HRESULT ExecMethod(LPCTSTR target, LPCTSTR methodName, IWbemClassObject* inParamsObj, KVSet & outRets, IWbemClassObject** ppoutRets)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);

			//////////////////////////////////////////////////////////////////////////
			CComPtr<IWbemClassObject> spout;
			hr = _Service->ExecMethod(
				CComBSTR(target),///* [in] */ __RPC__in const BSTR strObjectPath,
				CComBSTR(methodName),///* [in] */ __RPC__in const BSTR strMethodName,
				0,///* [in] */ long lFlags,
				NULL,///* [in] */ __RPC__in_opt IWbemContext *pCtx,
				inParamsObj,///* [in] */ __RPC__in_opt IWbemClassObject *pInParams,
				&spout,///* [unique][in][out] */ __RPC__deref_opt_inout_opt IWbemClassObject **ppOutParams,
				NULL///* [unique][in][out] */ __RPC__deref_opt_inout_opt IWbemCallResult **ppCallResult
			);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("ExecMethod"), hr, TEXT("%s.%s"), target, methodName);
				LEAVE_BLOCK(0);
			}

			hr = Helpers::GetProp(spout, outRets);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("GetProp"), hr, TEXT("%s.%s"), target, methodName);
				LEAVE_BLOCK(0);
			}

			if (ppoutRets)
			{
				*ppoutRets = spout.Detach();
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}
	public:
		SmlWmi(
			LPCTSTR Machine,
			LPCTSTR User,
			LPCTSTR Domain,
			LPCTSTR Password,
			LPCTSTR WmiNamespace
		) :
			_Machine(Machine),
			_User(User),
			_Domain(Domain),
			_Password(Password),
			_WmiNamespace(WmiNamespace)
		{
			Helpers::ComposeAuthId(_authid, _User, _Domain, _Password);
		}

		HRESULT Connect()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			BOOL LocalConn = (0 == _Machine.GetLength());
			if (LocalConn)
			{
				_FullNamespace.Format(TEXT("\\\\.\\%s"), _WmiNamespace);
			}
			else
			{
				_FullNamespace.Format(TEXT("\\\\%s\\%s"), _Machine, _WmiNamespace);
			}

			//////////////////////////////////////////////////////////////////////////
			if (LocalConn)
			{
				hr = CoInitializeSecurity(
					NULL,
					-1,                          // COM authentication
					NULL,                        // Authentication services
					NULL,                        // Reserved
					RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
					RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
					NULL,                        // Authentication info
					EOAC_NONE,                   // Additional capabilities
					NULL                         // Reserved
				);
			}
			else
			{
				hr = CoInitializeSecurity(
					NULL,
					-1,                          // COM authentication
					NULL,                        // Authentication services
					NULL,                        // Reserved
					RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
					RPC_C_IMP_LEVEL_IDENTIFY,    // Default Impersonation
					NULL,                        // Authentication info
					EOAC_NONE,                   // Additional capabilities
					NULL                         // Reserved
				);
			}
			if (FAILED(hr))
			{

				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("CoInitializeSecurity"), hr, TEXT(""));
				if (RPC_E_TOO_LATE != hr)
				{
					LEAVE_BLOCK(0);
				}
			}

			//////////////////////////////////////////////////////////////////////////
			CComPtr<IWbemLocator> spLocator;
			hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)& spLocator);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("CoCreateInstance"), hr, TEXT("CLSID_WbemLocator"));
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			CString strDomainUser;
			Helpers::CombineDomainUser(_Domain, _User, strDomainUser);

			//////////////////////////////////////////////////////////////////////////
			CComPtr<IWbemServices> spService;
			if (LocalConn)
			{
				hr = spLocator->ConnectServer(
					CComBSTR(_FullNamespace.GetString()),      // Object path of WMI namespace
					NULL,                    // User name. NULL = current user
					NULL,                    // User password. NULL = current
					0,                       // Locale. NULL indicates current
					NULL,                    // Security flags.
					0,                       // Authority (e.g. Kerberos)
					0,                       // Context object
					&spService                    // pointer to IWbemServices proxy
				);
			}
			else
			{
				hr = spLocator->ConnectServer(
					CComBSTR(_FullNamespace.GetString()),  // Object path of WMI namespace
					CComBSTR(strDomainUser.GetString()),             // User name
					CComBSTR(_Password.GetString()),              // User password
					NULL,                // Locale
					NULL,                // Security flags
					NULL,				 // Authority
					NULL,                // Context object
					&spService                // IWbemServices proxy
				);
			}
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("ConnectServer"), hr, TEXT("%s"), _FullNamespace.GetString());
				LEAVE_BLOCK(0);
			}

			if (LocalConn)
			{
				hr = CoSetProxyBlanket(
					spService,                        // Indicates the proxy to set
					RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
					RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
					NULL,                        // Server principal name
					RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
					RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
					NULL,                        // client identity
					EOAC_NONE                    // proxy capabilities
				);
			}
			else
			{

				hr = CoSetProxyBlanket(
					spService,                           // Indicates the proxy to set
					RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
					RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
					COLE_DEFAULT_PRINCIPAL,         // Server principal name
					RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx
					RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
					&_authid,                       // client identity
					EOAC_NONE                       // proxy capabilities
				);
			}
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("CoSetProxyBlanket"), hr, TEXT(""));
				if (RPC_E_TOO_LATE != hr)
				{
					LEAVE_BLOCK(0);
				}
			}

			_Service.Attach(spService.Detach());
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}
		HRESULT EnumInstances(
			IN LPCTSTR className,
			IN LPCTSTR whereClause,
			std::vector<CComPtr<IWbemClassObject>> & objects,
			KVSetVec & vecProps
		)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CString strWql;
			strWql.Format(TEXT("SELECT * FROM %s"), className);
			if (whereClause && whereClause[0])
			{
				strWql += TEXT(" WHERE ");
				strWql += whereClause;
			}

			hr = Query(strWql.GetString(), objects);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("Query"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			hr = Helpers::Objects2Props(objects, vecProps);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("Objects2Props"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;

		}
		HRESULT EnumAssociators(
			IN LPCTSTR parentClass,
			IN LPCTSTR propName,
			IN LPCTSTR propValue,
			IN LPCTSTR AssocClass,
			OUT std::vector<CComPtr<IWbemClassObject> >& objects,
			KVSetVec& vecProps
		)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CString strWql;
			if (propName && propName[0] && propValue && propValue[0])
			{
				strWql.Format(TEXT("Associators of {%s.%s=%s} where AssocClass=%s"), parentClass, propName, propValue, AssocClass);
			}
			else
			{
				strWql.Format(TEXT("Associators of {%s} where AssocClass=%s"), parentClass, AssocClass);
			}

			hr = Query(strWql.GetString(), objects);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("Query"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			hr = Helpers::Objects2Props(objects, vecProps);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("Objects2Props"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}
		HRESULT NewObject(LPCTSTR className, KVSet& props, CComVariant& object)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CComPtr<IWbemClassObject> spClass;
			//KVSet classProps;
			hr = GetClass(className, &spClass, nullptr);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("ClassName"), hr, TEXT("%s"), className);
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			CComPtr<IWbemClassObject> spObj;
			hr = spClass->SpawnInstance(0, &spObj);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("SpawnInstance"), hr, TEXT("%s"), className);
				LEAVE_BLOCK(0);
			}
			//for (auto & kv : props)
			//{
			//	auto iter = classProps.find(kv.first);
			//	if (iter != classProps.end())
			//	{
			//		iter->second = kv.second;
			//	}
			//}
			hr = Helpers::PutProp(spObj, props);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("Put"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			object = (IUnknown*)(spObj.p);
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

		HRESULT GetClass(LPCTSTR ClassName, IWbemClassObject** ppout, KVSet* props)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CComPtr<IWbemClassObject> spclass;
			hr = _Service->GetObject(CComBSTR(ClassName), 0, NULL, &spclass, NULL);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("GetObject"), hr, TEXT("%s"), ClassName);
				LEAVE_BLOCK(0);
			}

			if (props)
			{
				hr = Helpers::GetProp(spclass, *props);
				if (FAILED(hr))
				{
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("GetProp"), hr, TEXT("%s"), ClassName);
					LEAVE_BLOCK(0);
				}
			}

			*ppout = spclass.Detach();
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}


		HRESULT EnumMethods(LPCTSTR className, std::vector<MethodInfo>& mis)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			CComPtr<IWbemClassObject> spClass;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			hr = GetClass(className, &spClass, nullptr);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("GetClass"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			hr = spClass->BeginMethodEnumeration(
				0///* [in] */ long lEnumFlags
			);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("BeginMethodEnumeration"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}


			for (;;)
			{
				mis.resize(mis.size() + 1);
				auto & mi = mis.back();
				hr = spClass->NextMethod(
					0,///* [in] */ long lFlags,
					&mi._name,///* [unique][in][out] */ BSTR *pstrName,
					&mi._InSignature,///* [unique][in][out] */ IWbemClassObject **ppInSignature,
					&mi._OutSignature///* [unique][in][out] */ IWbemClassObject **ppOutSignature
				);
				if (FAILED(hr))
				{
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("NextMethod"), hr, TEXT(""));
					LEAVE_BLOCK(0);
					break;
				}
				else if (S_FALSE == hr)
				{
					break;
				}
				else if (NULL == (LPCTSTR)(mi._name))
				{
					mis.resize(mis.size() - 1);
					break;
				}

				if (mi._InSignature)
				{
					hr = Helpers::GetProp(mi._InSignature, mi._inParams);
					if (FAILED(hr))
					{
						break;
					}
				}
				
				if (mi._OutSignature)
				{
					hr = Helpers::GetProp(mi._OutSignature, mi._outReturns);
					if (FAILED(hr))
					{
						break;
					}
				}
				
			}
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			if (spClass)
			{
				HRESULT hrTemp = spClass->EndMethodEnumeration();
				if (FAILED(hrTemp))
				{
					hr = hrTemp;
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("EndMethodEnumeration"), hr, TEXT(""));
					//LEAVE_BLOCK(0);
				}
			}
			
			return hr;
		}

		/*
		one of 'obj' and 'class' can be null
		*/
		HRESULT InvokeMethod(IWbemClassObject* obj, LPCTSTR className, LPCTSTR methodName, KVSet & inparams, KVSet& outRets, IWbemClassObject ** outRetsObj)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CComPtr<IWbemClassObject> spinparams;
			CString strClass = className;
			if (obj)
			{
				//CComVariant propvalue;
				//Helpers::GetNamedProp(obj, TEXT("__CLASS"), &propvalue);
				//strClass = propvalue.bstrVal;
				Helpers::ObjClass(obj, strClass);
			}
			hr = MakeInParamsObject(strClass.GetString(), methodName, inparams, &spinparams);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("MakeInParamsObject"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			CString target = className;
			if (obj)
			{
				//CComVariant propvalue;
				//Helpers::GetNamedProp(obj, TEXT("__PATH"), &propvalue);
				//target = propvalue.bstrVal;
				Helpers::ObjPath(obj, target);
			}

			hr = ExecMethod(target, methodName, spinparams, outRets, outRetsObj);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("ExecMethod"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}
	
		HRESULT DeleteObject(LPCTSTR path)
		{
			HRESULT hr = _Service->DeleteInstance(CComBSTR(path), 0, nullptr, nullptr);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("DeleteInstance"), hr, TEXT("%s"), path);
				//LEAVE_BLOCK(0);
			}
			return hr;
		}

	};


	
}