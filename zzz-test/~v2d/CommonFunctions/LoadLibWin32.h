#pragma once

#include "stdafx.h"
#include "LoadLibInterface.h"

class CLoadLib : public ILoadLib
{
	HMODULE			m_hMoudle;
	CONST CString	m_ModuleName;
	LONG			m_RefCount;

public:
	CLoadLib(LPCTSTR pModuleName) :
	  m_hMoudle(NULL),
		  m_ModuleName(pModuleName),
		  m_RefCount(0)
	  {
	  }

	  virtual ~CLoadLib()
	  {
		  DestroyMember();
	  }


	  //struct ILoadLib : public ISimpleUnknown
	  //{
	  //	//struct ISimpleUnknown : public IUnknown
	  //	//{
	  //	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	  //	//	//	IUnknown
	  //	//	//{
	  //	//	//public:
	  //	//	//	BEGIN_INTERFACE
	  //	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	  //	//	//		/* [in] */ REFIID riid,
	  //	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	  //	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	  //	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	  //	//	//	END_INTERFACE
	  //	//	//};
	  IMP_IUNKNOWN_METHOD(m_RefCount)

		  virtual HRESULT CreateMember()
	  {
		  HRESULT hr = S_OK;
		  DWORD LastError = 0;

		  if (NULL == m_hMoudle)
		  {
			  m_hMoudle = LoadLibrary(m_ModuleName);
			  if (NULL == m_hMoudle)
			  {
				  LastError = GetLastError();
				  hr = HRESULT_FROM_WIN32(LastError);
			  }
		  }
		  return hr;
	  }

	  virtual HRESULT DestroyMember()
	  {
		  if (m_hMoudle)
		  {
			  FreeLibrary(m_hMoudle);
			  m_hMoudle = NULL;
		  }
		  return S_OK;
	  }
	  //	//};
	  virtual FARPROC GetProc(LPCTSTR pProcName)
	  {
		  HRESULT hr = S_OK;
		  DWORD	LastError = 0;
		  FARPROC ProcAddr = NULL;

		  BEGIN_BLOCK(0);
		  hr = CreateMember();
		  if (FAILED(hr))
		  {
			  break;
		  }

		  CStringA ProcNameA(pProcName);
		  ProcAddr = GetProcAddress(m_hMoudle, ProcNameA.GetString());
		  if (NULL == ProcAddr)
		  {
			  LastError = GetLastError();
			  hr = HRESULT_FROM_WIN32(LastError);
		  }
		  END_BLOCK(0);

		  return ProcAddr;
	  }
	  //};
	  //
};
