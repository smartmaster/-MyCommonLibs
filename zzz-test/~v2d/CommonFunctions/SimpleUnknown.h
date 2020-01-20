#pragma once

#include <tchar.h>
#include <atlbase.h>
#include <windows.h>

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

	virtual LPCTSTR WhoAmI() = 0;
	virtual HRESULT CreateMember() = 0;
	virtual HRESULT DestroyMember() = 0;
};


#define IMP_CLASS_DESTRUCTOR(__abc__xyz__class_name) \
virtual ~ __abc__xyz__class_name() \
{ \
	DestroyMember(); \
};

#define IMP_IUNKNOWN_METHODS(m_RefCount__abc__xyz__) \
virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) \
{ \
	return E_NOTIMPL; \
} \
virtual ULONG STDMETHODCALLTYPE AddRef( void) \
{ \
	LONG __abc__xyz__RefCount = InterlockedIncrement(&m_RefCount__abc__xyz__); \
	return __abc__xyz__RefCount; \
} \
virtual ULONG STDMETHODCALLTYPE Release( void) \
{ \
	LONG __abc__xyz__RefCount = InterlockedDecrement(&m_RefCount__abc__xyz__); \
	if (0 == __abc__xyz__RefCount) \
	{ \
	 	delete this; \
	} \
	return __abc__xyz__RefCount; \
}
