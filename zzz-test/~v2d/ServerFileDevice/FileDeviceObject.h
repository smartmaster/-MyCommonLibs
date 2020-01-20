// FileDeviceObject.h : Declaration of the CFileDeviceObject

#pragma once
#include "resource.h"       // main symbols



#include "ServerFileDevice_i.h"



using namespace ATL;


// CFileDeviceObject

class ATL_NO_VTABLE CFileDeviceObject :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFileDeviceObject, &CLSID_FileDeviceObject>,
	public IFileDeviceObject
{
public:
	CFileDeviceObject()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_FILEDEVICEOBJECT)


	BEGIN_COM_MAP(CFileDeviceObject)
		COM_INTERFACE_ENTRY(IFileDeviceObject)
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		InitMember();
		return S_OK;
	}

	void FinalRelease()
	{
		DestroyMember();
	}

private:	//******************************************
	CString m_strFileName;
	HANDLE m_hFile;

	//******************************************
	VOID InitMember();
	VOID DestroyMember();

public:
	STDMETHOD(SetPointerEx)(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	STDMETHOD(Read)(ULONG nNumberOfBytesToRead , ULONG* lpNumberOfBytesRead , BYTE* pBuffer);
	STDMETHOD(Write)(ULONG nNumberOfBytesToWrite ,  ULONG* lpNumberOfBytesWritten ,  BYTE* pBuffer);

	STDMETHOD(Create)(BSTR bstrFileName , ULONG ulDesiredAccess , ULONG ulShareMode , ULONG ulCreationDisposition , ULONG ulFlagsAndAttributes);
	STDMETHOD(Close)(void);

	STDMETHOD(GetFileSizeEx)(LONGLONG* pllFileSize);
};

OBJECT_ENTRY_AUTO(__uuidof(FileDeviceObject), CFileDeviceObject)
