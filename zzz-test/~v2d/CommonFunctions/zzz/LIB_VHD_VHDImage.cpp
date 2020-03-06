#include "StdAfx.h"
#include "LIB_VHD_VHDImage.h"
#include "LIB_VHD_DynamicVHD.h"
#include "LIB_VHD_DiffVHD.h"
#include "LIB_VHD_VHDHelper.h"
#include "LIB_VHD_autoHandle.h"

#pragma comment(lib, "Ws2_32.lib")

CVHDImage::CVHDImage(VOID) :
m_pBottomVHD(NULL),
m_pBAT(NULL),
m_llDiskSize(0)
{
	ZeroMemory(&m_stGeometry, sizeof(m_stGeometry));
	ZeroMemory(&m_ftCreation, sizeof(m_ftCreation));
	ZeroMemory(&m_ftModification, sizeof(m_ftModification));

	//m_arrayDisks = new CVirtualHardDisk*[MAX_DISK_CHAIN];
	//ZeroMemory(m_arrayDisks, sizeof(CVirtualHardDisk*)*MAX_DISK_CHAIN);
}

CVHDImage::~CVHDImage(VOID)
{
	Close();
	//CVHDAllocationTable::GetInstance()->DestroyInstance();
}

BOOL CVHDImage::Create(LPCTSTR szImageFileName, PIMAGE_PARAMETERS pParameters, IMAGE_CREATION_DISPOSITION nCreationDisposition, DWORD dwCompressRatio)
{
	INT32 nRes = VHD_RET_SUCCESSFULL;

	VHD_CREATEPARAM stVHDPara;
	ZeroMemory(&stVHDPara, sizeof(VHD_CREATEPARAM));

	if (!szImageFileName)
	{
		m_dwErrCode = VHD_RET_INVALID_PARAMETER;
		return FALSE;
	}

	if( nCreationDisposition == IMAGE_CREATE && !pParameters )
	{
		m_dwErrCode = VHD_RET_INVALID_PARAMETER;
		return FALSE;
	}

	m_strFilename = szImageFileName;

	if( m_pBAT )
	{
		delete[] m_pBAT;
		m_pBAT = NULL;
	}

	switch(nCreationDisposition)
	{
	case IMAGE_CREATE:
		{
			memcpy_s(&m_stGeometry,sizeof(DISK_GEOMETRY),&(pParameters->Geometry),sizeof(DISK_GEOMETRY));
			m_llDiskSize = pParameters->DiskSize.QuadPart;

			memcpy_s(&stVHDPara.stGeometry,sizeof(DISK_GEOMETRY),&(pParameters->Geometry),sizeof(DISK_GEOMETRY));
			stVHDPara.liDiskSize.QuadPart = pParameters->DiskSize.QuadPart;
			if (pParameters->szParentDiskFileName)
				wcscpy_s(stVHDPara.szParentDiskFileName,FILE_NAME_LENGTH,pParameters->szParentDiskFileName);

			if( pParameters->szParentDiskFileName )
			{
				//Create diff VHD.
				if( m_pBottomVHD )
				{
					delete m_pBottomVHD;
				}
				m_pBottomVHD = new CDiffVHD;
				if( !m_pBottomVHD )
				{
					//LOGGER<<"CVHDImage::Create: new CDiffVHD failed.";
					m_dwErrCode = GetLastError();
					return FALSE;
				}
			}
			else
			{
				//Create dynamic VHD by default.
				if( m_pBottomVHD )
				{
					delete m_pBottomVHD;
				}
				m_pBottomVHD = new CDynamicVHD;
				if( !m_pBottomVHD )
				{
					//LOGGER<<"CVHDImage::Create: new CDynamicVHD failed.";
					m_dwErrCode = GetLastError();
					return FALSE;
				}
			}

			UINT32 unBATSize = VHDHelper::GetMAXBATEntries(m_llDiskSize);
			m_pBAT = new BATELEMENT[unBATSize];
			for(UINT32 i=0; i<unBATSize; i++)
			{
				m_pBAT[i].nOffsetInSector = 0xffffffff;
				m_pBAT[i].pDisk = NULL;
			}
			m_dwErrCode = nRes = m_pBottomVHD->Create(szImageFileName, nCreationDisposition, m_pBAT, &stVHDPara);
		}
		break;
	default:
		{
			//Read VHD footer;
			VHDFOOTER stFooter;
			ZeroMemory(&stFooter, sizeof(stFooter));
			HANDLE hVHD = CreateFileW(szImageFileName,
				GENERIC_READ,
				FILE_SHARE_READ|FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if( hVHD == INVALID_HANDLE_VALUE )
			{
				m_dwErrCode = GetLastError();
				//LOGGER<<"CVHDImage::Create: CreateFileW["<<szImageFileName<<"] failed. err="<<m_dwErrCode;
				return FALSE;
			}

			if( !GetFileTime(hVHD, &m_ftCreation, NULL, &m_ftModification) )
			{
				m_dwErrCode = GetLastError();
				//LOGGER<<"CVHDImage::Create: GetFileTime["<<szImageFileName<<"] failed. err="<<m_dwErrCode;
				SAFECLOSEHANDLE(hVHD);
				return FALSE;
			}

			if( !VHDHelper::ReadFooter(hVHD, &stFooter) )
			{
				//LOGGER<<"CVHDImage::Create: ReadFooter["<<szImageFileName<<"] failed.";
				SAFECLOSEHANDLE(hVHD);
				m_dwErrCode = VHD_RET_INVALID_FOOTER;
				return FALSE;
			}
			SAFECLOSEHANDLE(hVHD);

			m_llDiskSize = stFooter.OriginalSize;

			m_stGeometry.BytesPerSector = 512;
			m_stGeometry.Cylinders.QuadPart = stFooter.DiskGeometry.Cylinder;
			m_stGeometry.SectorsPerTrack = stFooter.DiskGeometry.SectorsPerTrack_Cylinder;
			m_stGeometry.TracksPerCylinder = 255;
			m_stGeometry.MediaType = FixedMedia;

			if( m_pBottomVHD )
			{
				delete m_pBottomVHD;
			}

			switch(stFooter.DiskType)
			{
				//case CFixedVHD::VHDType:
				//   {
				//      m_pBottomVHD = new CFixedVHD;
				//      if( !m_pBottomVHD )
				//      {
				//         return FALSE;
				//      }
				//   }
				//   break;
			case CDynamicVHD::VHDType:
				{
					m_pBottomVHD = new CDynamicVHD;
					if( !m_pBottomVHD )
					{
						//LOGGER<<"CVHDImage::Create: new CDynamicVHD failed.";
						m_dwErrCode = GetLastError();
						return FALSE;
					}
				}
				break;
			case CDiffVHD::VHDType:
				{
					m_pBottomVHD = new CDiffVHD;
					if( !m_pBottomVHD )
					{
						//LOGGER<<"CVHDImage::Create: new CDiffVHD failed.";
						m_dwErrCode = GetLastError();
						return FALSE;
					}
				}
				break;
			default:
				{
					//LOGGER<<"CVHDImage::Create: disk type error. type="<<stFooter.DiskType;
					m_dwErrCode = VHD_RET_INVALID_DISK_TYPE;
					return FALSE;
				}
				break;
			}

			UINT32 unBATSize = VHDHelper::GetMAXBATEntries(m_llDiskSize);
			m_pBAT = new BATELEMENT[unBATSize];
			for(UINT32 i=0; i<unBATSize; i++)
			{
				m_pBAT[i].nOffsetInSector = 0xffffffff;
				m_pBAT[i].pDisk = NULL;
			}

			m_dwErrCode = nRes = m_pBottomVHD->Create(szImageFileName, nCreationDisposition, m_pBAT, &stVHDPara);

		}
		break;
	}

	if(nRes == VHD_RET_SUCCESSFULL )
	{
		m_listVHD.push_back(m_pBottomVHD);
		CVirtualHardDisk* pCurrentVHD = m_pBottomVHD;
		CVirtualHardDisk* pChildVHD = m_pBottomVHD;

		while(pCurrentVHD->GetFooter()->DiskType == CDiffVHD::VHDType && nRes == VHD_RET_SUCCESSFULL )
		{	
			LPCTSTR szParentFileName = dynamic_cast<CDiffVHD*>(pCurrentVHD)->GetParentDiskName();
			switch(pCurrentVHD->GetParentDiskType())
			{
			case CDynamicVHD::VHDType: 
				pCurrentVHD = new CDynamicVHD;
				break;
			case CDiffVHD::VHDType:
				pCurrentVHD = new CDiffVHD;
				break;
			default:
				pCurrentVHD = NULL;
				break;
			}

			if (pCurrentVHD)
			{
				m_dwErrCode = nRes = pCurrentVHD->Create(szParentFileName, CDiskImage::IMAGE_OPEN, m_pBAT);
				if(VHD_RET_SUCCESSFULL == nRes)
					m_listVHD.push_front(pCurrentVHD);
			}
			else
				break;

			pChildVHD->SetParent(pCurrentVHD);
			pChildVHD = pCurrentVHD;
		} 
	}

	if (nRes == VHD_RET_SUCCESSFULL)
		return TRUE;
	else
		return FALSE;
}

BOOL CVHDImage::Close()
{
	VHD_LIST_ITERATOR pIt = m_listVHD.begin();
	while( pIt != m_listVHD.end() )
	{
		if( *pIt )
		{
			(*pIt)->Close();
			delete (*pIt);
			*pIt = NULL;
		}
		pIt++;
	}

	m_pBottomVHD = NULL;

	if( m_pBAT )
	{
		delete[] m_pBAT;
		m_pBAT = NULL;
	}

	if( m_ftModification.dwHighDateTime != 0 || m_ftModification.dwLowDateTime !=0 )
	{
		HANDLE hFileChild = CreateFileW(m_strFilename.c_str(),
			GENERIC_WRITE,
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if( hFileChild == INVALID_HANDLE_VALUE )
		{
			return FALSE;
		}

		//set new child disk to correct time
		if( !SetFileTime(hFileChild, &m_ftCreation, NULL, &m_ftModification) )
		{
			return FALSE;
		}

		CloseHandle(hFileChild);
		hFileChild = INVALID_HANDLE_VALUE;

		ZeroMemory(&m_ftModification, sizeof(m_ftModification));
		ZeroMemory(&m_ftCreation, sizeof(m_ftCreation));
	}

	return TRUE;
}

BOOL CVHDImage::Seek(LARGE_INTEGER i64DistanceToMove, PLARGE_INTEGER pi64NewPosition, IMAGE_SEEK_METHOD nSeekMethod)
{
	if( !m_pBottomVHD )
	{
		return FALSE;
	}

	m_dwErrCode = m_pBottomVHD->Seek(i64DistanceToMove, pi64NewPosition, nSeekMethod);
	return m_dwErrCode == VHD_RET_SUCCESSFULL ? TRUE : FALSE;
}

BOOL CVHDImage::Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead)
{
	if( !m_pBottomVHD )
	{
		return FALSE;
	}

	m_dwErrCode = m_pBottomVHD->Read(lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead);

	return m_dwErrCode == VHD_RET_SUCCESSFULL ? TRUE : FALSE;
}

BOOL CVHDImage::Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten)
{
	if( !m_pBottomVHD )
	{
		return FALSE;
	}

	m_dwErrCode  = m_pBottomVHD->Write(lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten);
	return m_dwErrCode == VHD_RET_SUCCESSFULL ? TRUE : FALSE;
}

