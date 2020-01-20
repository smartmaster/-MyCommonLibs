#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include <vector>
using namespace std;

#include "..\PublicHeader\PublicSimpleUnknown.h"


//////////////////////////////////////////////////////////////////////////
class CSimpleEventHelper
{
private:
	HANDLE m_hEvent;

public:
	CSimpleEventHelper(BOOL bManualReset);
	BOOL IsValid() CONST;

	~CSimpleEventHelper();

	HRESULT Set();
	HRESULT Reset();
	DWORD Wait(DWORD dwMillSec);
};


//////////////////////////////////////////////////////////////////////////
namespace NS_MultipleEventHelper
{
	CONST LONG ERROR_INDEX = 0; 
	CONST LONG CANCEL_INDEX = 1;
	CONST LONG SYNC_INDEX = 2;
	CONST LONG DONE_INDEX = 3;
	CONST LONG SENDER_INDEX = 4;	//<sonmi01>2014-3-28 ###???
	CONST LONG RECEIVER_INDEX = 5;
	CONST LONG HANDLER_INDEX = 6;

	static CONST BOOL bManualResetArr[] = { TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE }; //<sonmi01>2014-3-28 ###???
}

//////////////////////////////////////////////////////////////////////////
struct IMultipleEventHelper : public ISimpleUnknown
{
	//struct ISimpleUnknown : public IUnknown
	//{
	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	IUnknown
	//	//{
	//	//public:
	//	//	BEGIN_INTERFACE
	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//		/* [in] */ REFIID riid,
	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	END_INTERFACE
	//	//};

	//	virtual LPCTSTR WhoAmI() = 0;
	//	virtual HRESULT CreateMember() = 0;
	//	virtual HRESULT DestroyMember() = 0;
	//};

	virtual LONG GetEventCount() = 0;

	virtual HRESULT Set(LONG Index) = 0;
	virtual HRESULT SetAll() = 0;
	virtual HRESULT Reset(LONG Index) = 0;
	virtual HRESULT ResetAll() = 0;
	virtual DWORD Wait(LONG Index, DWORD MillSec) = 0;

	virtual DWORD WaitMultiple(BOOL bWaitAll, DWORD MillSec) = 0;
};

//////////////////////////////////////////////////////////////////////////
class CMultipleEventHelper : public IMultipleEventHelper
{
private:
	vector<BOOL> m_bManualResetVector; /***INCOMMING***/
	LONG m_RefCount;  /***INTERNAL***/
	vector<HANDLE> m_hEvents; /***INTERNAL***/ /***TO CLEANUP***/

public:
	CMultipleEventHelper(CONST BOOL * pbManualReset, LONG Count);

	IMP_CLASS_DESTRUCTOR(CMultipleEventHelper);

	//struct ISimpleUnknown : public IUnknown
	//{
	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	IUnknown
	//	//{
	//	//public:
	//	//	BEGIN_INTERFACE
	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//		/* [in] */ REFIID riid,
	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	END_INTERFACE
	//	//};
	IMP_IUNKNOWN_METHODS(m_RefCount);

	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("CMultipleEventHelper:ISimpleUnknown"));


	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();

	virtual LONG GetEventCount();


	virtual HRESULT Set(LONG Index);
	virtual HRESULT SetAll();
	virtual HRESULT Reset(LONG Index);
	virtual HRESULT ResetAll();
	virtual DWORD Wait(LONG Index, DWORD MillSec);

	virtual DWORD WaitMultiple(BOOL bWaitAll, DWORD MillSec);
	//};
};


//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCMultipleEventHelper(CONST BOOL * pbManualReset, LONG Count, IMultipleEventHelper ** ppIMultipleEventHelper, BOOL bCreateMember);