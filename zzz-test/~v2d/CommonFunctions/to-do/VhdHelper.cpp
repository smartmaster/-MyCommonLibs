#include "stdafx.h"
#include "VhdHelper.h"
#include "VirtualDiskHelper.h"

//******************************************
DEFINE_GUID(VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT_COPY, 0xec984aec, 0xa0f9, 0x47e9, 0x90, 0x1f, 0x71, 0x41, 0x5a, 0x66, 0x34, 0x5b);

CVhdHelper::CVhdHelper() :
m_hVhd(NULL),
m_ulDiskNumber(-1)
{

}

INT CVhdHelper::CreateMember()
{
	return 0;
}

INT CVhdHelper::DestrotMember()
{
	if (VALID_HANLE(m_hVhd))
	{
		CloseHandle(m_hVhd);
		m_hVhd = NULL;
	}

	return 0;
}

CVhdHelper::~CVhdHelper()
{
	DestrotMember();
}

INT CVhdHelper::Create( LPCTSTR szPath, VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, ULONGLONG MaximumSize, LPCTSTR ParentPath )
{
	DSTART(999);

	INT nStatus = 0;

	CREATE_VIRTUAL_DISK_PARAMETERS Parameter;
	ZeroMemory(&Parameter, sizeof(CREATE_VIRTUAL_DISK_PARAMETERS));
	CVirtualDiskHelper::InitializeCreateVirtualDiskParameters(
		&Parameter,//PCREATE_VIRTUAL_DISK_PARAMETERS Parameter, 
		MaximumSize,//ULONGLONG MaximumSize,
		ParentPath,//LPCTSTR ParentPath,
		NULL//LPCTSTR SourcePath
		);

	nStatus = CVirtualDiskHelper::Create(
		szPath,//LPCTSTR szPath,  
		VirtualDiskAccessMask,//VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask,
		&Parameter,//PCREATE_VIRTUAL_DISK_PARAMETERS Parameters,
		&m_hVhd//OUT PHANDLE pHandle
		);
	if (nStatus)
	{
		m_hVhd = NULL;
		//dwLastError = GetLastError();
		_OLD_DSETLASTSTATUS(nStatus);
		//nStatus = ___api_error_status;
		_OLD_DAPIERR((0, DRNPOS, TEXT("Create"), nStatus, TEXT("[Path=%s, ParentPath=%s, MaximumSize=%I64d]"), szPath, ParentPath, MaximumSize));
		//break;
	}

	return nStatus;
}

INT CVhdHelper::Attach(ATTACH_VIRTUAL_DISK_FLAG AttachFlags)
{
	DSTART(999);

	INT nStatus = CVirtualDiskHelper::Attach(
		m_hVhd,//HANDLE hVhd
		AttachFlags
		);
	if (nStatus)
	{
		//dwLastError = GetLastError();
		_OLD_DSETLASTSTATUS(nStatus);
		//nStatus = ___api_error_status;
		_OLD_DAPIERR((0, DRNPOS, TEXT("Attach"), nStatus));
		//break;
	}

	return nStatus;
}

ULONG CVhdHelper::FindUninitalizedVHD()
{
	m_ulDiskNumber = CVirtualDiskHelper::FindUninitalizedVHD();
	return m_ulDiskNumber;
}

INT CVhdHelper::Dettach()
{
	DSTART(999);

	INT nStatus = CVirtualDiskHelper::Dettach(
		m_hVhd//HANDLE hVhd
		);
	if (nStatus)
	{
		//dwLastError = GetLastError();
		_OLD_DSETLASTSTATUS(nStatus);
		//nStatus = ___api_error_status;
		_OLD_DAPIERR((0, DRNPOS, TEXT("Dettach"), nStatus));
		//break;
	}

	return nStatus;
}

INT CVhdHelper::Close()
{
	DestrotMember();
	return 0;
}

CVdsVhdHelper::CVdsVhdHelper()
{

}

