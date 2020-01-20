#pragma once

#include "stdafx.h"
#include <virtdisk.h>
#include "MiscHelper.h"
#include "VirtualDiskHelper.h"
#include "VDSVolumeDiskHelper.h"


//******************************************
struct IVhdHelper
{
	virtual INT Create(
		LPCTSTR szPath, 
		VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, 
		ULONGLONG MaximumSize, 
		LPCTSTR ParentPath) = 0;
	virtual INT Attach(ATTACH_VIRTUAL_DISK_FLAG AttachFlags) = 0;
	virtual ULONG FindUninitalizedVHD() = 0;
	virtual INT Dettach() = 0;
	virtual INT Close() = 0;
};

//******************************************
class CVhdHelper : public IVhdHelper
{
private:
	HANDLE m_hVhd; //internal object
	ULONG m_ulDiskNumber;

public: //******************************************
	CVhdHelper();
	INT CreateMember();
	INT DestrotMember();
	virtual ~CVhdHelper();

	//******************************************
	virtual INT Create(
		LPCTSTR szPath, 
		VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, 
		ULONGLONG MaximumSize, 
		LPCTSTR ParentPath);
	virtual INT Attach(ATTACH_VIRTUAL_DISK_FLAG AttachFlags);
	virtual ULONG FindUninitalizedVHD();
	virtual INT Dettach();
	virtual INT Close();

private: //******************************************
	CVhdHelper(CONST CVhdHelper&);
	CVhdHelper& operator=(CONST CVhdHelper&);
};

//******************************************
class CVdsVhdHelper : public IVhdHelper
{
private:
	CVDSService m_MyService;
	CVdsVdProvider m_MyVdProvider;
	CComPtr<IVdsVDisk> m_spIVdsVDisk;
	CComPtr<IVdsOpenVDisk> m_spIVdsOpenVDisk;
	CVdsDisk m_MyDisk;


public: //******************************************
	CVdsVhdHelper();
	INT CreateMember();
	INT DestrotMember();
	virtual ~CVdsVhdHelper();

	//******************************************
	virtual INT Create(
		LPCTSTR szPath, 
		VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, 
		ULONGLONG MaximumSize, 
		LPCTSTR ParentPath);
	virtual INT Attach(ATTACH_VIRTUAL_DISK_FLAG AttachFlags);
	virtual ULONG FindUninitalizedVHD();
	virtual INT Dettach();
	virtual INT Close();

private: //******************************************
	CVdsVhdHelper(CONST CVdsVhdHelper&);
	CVdsVhdHelper& operator=(CONST CVdsVhdHelper&);
};