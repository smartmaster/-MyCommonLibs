#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <openssl/crypto.h>
#include <openssl/comp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "CompressEncryptInterface.h"

#include "OpensslHelper.h"


class  COpensslEnc : public ITransform
{
private:
	LONG m_RefCount; /**INTERNAL**/

	E_CIPPHER m_E_CIPPHER; /**INCOMMING**/
	E_MD m_E_MD; /**INCOMMING**/
	vector<BYTE> m_Salt; /**INCOMMING**/ /**INTERNAL**/
	vector<BYTE> m_password; /**INCOMMING**/

	const EVP_CIPHER * m_cipher; /**INTERNAL**/
	const EVP_MD * m_md; /**INTERNAL**/
	vector<BYTE> m_key; /**INTERNAL**/
	vector<BYTE> m_iv; /**INTERNAL**/
	CSslErrorCallback m_ErrorCallback; /**INTERNAL**/

public:
	COpensslEnc(E_CIPPHER _E_CIPPHER, E_MD _E_MD, LPCBYTE pSalt, LONG saltLen, LPCBYTE password, LONG pwdLen);

	IMP_CLASS_DESTRUCTOR(COpensslEnc);


public:
	//struct ITransform : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//		/* [in] */ REFIID riid,
	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	END_INTERFACE
	//	//	//};
	IMP_IUNKNOWN_METHODS(m_RefCount);

	//	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("COpensslEncDec:ITransform"));
	//	//	virtual HRESULT CreateMember() = 0;
	virtual HRESULT CreateMember();
	//	//	virtual HRESULT DestroyMember() = 0;
	virtual HRESULT DestroyMember();
	//	//};

	//	virtual HRESULT Transform(LPCBYTE origBuffer, LONG origSize, LPBYTE transformedBuffer, LONG transformedSize, LONG & outTransformedSize) = 0;
	virtual HRESULT Transform(LPCBYTE origBuffer, LONG origSize, LPBYTE transformedBuffer, LONG transformedSize, LONG & outTransformedSize);
	//	virtual HRESULT Recover(LPCBYTE transformedBuffer, LONG transformedSize, LPBYTE origBuffer, LONG origSize, LONG & outOrigSize) = 0;
	virtual HRESULT Recover(LPCBYTE transformedBuffer, LONG transformedSize, LPBYTE origBuffer, LONG origSize, LONG & outOrigSize);
	//};
};

class COpensslComp : public ITransform
{
private:
	LONG m_refCount; /**INTERNAL**/
	E_COMP m_E_COMP; /**INCOMMING**/
	BOOL m_bReuseCtx; /**INCOMMING**/

	COMP_METHOD * m_method; /**INTERNAL**/
	COMP_CTX * m_ctx; /**INTERNAL**//**CLEANUP**/
	CSslErrorCallback m_ErrorCallback; /**INTERNAL**/
	BOOL m_zlib_cleanup; /**INTERNAL**/

public:
	COpensslComp(E_COMP _E_COMP, BOOL _bUseCtx);
	
	IMP_CLASS_DESTRUCTOR(COpensslComp);

public:
	//struct ITransform : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//		/* [in] */ REFIID riid,
	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	END_INTERFACE
	//	//	//};
	IMP_IUNKNOWN_METHODS(m_refCount);

	//	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("COpensslComp:ITransform"));
	//	//	virtual HRESULT CreateMember() = 0;
	virtual HRESULT CreateMember();
	//	//	virtual HRESULT DestroyMember() = 0;
	virtual HRESULT DestroyMember();
	//	//};

