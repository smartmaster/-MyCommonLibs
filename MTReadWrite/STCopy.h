#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <map>
using namespace std;

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "FileCopyInterface.h"
#include "VMemPtr.h"
#include "StopTimer.h"

class CSTCopy : public IFileCopy
{
private:
	IFileDevice * m_pSource; /***Incomming***/
	IFileDevice * m_pDest; /***Incomming***/
	CONST map<LONGLONG, LONGLONG> * m_pDataRange; /***Incomming***/ //pairs of beginning offset and end offset
	LONGLONG m_OffsetDiff; /***Incomming***/

	CVMemPtr<BYTE> m_RWBuffer; /***Internal***/
	static CONST ULONG IO_BUFFER_MAX_DATA_LENGTH = 2 * 1024 * 1024;

	LONG m_RefCount; /***Internal***/

	BOOL m_bStartPerf; /***INTERNAL***/
	CStopTimer m_StopTimer; /***INTERNAL***/

public:
	CSTCopy(IFileDevice * pSource, IFileDevice * pDest, CONST map<LONGLONG, LONGLONG> * pDataRange, LONGLONG OffsetDiff);
	IMP_CLASS_DESTRUCTOR(CSTCopy);

public:
	//struct IFileCopy : public ISimpleUnknown
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
	IMP_WHO_AM_I(TEXT("CSTCopy:IFileCopy"));

	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//};
	virtual HRESULT Copy();
	virtual HRESULT InitParams(LPVOID pPrarams);
	//};

private:
	HRESULT ProcessBuffer(LONGLONG StartOffset, LONGLONG DataLength);

};

//////////////////////////////////////////////////////////////////////////
//// API
HRESULT CreateInstanceCSTCopy(IFileDevice * pSource, IFileDevice * pDest, CONST map<LONGLONG, LONGLONG> * pDataRange, LONGLONG OffsetDiff, IFileCopy ** ppObj, BOOL bCreateMember);

HRESULT SimpleSTCopy(IFileDevice * pSource, IFileDevice * pDest);