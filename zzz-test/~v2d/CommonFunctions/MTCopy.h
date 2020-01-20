#pragma once

#include "stdafx.h"
#include "MTQueque.h"
#include "..\ServerFileDevice\ServerFileDevice_i.h"
#include "SimpleBitArray.h"
#include "VMPtr.h"
#include "LocalFileDevice.h"
#include "LocalFileDevicePreAlloc.h"

namespace NS_MTCopy
{
	struct CBufferItem
	{
		LONGLONG m_Offset;
		LONGLONG m_DataLen;
		BYTE * m_pBuffer;

		CBufferItem();
		HRESULT CreateMember();
		HRESULT DestroyMember();

		static VOID GetBlockSize();
		static ULONG BUFFER_SIZE;
	};

	class CMTCopy
	{
	private:
		IN IFileDeviceObject * m_pSource;
		IN IFileDeviceObject * m_pDest; 
		IN CSimpleBitArray * m_pSimpleBitArray; 
		IN ULONG m_ulBlockSize; 
		IN LONGLONG m_llDestSrcOffsetDiff;
		IN LONGLONG m_liTotalData;
		IN LONGLONG m_llSourceStartOffset;

		LONGLONG m_TotalRead;
		LONGLONG m_TotalWrite;

		NS_MTQ::CSharedBuffer<CBufferItem> m_SharedBuffers;
		NS_MTQ::CMTQueue<CBufferItem> m_Queue;


	public:
		CMTCopy
			(
			IN IFileDeviceObject * pSource,
			IN IFileDeviceObject * pDest, 
			IN CSimpleBitArray * pSimpleBitArray, 
			IN ULONG ulBlockSize, 
			IN LONGLONG llOffsetDiff,
			IN LONGLONG liTotalData,
			IN LONGLONG llSourceStartOffset
			);
		HRESULT CreateMember(ULONG nBufferItemCount);
		HRESULT DestroyMember();
		virtual ~CMTCopy();

		HRESULT Copy();

	private:
		static DWORD WINAPI ProcessDestThreadProc(LPVOID lpParameter);
		HRESULT ProcessDest();

		HRESULT ProcessSource();
		HRESULT ProcessSourceBuffer(LONGLONG llStartOffset, LONGLONG llDataLen);
	};

	//////////////////////////////////////////////////////////////////////////
	//UT start
	struct TestApp
	{
		struct CCmdLine
		{
			CString m_strSource;
			CString m_strDest;
			BOOL m_bSourceBufferIO;
			BOOL m_bDestBufferIO;
			ULONG m_RWBufferSize;
			ULONG m_RWBuffersCount;
			ULONG m_PreAllocDeltaMB;

			CCmdLine();
			VOID Parse(INT argc, TCHAR ** argv);
		};

		static HRESULT Main(CCmdLine & cmdLine);
	};
	//UT end
	//////////////////////////////////////////////////////////////////////////
}
