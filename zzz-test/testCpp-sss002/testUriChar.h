#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <msxml2.h>

namespace SmartLib
{
	class TestUriChar
	{
	public:
		

	// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)

	// Macro to verify memory allocation.
#define CHK_ALLOC(p)        do { if (!(p)) { hr = E_OUTOFMEMORY; goto CleanUp; } } while(0)

	// Macro that releases a COM object if not NULL.
#define SAFE_RELEASE(p)     do { if ((p)) { (p)->Release(); (p) = NULL; } } while(0)\

	// Helper function to create a VT_BSTR variant from a null terminated string. 
			static HRESULT VariantFromString(PCWSTR wszValue, VARIANT &Variant)
			{
				HRESULT hr = S_OK;
				BSTR bstr = SysAllocString(wszValue);
				CHK_ALLOC(bstr);

				V_VT(&Variant) = VT_BSTR;
				V_BSTR(&Variant) = bstr;

			CleanUp:
				return hr;
			}

			// Helper function to create a DOM instance. 
			static HRESULT CreateAndInitDOM(IXMLDOMDocument **ppDoc)
			{
				HRESULT hr = CoCreateInstance(__uuidof(DOMDocument30), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppDoc));
				if (SUCCEEDED(hr))
				{
					// these methods should not fail so don't inspect result
					(*ppDoc)->put_async(VARIANT_FALSE);
					(*ppDoc)->put_validateOnParse(VARIANT_FALSE);
					(*ppDoc)->put_resolveExternals(VARIANT_FALSE);
				}
				return hr;
			}

			static BOOL loadDOMRaw(LPCTSTR pXmlFile)
			{
				BOOL bRet = FALSE;

				HRESULT hr = S_OK;
				IXMLDOMDocument *pXMLDom = NULL;
				IXMLDOMParseError *pXMLErr = NULL;

				BSTR bstrXML = NULL;
				BSTR bstrErr = NULL;
				VARIANT_BOOL varStatus;
				VARIANT varFileName;
				VariantInit(&varFileName);

				CHK_HR(CreateAndInitDOM(&pXMLDom));

				// XML file name to load
				CHK_HR(VariantFromString(pXmlFile, varFileName));
				CHK_HR(pXMLDom->load(varFileName, &varStatus));
				if (varStatus == VARIANT_TRUE)
				{
					bRet = TRUE;
					CHK_HR(pXMLDom->get_xml(&bstrXML));
					//xxx_printf("XML DOM loaded from stocks.xml:\n%S\n", bstrXML);
					//D2DDEBUGLOG(0, 0, TEXT(__FUNCTION__) TEXT("::XML DOM loaded from %s"), pXmlFile);
				}
				else
				{
					// Failed to load xml, get last parsing error
					CHK_HR(pXMLDom->get_parseError(&pXMLErr));
					CHK_HR(pXMLErr->get_reason(&bstrErr));
					//xxx_printf("Failed to load DOM from stocks.xml. %S\n", bstrErr);
					//D2DDEBUGLOG(0, -1, TEXT(__FUNCTION__) TEXT("::Failed to load DOM from %s, error is %s"), pXmlFile, bstrErr);
				}

			CleanUp:
				SAFE_RELEASE(pXMLDom);
				SAFE_RELEASE(pXMLErr);
				SysFreeString(bstrXML);
				SysFreeString(bstrErr);
				VariantClear(&varFileName);

				return bRet;
			}


			static BOOL IsXMLFile(LPCTSTR pXmlFile)
			{
				HRESULT hr = CoInitialize(NULL);
				CString str = pXmlFile;
				str.Replace(TEXT("%"), TEXT("%25"));
				BOOL bRet = loadDOMRaw(str.GetString());
				if (SUCCEEDED(hr))
				{
					CoUninitialize();
				}
				return bRet;
			}

			static void Case0()
			{
				LPCTSTR xmlPath = TEXT("D:\\zzz-temp\\!  ' ( ) ;  @ & = + $ ,   %20 # [ ]\\111.xml");
				IsXMLFile(xmlPath);

				xmlPath = TEXT("D:\\zzz-temp\\!  ' ( ) ;  @ & = + $ ,   %%% # [ ]\\111.xml");
				IsXMLFile(xmlPath);

				xmlPath = TEXT("D:\\zzz-temp\\!  ' ( ) ;  @ & = + $ ,   %20%20%25 # [ ]\\111.xml");
				IsXMLFile(xmlPath);
			}

	};
}