BOOL CVHDImage::GetGeometry(PDISK_GEOMETRY pGeometry) CONST
{
	if( !pGeometry )
	{
		return FALSE;
	}

	memcpy_s(pGeometry,sizeof(DISK_GEOMETRY),&m_stGeometry,sizeof(DISK_GEOMETRY));

	return TRUE;
}

BOOL CVHDImage::GetDiskSize(PLARGE_INTEGER pi64DiskSize) CONST
{
	if( !pi64DiskSize )
	{
		return FALSE;
	}

	pi64DiskSize->QuadPart = m_llDiskSize;

	return TRUE;
}
CVirtualHardDisk* CVHDImage::GetBottomVHD(VOID) CONST
{
	return m_pBottomVHD;
}

LPCTSTR   CVHDImage::GetFilename() CONST
{
	return m_strFilename.c_str();
}

FILETIME  CVHDImage::GetModificationTime() CONST
{
	return m_ftModification;
}


INT CVHDImage::SectorSortFunc(VOID* pParam, CONST VOID *arg1, CONST VOID *arg2 )
{
	CVHDImage* pThis = (CVHDImage*)pParam;
	//	CDynamicVHD* pVHD = (CDynamicVHD*)(pThis->m_pBottomVHD);
	ULONG* pSector1 = (ULONG*)arg1;
	ULONG* pSector2 = (ULONG*)arg2;

	//USHORT usDist1 = (USHORT)(*(pSector1+1));//pVHD->GetSectorDistance(*pSector1);
	//USHORT usDist2 = (USHORT)(*(pSector2+1));//pVHD->GetSectorDistance(*pSector2);
	PBATELEMENT pBAT = pThis->GetBATRef();
	ULONG ulBATIndex1 = (ULONG)((LONGLONG)(*pSector1)*VHD_SECTOR_SIZE/(LONGLONG)(pThis->GetVHD()->GetHeader()->BlockSize));
	ULONG ulBATIndex2 = (ULONG)((LONGLONG)(*pSector2)*VHD_SECTOR_SIZE/(LONGLONG)(pThis->GetVHD()->GetHeader()->BlockSize));

	ULONG_PTR ulDisk1 = (ULONG_PTR)pBAT[ulBATIndex1].pDisk;
	ULONG_PTR ulDisk2 = (ULONG_PTR)pBAT[ulBATIndex2].pDisk;

	if( ulDisk1 > ulDisk2 )
	{
		return 1;
	}

	if( ulDisk1 < ulDisk2 )
	{
		return -1;
	}

	return 0;
}