	//	virtual HRESULT Transform(LPCBYTE origBuffer, LONG origSize, LPBYTE transformedBuffer, LONG transformedSize, LONG & outTransformedSize) = 0;
	HRESULT Transform(LPCBYTE origBuffer, LONG origSize, LPBYTE transformedBuffer, LONG transformedSize, LONG & outTransformedSize);
	//	virtual HRESULT Recover(LPCBYTE transformedBuffer, LONG transformedSize, LPBYTE origBuffer, LONG origSize, LONG & outOrigSize) = 0;
	virtual HRESULT Recover(LPCBYTE transformedBuffer, LONG transformedSize, LPBYTE origBuffer, LONG origSize, LONG & outOrigSize);
	//};

};

class COpensslBase64 : public ITransform
{
private:
	LONG m_RefCount; /**INTERNAL**/
public:
	COpensslBase64();
	
	IMP_CLASS_DESTRUCTOR(COpensslBase64);

public:

	//struct ITransform : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//		/* [in] */ REFIID riid,
	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	END_INTERFACE
	//	//	//};
	IMP_IUNKNOWN_METHODS(m_RefCount);

	//	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("COpensslBase64:ITransform"));
	//	//	virtual HRESULT CreateMember() = 0;
	virtual HRESULT CreateMember();
	//	//	virtual HRESULT DestroyMember() = 0;
	virtual HRESULT DestroyMember();
	//	//};

	//	virtual HRESULT Transform(LPCBYTE origBuffer, LONG origSize, LPBYTE transformedBuffer, LONG transformedSize, LONG & outTransformedSize) = 0;
	virtual HRESULT Transform(LPCBYTE origBuffer, LONG origSize, LPBYTE transformedBuffer, LONG transformedSize, LONG & outTransformedSize);
	//	virtual HRESULT Recover(LPCBYTE transformedBuffer, LONG transformedSize, LPBYTE origBuffer, LONG origSize, LONG & outOrigSize) = 0;
	virtual HRESULT Recover(LPCBYTE transformedBuffer, LONG transformedSize, LPBYTE origBuffer, LONG origSize, LONG & outOrigSize);
	//};


};

class COpensslMD : public IMessageDigest
{
private:
	LONG m_RefCount; /**INTERNAL**/

	E_MD m_E_MD; /**INCOMMING**/
	vector<BYTE> m_salt; /**INCOMMING**/ /**INTERNAL**/ 
	CONST EVP_MD * m_md; /**INTERNAL**/
	CSslErrorCallback m_ErrorCallback; /**INTERNAL**/

public:
	COpensslMD(E_MD e_md, LPCBYTE pSalt, LONG saltLen);

	IMP_CLASS_DESTRUCTOR(COpensslMD);

public:
	//struct IMessageDigest : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//		/* [in] */ REFIID riid,
	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	END_INTERFACE
	//	//	//};
	IMP_IUNKNOWN_METHODS(m_RefCount);

	//	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("COpensslMD:IMessageDigest"));
	//	//	virtual HRESULT CreateMember() = 0;
	virtual HRESULT CreateMember();
	//	//	virtual HRESULT DestroyMember() = 0;
	virtual HRESULT DestroyMember();
	//	//};
	//	virtual HRESULT Hash(LPCBYTE pData, LONG dataLen, LPCBYTE pSalt, LONG saltLen, vector<BYTE> & mdValue) = 0;
	virtual HRESULT Hash(LPCBYTE pData, LONG dataLen, vector<BYTE> & mdValue);
	//};

};


//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCOpensslEnc(E_CIPPHER _E_CIPPHER, E_MD _E_MD, LPCBYTE pSalt, LONG saltLen, LPCBYTE password, LONG pwdLen, ITransform ** ppObj, BOOL bCreateMember);
HRESULT CreateInstanceCOpensslComp(E_COMP _E_COMP, BOOL _bReuseCtx, ITransform ** ppObj, BOOL bCreateMember);
HRESULT CreateInstanceCOpensslBase64(ITransform ** ppObj, BOOL bCreateMember);

//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCOpensslMD(E_MD e_md, LPCBYTE pSalt, LONG saltLen, IMessageDigest ** ppObj, BOOL bCreateMember);
