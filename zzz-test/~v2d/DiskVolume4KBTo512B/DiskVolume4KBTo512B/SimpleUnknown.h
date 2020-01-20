#pragma once

struct ISimpleUnknown : public IUnknown
{
	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	IUnknown
	//{
	//public:
	//	BEGIN_INTERFACE
	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//		/* [in] */ REFIID riid,
	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	END_INTERFACE
	//};

	virtual HRESULT CreateMember() = 0;
	virtual HRESULT DestroyMember() = 0;
};


#define IMP_IUNKNOWN_METHOD(m_RefCount) \
virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) \
{ \
	return E_NOTIMPL; \
} \
virtual ULONG STDMETHODCALLTYPE AddRef( void) \
{ \
	LONG __abc__xyz__RefCount = InterlockedIncrement(&m_RefCount); \
	return __abc__xyz__RefCount; \
} \
virtual ULONG STDMETHODCALLTYPE Release( void) \
{ \
	LONG __abc__xyz__RefCount = InterlockedDecrement(&m_RefCount); \
	if (0 == __abc__xyz__RefCount) \
	{ \
	 	delete this; \
	} \
	return __abc__xyz__RefCount; \
}
