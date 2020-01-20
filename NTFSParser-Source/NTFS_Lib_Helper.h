#pragma once 

#include "stdafx.h"
#include "NTFS_Lib.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicMiscHelper.h"

struct CNtfsParserHelper 
{
	//////////////////////////////////////////////////////////////////////////
	static INT InitVolume(IFileDevice * pFileVolume, CNTFSVolume ** ppNTFSVolume);
	static INT FindAndParseFileRecord(CONST CNTFSVolume * pNTFSVolume, LPCTSTR pszFullPath, CFileRecord ** ppFileRecord); //pszFullPath not including volume root path, and MUST begin with "\"
	static INT ParseFileRecordByRefecenceNumber(CONST CNTFSVolume * pNTFSVolume, LONGLONG llFileReference, CFileRecord ** ppFileRecord);

	static INT GetReparsePointData(CONST CNTFSVolume * pNTFSVolume, LONGLONG llFileReference, OUT CFileRecord ** ppFileRecord, OUT BYTE ** ppData, vector<CString> & actualPath);

	
	//////////////////////////////////////////////////////////////////////////
	//<sonmi01>2015-4-24 ###???
	struct IFindPathForRefecenceNumberCallback
	{
		virtual INT Operate(CONST CFileRecord * fr) = 0;
	};

	class CFindPathForRefecenceNumberCallback : public IFindPathForRefecenceNumberCallback
	{
	public:
		vector<CString> m_fileNames;
	public:
		virtual INT Operate(CONST CFileRecord * fr)
		{
			TCHAR szName[512] = { 0 };
			fr->GetFileName(szName, _countof(szName));
			m_fileNames.push_back(szName);
			return S_OK;
		}
	};

	static HRESULT FindPathForRefecenceNumber(CONST CNTFSVolume * pNTFSVolume, LONGLONG llFileReference, IFindPathForRefecenceNumberCallback * pCallback);

	//////////////////////////////////////////////////////////////////////////
	static INT EnumHelper(
		CNTFSVolume * pNTFSVolume, 
		CFileRecord * pFileRecord,
		ISubEntryCallBack * pSubEntryCallBackFile, 
		ISubEntryCallBack * pSubEntryCallBackPreDir,
		ISubEntryCallBack * pSubEntryCallBackPostDir
		);
	static INT Enum(
		IFileDevice * pFileDeviceVolume,
		LPCTSTR pszPath,
		ISubEntryCallBack * pSubEntryCallBackFile,
		ISubEntryCallBack * pSubEntryCallBackPreDir,
		ISubEntryCallBack * pSubEntryCallBackPostDir
		);
	static VOID EnumFindFiles( 
		IFileDevice * pFileDeviceVolume, 
		LPCTSTR pszRootPath, 
		LPCTSTR pszIS, 
		LPCTSTR pszISNOT, 
		vector<CString> * pFiles,
		vector<LONGLONG> * pFileRefences);

private:
	static INT FindAndParseFileRecordHelper(CONST CNTFSVolume * pNTFSVolume, LPCTSTR pszPath, LPCTSTR pszName, CFileRecord ** ppFileRecord);

	class CSubEntryCallBack : public ISubEntryCallBack
	{
		CNTFSVolume * m_pNTFSVolume; //external object
		CFileRecord * m_pParentFileRecord; //external object
		ISubEntryCallBack * m_pSubEntryCallBackFile; //external object
		ISubEntryCallBack * m_pSubEntryCallBackPreDir; //external object
		ISubEntryCallBack * m_pSubEntryCallBackPostDir; //external object

	public:
		CSubEntryCallBack(
			CNTFSVolume * pNTFSVolume, 
			CFileRecord * pParentFileRecord, 
			ISubEntryCallBack * pSubEntryCallBackFile, 
			ISubEntryCallBack * pSubEntryCallBackPreDir,
			ISubEntryCallBack * pSubEntryCallBackPostDir
			);

		virtual INT Operate(const CIndexEntry *ie);
	};

	struct CEnumFindFileWorker
	{
	private:
		INT m_nDepth;
		vector<CString> m_Paths;

		//******************************************
		CString m_strIs;
		CString m_strIsNot;

		//******************************************
		vector<CString> * m_pFiles;
		vector<LONGLONG> * m_pFileReferences;


		LONGLONG m_FileCount;
		LONGLONG m_DirCount;

		VOID ComposeFullPath(CString & strFullPath);

	public:
		CEnumFindFileWorker(vector<CString> * pFiles, vector<LONGLONG> * pFileReferences);

		virtual ~CEnumFindFileWorker();

		VOID SetPattern(LPCTSTR pszIs, LPCTSTR pszIsNot);

		VOID EnterDir(CONST CIndexEntry * pie);
		VOID LeaveDir(CONST CIndexEntry * pie);

		VOID EnterFile(CONST CIndexEntry * pie);
		VOID LeaveFile(CONST CIndexEntry * pie);
	};

	class CEnumFindFileOp : public ISubEntryCallBack
	{
		CEnumFindFileWorker & m_worker;

	public:
		CEnumFindFileOp(CEnumFindFileWorker & worker);
		virtual INT Operate(const CIndexEntry * ie);
	};

	class CEnumFindDirPreOp : public ISubEntryCallBack
	{
		CEnumFindFileWorker & m_worker;

	public:
		CEnumFindDirPreOp(CEnumFindFileWorker & worker);
		virtual INT Operate(const CIndexEntry * ie);
	};

	class CEnumFindDirPostOp : public ISubEntryCallBack
	{	
		CEnumFindFileWorker & m_worker;

	public:
		CEnumFindDirPostOp(CEnumFindFileWorker & worker);
		virtual INT Operate(const CIndexEntry * ie);
	};
};

class CNtfsAttrReader : public IFileDevice
{
private:
	CONST CAttrBase * m_pAttrBase; //external object
	LONGLONG m_FilePointer;
	LONGLONG m_TotalSize;
	LONG m_RefCount;

public:
	CNtfsAttrReader( CONST CAttrBase * pAttrBase );
	IMP_CLASS_DESTRUCTOR(CNtfsAttrReader);

public:
	//struct IFileDevice : public ISimpleUnknown
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

	virtual LPCTSTR WhoAmI();
	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);
	virtual HRESULT GetFileSize(LONGLONG * pFileSize);
	virtual HANDLE GetRawHandle();

	//};

private: //******************************************
	CNtfsAttrReader(CONST CNtfsAttrReader&);
	CNtfsAttrReader& operator=(CONST CNtfsAttrReader&);
};


HRESULT CreateInstanceCNtfsAttrReader( CONST CAttrBase * pAttrBase, IFileDevice ** ppObj, BOOL bCreateMember );


#if 0 //<sonmi01>2015-4-23 ###???
namespace ParseReparsePoints_NS
{


	class CParseReparsePoints
	{
	public:
		CParseReparsePoints();
		~CParseReparsePoints();

	public:
		static HRESULT ParseIndexEntriesMemory(CONST INDEX_ENTRY_RP * entry, LONG length, vector<LONGLONG> & vecFileRef); //no header
		static HRESULT ParseIndexEntriesInIndexRoot(CAttr_IndexRoot * attr, vector<LONGLONG> & vecFileRef);
		
		static BOOL IsSubNodePtr(CONST INDEX_ENTRY_RP * ieRp);
		ULONGLONG GetSubNodeVCN(CONST INDEX_ENTRY_RP * ieRp) CONST;
	};
} //end namespace ParseReparsePoints_NS
#endif