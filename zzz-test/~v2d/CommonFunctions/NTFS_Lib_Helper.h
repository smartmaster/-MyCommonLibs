#pragma once 

#include "stdafx.h"
#include "NTFS_Lib.h"
#include "..\ServerFileDevice\ServerFileDevice_i.h"
#include "MiscHelper.h"

struct CNtfsParserHelper 
{
	static INT InitVolume(IFileDeviceObject * pFileVolume, CNTFSVolume ** ppNTFSVolume);
	static INT FindAndParseFileRecord(CONST CNTFSVolume * pNTFSVolume, LPCTSTR pszFullPath, CFileRecord ** ppFileRecord); //pszFullPath not including volume root path, and MUST begin with "\"
	static INT ParseFileRecordByRefecenceNumber(CONST CNTFSVolume * pNTFSVolume, LONGLONG llFileReference, CFileRecord ** ppFileRecord);


	static INT EnumHelper(
		CNTFSVolume * pNTFSVolume, 
		CFileRecord * pFileRecord,
		ISubEntryCallBack * pSubEntryCallBackFile, 
		ISubEntryCallBack * pSubEntryCallBackPreDir,
		ISubEntryCallBack * pSubEntryCallBackPostDir
		);
	static INT Enum(
		IFileDeviceObject * pFileDeviceVolume,
		LPCTSTR pszPath,
		ISubEntryCallBack * pSubEntryCallBackFile,
		ISubEntryCallBack * pSubEntryCallBackPreDir,
		ISubEntryCallBack * pSubEntryCallBackPostDir
		);
	static VOID EnumFindFiles( 
		IFileDeviceObject * pFileDeviceVolume, 
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

		VOID ComposeFullPath(CString & strFullPath);

	public:
		CEnumFindFileWorker(vector<CString> * pFiles, vector<LONGLONG> * pFileReferences);

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

class CNtfsAttrReader : public IFileDeviceObject
{
private:
	CONST CAttrBase * m_pAttrBase; //external object
	LONGLONG m_FilePointer;
	LONGLONG m_TotalSize;

public:
	CNtfsAttrReader(CONST CAttrBase * pAttrBase);

	//******************************************
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);

	//******************************************
	virtual HRESULT STDMETHODCALLTYPE Create(BSTR bstrFileName = NULL, ULONG ulDesiredAccess = 0, ULONG ulShareMode = 0, ULONG ulCreationDisposition = 0, ULONG ulFlagsAndAttributes = 0);
	virtual HRESULT STDMETHODCALLTYPE SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	virtual HRESULT STDMETHODCALLTYPE Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
	virtual HRESULT STDMETHODCALLTYPE Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, BYTE* pBuffer);
	virtual HRESULT STDMETHODCALLTYPE Close(void);
	virtual HRESULT STDMETHODCALLTYPE GetFileSizeEx(LONGLONG* pllFileSize);

private: //******************************************
	CNtfsAttrReader(CONST CNtfsAttrReader&);
	CNtfsAttrReader& operator=(CONST CNtfsAttrReader&);
};
