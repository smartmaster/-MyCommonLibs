#include "stdafx.h"

#include "VHDXLogReplayCallback.h"


TReplayedData::TReplayedData()
{
	ZeroMemory(this, sizeof(TReplayedData));
}

HRESULT TReplayedData::Allocate(LONG BuffSize)
{
	HRESULT hr = S_OK;
	ATLASSERT(NULL == m_pData);
	CVMemPtr<BYTE> apBuffer;
	apBuffer.Allocate(BuffSize);
	if (NULL == apBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("BuffSize=%d"), BuffSize);
	}
	else
	{
		m_pData = apBuffer.Detach();
	}
	return hr;
}

HRESULT TReplayedData::Free()
{
	if (m_pData)
	{
		CVMemPtr<BYTE> apBuffer;
		apBuffer.Attach(m_pData);
		m_pData = NULL;
		apBuffer.Free();
	}

	return S_OK;
}

CVHDXLogReplayCallback::CVHDXLogReplayCallback(ULONGLONG PayloadBlockSartOffset) :
m_RefCount(0),
m_PayloadBlockSartOffset(PayloadBlockSartOffset),
m_MinOffset(-1LL),
m_MaxOffset(-1LL)
{

}

HRESULT CVHDXLogReplayCallback::CreateMember()
{
	return S_OK;
}

HRESULT CVHDXLogReplayCallback::DestroyMember()
{
	for (size_t ii = 0; ii < m_ReplayedData.size(); ++ii)
	{
		m_ReplayedData[ii].Free();
	}
	m_ReplayedData.clear();

	return S_OK;
}

HRESULT CVHDXLogReplayCallback::OnReplayedData(ULONGLONG FileOffset, ULONGLONG Length, CONST BYTE * pBuffer, BOOL bZero)
{
	HRESULT hr = S_OK;

	//////////////////////////////////////////////////////////////////////////
	if (FileOffset < m_PayloadBlockSartOffset) //only record replayed data for payload blocks - MS ate what mentioned in VHDX spec
	{
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	if (-1LL == m_MaxOffset)
	{
		m_MinOffset = FileOffset;
		m_MaxOffset = FileOffset + Length;
	}
	else
	{
		if (m_MinOffset > FileOffset)
		{
			m_MinOffset = FileOffset;
		}
		if (m_MaxOffset < FileOffset + Length)
		{
			m_MaxOffset = FileOffset + Length;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	TReplayedData ReplayedData; //all fields zeroed
	//ReplayedData.m_Flags;
	//ReplayedData.m_Enum;
	ReplayedData.m_FileOffset = FileOffset;
	ReplayedData.m_DataLength = Length;

	if (!bZero)
	{
		hr = ReplayedData.Allocate(Length);
		if (FAILED(hr))
		{
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT(""), ...);
			D_API_ERR(0, TEXT("Allocate"), hr, TEXT("ReplayedData.Allocate"));
			//LEAVE_BLOCK(0);
		}
		else
		{
			CopyMemory(ReplayedData.m_pData, pBuffer, Length);
		}
	}

	m_ReplayedData.push_back(ReplayedData);

	return hr;
}

CONST vector<TReplayedData> & CVHDXLogReplayCallback::GetReplayedData()
{
	return m_ReplayedData;
}

ULONGLONG CVHDXLogReplayCallback::GetPayloadBlockSartOffset()
{
	return m_PayloadBlockSartOffset;
}

LONGLONG CVHDXLogReplayCallback::GetMaxOffset()
{
	return m_MaxOffset;
}

LONGLONG CVHDXLogReplayCallback::GetMinOffset()
{
	return m_MinOffset;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstcanceCVHDXLogReplayCallback(ULONGLONG PayloadBlockSartOffset, IVHDXLogReplayCallback ** ppObj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new CVHDXLogReplayCallback(PayloadBlockSartOffset), ppObj, bCreateMember);
}