BOOL CVHDImage::GetSectorList(ULONG * SectorArray, DWORD Size)
{
	BOOL bRet = false;

	for(ULONG i =0; i < Size; i++)
	{
		ULONG sector = SectorArray[i];

		PBATELEMENT pBAT = GetBATRef();
		ULONG ulBATIndex1 = (ULONG)((LONGLONG)(sector)*VHD_SECTOR_SIZE/(LONGLONG)(GetVHD()->GetHeader()->BlockSize));

		if( pBAT[ulBATIndex1].pDisk == m_pBottomVHD )
		{
			SectorArray[i] = 1;
			bRet = true;
		}
		else
			SectorArray[i] = 0;
	}//end for

	return bRet;
}

BOOL CVHDImage::Sort(ULONG * SectorArray, DWORD Size)
{
	ULONG ulCount = Size;
	ULONG* pSectorIndexArray = SectorArray;//new ULONG[ulCount*2];

	CDynamicVHD* pVHD = (CDynamicVHD*)(m_pBottomVHD);
	//for( ULONG i=0; i<ulCount; i++)
	//{
	//	pSectorIndexArray[i*2] = SectorArray[i];
	//	//pSectorIndexArray[i*2+1] = pVHD->GetSectorDistance(pSectorIndexArray[i*2] );
	//}
	qsort_s(pSectorIndexArray, ulCount, sizeof(ULONG), SectorSortFunc, (VOID*)this);

	//for( ULONG i=0; i<ulCount; i++ )
	//{
	//	SectorArray[i] = pSectorIndexArray[i*2];
	//}

	return TRUE;
}

