#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include <vector>
using namespace std;

#include "..\PublicHeader\PublicSimpleUnknown.h"
#include "..\PublicHeader\PublicVMemPtr.h"
#include "..\PublicHeader\PublicDTraceLib.h"


//////////////////////////////////////////////////////////////////////////
struct TReplayedData
{
	ULONG m_Flags;
	ULONG m_Enum;
	ULONGLONG m_FileOffset;
	ULONGLONG m_DataLength;
	BYTE * m_pData;

	//////////////////////////////////////////////////////////////////////////
	TReplayedData();
	HRESULT Allocate(LONG BuffSize);
	HRESULT Free();
};


//////////////////////////////////////////////////////////////////////////
struct IVHDXLogReplayCallback : public ISimpleUnknown
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

	virtual HRESULT OnReplayedData(ULONGLONG FileOffset, ULONGLONG Length, CONST BYTE * pBuffer, BOOL bZero) = 0;
	virtual CONST vector<TReplayedData> & GetReplayedData() = 0;
	virtual ULONGLONG GetPayloadBlockSartOffset() = 0;
	virtual LONGLONG GetMaxOffset() = 0;
	virtual LONGLONG GetMinOffset() = 0;
};


//////////////////////////////////////////////////////////////////////////
class CVHDXLogReplayCallback : public IVHDXLogReplayCallback
{
private:
	ULONGLONG m_PayloadBlockSartOffset; /***INCOMMING***/

	LONG m_RefCount; /***INTERNAL***/
	vector<TReplayedData> m_ReplayedData; /***INTERNAL***/ /***TO CLEANUP***/
	LONGLONG m_MinOffset; /***INTERNAL***/
	LONGLONG m_MaxOffset; /***INTERNAL***/

public:
	CVHDXLogReplayCallback(ULONGLONG PayloadBlockSartOffset);

	IMP_CLASS_DESTRUCTOR(CVHDXLogReplayCallback);

	//struct IVHDXLogReplayCallback : public ISimpleUnknown
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
	IMP_WHO_AM_I(TEXT("CVHDXLogReplayCallback:IVHDXLogReplayCallback"));
	//	//	virtual HRESULT CreateMember() = 0;
	virtual HRESULT CreateMember();
	//	//	virtual HRESULT DestroyMember() = 0;
	virtual HRESULT DestroyMember();
	//	//};

	//	virtual HRESULT OnReplayedData(ULONGLONG FileOffset, ULONGLONG Length, CONST BYTE * pBuffer, BOOL bZero) = 0;
	virtual HRESULT OnReplayedData(ULONGLONG FileOffset, ULONGLONG Length, CONST BYTE * pBuffer, BOOL bZero);
	//	virtual CONST vector<TReplayedData> & GetReplayedData() = 0;
	virtual CONST vector<TReplayedData> & GetReplayedData();
	virtual ULONGLONG GetPayloadBlockSartOffset();
	virtual LONGLONG GetMaxOffset();
	virtual LONGLONG GetMinOffset();
	//};

private:

};

//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstcanceCVHDXLogReplayCallback(ULONGLONG PayloadBlockSartOffset, IVHDXLogReplayCallback ** ppObj, BOOL bCreateMember);