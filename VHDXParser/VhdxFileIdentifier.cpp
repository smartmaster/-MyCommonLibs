#include "stdafx.h"
#include "VhdxFileIdentifier.h"
#include "VhdxPrintField.h"




VOID VhdxFileIdentifier::Print()
{
	//D_INFO(0, TEXT("VhdxFileIdentifier+++++++++++++++++START"));
	PRINT_FIELD(m_Signature);
	PRINT_FIELD((LPCTSTR)m_Creator);
	//D_INFO(0, TEXT("VhdxFileIdentifier+++++++++++++++++END\r\n\r\n\r\n"));
}

UINT64 VhdxFileIdentifier::SignatureSpecConst()
{
	return 0x656C696678646876ULL; //VHDX file type identifier signature ("vhdxfile")
}

INT VhdxFileIdentifier::OffsetSpecConst()
{
	return 0;
}

LONG VhdxFileIdentifier::SizeSpecConst()
{
	return 64 * 1024;
}

BOOL VhdxFileIdentifier::IsValid()
{
	return (m_Signature == SignatureSpecConst());
}

VOID VhdxFileIdentifier::ReadCreator( CString & strCreator )
{
	LPTSTR pBuffer = strCreator.GetBuffer(_countof(m_Creator) + 1);
	CopyMemory(pBuffer, m_Creator, sizeof(m_Creator));
	pBuffer[_countof(m_Creator)] = 0;
	strCreator.ReleaseBuffer();
}

VOID VhdxFileIdentifier::WriteSignature()
{
	m_Signature = SignatureSpecConst();
}

VOID VhdxFileIdentifier::WriteCreator( LPCTSTR szCreator )
{
	ZeroMemory(m_Creator, sizeof(m_Creator));
	_tcscpy_s((LPTSTR)m_Creator, _countof(m_Creator), szCreator);
}

LONG VhdxFileIdentifier::HeadersSizeSpecConst()
{
	return 320 * 1024;
}

LONG VhdxFileIdentifier::HeadersTotalSizeSpecConst()
{
	return 1024 * 1024;
}