BOOL CVHDImage::Merge(ICallBacks * pCallBack)
{
	return 0;
	//FILETIME ftModifyTime;
	//GUID uuidChild;

	//CStringW strFilename;
	//CStringW strParentFilename;

	//if( !m_pBottomVHD->GetParentDisk() )
	//{
	//     //LOGGER<<"CVHDImage::Merge: no parent disk";
	//	return FALSE;
	//}

	//CDynamicVHD* pVHD = (CDynamicVHD*)m_pBottomVHD;
	//CDynamicVHD* pParentVHD = (CDynamicVHD*)pVHD->m_pParentDisk;

	////must merge the first 2 disks in disk chain
	////if(pParentVHD->GetFooter()->DiskType != CDynamicVHD::VHDType )
	////{
	////     //LOGGER<<"CVHDImage::Merge: parent is not dynamic vhd";
	////	return FALSE;
	////}

	//ftModifyTime = GetModificationTime();
	//memcpy_s(&uuidChild, sizeof(GUID), &(m_pBottomVHD->GetFooter()->UniqueId), sizeof(GUID));

	//strFilename = m_strFilename.c_str();
	//strParentFilename = pParentVHD->GetDiskName();

	//Close();

	//if( 0 != MergewithParentVHD(strFilename, 0, 0) )
	//{
	//	return FALSE;
	//}

	////update UUID
	//CSHandle<> hVHD = CreateFileW(strParentFilename,
	//	GENERIC_READ|GENERIC_WRITE,
	//	0,
	//	NULL,
	//	OPEN_EXISTING,
	//	FILE_ATTRIBUTE_NORMAL,
	//	NULL);
	//if( hVHD == INVALID_HANDLE_VALUE )
	//{
	//     DWORD dwErr = GetLastError();
	//	//TELOG(1)<<"CVHDImage::Merge: CreateFileW["<<strParentFilename<<"] failed. err["<<dwErr<<"]";
	//	return FALSE;
	//}

	//VHDFOOTER stFooter;
	//if( !VHDHelper::ReadFooter(hVHD, &stFooter, FALSE) )
	//{
	//     //LOGGER<<"CVHDImage::Merge: ReadFooter failed.";
	//	CloseHandle(hVHD);
	//	hVHD = INVALID_HANDLE_VALUE;
	//	return FALSE;
	//}

	//memcpy_s(&stFooter.UniqueId, sizeof(GUID), &uuidChild, sizeof(GUID));
	//stFooter.Checksum = 0;
	//stFooter.Checksum = VHDHelper::CalcChecksum(&stFooter, sizeof(stFooter));
	//VHDHelper::ConvFooter(&stFooter);
	//LARGE_INTEGER liSeek;
	//liSeek.QuadPart = 0;
	//if( !SetFilePointerEx(hVHD, liSeek, NULL, FILE_BEGIN) )
	//{
	//     DWORD dwErr = GetLastError();
	//	//TELOG(1)<<"CVHDImage::Merge: SetFilePointerEx["<<strParentFilename<<"] failed. err["<<dwErr<<"]";
	//	return FALSE;
	//}

	//DWORD dwWritten = 0;
	//WriteFile(hVHD, &stFooter, sizeof(stFooter), &dwWritten, NULL);
	//if( dwWritten != sizeof(stFooter) )
	//{
	//     DWORD dwErr = GetLastError();
	//	//TELOG(1)<<"CVHDImage::Merge: WriteFile["<<strParentFilename<<"] failed. err["<<dwErr<<"]";
	//	return FALSE;
	//}

	//liSeek.QuadPart = -512;
	//if( !SetFilePointerEx(hVHD, liSeek, NULL, FILE_END) )
	//{
	//     DWORD dwErr = GetLastError();
	//	//TELOG(1)<<"CVHDImage::Merge: SetFilePointerEx["<<strParentFilename<<"] from end failed. err["<<dwErr<<"]";
	//	return FALSE;
	//}

	//dwWritten = 0;
	//WriteFile(hVHD, &stFooter, sizeof(stFooter), &dwWritten, NULL);
	//if( dwWritten != sizeof(stFooter) )
	//{
	//     DWORD dwErr = GetLastError();
	//	//TELOG(1)<<"CVHDImage::Merge: WriteFile["<<strParentFilename<<"] at end failed. err["<<dwErr<<"]";
	//	return FALSE;
	//}

	//SetFileTime(hVHD, NULL, NULL, &ftModifyTime);
	//CloseHandle(hVHD);
	//hVHD = INVALID_HANDLE_VALUE;

	////delete child disk and move parent disk
	//if( !DeleteFileW(strFilename) )
	//{
	//     DWORD dwErr = GetLastError();
	//     //TELOG(1)<<"CVHDImage::Merge: DeleteFileW["<<strFilename<<"] failed. err["<<dwErr<<"]";
	//	return FALSE;
	//}

	//if( !MoveFileW(strParentFilename, strFilename) )
	//{
	//     DWORD dwErr = GetLastError();
	//     //TELOG(1)<<"CVHDImage::Merge: MoveFileW["<<strParentFilename<<", "<<strFilename<<"] failed. err["<<dwErr<<"]";
	//	return FALSE;
	//}

	//return TRUE;
}

BOOL CVHDImage::GetFooter(BYTE* pFooter, DWORD dwBufSize)
{
	memcpy_s(pFooter, dwBufSize, m_pBottomVHD->GetFooter(), 512);
	return TRUE;
}

PBATELEMENT CVHDImage::GetBATRef()
{
	return m_pBAT;
}