INT CVdsVhdHelper::CreateMember()
{
	INT nStatus = 0;
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************
		hr = m_MyService.Load();
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("m_MyService.Load"), hr));
			break;
		}

		//******************************************
		vector<IVdsVdProvider*> vdProvides;
		hr = m_MyService.QueryVirtualDiskProvides(vdProvides);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("m_MyService.QueryVirtualDiskProvides"), hr));
			break;
		}

		//******************************************
		ATLASSERT(1 == vdProvides.size());
		hr = m_MyVdProvider.Initialize(vdProvides[0], FALSE);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("m_MyVdProvider.Initialize"), hr));
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

INT CVdsVhdHelper::DestrotMember()
{
	return 0;
}

CVdsVhdHelper::~CVdsVhdHelper()
{
	DestrotMember();
}

INT CVdsVhdHelper::Create( LPCTSTR szPath, VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, ULONGLONG MaximumSize, LPCTSTR ParentPath )
{
	INT nStatus = 0;
	HRESULT hr= S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************
		VIRTUAL_STORAGE_TYPE VirtualStorageType;
		VirtualStorageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_VHD;
		VirtualStorageType.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT_COPY;

		VDS_CREATE_VDISK_PARAMETERS CreateDiskParameters;
		ZeroMemory(&CreateDiskParameters.UniqueId, sizeof(GUID));
		CreateDiskParameters.MaximumSize = MaximumSize;
		CreateDiskParameters.BlockSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_BLOCK_SIZE;
		CreateDiskParameters.SectorSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_SECTOR_SIZE;
		//CString strParentPath =ParentPath;
		CreateDiskParameters.pParentPath = const_cast<LPTSTR>(ParentPath);
		CreateDiskParameters.pSourcePath = NULL;

		CComPtr<IVdsAsync> spIVdsAsync;
		CString strPath = szPath;
		hr = m_MyVdProvider.m_pIVdsVdProvider->CreateVDisk(
			&VirtualStorageType,//[in]   PVIRTUAL_STORAGE_TYPE VirtualDeviceType,
			const_cast<LPTSTR>(strPath.GetString()),//[in]   LPWSTR pPath,
			//VirtualDiskAccessMask,//[in]	VIRTUAL_DISK_ACCESS_MASK AccessMask,
			NULL,//[in]   LPWSTR pStringSecurityDescriptor,
			CREATE_VIRTUAL_DISK_FLAG_NONE,//[in]   CREATE_VIRTUAL_DISK_FLAG Flags,
			0,//[in]   ULONG ProviderSpecificFlags,
			0,//[in]   ULONG Reserved,
			&CreateDiskParameters,//[in]   PVDS_CREATE_VDISK_PARAMETERS pCreateDiskParameters,
			&spIVdsAsync//[out]  IVdsAsync **ppAsync
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateVDisk"), hr));
			break;
		}

		//******************************************
		HRESULT hrResult = E_FAIL;
		VDS_ASYNC_OUTPUT AsyncOut;
		ZeroMemory(&AsyncOut, sizeof(VDS_ASYNC_OUTPUT));
		hr = spIVdsAsync->Wait(
			&hrResult,//[out]  HRESULT *pHrResult,
			&AsyncOut//[out]  VDS_ASYNC_OUTPUT *pAsyncOut
			);
		if (FAILED(hr) || FAILED(hrResult))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("Wait"), hr, TEXT("[hrResult=0x%08x]"), hrResult));
			break;
		}

		//******************************************
		CComPtr<IUnknown> spIUnknown;
		spIUnknown.Attach(AsyncOut.cvd.pVDiskUnk); AsyncOut.cvd.pVDiskUnk = NULL;
		hr = spIUnknown.QueryInterface(&m_spIVdsVDisk);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("QueryInterface(&m_spIVdsVDisk)"), hr));
			break;
		}

		//******************************************
		hr = m_spIVdsVDisk->Open(
			VirtualDiskAccessMask,//[in]   VIRTUAL_DISK_ACCESS_MASK AccessMask,
			OPEN_VIRTUAL_DISK_FLAG_NONE,//[in]   OPEN_VIRTUAL_DISK_FLAG Flags,
			1,//[in]   ULONG ReadWriteDepth,
			&m_spIVdsOpenVDisk//[out]  IVdsOpenVDisk **ppOpenVDisk
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("Open"), hr));
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

