#include "StdAfx.h"
#include "LIB_VHD_VhdDef.h"
#include "LIB_VHD_DynamicVHD.h"
#include "LIB_VHD_VHDHelper.h"
#include <comdef.h>
#include <assert.h>

CDynamicVHD::CDynamicVHD(VOID)
{
	ZeroMemory(&m_stHeader, sizeof(m_stHeader));
	m_pBAT = NULL;
	m_ulSectorsPerBlock = 0;
	m_ulBlockBitmapSectorCount = 0;
	m_ulChunkSize = 0;
	m_ullCurPos = 0;
	m_pChunkBuf = NULL;
	m_bNeedWriteFooterWhenClose = FALSE;
	m_bD2D = TRUE;

	m_bUseBuffering = FALSE;
}

CDynamicVHD::~CDynamicVHD(VOID)
{
}

INT32 CDynamicVHD::Create(LPCTSTR pszFilename, DWORD dwCreationDisposition, PBATELEMENT pGlobalBAT, PVHD_CREATEPARAM pParam)
{
	if (!pszFilename)
	{
		//LOGGER<<"CDynamicVHD::Create: !pszFilename";
		m_dwErrorCode = VHD_RET_INVALID_PARAMETER;
		return VHD_RET_FAILED;
	}

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
		//LOGGER<<"CDynamicVHD::Create: CreateFileW["<<pszFilename<<"] failed. err["<<m_dwErrorCode<<"]";
		return VHD_RET_FAILED;
	}

	m_bNeedWriteFooterWhenClose = FALSE;

	switch(dwCreationDisposition)
	{
	case CREATE_NEW:
		{
			m_bNeedWriteFooterWhenClose = TRUE;
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
			m_stFooter.TimeStamp = VHDHelper::ConvTimeStamp(tCurTime);//(UINT32)tCurTime;
			memcpy_s(m_stFooter.CreatorApplication, sizeof(m_stFooter.CreatorApplication), "BAB", 4);
			m_stFooter.CreatorVersion = 0x01000000;
			memcpy_s(m_stFooter.CreatorHostOS, sizeof(m_stFooter.CreatorHostOS), "Wi2k", 4);
			m_stFooter.OriginalSize = pParam->liDiskSize.QuadPart;
			m_stFooter.CurrentSize = pParam->liDiskSize.QuadPart;
			ULONGLONG ullTotalSectors = pParam->stGeometry.Cylinders.QuadPart*pParam->stGeometry.TracksPerCylinder*pParam->stGeometry.SectorsPerTrack;
			m_stFooter.DiskGeometry.Value = VHDHelper::CalcCHS(ullTotalSectors);
			m_stFooter.DiskType = VHDType;
			CoCreateGuid(&m_stFooter.UniqueId);
			m_stFooter.SavedState = 0;
			m_stFooter.Checksum = VHDHelper::CalcChecksum(&m_stFooter, sizeof(m_stFooter));

			//write the copy of footer at the top of DYN VHD
			VHDFOOTER stFooter;
			memcpy_s(&stFooter, sizeof(stFooter), &m_stFooter, sizeof(m_stFooter));
			VHDHelper::ConvFooter(&stFooter);
			DWORD dwWritten = 0;
			WriteFile(m_hVHDFile, &stFooter, sizeof(stFooter), &dwWritten, NULL);
			if( dwWritten != sizeof(stFooter) )
			{
				m_dwErrorCode = GetLastError();
				//LOGGER<<"CDynamicVHD::Create: write copy footer failed. err["<<m_dwErrorCode<<"]";
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			//Gen DYN header
			memcpy_s(m_stHeader.Cookie, sizeof(m_stHeader.Cookie), "cxsparse", 8);
			m_stHeader.DataOffset = 0xffffffffffffffff;
			m_stHeader.TableOffset = sizeof(VHDFOOTER)+sizeof(VHDDYNHEADER);
			m_stHeader.HeaderVersion = 0x00010000;
			m_stHeader.BlockSize = VHD_BLOCK_SIZE;
			m_stHeader.MaxTableEntries = (UINT32)(pParam->liDiskSize.QuadPart/m_stHeader.BlockSize);
			m_stHeader.MaxTableEntries = ((m_stHeader.MaxTableEntries*sizeof(UINT32)+VHD_SECTOR_SIZE-1)&~(VHD_SECTOR_SIZE-1))/sizeof(UINT32);
			m_stHeader.Checksum = VHDHelper::CalcChecksum(&m_stHeader, sizeof(m_stHeader));

			//write DYN header
			VHDDYNHEADER stHeader;
			memcpy_s(&stHeader, sizeof(stHeader), &m_stHeader, sizeof(m_stHeader));
			VHDHelper::ConvHeader(&stHeader);
			WriteFile(m_hVHDFile, &stHeader, sizeof(stHeader), &dwWritten, NULL);
			if( dwWritten != sizeof(VHDDYNHEADER))
			{
				m_dwErrorCode = GetLastError();
				//LOGGER<<"CDynamicVHD::Create: write header failed. err["<<m_dwErrorCode<<"]";
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			UINT32* pBAT = new UINT32[m_stHeader.MaxTableEntries];
			memset(pBAT, 0xff, sizeof(ULONG)*m_stHeader.MaxTableEntries);

			WriteFile(m_hVHDFile, pBAT, m_stHeader.MaxTableEntries*sizeof(UINT32), &dwWritten, NULL);
			delete[] pBAT;

			if( dwWritten != m_stHeader.MaxTableEntries*sizeof(INT32) )
			{
				m_dwErrorCode = GetLastError();
				//LOGGER<<"CDynamicVHD::Create: write bat failed. err["<<m_dwErrorCode<<"]";

				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			//do not write actual footer here.
		}
		break;
	case OPEN_EXISTING:
		{
			if( !VHDHelper::ReadFooter(m_hVHDFile, &m_stFooter) )
			{
				//LOGGER<<"CDynamicVHD::Create: ReadFooter failed";
				SAFECLOSEHANDLE(m_hVHDFile);
				m_dwErrorCode = VHD_RET_INVALID_FOOTER;
				return VHD_RET_FAILED;
			}

			if( m_stFooter.DiskType != VHDType )
			{
				//LOGGER<<"CDynamicVHD::Create: disk type mismatch";
				SAFECLOSEHANDLE(m_hVHDFile);
				m_dwErrorCode = VHD_RET_INVALID_DISK_TYPE;
				return VHD_RET_FAILED;
			}

			LARGE_INTEGER li;
			li.QuadPart = m_stFooter.DataOffset;
			if( !SetFilePointerEx(m_hVHDFile, li, NULL, FILE_BEGIN) )
			{
				m_dwErrorCode = GetLastError();
				//LOGGER<<"CDynamicVHD::Create: SetFilePointerEx1 failed. err["<<m_dwErrorCode<<"]";
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			DWORD dwRead = 0;
			ReadFile(m_hVHDFile, &m_stHeader, sizeof(m_stHeader), &dwRead, NULL);
			if( dwRead != sizeof(m_stHeader) )
			{
				m_dwErrorCode = GetLastError();
				//LOGGER<<"CDynamicVHD::Create: ReadFile1 failed. err["<<m_dwErrorCode<<"]";
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			VHDHelper::ConvHeader(&m_stHeader);
			UINT32 unChecksum = m_stHeader.Checksum;
			m_stHeader.Checksum = 0;
			if( unChecksum != VHDHelper::CalcChecksum(&m_stHeader, sizeof(m_stHeader)) )
			{
				//LOGGER<<"CDynamicVHD::Create: check sum error.";
				SAFECLOSEHANDLE(m_hVHDFile);
				m_dwErrorCode = VHD_RET_CHKSUM_MISMATCH;
				return VHD_RET_FAILED;
			}

			LARGE_INTEGER liFileSize;

			if( !GetFileSizeEx(m_hVHDFile, &liFileSize) )
			{
				m_dwErrorCode = GetLastError();
				//LOGGER<<"CDynamicVHD::Create: GetFileSizeEx failed. err["<<m_dwErrorCode<<"]";
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			UINT32 nBATSize = m_stHeader.MaxTableEntries;

			LARGE_INTEGER liPos;
			liPos.QuadPart = m_stHeader.TableOffset;
			if( !SetFilePointerEx(m_hVHDFile, liPos, NULL, FILE_BEGIN) )
			{
				m_dwErrorCode = GetLastError();
				//LOGGER<<"CDynamicVHD::Create: SetFilePointerEx2 failed. err["<<m_dwErrorCode<<"]";
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			UINT32* pBuf = new UINT32[nBATSize];
			if( !pBuf )
			{
				SAFECLOSEHANDLE(m_hVHDFile);
				m_dwErrorCode = GetLastError();
				return VHD_RET_FAILED;
			}

			ReadFile(m_hVHDFile, pBuf, nBATSize*sizeof(UINT32), &dwRead, NULL);
			if( dwRead != nBATSize*sizeof(UINT32) )
			{
				m_dwErrorCode = GetLastError();
				//LOGGER<<"CDynamicVHD::Create: ReadFile2 failed. err["<<m_dwErrorCode<<"]";
				delete[] pBuf;
				SAFECLOSEHANDLE(m_hVHDFile);
				return VHD_RET_FAILED;
			}

			for( UINT32 i=0; i<m_stHeader.MaxTableEntries; i++ )
			{
				if( pBuf[i] != 0xffffffff )
				{
					pBuf[i] = ntohl(pBuf[i]);
					//update global BAT
					if( pGlobalBAT[i].nOffsetInSector == 0xffffffff )
					{
						pGlobalBAT[i].nOffsetInSector = pBuf[i];
						pGlobalBAT[i].pDisk = this;
					}
				}
			}

			delete[] pBuf;
		}
		break;
	default:
		{
			//LOGGER<<"CDynamicVHD::Create: default";

			SAFECLOSEHANDLE(m_hVHDFile);
			m_dwErrorCode = VHD_RET_INVALID_PARAMETER;
			return VHD_RET_FAILED;
		}
		break;
	}

	m_pBAT = pGlobalBAT;

	m_ulSectorsPerBlock = m_stHeader.BlockSize/VHD_SECTOR_SIZE; //sector per block
	m_ulBlockBitmapSectorCount = (m_ulSectorsPerBlock+7)/8;   //how many bytes
	m_ulBlockBitmapSectorCount = (m_ulBlockBitmapSectorCount+VHD_SECTOR_SIZE-1)/VHD_SECTOR_SIZE;   //how many sectors
	m_ulChunkSize = m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE+m_stHeader.BlockSize;

	// if (m_pChunkBuf)
	//delete m_pChunkBuf;
	// m_pChunkBuf = new BYTE[m_ulChunkSize];
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
		//LOGGER<<"CDynamicVHD::Create: CreateFileW["<<pszFilename<<"] failed. err["<<m_dwErrorCode<<"]";
		return VHD_RET_FAILED;
	}

	LARGE_INTEGER li0;
	li0.QuadPart = 0;
	m_ullCurPos = 0xffffffffffffffff;
	Seek(li0, NULL, FILE_BEGIN);
	return VHD_RET_SUCCESSFULL;
}

INT32 CDynamicVHD::Close()
{
	if( m_hVHDFile == INVALID_HANDLE_VALUE )
	{
		return VHD_RET_SUCCESSFULL;
	}

	////flush chunk buffer
	//if( m_bChunkBufferDirty )
	//{
	//   //currently, the file pointer must be point to the correct position to flush chunk
	//   DWORD dwWritten = 0;
	//   WriteFile(m_hVHDFile, m_pChunkBuf, m_ulChunkSize, &dwWritten, 0);
	//   if( dwWritten != m_ulChunkSize )
	//   {
	//      DWORD dwErr = GetLastError();
	//      //AFLOG<<"flush chunk buffer failed. err="<<dwErr;
	//      return dwErr;
	//   }
	//}

	//save BAT to vhd
	if( m_bNeedWriteFooterWhenClose )
	{
		UINT32 nBATSize = m_stHeader.MaxTableEntries;
		nBATSize = ((nBATSize+127)/128)*128;

		UINT32* pBAT = (UINT32*)VirtualAlloc(0, nBATSize*sizeof(UINT32), MEM_COMMIT, PAGE_READWRITE);
		memset(pBAT, 0xff, nBATSize);
		VHDHelper::BAT_GtoL(m_pBAT, pBAT, this, m_stHeader.MaxTableEntries);

		LARGE_INTEGER liPos;
		liPos.QuadPart = m_stHeader.TableOffset;
		if( !SetFilePointerEx(m_hVHDFile, liPos, NULL, FILE_BEGIN) )
		{
			DWORD dwErr = GetLastError();
			//LOGGER<<"CDynamicVHD::Close: SetFilePointerEx3 failed. err["<<dwErr<<"]";
			delete[] pBAT;
			return dwErr;
		}

		DWORD dwWritten = 0;
		WriteFile(m_hVHDFile, pBAT, nBATSize*sizeof(UINT32), &dwWritten, NULL);
		VirtualFree(pBAT, 0, MEM_RELEASE);
		if( dwWritten != nBATSize*sizeof(UINT32) )
		{
			DWORD dwErr = GetLastError();
			//LOGGER<<"CDynamicVHD::Close: WriteFile3 failed. err["<<dwErr<<"]";
			return dwErr;
		}

		//Write footer
		VHDFOOTER stFooter;
		memcpy_s(&stFooter, sizeof(stFooter), &m_stFooter, sizeof(m_stFooter));
		stFooter.Checksum = 0;
		stFooter.Checksum = VHDHelper::CalcChecksum(&stFooter, sizeof(stFooter));
		VHDHelper::ConvFooter(&stFooter);

		liPos.QuadPart = 0;
		if( !SetFilePointerEx(m_hVHDFile, liPos, NULL, FILE_END) )
		{
			DWORD dwErr = GetLastError();
			//LOGGER<<"CDynamicVHD::Close: SetFilePointerEx4 failed. err["<<dwErr<<"]";
			return dwErr;
		}

		VOID* pFooterBuf = VirtualAlloc(0, sizeof(stFooter), MEM_COMMIT, PAGE_READWRITE);
		memcpy_s(pFooterBuf, sizeof(stFooter), &stFooter, sizeof(stFooter));
		WriteFile(m_hVHDFile, pFooterBuf, sizeof(stFooter), &dwWritten, NULL);
		VirtualFree(pFooterBuf, 0, MEM_RELEASE);
		if( dwWritten != sizeof(stFooter) )
		{
			DWORD dwErr = GetLastError();
			//LOGGER<<"CDynamicVHD::Close: WriteFile4 failed. err["<<dwErr<<"]";
			return dwErr;
		}

		m_bNeedWriteFooterWhenClose = FALSE;
	}

	FlushFileBuffers(m_hVHDFile);
	SAFECLOSEHANDLE(m_hVHDFile);

	if( m_pChunkBuf )
	{
		//delete[] m_pChunkBuf;
		VirtualFree(m_pChunkBuf, 0, MEM_RELEASE);
		m_pChunkBuf = NULL;
	}
	return VHD_RET_SUCCESSFULL;
}

INT32 CDynamicVHD::Seek(LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER pliNewPosition, DWORD dwMoveMethod)
{
	if( liDistanceToMove.QuadPart % VHD_SECTOR_SIZE )
	{
		return VHD_RET_FAILED;
	}

	LARGE_INTEGER liPos;
	switch(dwMoveMethod)
	{
	case FILE_BEGIN:
		{
			liPos.QuadPart = liDistanceToMove.QuadPart;
		}
		break;
	case FILE_CURRENT:
		{
			liPos.QuadPart = (LONGLONG)m_ullCurPos + liDistanceToMove.QuadPart;
		}
		break;
	case FILE_END:
		{
			liPos.QuadPart = (LONGLONG)m_stFooter.OriginalSize + liDistanceToMove.QuadPart;
		}
		break;
	default:
		{
			return VHD_RET_FAILED;
		}
		break;
	}

	if( liPos.QuadPart > (LONGLONG)m_stFooter.OriginalSize || liPos.QuadPart < 0 )
	{
		return VHD_RET_FAILED;
	}

	m_ullCurPos = liPos.QuadPart;
	if(pliNewPosition)
		pliNewPosition->QuadPart = m_ullCurPos;

	return VHD_RET_SUCCESSFULL;
}

INT32 CDynamicVHD::Read(CONST VOID* pBuffer, DWORD dwNumberOfBytesToRead, PDWORD pdwNumberOfBytesRead)
{
	if( pdwNumberOfBytesRead )
	{
		*pdwNumberOfBytesRead = 0;
	}

	if( dwNumberOfBytesToRead % VHD_SECTOR_SIZE )
	{
		//LOGGER<<"CDynamicVHD::Read: dwNumberOfBytesToRead["<<dwNumberOfBytesToRead<<"] % VHD_SECTOR_SIZE";
		return VHD_RET_FAILED;
	}

	if( m_ullCurPos >= m_stFooter.OriginalSize )
	{
		//LOGGER<<"CDynamicVHD::Read: m_ullCurPos["<<m_ullCurPos<<"] >= m_stFooter.OriginalSize["<<m_stFooter.OriginalSize<<"]";
		return VHD_RET_FAILED;
	}

	BYTE* pDstBuf = (BYTE*)pBuffer;

	ULONG ulSectorLeftInCurBlock = (ULONG)(m_ullCurPos % m_stHeader.BlockSize);
	ulSectorLeftInCurBlock /= VHD_SECTOR_SIZE;
	ulSectorLeftInCurBlock = m_ulSectorsPerBlock - ulSectorLeftInCurBlock;

	ULONG ulLeft = dwNumberOfBytesToRead;
	if( m_ullCurPos + ulLeft > m_stFooter.OriginalSize )
	{
		ulLeft = (ULONG)(m_stFooter.OriginalSize - m_ullCurPos);
	}

	DWORD dwRead = 0;
	while( ulLeft )
	{
		ULONG ulReadInCur = ulSectorLeftInCurBlock*VHD_SECTOR_SIZE;
		if( ulLeft< ulReadInCur )
		{
			ulReadInCur = ulLeft;
		}

		ULONG ulBATIndex = (ULONG)(m_ullCurPos/m_stHeader.BlockSize);
		CVirtualHardDisk* pDisk = m_pBAT[ulBATIndex].pDisk;
		if( pDisk != this && pDisk != NULL )
		{
			LARGE_INTEGER liSeek;
			liSeek.QuadPart = m_ullCurPos;
			if( VHD_RET_SUCCESSFULL != pDisk->Seek(liSeek, NULL, FILE_BEGIN) )
			{
				m_dwErrorCode = pDisk->GetLastErrCode();
				//AFLOG<<"pDisk->Seek failed. err="<<m_dwErrorCode;
				return m_dwErrorCode;
			}

			pDisk->Read(pDstBuf, ulReadInCur, &dwRead);
			if( dwRead != ulReadInCur )
			{
				m_dwErrorCode = pDisk->GetLastErrCode();
				//AFLOG<<"pDisk->Read failed. err="<<m_dwErrorCode;
				return m_dwErrorCode;
			}
		}
		else
		{
			if( m_pBAT[ulBATIndex].nOffsetInSector != 0xffffffff )
			{
				LARGE_INTEGER liPos;
				ULONG ulBlockSectorCount = m_ulBlockBitmapSectorCount + m_stHeader.BlockSize/VHD_SECTOR_SIZE;

				liPos.QuadPart = (LONGLONG)((LONGLONG)(m_pBAT[ulBATIndex].nOffsetInSector)+(LONGLONG)m_ulBlockBitmapSectorCount)*VHD_SECTOR_SIZE+(LONGLONG)m_stHeader.BlockSize-(LONGLONG)ulSectorLeftInCurBlock*VHD_SECTOR_SIZE;
				if( !SetFilePointerEx(m_hVHDFile, liPos, NULL, FILE_BEGIN) )
				{
					m_dwErrorCode = GetLastError();
					//AFLOG<<"SetFilePointerEx("<<liPos<<") failed. err="<<m_dwErrorCode;
					return m_dwErrorCode;
				}

				//ReadFile( m_hVHDFile, pDstBuf, ulReadInCur, &dwRead, NULL);
				ReadFile( m_hVHDFile, m_pChunkBuf+m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE, ulReadInCur, &dwRead, NULL);
				if( ulReadInCur != dwRead )
				{
					m_dwErrorCode = GetLastError();
					//AFLOG<<"ReadFile("<<ulReadInCur<<", "<<dwRead<<") failed. err="<<m_dwErrorCode;
					return m_dwErrorCode;
				}

				memcpy_s(pDstBuf, ulReadInCur, m_pChunkBuf+m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE, ulReadInCur);
			}
			else
			{
				ZeroMemory(pDstBuf, ulReadInCur);
				dwRead = ulReadInCur;
			}
		}

		m_ullCurPos += dwRead;
		pDstBuf += dwRead;

		if( pdwNumberOfBytesRead )
		{
			*pdwNumberOfBytesRead += dwRead;
		}

		ulLeft -= ulReadInCur;
		ulSectorLeftInCurBlock = m_ulSectorsPerBlock;
	}

	return VHD_RET_SUCCESSFULL;
}

INT32 CDynamicVHD::Write(CONST VOID* pBuffer, DWORD dwNumberOfBytesToWrite, PDWORD pdwNumberOfBytesWritten)
{
	if( pdwNumberOfBytesWritten )
	{
		*pdwNumberOfBytesWritten = 0;
	}

	if( dwNumberOfBytesToWrite % VHD_SECTOR_SIZE )
	{
		//LOGGER<<"CDynamicVHD::Write: dwNumberOfBytesToWrite["<<dwNumberOfBytesToWrite<<"] % VHD_SECTOR_SIZE";
		return VHD_RET_FAILED;
	}

	if( m_ullCurPos >= m_stFooter.OriginalSize && dwNumberOfBytesToWrite>0 )
	{
		//LOGGER<<"CDynamicVHD::Write: m_ullCurPos["<<m_ullCurPos<<"] >= m_stFooter.OriginalSize["<<m_stFooter.OriginalSize<<"] && dwNumberOfBytesToWrite["<<dwNumberOfBytesToWrite<<"]>0";
		return VHD_RET_FAILED;
	}

	BYTE* pSrcBuf = (BYTE*)pBuffer;

	ULONG ulSectorLeftInCurBlock = (ULONG)(m_ullCurPos % m_stHeader.BlockSize);
	ulSectorLeftInCurBlock /= VHD_SECTOR_SIZE;
	ulSectorLeftInCurBlock = m_ulSectorsPerBlock - ulSectorLeftInCurBlock;

	ULONG ulLeft = dwNumberOfBytesToWrite;
	if( m_ullCurPos + ulLeft > m_stFooter.OriginalSize )
	{
		ulLeft = (ULONG)(m_stFooter.OriginalSize - m_ullCurPos);
	}

	DWORD dwRead = 0;
	DWORD dwWritten = 0;
	while( ulLeft )
	{
		ULONG ulWriteInCur = ulSectorLeftInCurBlock*VHD_SECTOR_SIZE;
		if( ulLeft< ulWriteInCur )
		{
			ulWriteInCur = ulLeft;
		}

		ULONG ulBATIndex = (ULONG)(m_ullCurPos/m_stHeader.BlockSize);

		CVirtualHardDisk* pDisk = m_pBAT[ulBATIndex].pDisk;
		BOOL bUseChunkBuffer = FALSE;
		if( m_pBAT[ulBATIndex].nOffsetInSector != 0xffffffff && pDisk == this)
		{
			//current is a allocated block
			//space the m_hVHDFile to actual position in block data
			//LARGE_INTEGER liPos;

			//liPos.QuadPart = (LONGLONG)((LONGLONG)((*m_pBAT)[ulBATIndex].nOffsetInSector)+(LONGLONG)m_ulBlockBitmapSectorCount)*VHD_SECTOR_SIZE+(LONGLONG)m_stHeader.BlockSize-(LONGLONG)ulSectorLeftInCurBlock*VHD_SECTOR_SIZE;
			//if( !SetFilePointerEx(m_hVHDFile, liPos, NULL, FILE_BEGIN) )
			//{
			//   m_dwErrorCode = GetLastError();
			//   //VHDLOG( //TELOG(1)<<"CDynamicVHD::Create: SetFilePointerEx1(m_hVHDFile, liPos["<<liPos<<"], NULL, FILE_BEGIN)  err["<<m_dwErrorCode<<"]"; )
			//   return VHD_RET_FAILED;
			//}
		}
		else   //allocate a new block
		{
			bUseChunkBuffer = TRUE;
			LARGE_INTEGER liPos;

			//now, remove the footer
			if( !m_bNeedWriteFooterWhenClose )
			{
				liPos.QuadPart = -512;
				if( !SetFilePointerEx(m_hVHDFile, liPos, NULL, FILE_END) )
				{
					m_dwErrorCode = GetLastError();
					//VHDLOG( //TELOG(1)<<"CDynamicVHD::Create: SetFilePointerEx2(m_hVHDFile, liPos["<<liPos<<"], NULL, FILE_END)  err["<<m_dwErrorCode<<"]"; )
					return m_dwErrorCode;
				}

				SetEndOfFile(m_hVHDFile);  //Remove footer

				m_bNeedWriteFooterWhenClose = TRUE;
			}

			liPos.QuadPart = 0;
			if( !SetFilePointerEx(m_hVHDFile, liPos, &liPos, FILE_END) )
			{
				m_dwErrorCode = GetLastError();
				//VHDLOG( //TELOG(1)<<"CDynamicVHD::Create: SetFilePointerEx3(m_hVHDFile, liPos["<<liPos<<"], &liPos, FILE_END)  err["<<m_dwErrorCode<<"]"; )
				return m_dwErrorCode;
			}

			UINT32 nNewOffset = (UINT32)(liPos.QuadPart/VHD_SECTOR_SIZE);

			if( pDisk != this && pDisk != NULL && ulWriteInCur < m_ulSectorsPerBlock*VHD_SECTOR_SIZE)
			{
				//copy chunk from parent disk
				LARGE_INTEGER liSeek;
				liSeek.QuadPart = (ULONGLONG)ulBATIndex*(ULONGLONG)m_stHeader.BlockSize;
				if( VHD_RET_SUCCESSFULL != pDisk->Seek(liSeek, NULL, FILE_BEGIN) )
				{
					m_dwErrorCode = pDisk->GetLastErrCode();
					//AFLOG<<"pDisk->Seek(liSeek["<<liSeek<<"], NULL, FILE_BEGIN) failed. err="<<m_dwErrorCode; 
					return m_dwErrorCode;
				}

				DWORD dwReadRet = 0;
				DWORD dwRead;
				dwReadRet = pDisk->Read(m_pChunkBuf+m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE, m_stHeader.BlockSize, &dwRead);
				if( dwReadRet != 0 )
				{
					m_dwErrorCode = pDisk->GetLastErrCode();
					//AFLOG<<"pDisk->Read(m_pChunkBuf+m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE, m_stHeader.BlockSize["<<m_stHeader.BlockSize<<"], &dwRead["<<dwRead<<"]) failed. err="<<m_dwErrorCode;
					return m_dwErrorCode;
				}
				//WriteFile(m_hVHDFile, m_pChunkBuf, m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE+m_stHeader.BlockSize, &dwWritten, NULL);
				//if( dwWritten != m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE+m_stHeader.BlockSize )
				//{
				//   //VHDLOG( //TELOG(1)<<"CDynamicVHD::Create: WriteFile(m_hVHDFile, m_pChunkBuf, m_ulBlockBitmapSectorCount["<<m_ulBlockBitmapSectorCount<<"]*VHD_SECTOR_SIZE+m_stHeader.BlockSize["<<m_stHeader.BlockSize<<"], &dwWritten["<<dwWritten<<"], NULL)  err["<<GetLastError()<<"]"; )
				//      return VHD_RET_FAILED;
				//}
			}
			else
			{
				//allocate block space (do not include the footer)
				ZeroMemory(m_pChunkBuf+m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE, m_stHeader.BlockSize);
				//WriteFile(m_hVHDFile, m_pChunkBuf, m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE, &dwWritten, NULL);
				//if( dwWritten != m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE )
				//{
				//   //VHDLOG( //TELOG(1)<<"CDynamicVHD::Create: WriteFile(m_hVHDFile, m_pChunkBuf, m_ulBlockBitmapSectorCount["<<m_ulBlockBitmapSectorCount<<"]*VHD_SECTOR_SIZE, &dwWritten["<<dwWritten<<"], NULL)  err["<<GetLastError()<<"]"; )
				//      return VHD_RET_FAILED;
				//}

				//liPos.QuadPart = (LONGLONG)m_stHeader.BlockSize;
				//if( !SetFilePointerEx(m_hVHDFile, liPos, NULL, FILE_END) )
				//{
				//   //VHDLOG( //TELOG(1)<<"CDynamicVHD::Create: SetFilePointerEx4(m_hVHDFile, liPos["<<liPos<<"], NULL, FILE_END) err["<<GetLastError()<<"]"; )
				//      return VHD_RET_FAILED;
				//}

				//SetEndOfFile(m_hVHDFile);
			}

			//allocate a new element in BAT and write it to file.
			m_pBAT[ulBATIndex].nOffsetInSector = nNewOffset;
			m_pBAT[ulBATIndex].pDisk = this;

			//space m_hVHDFile to the correct position to write
			//long lSeek = ulSectorLeftInCurBlock*VHD_SECTOR_SIZE;
			//lSeek = -lSeek;
			//liPos.QuadPart = lSeek;
			//if( !SetFilePointerEx(m_hVHDFile, liPos, NULL, FILE_END) )
			//{
			//   //VHDLOG( //TELOG(1)<<"CDynamicVHD::Create: SetFilePointerEx5(m_hVHDFile, liPos["<<liPos<<"], NULL, FILE_END) err["<<GetLastError()<<"]"; )
			//      return VHD_RET_FAILED;
			//}
		}

		//write data to file
		if( bUseChunkBuffer )
		{
			ULONG ulCopyTo = m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE+m_stHeader.BlockSize-ulSectorLeftInCurBlock*VHD_SECTOR_SIZE;
			memcpy_s(&m_pChunkBuf[ulCopyTo], m_ulChunkSize-ulCopyTo, pSrcBuf, ulWriteInCur);

			LARGE_INTEGER liPos;

			liPos.QuadPart = (LONGLONG)(m_pBAT[ulBATIndex].nOffsetInSector)*VHD_SECTOR_SIZE;
			if( !SetFilePointerEx(m_hVHDFile, liPos, NULL, FILE_BEGIN) )
			{
				m_dwErrorCode = GetLastError();
				//AFLOG<<"SetFilePointerEx1(m_hVHDFile, liPos["<<liPos<<"], NULL, FILE_BEGIN)  err["<<m_dwErrorCode<<"]";
				return m_dwErrorCode;
			}
			WriteFile( m_hVHDFile, m_pChunkBuf, m_ulChunkSize, &dwWritten, NULL);
			if( dwWritten != m_ulChunkSize )
			{
				m_dwErrorCode = GetLastError();
				//AFLOG<<"WriteFile failed. err="<<m_dwErrorCode;
				return m_dwErrorCode;
			}
		}
		else
		{
			LARGE_INTEGER liPos;

			liPos.QuadPart = (LONGLONG)((LONGLONG)(m_pBAT[ulBATIndex].nOffsetInSector)+(LONGLONG)m_ulBlockBitmapSectorCount)*VHD_SECTOR_SIZE+(LONGLONG)m_stHeader.BlockSize-(LONGLONG)ulSectorLeftInCurBlock*VHD_SECTOR_SIZE;
			if( !SetFilePointerEx(m_hVHDFile, liPos, NULL, FILE_BEGIN) )
			{
				m_dwErrorCode = GetLastError();
				//AFLOG<<"SetFilePointerEx1(m_hVHDFile, liPos["<<liPos<<"], NULL, FILE_BEGIN)  err["<<m_dwErrorCode<<"]";
				return m_dwErrorCode;
			}

			memcpy_s(m_pChunkBuf+m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE, m_stHeader.BlockSize, pSrcBuf, ulWriteInCur);
			WriteFile( m_hVHDFile, m_pChunkBuf+m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE, ulWriteInCur, &dwWritten, NULL);
			if( dwWritten != ulWriteInCur )
			{
				m_dwErrorCode = GetLastError();
				//AFLOG<<"WriteFile failed. err="<<m_dwErrorCode;
				return m_dwErrorCode;
			}
		}

		dwWritten = ulWriteInCur;

		m_ullCurPos += dwWritten;
		pSrcBuf += dwWritten;

		if( pdwNumberOfBytesWritten )
		{
			*pdwNumberOfBytesWritten += dwWritten;
		}

		ulLeft -= ulWriteInCur;
		ulSectorLeftInCurBlock = m_ulSectorsPerBlock;
	}


	return VHD_RET_SUCCESSFULL;
}

VOID CDynamicVHD::LtoP(LONGLONG llLogic, LONGLONG* pllPhysical)
{
	ULONG ulIndex = (ULONG)(llLogic/m_stHeader.BlockSize);
	ULONG ulLeft = (ULONG)(llLogic%m_stHeader.BlockSize);
	(*pllPhysical) = (LONGLONG)m_pBAT[ulIndex].nOffsetInSector*VHD_SECTOR_SIZE+m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE+ulLeft;
}