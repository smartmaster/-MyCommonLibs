#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
using namespace std;


#include "..\PublicHeader\PublicSimpleUnknown.h"
#include "..\PublicHeader\PublicCodeBlock.h"

//////////////////////////////////////////////////////////////////////////
enum E_CIPPHER
{
	E_CIPPHER_unkown,
	E_CIPPHER_aes_128_cbc,
	E_CIPPHER_aes_192_cbc,
	E_CIPPHER_aes_256_cbc,
};

enum E_MD
{
	E_MD_unkown,
	E_MD_md5,
	E_MD_sha,
	E_MD_sha1,
	E_MD_sha256,
	E_MD_sha512,
};


enum E_COMP
{
	E_COMP_unkown,
	E_COMP_zlib,
};


//////////////////////////////////////////////////////////////////////////
struct ITransform : public ISimpleUnknown
{
	//struct ISimpleUnknown : public IUnknown
	//{
	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	IUnknown
	//	//{
	//	//public:
	//	//	BEGIN_INTERFACE
	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//		/* [in] */ REFIID riid,
	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	END_INTERFACE
	//	//};

	//	virtual LPCTSTR WhoAmI() = 0;
	//	virtual HRESULT CreateMember() = 0;
	//	virtual HRESULT DestroyMember() = 0;
	//};

	virtual HRESULT Transform(LPCBYTE origBuffer, LONG origSize, LPBYTE transformedBuffer, LONG transformedSize, LONG & outTransformedSize) = 0;
	virtual HRESULT Recover(LPCBYTE transformedBuffer, LONG transformedSize, LPBYTE origBuffer, LONG origSize, LONG & outOrigSize) = 0;
};



//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCOpensslEnc(E_CIPPHER _E_CIPPHER, E_MD _E_MD, LPCBYTE pSalt, LONG saltLen, LPCBYTE password, LONG pwdLen, ITransform ** ppObj, BOOL bCreateMember);
HRESULT CreateInstanceCOpensslComp(E_COMP _E_COMP, BOOL _bReuseCtx, ITransform ** ppObj, BOOL bCreateMember);
HRESULT CreateInstanceCOpensslBase64(ITransform ** ppObj, BOOL bCreateMember);


struct IMessageDigest : public ISimpleUnknown
{
	//struct ISimpleUnknown : public IUnknown
	//{
	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	IUnknown
	//	//{
	//	//public:
	//	//	BEGIN_INTERFACE
	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//		/* [in] */ REFIID riid,
	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	END_INTERFACE
	//	//};

	//	virtual LPCTSTR WhoAmI() = 0;
	//	virtual HRESULT CreateMember() = 0;
	//	virtual HRESULT DestroyMember() = 0;
	//};
	virtual HRESULT Hash(LPCBYTE pData, LONG dataLen, vector<BYTE> & mdValue) = 0;
};



//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCOpensslMD(E_MD e_md, LPCBYTE pSalt, LONG saltLen, IMessageDigest ** ppObj, BOOL bCreateMember);