INT CVdsVhdHelper::Attach(ATTACH_VIRTUAL_DISK_FLAG AttachFlags)
{
	INT nStatus = 0;
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************
		CComPtr<IVdsAsync> spIVdsAsync;
		hr = m_spIVdsOpenVDisk->Attach(
			NULL,//[in]   LPWSTR pStringSecurityDescriptor,
			AttachFlags,//[in]   ATTACH_VIRTUAL_DISK_FLAG Flags,
			0,//[in]   ULONG ProviderSpecificFlags,
			0,//[in]   ULONG TimeoutInMs,
			&spIVdsAsync//[out]  IVdsAsync **ppAsync
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("Attach"), hr));
			break;
		}

		//******************************************
		HRESULT hrResult = E_FAIL;
		VDS_ASYNC_OUTPUT AsyncOut;
		ZeroMemory(&AsyncOut, sizeof(VDS_ASYNC_OUTPUT));
		hr = spIVdsAsync->Wait(
			&hrResult,//[out]  HRESULT *pHrResult,
			&AsyncOut//[out]  VDS_ASYNC_OUTPUT *pAsyncOut
			);
		if (FAILED(hr) || FAILED(hrResult))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("Wait"), hr, TEXT("[hrResult=0x%08x]"), hrResult));
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

ULONG CVdsVhdHelper::FindUninitalizedVHD()
{
	INT nStatus = 0;
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	ULONG ulDiskNumber = -1;

	do 
	{
		//******************************************
		CComPtr<IVdsDisk> spIVdsDisk;
		hr = m_MyVdProvider.m_pIVdsVdProvider->GetDiskFromVDisk(
			m_spIVdsVDisk,//[in]   IVdsVDisk *pVDisk,
			&spIVdsDisk//[out]  IVdsDisk **ppDisk
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("GetDiskFromVDisk"), hr));
			break;
		}

		//******************************************
		hr = m_MyDisk.Initialize(spIVdsDisk.Detach(), FALSE);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("m_MyDisk.Initialize"), hr));
			break;
		}

		//******************************************
		CONST TCHAR SZ_PHYSICAL_DRIVE[] = TEXT("\\\\?\\PhysicalDrive");
		CString strDiskNumber = m_MyDisk.m_MyVDSDiskProp.m_strName.Mid(_countof(SZ_PHYSICAL_DRIVE) - 1);
		if (0 == m_MyDisk.m_MyVDSDiskProp.m_strName.Left(_countof(SZ_PHYSICAL_DRIVE) - 1).CompareNoCase(SZ_PHYSICAL_DRIVE) &&
			strDiskNumber.GetLength() && 
			(strDiskNumber[0] >= TEXT('0') && strDiskNumber[0] <= TEXT('9')))
		{
			LPTSTR szEndPtr = NULL;
			ulDiskNumber = _tcstoul(strDiskNumber, &szEndPtr, 10);
		}

		//******************************************
	} while (FALSE);

	return ulDiskNumber;
}

INT CVdsVhdHelper::Dettach()
{
	INT nStatus = 0;
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************
		hr = m_spIVdsOpenVDisk->Detach(
			DETACH_VIRTUAL_DISK_FLAG_NONE,//[in]  DETACH_VIRTUAL_DISK_FLAG Flags,
			0//[in]  ULONG ProviderSpecificFlags
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("Detach"), hr));
			break;
		}

	} while (FALSE);

	return nStatus;
}

INT CVdsVhdHelper::Close()
{
	DestrotMember();
	return 0;
}