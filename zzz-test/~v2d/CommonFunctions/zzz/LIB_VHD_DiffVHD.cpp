#include "StdAfx.h"

#include "LIB_VHD_VhdDef.h"
#include "LIB_VHD_DiffVHD.h"
#include "LIB_VHD_VHDHelper.h"
#include <comdef.h>
#include <vector>


CDiffVHD::CDiffVHD(VOID) :
m_nParentDiskType(0)
{
}

CDiffVHD::~CDiffVHD(VOID)
{
}

INT32 CDiffVHD::Create(LPCTSTR pszFilename, DWORD dwCreationDisposition, PBATELEMENT pGlobalBAT, PVHD_CREATEPARAM pParam)
{
	if (!pszFilename)
	{
		DAPIERR((0, DRNPOS, TEXT("Create"), VHD_RET_INVALID_PARAMETER));
		m_dwErrorCode = VHD_RET_INVALID_PARAMETER;
		return VHD_RET_FAILED;
	}

	//m_bNewBAT = FALSE;
	m_strFilename = pszFilename;
	m_hVHDFile = CreateFileW(pszFilename,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		dwCreationDisposition,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if( m_hVHDFile == INVALID_HANDLE_VALUE )
	{
		m_dwErrorCode = GetLastError();
		DAPIERR((0, DRNPOS, TEXT("CreateFile"), m_dwErrorCode, TEXT("[File=%s]"), pszFilename));
		return VHD_RET_FAILED;
	}

	m_bNeedWriteFooterWhenClose = FALSE;

	switch(dwCreationDisposition)
	{
	case CREATE_NEW:
		{
			if( !pParam->szParentDiskFileName )
			{
				DAPIERR((0, DRNPOS, TEXT("CREATE_NEW"), E_INVALIDARG, TEXT("[File=%s]"), pParam->szParentDiskFileName));
				SAFECLOSEHANDLE(m_hVHDFile);
				m_dwErrorCode = VHD_RET_INVALID_PARAMETER;
				return VHD_RET_FAILED;
			}

			m_bNeedWriteFooterWhenClose = TRUE;

			m_strParent = pParam->szParentDiskFileName;

			//read parent unique id and time
			UINT32 unParentTimeStamp = 0;
			HANDLE hParent = CreateFileW(pParam->szParentDiskFileName,
				GENERIC_READ,
				FILE_SHARE_READ|FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if( hParent == INVALID_HANDLE_VALUE )
			{
				m_dwErrorCode = GetLastError();
				DAPIERR((0, DRNPOS, TEXT("CreateFileW"), m_dwErrorCode, TEXT("Open parent disk %s fail"), pParam->szParentDiskFileName));
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			CHeapObj<VHDFOOTER> pParentFooter;
			ZeroMemory(pParentFooter, sizeof(VHDFOOTER));
			if( !VHDHelper::ReadFooter(hParent, pParentFooter) )
			{
				DAPIERR((0, DRNPOS, TEXT("ReadFooter"), E_FAIL));
				SAFECLOSEHANDLE(hParent);
				SAFECLOSEHANDLE(m_hVHDFile);
				m_dwErrorCode = VHD_RET_INVALID_FOOTER;
				return VHD_RET_FAILED;
			}

			FILETIME fTime;
			GetFileTime(hParent, NULL, NULL, &fTime);
			unParentTimeStamp = ConvFileTimeToTimeStamp(&fTime);

			CloseHandle(hParent);
			hParent = INVALID_HANDLE_VALUE;

			//Gen footer
			ZeroMemory(&m_stFooter, sizeof(m_stFooter));

			CONST WCHAR* p = wcsrchr(pszFilename, L'.');
			if( p && _wcsicmp(p, VHD_DISK_NAME_EXT) == 0 )
			{
				m_bD2D = TRUE;
			}
			else
			{
				m_bD2D = FALSE;
			}

			memcpy_s(m_stFooter.Cookie, sizeof(m_stFooter.Cookie), "conectix", 8);

			m_stFooter.Features = 0x00000002;
			m_stFooter.FileFormatVersion = 0x00010000;
			m_stFooter.DataOffset = 0x200;
			time_t tCurTime = 0;
			time(&tCurTime);
			m_stFooter.TimeStamp = VHDHelper::ConvTimeStamp(tCurTime);
			memcpy_s(m_stFooter.CreatorApplication, sizeof(m_stFooter.CreatorApplication), "BAB", 4);
			m_stFooter.CreatorVersion = 0x01000000;
			memcpy_s(m_stFooter.CreatorHostOS, sizeof(m_stFooter.CreatorHostOS), "Wi2k", 4);
			m_stFooter.OriginalSize = pParentFooter->OriginalSize;
			m_stFooter.CurrentSize = pParentFooter->CurrentSize;
			m_stFooter.DiskGeometry.Value = pParentFooter->DiskGeometry.Value;
			m_stFooter.DiskType = VHDType;
			CoCreateGuid(&m_stFooter.UniqueId);
			m_stFooter.SavedState = 0;
			m_stFooter.Checksum = VHDHelper::CalcChecksum(&m_stFooter, sizeof(m_stFooter));

			//write the copy of footer at the top of DYN VHD
			CHeapObj<VHDFOOTER> pFooter;
			memcpy_s(pFooter, sizeof(VHDFOOTER), &m_stFooter, sizeof(m_stFooter));
			VHDHelper::ConvFooter(pFooter);
			DWORD dwWritten = 0;
			WriteFile(m_hVHDFile, pFooter, sizeof(VHDFOOTER), &dwWritten, NULL);
			if( dwWritten != sizeof(VHDFOOTER) )
			{
				m_dwErrorCode = GetLastError();
				DAPIERR((0, DRNPOS, TEXT("write secondary footer failed"), m_dwErrorCode));
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			//Gen DYN header
			memcpy_s(m_stHeader.Cookie, sizeof(m_stHeader.Cookie), "cxsparse", 8);
			m_stHeader.DataOffset = 0xffffffffffffffff;
			m_stHeader.TableOffset = sizeof(VHDFOOTER)+sizeof(VHDDYNHEADER);
			m_stHeader.HeaderVersion = 0x00010000;
			m_stHeader.BlockSize = VHD_BLOCK_SIZE;
			m_stHeader.MaxTableEntries = (UINT32)(m_stFooter.CurrentSize/m_stHeader.BlockSize);
			m_stHeader.MaxTableEntries = ((m_stHeader.MaxTableEntries*sizeof(UINT32)+VHD_SECTOR_SIZE-1)&~(VHD_SECTOR_SIZE-1))/sizeof(UINT32);
			CONST WCHAR* pName = wcsrchr(pParam->szParentDiskFileName, L'\\');
			if( !pName )
			{
				pName = pParam->szParentDiskFileName;
			}
			else
			{
				pName++;
			}

			wcscpy_s(m_stHeader.ParentUnicodeName, _countof(m_stHeader.ParentUnicodeName), pName);
			if( m_bD2D )
			{
				//change the parent ext name to .VHD
				WCHAR* pExt = wcsrchr(m_stHeader.ParentUnicodeName, L'.');
				if( pExt )
				{
					*pExt = 0;
					wcscat_s(m_stHeader.ParentUnicodeName, _countof(m_stHeader.ParentUnicodeName), L".VHD");
				}
			}

			memcpy_s(&m_stHeader.ParentUniqueID, sizeof(m_stHeader.ParentUniqueID), &pParentFooter->UniqueId, sizeof(pParentFooter->UniqueId));
			m_stHeader.ParentTimeStamp = unParentTimeStamp;//stParentFooter.TimeStamp;

			m_nParentDiskType = pParentFooter->DiskType;

			//generate parent locators
			BYTE* pLocatorBuf = new BYTE[512*8];
			if( !pLocatorBuf )
			{
				DAPIERR((0, DRNPOS, TEXT("Allocate LocatorBuf"), E_OUTOFMEMORY));
				SAFECLOSEHANDLE(m_hVHDFile);
				m_dwErrorCode = GetLastError();
				return VHD_RET_FAILED;
			}

			INT nLocatorCount = GenParentLocator(pszFilename, pParam->szParentDiskFileName, m_stHeader.TableOffset+sizeof(UINT32)*m_stHeader.MaxTableEntries, pLocatorBuf);
			if( nLocatorCount == 0 )
			{
				DAPIERR((0, DRNPOS, TEXT("GenParentLocator"), E_FAIL, TEXT("[Filename=%s, ParentDiskFileName=%s]"), pszFilename, pParam->szParentDiskFileName));
				SAFECLOSEHANDLE(m_hVHDFile);
				delete[] pLocatorBuf;
				pLocatorBuf = NULL;
				m_dwErrorCode = VHD_RET_FOUND_NO_PARENT;
				return VHD_RET_FAILED;
			}

			m_stHeader.Checksum = VHDHelper::CalcChecksum(&m_stHeader, sizeof(m_stHeader));

			//write DYN header
			CHeapObj<VHDDYNHEADER> pHeader;
			memcpy_s(pHeader, sizeof(VHDDYNHEADER), &m_stHeader, sizeof(m_stHeader));
			VHDHelper::ConvHeader(pHeader);
			WriteFile(m_hVHDFile, pHeader, sizeof(VHDDYNHEADER), &dwWritten, NULL);
			if( dwWritten != sizeof(VHDDYNHEADER))
			{
				m_dwErrorCode = GetLastError();
				DAPIERR((0, DRNPOS, TEXT("write header"), m_dwErrorCode));
				SAFECLOSEHANDLE(m_hVHDFile);
				delete[] pLocatorBuf;
				pLocatorBuf = NULL;
				return VHD_RET_FAILED;
			}

			UINT32* pBAT = new UINT32[m_stHeader.MaxTableEntries];
			memset(pBAT, 0xff, sizeof(ULONG)*m_stHeader.MaxTableEntries);

			//write BAT
			WriteFile( m_hVHDFile, pBAT, m_stHeader.MaxTableEntries*sizeof(UINT32), &dwWritten, NULL);
			delete[] pBAT;
			if( dwWritten != m_stHeader.MaxTableEntries*sizeof(UINT32) )
			{
				m_dwErrorCode = GetLastError();
				DAPIERR((0, DRNPOS, TEXT("write BAT"), m_dwErrorCode));
				SAFECLOSEHANDLE(m_hVHDFile);
				delete[] pLocatorBuf;
				pLocatorBuf = NULL;
				return VHD_RET_FAILED;
			}

			//write locator buffer to VHD
			WriteFile(m_hVHDFile, pLocatorBuf, nLocatorCount*512, &dwWritten, NULL);
			if( dwWritten != nLocatorCount*512)
			{
				m_dwErrorCode = GetLastError();
				DAPIERR((0, DRNPOS, TEXT("write locator buffe"), m_dwErrorCode));
				SAFECLOSEHANDLE(m_hVHDFile);
				delete[] pLocatorBuf;
				pLocatorBuf = NULL;
				return VHD_RET_FAILED;
			}

			delete[] pLocatorBuf;
			pLocatorBuf = NULL;
		}
		break;
	case OPEN_EXISTING:
		{
			DWORD dwRead = 0;

			if( !VHDHelper::ReadFooter(m_hVHDFile, &m_stFooter) )
			{
				DAPIERR((0, DRNPOS, TEXT("ReadFooter"), E_FAIL));
				SAFECLOSEHANDLE(m_hVHDFile);
				m_dwErrorCode = VHD_RET_INVALID_FOOTER;
				return VHD_RET_FAILED;
			}

			if( m_stFooter.DiskType != VHDType )
			{
				DAPIERR((0, DRNPOS, TEXT("Disk type mismatch"), E_FAIL));
				SAFECLOSEHANDLE(m_hVHDFile);
				m_dwErrorCode = VHD_RET_INVALID_DISK_TYPE;
				return VHD_RET_FAILED;
			}

			LARGE_INTEGER li;

			li.QuadPart = m_stFooter.DataOffset;
			if( !SetFilePointerEx(m_hVHDFile, li, NULL, FILE_BEGIN) )
			{
				m_dwErrorCode = GetLastError();
				DAPIERR((0, DRNPOS, TEXT("SetFilePointerEx"), m_dwErrorCode));
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			ReadFile(m_hVHDFile, &m_stHeader, sizeof(m_stHeader), &dwRead, NULL);
			if( dwRead != sizeof(m_stHeader) )
			{
				m_dwErrorCode = GetLastError();
				DAPIERR((0, DRNPOS, TEXT("ReadFile Header"), m_dwErrorCode));
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			VHDHelper::ConvHeader(&m_stHeader);
			UINT32 unChecksum = m_stHeader.Checksum;
			m_stHeader.Checksum = 0;
			if( unChecksum != VHDHelper::CalcChecksum(&m_stHeader, sizeof(m_stHeader)) )
			{
				DAPIERR((0, DRNPOS, TEXT("check sum"), E_FAIL, TEXT("[Checksum=0x%08X]"), unChecksum));
				SAFECLOSEHANDLE(m_hVHDFile);
				m_dwErrorCode = VHD_RET_CHKSUM_MISMATCH;
				return VHD_RET_FAILED;
			}

			//get correct parent
			CHeapArray<CHAR> szParentName(512);
			//CHAR szParentName[512];
			BOOL bFoundParent = FALSE;
			for( INT i=0; i<8 && !bFoundParent; i++)
			{
				if( m_stHeader.ParentLocators[i].PlatformCode == PLATFORMCODE_WI2R ||
					m_stHeader.ParentLocators[i].PlatformCode == PLATFORMCODE_WI2K ||
					m_stHeader.ParentLocators[i].PlatformCode == PLATFORMCODE_W2RU ||
					m_stHeader.ParentLocators[i].PlatformCode == PLATFORMCODE_W2KU )
				{
					LARGE_INTEGER liPos;
					liPos.QuadPart = m_stHeader.ParentLocators[i].PlatformDataOffset;
					if( !SetFilePointerEx(m_hVHDFile, liPos, NULL, FILE_BEGIN) )
					{
						m_dwErrorCode = GetLastError();
						DAPIERR((0, DRNPOS, TEXT("SetFilePointerEx"), m_dwErrorCode));
						SAFECLOSEHANDLE(m_hVHDFile);
						return VHD_RET_FAILED;
					}

					DWORD dwRead = 0;
					ReadFile(m_hVHDFile, szParentName, 512, &dwRead, NULL);
					if( dwRead != 512 )
					{
						m_dwErrorCode = GetLastError();
						DAPIERR((0, DRNPOS, TEXT("ReadFile ParentName"), m_dwErrorCode));
						SAFECLOSEHANDLE(m_hVHDFile);
						return VHD_RET_FAILED;
					}
				}

				if( m_bD2D )
				{
					//change ext name to .D2D
					switch( m_stHeader.ParentLocators[i].PlatformCode )
					{
					case PLATFORMCODE_WI2R:
					case PLATFORMCODE_WI2K:
						{
							CHAR* pExt = strrchr((LPSTR)szParentName, '.');
							if( pExt )
							{
								*pExt = 0;
								CStringA strExt (VHD_DISK_NAME_EXT) ;
								strcat_s(szParentName, 512, strExt);
							}
						}
						break;
					case PLATFORMCODE_W2RU:
					case PLATFORMCODE_W2KU:
						{
							WCHAR* pExt = wcsrchr((WCHAR*)(LPSTR)szParentName, L'.');
							if( pExt )
							{
								*pExt = 0;
								wcscat_s((WCHAR*)(LPSTR)szParentName, 256, VHD_DISK_NAME_EXT);
							}
						}
						break;
					}
				}

				switch( m_stHeader.ParentLocators[i].PlatformCode )
				{
				case PLATFORMCODE_WI2R:
					{
						//convert ..\..\... to actual path name
						CHeapArray<CHAR> szImageFileNameA(MAX_PATH);
						//CHAR szImageFileNameA[MAX_PATH];
						CStringA strTmp  (pszFilename);
						sprintf_s(szImageFileNameA, MAX_PATH, "%s", (LPCSTR)strTmp);
						if( !GetParentFilename(szParentName, szImageFileNameA) )
						{
							DAPIERR((0, DRNPOS, TEXT("GetParentFilename"), E_FAIL, TEXT("%S"), szParentName));
							SAFECLOSEHANDLE(m_hVHDFile);
							m_dwErrorCode = VHD_RET_FOUND_NO_PARENT;
							return VHD_RET_FAILED;
						}
					}  //should not break here
				case PLATFORMCODE_WI2K:
					{
						CHeapObj<VHDFOOTER> pParentFooter;
						HANDLE hParent = CreateFileA(szParentName,
							GENERIC_READ,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
						if( hParent == INVALID_HANDLE_VALUE )
						{
#if 0
							DWORD dwErr = GetLastError();
							DAPIERR((0, DRNPOS, TEXT(""), dwErr, TEXT("%S"), szParentName));
							WCHAR szAnotherParentName[MAX_PATH];
							CStringW strParentW ((LPCSTR)szParentName);
							if( 0 != AFGetVhdPath(pszFilename, strParentW, szAnotherParentName, MAX_PATH) )
							{
								DAPIERR((0, DRNPOS, TEXT("GetVhdPath"), E_FAIL, TEXT("%s"), strParentW));
								m_dwErrorCode = VHD_RET_FOUND_NO_DISK;
								return VHD_RET_FAILED;
							}

							CStringA strParentA (szAnotherParentName);
							strcpy_s(szParentName, 512, strParentA);
							hParent = CreateFileA(szParentName,
								GENERIC_READ,
								FILE_SHARE_READ|FILE_SHARE_WRITE,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
#endif

							if( hParent == INVALID_HANDLE_VALUE )
							{
								m_dwErrorCode = GetLastError();
								DAPIERR((0, DRNPOS, TEXT("CreateFile"), m_dwErrorCode, TEXT("ParentName=%S"), (LPSTR)szParentName));
								SAFECLOSEHANDLE(m_hVHDFile);
								return VHD_RET_FAILED;
							}
						}
						if( !VHDHelper::ReadFooter(hParent, pParentFooter) )
						{
							DAPIERR((0, DRNPOS, TEXT("ReadFooter"), E_FAIL));
							SAFECLOSEHANDLE(m_hVHDFile);
							SAFECLOSEHANDLE(hParent);
							m_dwErrorCode = VHD_RET_INVALID_FOOTER;
							return VHD_RET_FAILED;
						}
						CloseHandle(hParent);
						hParent = INVALID_HANDLE_VALUE;

						//if GUID is equal, then this is correct parent
						if( IsEqualGUID(m_stHeader.ParentUniqueID, pParentFooter->UniqueId) )
						{
							//size_t tSize;
							//CHeapArray<WCHAR> szParentNameW(MAX_PATH);
							////WCHAR szParentNameW[MAX_PATH];
							//mbstowcs_s(&tSize, szParentNameW, MAX_PATH, szParentName, strlen(szParentName));
							//m_strParent = szParentNameW;
							CStringW strTmpW (szParentName);
							m_strParent = (LPCTSTR)strTmpW;

							bFoundParent = TRUE;
						}
					}
					break;
				case PLATFORMCODE_W2RU:
					{
						//convert ..\..\... to actual path name
						if( !GetParentFilenameW((WCHAR*)(VOID*)szParentName, pszFilename) )
						{
							DAPIERR((0, DRNPOS, TEXT("GetParentFilename"), E_FAIL));
							SAFECLOSEHANDLE(m_hVHDFile);
							m_dwErrorCode = VHD_RET_FOUND_NO_PARENT;
							return VHD_RET_FAILED;
						}
					}
				case PLATFORMCODE_W2KU:
					{
						CHeapObj<VHDFOOTER> pParentFooter;
						HANDLE hParent = CreateFileW((WCHAR*)(VOID*)szParentName,
							GENERIC_READ,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
						if( hParent == INVALID_HANDLE_VALUE )
						{
#if 0
							DWORD dwErr = GetLastError();
							//LOGGER<<"CDiffVHD::Create: CreateFileW["<<(WCHAR*)(VOID*)szParentName<<"] failed. err["<<dwErr<<"]";
							//LOGGER<<"CDiffVHD::Create: Get another possible parent file name";
							WCHAR szAnotherParentName[MAX_PATH];
							if( 0 != AFGetVhdPath(pszFilename, (WCHAR*)(VOID*)szParentName, szAnotherParentName, MAX_PATH) )
							{
								//LOGGER<<"CDiffVHD::AFGetVhdPath["<<(WCHAR*)(VOID*)szParentName<<"] failed.";
								m_dwErrorCode = VHD_RET_FOUND_NO_DISK;
								return VHD_RET_FAILED;
							}

							wcscpy_s((WCHAR*)(VOID*)szParentName, 256, szAnotherParentName);
							HANDLE hParent = CreateFileW((WCHAR*)(VOID*)szParentName,
								GENERIC_READ,
								FILE_SHARE_READ|FILE_SHARE_WRITE,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
#endif
							if( hParent == INVALID_HANDLE_VALUE )
							{
								m_dwErrorCode = GetLastError();
								//LOGGER<<"CDiffVHD::Create: CreateFileW["<<(WCHAR*)(VOID*)szParentName<<"] failed. err["<<m_dwErrorCode<<"]";
								SAFECLOSEHANDLE(m_hVHDFile);
								return VHD_RET_FAILED;
							}
						}
						if( !VHDHelper::ReadFooter(hParent, pParentFooter) )
						{
							//LOGGER<<"CDiffVHD::Create: ReadFooter failed.";
							SAFECLOSEHANDLE(m_hVHDFile);
							SAFECLOSEHANDLE(hParent);
							m_dwErrorCode = VHD_RET_INVALID_FOOTER;
							return VHD_RET_FAILED;
						}
						CloseHandle(hParent);
						hParent = INVALID_HANDLE_VALUE;

						//if GUID is equal, then this is correct parent
						if( IsEqualGUID(m_stHeader.ParentUniqueID, pParentFooter->UniqueId) )
						{
							m_strParent = (WCHAR*)(VOID*)szParentName;
							bFoundParent = TRUE;
						}
					}
					break;
				}
			}

			HANDLE hParent = CreateFileW(m_strParent.c_str(),
				GENERIC_READ,
				FILE_SHARE_READ|FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if( hParent == INVALID_HANDLE_VALUE )
			{
				m_dwErrorCode = GetLastError();
				//LOGGER<<"CDiffVHD::Create: CreateFileW["<<m_strParent.c_str()<<"] failed. err["<<m_dwErrorCode<<"]";
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			CHeapObj<VHDFOOTER> pParentFooter;
			if(!VHDHelper::ReadFooter(hParent, pParentFooter))
			{
				//LOGGER<<"CDiffVHD::Create: ReadFooter failed.";
				SAFECLOSEHANDLE(m_hVHDFile);
				m_dwErrorCode = VHD_RET_INVALID_FOOTER;
				return VHD_RET_FAILED;
			}
			CloseHandle(hParent);
			hParent = INVALID_HANDLE_VALUE;

			//VHDHelper::ConvFooter(pParentFooter);

			//if( m_pParent )
			//{
			//   delete m_pParent;
			//}

			m_nParentDiskType = pParentFooter->DiskType;

			LARGE_INTEGER liPos;
			liPos.QuadPart = m_stHeader.TableOffset;
			if( !SetFilePointerEx(m_hVHDFile, liPos, NULL, FILE_BEGIN) )
			{
				m_dwErrorCode = GetLastError();
				//LOGGER<<"CDiffVHD::Create: SetFilePointerEx3 failed. err["<<m_dwErrorCode<<"]";
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			UINT32* pBAT = new UINT32[m_stHeader.MaxTableEntries];

			ReadFile(m_hVHDFile, pBAT, m_stHeader.MaxTableEntries*sizeof(UINT32), &dwRead, NULL);
			if( dwRead != m_stHeader.MaxTableEntries*sizeof(UINT32) )
			{
				m_dwErrorCode = GetLastError();
				//LOGGER<<"CDiffVHD::Create: ReadFile3 failed. err["<<m_dwErrorCode<<"]";
				SAFECLOSEHANDLE(m_hVHDFile);
				delete[] pBAT;
				return VHD_RET_FAILED;
			}

			for( UINT32 i=0; i<m_stHeader.MaxTableEntries; i++)
			{
				if( pBAT[i] != 0xffffffff )
				{
					pBAT[i] = ntohl(pBAT[i]);
					//update global BAT
					if( pGlobalBAT[i].nOffsetInSector == 0xffffffff )
					{
						pGlobalBAT[i].nOffsetInSector = pBAT[i];
						pGlobalBAT[i].pDisk = this;
					}
				}
			}

			delete[] pBAT;
		}
		break;
	default:
		{
			m_dwErrorCode = GetLastError();
			//LOGGER<<"CDiffVHD::Create: default";
			SAFECLOSEHANDLE(m_hVHDFile);
			return VHD_RET_FAILED;
		}
		break;
	}

	m_pBAT = pGlobalBAT;

	m_ulSectorsPerBlock = m_stHeader.BlockSize/VHD_SECTOR_SIZE; //sector per block
	m_ulBlockBitmapSectorCount = (m_ulSectorsPerBlock+7)/8;   //how many bytes
	m_ulBlockBitmapSectorCount = (m_ulBlockBitmapSectorCount+VHD_SECTOR_SIZE-1)/VHD_SECTOR_SIZE;   //how many sectors
	m_ulChunkSize = m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE+m_stHeader.BlockSize;

	//if (m_pChunkBuf)
	//   delete m_pChunkBuf;
	//m_pChunkBuf = new BYTE[m_ulChunkSize];
	if( m_pChunkBuf )
	{
		VirtualFree(m_pChunkBuf, 0, MEM_RELEASE);
	}

	m_pChunkBuf = (UCHAR*)VirtualAlloc(0, m_ulChunkSize, MEM_COMMIT, PAGE_READWRITE);

	memset(m_pChunkBuf, 0xff, m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE);

	CloseHandle(m_hVHDFile);

	m_bUseBuffering = FALSE;
	if( m_bUseBuffering )
	{
		//LOGGER<<"Using buffering mode...";
	}

	DWORD dwFlags = FILE_ATTRIBUTE_NORMAL;
	if(!m_bUseBuffering)
	{
		dwFlags |= FILE_FLAG_NO_BUFFERING|FILE_FLAG_WRITE_THROUGH;
	}
	m_hVHDFile = CreateFileW(pszFilename,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		dwFlags,
		NULL);
	if( m_hVHDFile == INVALID_HANDLE_VALUE )
	{
		m_dwErrorCode = GetLastError();
		//LOGGER<<"CDiffVHD::Create: CreateFileW["<<pszFilename<<"] failed. err["<<m_dwErrorCode<<"]";
		return VHD_RET_FAILED;
	}


	LARGE_INTEGER li0;
	li0.QuadPart = 0;
	m_ullCurPos = 0xffffffffffffffff;
	Seek(li0, NULL, FILE_BEGIN);
	return VHD_RET_SUCCESSFULL;
}

INT32 CDiffVHD::Close()
{
	return CDynamicVHD::Close();
}

INT CDiffVHD::CompareFilenames(LPCTSTR pszFilename1, LPCTSTR pszFilename2, INT* pnBackSlashCount)
{
	LPCTSTR pSearch1 = pszFilename1;
	LPCTSTR pSearch2 = pszFilename2;

	*pnBackSlashCount = 0;
	INT nRet = 0;
	while( 1 )
	{
		pSearch1 = wcschr(pSearch1, L'\\');
		pSearch2 = wcschr(pSearch2, L'\\');
		if( !pSearch1 || !pSearch2 )
		{
			return nRet;
		}

		pSearch1++;
		pSearch2++;
		DWORD dwLen1 = (DWORD)(pSearch1 - pszFilename1);
		DWORD dwLen2 = (DWORD)(pSearch2 - pszFilename2);

		if(dwLen1 != dwLen2 )
		{
			return nRet;
		}

		if( _wcsnicmp(pszFilename1, pszFilename2, dwLen1) )
		{
			return nRet;
		}

		nRet = dwLen1;
		(*pnBackSlashCount)++;
	}

	return 0;
}

BOOL CDiffVHD::GetParentFilename(LPSTR pszParentName, LPCSTR pszChildName)
{
	INT nBackCount = 0;
	//CStringA strName;
	std::string strName;
	LPSTR p = strstr(pszParentName, "..\\");
	LPSTR pName = pszParentName;


	strName = pName;

	while( p )
	{
		nBackCount++;
		pName = p+3;
		p = strstr(p+3, "..\\");
	}

	if( !_strnicmp(pName, ".\\", 2) )
	{
		pName+=2;
	}

	strName = pName;

	strcpy_s(pszParentName, 512, pszChildName);

	//remove the file name
	p = strrchr(pszParentName, '\\');
	if( !p )
	{
		return FALSE;
	}
	*p=0;

	for( INT i=0; i<nBackCount; i++)
	{
		p = strrchr(pszParentName, '\\');
		if( !p )
		{
			return FALSE;
		}

		*p=0;
	}

	strcat_s(pszParentName, 512, "\\");
	strcat_s(pszParentName, 512, strName.c_str());
	return TRUE;
}

BOOL CDiffVHD::GetParentFilenameW(LPWSTR pszParentName, LPCTSTR pszChildName)
{
	INT nBackCount = 0;
	//CStringW strName;
	std::wstring strName;
	LPWSTR p = wcsstr(pszParentName, L"..\\");
	LPWSTR pName = pszParentName;

	while( p )
	{
		nBackCount++;
		pName = p+3;
		p = wcsstr(p+3, L"..\\");
	}

	if( !_wcsnicmp(pName, L".\\", 2) )
	{
		pName+=2;
	}

	strName = pName;

	wcscpy_s(pszParentName, 256, pszChildName);

	//remove the file name
	p = wcsrchr(pszParentName, L'\\');
	if( !p )
	{
		return FALSE;
	}
	*p=0;

	for( INT i=0; i<nBackCount; i++)
	{
		p = wcsrchr(pszParentName, L'\\');
		if( !p )
		{
			return FALSE;
		}

		*p=0;
	}
	wcscat_s(pszParentName, 256, L"\\");
	wcscat_s(pszParentName, 256, strName.c_str());
	return TRUE;
}

UINT32 CDiffVHD::ConvFileTimeToTimeStamp(FILETIME* pTime)
{
	//    FILETIME fTimeOut;
	//    FileTimeToLocalFileTime(pTime, &fTimeOut);
	//    SYSTEMTIME stTime;
	//    FileTimeToSystemTime(&fTimeOut, &stTime);
	//    tm t;
	//    ZeroMemory(&t, sizeof(t));
	//    t.tm_year = stTime.wYear-1900;
	//    t.tm_mon = stTime.wMonth;
	//    t.tm_mday = stTime.wDay;
	//    t.tm_hour = stTime.wHour;
	//    t.tm_min = stTime.wMinute;
	//    t.tm_sec = stTime.wSecond;
	//    t.tm_wday = stTime.wDayOfWeek;
	// 
	//    return mktime(&t);
	SYSTEMTIME stTime;
	FileTimeToSystemTime(pTime, &stTime);
	tm t;
	ZeroMemory(&t, sizeof(t));
	t.tm_year = stTime.wYear-1900;   //tm_year of tm structure is from 1900.
	t.tm_mon = stTime.wMonth-1;
	t.tm_mday = stTime.wDay;
	t.tm_hour = stTime.wHour;
	t.tm_min = stTime.wMinute;
	t.tm_sec = stTime.wSecond;
	t.tm_wday = stTime.wDayOfWeek-1;

	time_t tFileTime = mktime(&t);
	ZeroMemory(&t, sizeof(t));
	t.tm_year = 2000-1900; //year of time stamp is from 2000
	t.tm_mday = 1;
	time_t t2000 = mktime(&t);

	UINT32 unRet = (UINT32)(tFileTime-t2000);
	return unRet;
}

INT CDiffVHD::GenParentLocator(LPCTSTR pszImageFilename, LPCTSTR pszParentFilename, ULONGLONG ullFirstCodePos, BYTE* pCodeBuf4KB)
{
	ZeroMemory(m_stHeader.ParentLocators, sizeof(m_stHeader.ParentLocators));

	WCHAR szImageTmp[MAX_PATH], szParentTmp[MAX_PATH];
	if( !GetVolumePathNameW(pszImageFilename, szImageTmp, _countof(szImageTmp)) )
	{
		return 0;
	}

	if( !GetVolumePathNameW(pszParentFilename, szParentTmp, _countof(szParentTmp)) )
	{
		return 0;
	}

	BOOL bUseRelatedPath = FALSE;
	if( !_wcsicmp(szImageTmp, szParentTmp) )
	{
		bUseRelatedPath = TRUE;
	}

	if( bUseRelatedPath )
	{
		INT nBackSlash = 0;
		INT nPos = CompareFilenames(pszParentFilename, pszImageFilename, &nBackSlash);
		if( nBackSlash == 0 )
		{
			bUseRelatedPath = FALSE;
		}

		INT nRemainBackSlash = 0;
		CONST WCHAR* p = wcschr(pszImageFilename+nPos, L'\\');
		while( p )
		{
			p++;
			p = wcschr(p, L'\\');
			nRemainBackSlash++;
		}

		szParentTmp[0] = 0;
		for(INT i=0; i<nRemainBackSlash; i++)
		{
			wcscat_s(szParentTmp, _countof(szParentTmp), L"..\\");
		}

		if( nRemainBackSlash == 0 )
		{
			wcscpy_s(szParentTmp, _countof(szParentTmp), L".\\");
		}

		wcscat_s(szParentTmp, _countof(szParentTmp), pszParentFilename+nPos);
	}

	CStringW strParentFilename = pszParentFilename;
	CStringW strParentTmp = szParentTmp;
	if( m_bD2D )
	{
		//change the ext file name to .VHD
		strParentFilename.MakeUpper();
		strParentFilename.TrimRight(VHD_DISK_NAME_EXT_WITHOUT_DOT);
		strParentFilename += L"VHD";

		strParentTmp.MakeUpper();
		strParentTmp.TrimRight(VHD_DISK_NAME_EXT_WITHOUT_DOT);
		strParentTmp += L"VHD";
	}

	ULONGLONG ullCodePos = ullFirstCodePos;
	ZeroMemory(pCodeBuf4KB, 512*8);
	INT nIndex = 7;
	INT nOffset = 0;

	//Wi2k
	CStringA strTmp  (strParentFilename);
	sprintf_s((CHAR*)szImageTmp, sizeof(szImageTmp), "%s", (LPCSTR)strTmp);
	memcpy_s(&m_stHeader.ParentLocators[nIndex].PlatformCode, sizeof(m_stHeader.ParentLocators[nIndex].PlatformCode), "Wi2k", 4);
	m_stHeader.ParentLocators[nIndex].PlatformDataLength = (UINT32)(strlen((CHAR*)szImageTmp));
	m_stHeader.ParentLocators[nIndex].PlatformDataSpace = 512;
	m_stHeader.ParentLocators[nIndex].PlatformDataOffset = ullCodePos;
	memcpy_s(pCodeBuf4KB+nOffset, 512, szImageTmp, strlen((CHAR*)szImageTmp));
	ullCodePos += 512;
	nOffset += 512;
	nIndex--;

	//Wi2r
	if( bUseRelatedPath )
	{
		strTmp = strParentTmp;
		sprintf_s((CHAR*)szImageTmp, sizeof(szImageTmp), "%s", (LPCSTR)strTmp);
		memcpy_s(&m_stHeader.ParentLocators[nIndex].PlatformCode, sizeof(m_stHeader.ParentLocators[nIndex].PlatformCode), "Wi2r", 4);
		m_stHeader.ParentLocators[nIndex].PlatformDataLength = (UINT32)(strlen((CHAR*)szImageTmp));
		m_stHeader.ParentLocators[nIndex].PlatformDataSpace = 512;
		m_stHeader.ParentLocators[nIndex].PlatformDataOffset = ullCodePos;
		memcpy_s(pCodeBuf4KB+nOffset, 512, szImageTmp, strlen((CHAR*)szImageTmp));
		ullCodePos += 512;
		nOffset += 512;
		nIndex--;
	}

	//W2ku
	memcpy_s(&m_stHeader.ParentLocators[nIndex].PlatformCode, sizeof(m_stHeader.ParentLocators[nIndex].PlatformCode), "W2ku", 4);
	m_stHeader.ParentLocators[nIndex].PlatformDataLength = (UINT32)(strParentFilename.GetLength()*sizeof(WCHAR));
	m_stHeader.ParentLocators[nIndex].PlatformDataSpace = 512;
	m_stHeader.ParentLocators[nIndex].PlatformDataOffset = ullCodePos;
	memcpy_s(pCodeBuf4KB+nOffset, 512, strParentFilename, strParentFilename.GetLength()*sizeof(WCHAR));
	ullCodePos += 512;
	nOffset += 512;
	nIndex--;

	//W2ru
	if( bUseRelatedPath )
	{
		memcpy_s(&m_stHeader.ParentLocators[nIndex].PlatformCode, sizeof(m_stHeader.ParentLocators[nIndex].PlatformCode), "W2ru", 4);
		m_stHeader.ParentLocators[nIndex].PlatformDataLength = (UINT32)(strParentTmp.GetLength()*sizeof(WCHAR));
		m_stHeader.ParentLocators[nIndex].PlatformDataSpace = 512;
		m_stHeader.ParentLocators[nIndex].PlatformDataOffset = ullCodePos;
		memcpy_s(pCodeBuf4KB+nOffset, 512, strParentTmp, strParentTmp.GetLength()*sizeof(WCHAR));
		ullCodePos += 512;
		nOffset += 512;
		nIndex--;
	}

	return bUseRelatedPath?4:2;
}
