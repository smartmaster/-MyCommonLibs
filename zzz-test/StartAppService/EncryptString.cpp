#include "stdafx.h"
#include "CodeBlockDef.h"
#include "D2DCryptoError.h"
#include "ICryptoWrapperInterface.h"


LONG EncryptString(CONST wstring & Password, CONST wstring & PlainText, wstring & EncryptedText)
{
	LONG Status = 0;
	ICryptoWrapperInterface* pCryptoWrap = NULL;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CCryptoParam EncInfo;
	EncInfo.m_dwEncLibType = ELT_MSCRYPTO;
	EncInfo.m_dwEncAlgType = EEAT_AES_128BIT;
	EncInfo.m_dwHashAlgType = EHAT_MD5;
	EncInfo.m_dwEncAPIType = ECAT_CAPI; //ECAT_CNG; //<sonmi01>2012-9-25 ###???
	EncInfo.m_wsPassword = Password;
	pCryptoWrap = ICryptoWrapFactory::CryptoWrapInstance(EncInfo, true);
	
	//////////////////////////////////////////////////////////////////////////
	DWORD CipherDataBufLen = 0;
	DWORD PlainTextLen = PlainText.size() * sizeof(WCHAR) + sizeof(WCHAR);
	Status = pCryptoWrap->EncryptData(
		(CONST BYTE*)PlainText.c_str(),//IN     const BYTE* pPlainbataBuf,
		PlainTextLen,//IN     DWORD       dwPlainDataBufLen,
		NULL,//OUT    BYTE*       pbCipherDataBuf,
		&CipherDataBufLen,//IN OUT DWORD*      pdwCipherDataBufLen,
		true//IN OPT bool        bPadding = false
		);
	if (0 != Status)
	{
		if (D2DCRYPTO_E_MORE_DATA != Status)
		{
			//error
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CHeapPtr<BYTE> apBuffer;
	apBuffer.Allocate(CipherDataBufLen);
	Status = pCryptoWrap->EncryptData(
		(CONST BYTE*)PlainText.c_str(),//IN     const BYTE* pPlainbataBuf,
		PlainText.size() * sizeof(WCHAR) + sizeof(WCHAR),//IN     DWORD       dwPlainDataBufLen,
		apBuffer.m_pData,//OUT    BYTE*       pbCipherDataBuf,
		&CipherDataBufLen,//IN OUT DWORD*      pdwCipherDataBufLen,
		true//IN OPT bool        bPadding = false
		);
	if (0 != Status)
	{
		//error
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	CHeapPtr<WCHAR> apStringBuffer;
	DWORD StrBufferLen = CipherDataBufLen * 4;
	apStringBuffer.Allocate(StrBufferLen);
	Status = ICryptoWrapFactory::Binary2StringW(
		apStringBuffer.m_pData,//OUT    WCHAR*      pwzString,
		&StrBufferLen,//IN OUT DWORD*      pdwStrSize,
		apBuffer.m_pData,//IN     const PBYTE pbBinary,
		CipherDataBufLen//IN     DWORD       dwBinSize
		);
	if (0 != Status)
	{
		//error
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	EncryptedText = apStringBuffer.m_pData;
	END_BLOCK(0);

	ICryptoWrapFactory::ReleaseCryptoWrapInstance(&pCryptoWrap);

	return Status;
}

LONG DecryptString(CONST wstring & Password, CONST wstring & EncryptedText, wstring & PlainText)
{
	LONG Status = 0;
	ICryptoWrapperInterface* pCryptoWrap = NULL;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CCryptoParam EncInfo;
	EncInfo.m_dwEncLibType = ELT_MSCRYPTO;
	EncInfo.m_dwEncAlgType = EEAT_AES_128BIT;
	EncInfo.m_dwHashAlgType = EHAT_MD5;
	EncInfo.m_dwEncAPIType = ECAT_CAPI; //ECAT_CNG; //<sonmi01>2012-9-25 ###???
	EncInfo.m_wsPassword = Password;
	pCryptoWrap = ICryptoWrapFactory::CryptoWrapInstance(EncInfo, true);


	//////////////////////////////////////////////////////////////////////////
	CHeapPtr<BYTE> apEncBuffer;
	DWORD EncBufferLen = EncryptedText.size() * sizeof(WCHAR) * 4;
	DWORD PlainBufferLen = EncBufferLen;
	apEncBuffer.Allocate(EncBufferLen);
	Status = ICryptoWrapFactory::WString2Binary(
		apEncBuffer.m_pData,//OUT     PBYTE        pbBinary,
		&EncBufferLen,//IN OUT  DWORD*       pdwBinSize,
		EncryptedText.c_str(),//IN      const WCHAR* pwzString,
		EncryptedText.size()//IN      DWORD        dwStrSize
		);
	if (Status)
	{
		//error
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	CHeapPtr<BYTE> apPlainBuffer;
	apPlainBuffer.Allocate(PlainBufferLen);
	Status = pCryptoWrap->DecryptData( 
		apEncBuffer.m_pData,//IN     const BYTE* pbCipherDataBuf,
		EncBufferLen,//IN     DWORD       dwCipherDataBufLen, 
		apPlainBuffer.m_pData,//OUT    BYTE*       pPlainbataBuf, 
		&PlainBufferLen,//IN OUT DWORD*      pdwPlainDataBufLen,
		true//IN OPT bool        bPadding = false
		);
	if (Status)
	{
		//error
		break;
	}

	PlainText = (CONST WCHAR*)apPlainBuffer.m_pData;
	END_BLOCK(0);

	ICryptoWrapFactory::ReleaseCryptoWrapInstance(&pCryptoWrap);

	return Status;
}