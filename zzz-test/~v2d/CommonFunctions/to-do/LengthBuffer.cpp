#include "stdafx.h"
#include "LengthBuffer.h"


CLengthBuffer::CLengthBuffer() :
m_pGrand(NULL),
m_pData(NULL),
m_GrandLength(0),
m_DataLength(0)
{

}

VOID CLengthBuffer::Allocate( LONGLONG DataLengh )
{
	m_DataLength = DataLengh;
	m_GrandLength = HEADER_LENGTH + AtlAlignUp(m_DataLength, 8);
	if (m_GrandLength > m_Buffer.size())
	{
		m_Buffer.resize(m_GrandLength);
	}

	m_pGrand = &m_Buffer[0];
	m_pData = NULL;
	if (m_GrandLength > HEADER_LENGTH)
	{
		m_pData = m_pGrand + HEADER_LENGTH;
	}

	CopyMemory(m_pGrand, &m_GrandLength, sizeof(LONGLONG));
	CopyMemory(m_pGrand + sizeof(LONGLONG), &m_DataLength, sizeof(LONGLONG));
}

LONGLONG CLengthBuffer::GetGrandLength() CONST
{
	return m_GrandLength;
}

LONGLONG CLengthBuffer::GetDataLength() CONST
{
	return m_DataLength;
}

BYTE * CLengthBuffer::GetGrandBuffer() CONST
{
	return m_pGrand;
}

BYTE * CLengthBuffer::GetDataBuffer() CONST
{
	return m_pData;
}
