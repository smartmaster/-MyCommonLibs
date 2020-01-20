#pragma once

#include "HPTHandler.h"

class CHPTServerHandlerFactory : public IHPTServerHandlerFactory
{
private:
	LONG m_RefCount; /***INTERNAL***/

	HPTBody m_TempBody; /***INTERNAL***/
	HPTHeader m_TempHeader; /***INTERNAL***/

	BODY_HANDLER_STATUS_STATISTIC_TOTAL m_StatusStatistic; /***INTERNAL***/


public:
	CHPTServerHandlerFactory();

	IMP_CLASS_DESTRUCTOR(CHPTServerHandlerFactory);
	//struct IHPTServerHandlerFactory : public ISimpleUnknown
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
	IMP_WHO_AM_I(TEXT("CHPTServerHandlerFactory:IHPTServerHandlerFactory"));
	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//};

	virtual HRESULT CreateServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppNewIHPTHandler, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);

	virtual CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * GetStatusStatistic();

	//};
};


