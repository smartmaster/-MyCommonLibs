#include "stdafx.h"
#include "InterfaceRunApp.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\PublicHeader\PublicDTraceLib.h"

class CRunAppWin32 : public IRunApp
{
private:
	CString m_Username; /***Incomming***/
	CString m_Domain; /***Incomming***/
	CString m_Password; /***Incomming***/

	CString m_CommandLine; /***Incomming***/
	CString m_CurDir; /***Incomming***/


	LONG m_RefCount; /***Internal***/

	HANDLE m_hToken; /***Internal to cleanup***/
	PROCESS_INFORMATION m_ProcInfo; /***Internal to cleanup***/
public:
	CRunAppWin32(LPCTSTR Username, LPCTSTR Domain, LPCTSTR Password, LPCTSTR CommandLine, LPCTSTR CurDir);

	IMP_CLASS_DESTRUCTOR(CRunAppWin32);

public: //begin IRunApp 
	//struct IRunApp : public ISimpleUnknown
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
	IMP_IUNKNOWN_METHODS(m_RefCount);

	//	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("CRunAppWin32:IRunApp"));

	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//};

	virtual HRESULT Start();
	virtual HRESULT Wait(ULONG MillSec);
	virtual HRESULT Terminate();
	virtual HRESULT GetExitCode(ULONG & ExitCode);
	//};
	//end IRunApp
};