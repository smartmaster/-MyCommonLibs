#include "stdafx.h"

#import <msxml.Tlb>

#include "XmlHelper.h"
#include "MyStgHelper.h"


HRESULT CXmlElement::CreateXmlElement( IXMLDOMDocument * pDocument, IXMLDOMElement ** ppElement )
{
	HRESULT hr = S_OK;

	do 
	{
		CComPtr<IXMLDOMElement> spElement;

		//******************************************
		CComBSTR tagName;
		tagName.Attach(m_strTagName.AllocSysString());
		hr = pDocument->createElement( 
			tagName,///* [in] */ BSTR tagName,
			&spElement///* [retval][out] */ IXMLDOMElement **element
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("createElement"), hr, TEXT("createElement %s"), (BSTR)(tagName)));
			break;
		}

		//******************************************
		for (size_t ii=0; ii<m_Attrs.size(); ++ii)
		{
			CComBSTR attrName;
			CComVariant attrVal;

			attrName.Attach(m_Attrs[ii].first.AllocSysString());
			attrVal = (LPCTSTR)(m_Attrs[ii].second);

			hr = spElement->setAttribute( 
				attrName,///* [in] */ BSTR name,
				attrVal///* [in] */ VARIANT value
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("setAttribute"), hr, TEXT("[%s, %s]"), m_Attrs[ii].first, m_Attrs[ii].second));
				break;
			}
		}
		if (FAILED(hr))
		{
			break;
		}

		//******************************************
		if (m_strText.GetLength())
		{
			CComBSTR bstrText;
			bstrText.Attach(m_strText.AllocSysString());
			hr = spElement->put_text(bstrText);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("put_text"), hr, TEXT("[%s]"), m_strText));
				break;
			}
		}


		//******************************************
		*ppElement = spElement.Detach();

	} while (FALSE);

	return hr;
}

HRESULT CXmlHelper::AppendXmlChild( IXMLDOMNode * pNode, IXMLDOMNode * pChildNode, IXMLDOMNode ** ppNewChildNode )
{
	CComPtr<IXMLDOMNode> spNewChildNode;
	HRESULT hr = pNode->appendChild(pChildNode, &spNewChildNode);
	if (FAILED(hr))
	{	
		_OLD_DSETLASTSTATUS(hr);
		_OLD_DAPIERR((0, DRNPOS, TEXT("appendChild"), hr));
	}
	else
	{
		if (ppNewChildNode)
		{
			*ppNewChildNode = spNewChildNode.Detach();
		}
	}

	return hr;
}

HRESULT CMySimpleXmlUT::WriteToNode( IXMLDOMNode * pParentNode, IXMLDOMDocument * pDocument )
{
	HRESULT hr = S_OK;

	static LPCTSTR szTags[] = 
	{
		TEXT("m_Name"),
		TEXT("m_Weight"),
		TEXT("m_Price"),
	};

	for (size_t ii=0; ii<_countof(szTags); ++ii)
	{
		CXmlElement element;
		element.m_strTagName = szTags[ii];
		if (0 == ii)
		{
			element.m_strText = m_Name;
		}
		else if (1 == ii)
		{
			element.m_strText.Format(TEXT("%uKG"), m_Weight);
		}
		else if (2 == ii)
		{
			element.m_strText.Format(TEXT("$%.02f"), m_Price);
		}

		CComPtr<IXMLDOMElement> spElement;
		hr = element.CreateXmlElement(pDocument, &spElement);
		hr = CXmlHelper::AppendXmlChild(pParentNode, spElement, NULL);
	}

	return hr;
}

HRESULT CMySimpleXmlUT::WriteToStorage( IStorage * pParentStorage )
{
	HRESULT hr = S_OK;

	hr = :: WriteClassStg(
		pParentStorage,//__in          IStorage* pStg,
		NS_MySimpleXmlUT::StgGUID//__in          REFCLSID rclsid
		);

	for (size_t ii=0; ii<_countof(NS_MySimpleXmlUT::szStreamNames); ++ii)
	{
		CComPtr<IStream> spIStream;
		DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
		hr = pParentStorage->CreateStream(
			NS_MySimpleXmlUT::szStreamNames[ii],//[in]                 const WCHAR* pwcsName,
			grfMode,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved1,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);

		CComVariant var;
		if (0 == ii)
		{
			var = (LPCTSTR)(m_Name);
		}
		else if (1 == ii)
		{
			var = m_Weight;
		}
		else if (2 == ii)
		{
			var = m_Price;
		}

		hr = var.WriteToStream(spIStream);
	}

	return hr;
}

HRESULT CMySimpleXmlUT::ReadFromStorage( IStorage * pParentStorage )
{
	HRESULT hr = S_OK;

	GUID stgGUID = {0};
	hr = :: ReadClassStg(
		pParentStorage,//__in          IStorage* pStg,
		&stgGUID//__in          REFCLSID rclsid
		);
	BOOL bEqual = IsEqualGUID(stgGUID, NS_MySimpleXmlUT::StgGUID);

	for (size_t ii=0; ii<_countof(NS_MySimpleXmlUT::szStreamNames); ++ii)
	{
		CComPtr<IStream> spIStream;
		DWORD grfMode = STGM_READ | STGM_SHARE_EXCLUSIVE;
		hr = pParentStorage->OpenStream(
			NS_MySimpleXmlUT::szStreamNames[ii],//[in]                 const WCHAR* pwcsName,
			NULL,//[in]                 void* reserved1,
			grfMode,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);

		CComVariant var;
		hr = var.ReadFromStream(spIStream);

		if (0 == ii)
		{
			m_Name = var.bstrVal;
		}
		else if (1 == ii)
		{
			m_Weight = var.ulVal;
		}
		else if (2 == ii)
		{
			m_Price = var.dblVal;
		}
	}

	return hr;
}
HRESULT CComplexXmlUT::WriteToNode( IXMLDOMNode * pParentNode, IXMLDOMDocument * pDocument )
{
	HRESULT hr = S_OK;

	static LPCTSTR szTags[] =
	{
		TEXT("m_Name"),
		TEXT("m_Weight"),
		TEXT("m_Price"),
	};

	static LPCTSTR szTags2[] =
	{
		TEXT("m_InnerSimpleXml"),
		TEXT("m_InnerSimpleXml2"),
	};

	for (size_t ii=0; ii<_countof(szTags); ++ii)
	{
		CXmlElement element;
		element.m_strTagName = szTags[ii];
		if (0 == ii)
		{
			element.m_strText = m_Name;
		}
		else if (1 == ii)
		{
			element.m_strText.Format(TEXT("%uKG"), m_Weight);
		}
		else if (2 == ii)
		{
			element.m_strText.Format(TEXT("$%.02f"), m_Price);
		}

		CComPtr<IXMLDOMElement> spElement;
		hr = element.CreateXmlElement(pDocument, &spElement);
		hr = CXmlHelper::AppendXmlChild(pParentNode, spElement, NULL);
	}

	for (size_t ii=0; ii<_countof(szTags2); ++ii)
	{
		CXmlElement element;
		element.m_strTagName = szTags2[ii];

		CComPtr<IXMLDOMElement> spElement;
		hr = element.CreateXmlElement(pDocument, &spElement);

		if (0 == ii)
		{
			m_InnerSimpleXml.WriteToNode(spElement, pDocument);
		}
		else if (1 == ii)
		{
			m_InnerSimpleXml2.WriteToNode(spElement, pDocument);
		}

		hr = CXmlHelper::AppendXmlChild(pParentNode, spElement, NULL);
	}

	return hr;
}

HRESULT CComplexXmlUT::XmlHelperUT( LPCTSTR pszXmlFile )
{
	HRESULT hr = S_OK;

	do 
	{
		hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

		CComPtr<IXMLDOMDocument> spDocument;
		hr = spDocument.CoCreateInstance(__uuidof(DOMDocument));

		CComplexXmlUT myXml;
		myXml.m_Name = TEXT("Mike");	//0
		myXml.m_Weight = 99;	//1
		myXml.m_Price = 88.88;	//2

		myXml.m_InnerSimpleXml.m_Name = TEXT("Ivan");	//0
		myXml.m_InnerSimpleXml.m_Weight = 88;	//1
		myXml.m_InnerSimpleXml.m_Price = 77.77;	//2

		myXml.m_InnerSimpleXml2.m_Name = TEXT("John");	//0
		myXml.m_InnerSimpleXml2.m_Weight = 425;	//1
		myXml.m_InnerSimpleXml2.m_Price = 9.88;	//2

		CXmlElement root;
		root.m_strTagName = TEXT("myRoot");
		root.m_Attrs.push_back(make_pair(CString(TEXT("type")), CString(TEXT("human")) ) );
		root.m_Attrs.push_back(make_pair(CString(TEXT("type2")), CString(TEXT("things")) ) );
		CComPtr<IXMLDOMElement>  spRoot;
		hr = root.CreateXmlElement(spDocument, &spRoot);
		hr = myXml.WriteToNode(spRoot, spDocument);
		hr = CXmlHelper::AppendXmlChild(spDocument, spRoot, NULL);

		CComVariant varDest(pszXmlFile);
		hr = spDocument->save(varDest);


	} while (FALSE);

	CoUninitialize();

	return hr;
}

HRESULT CComplexXmlUT::WriteToStorage( IStorage * pParentStorage )
{
	HRESULT hr = S_OK;

	hr = :: WriteClassStg(
		pParentStorage,//__in          IStorage* pStg,
		NS_ComplexXmlUT::StgGUID//__in          REFCLSID rclsid
		);

	for (size_t ii=0; ii<_countof(NS_ComplexXmlUT::szStreamNames); ++ii)
	{
		CComPtr<IStream> spIStream;
		DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
		hr = pParentStorage->CreateStream(
			NS_ComplexXmlUT::szStreamNames[ii],//[in]                 const WCHAR* pwcsName,
			grfMode,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved1,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);

		CComVariant var;
		if (0 == ii)
		{
			var = (LPCTSTR)(m_Name);
		}
		else if (1 == ii)
		{
			var = m_Weight;
		}
		else if (2 == ii)
		{
			var = m_Price;
		}

		hr = var.WriteToStream(spIStream);
	}

	for (size_t ii=0; ii<_countof(NS_ComplexXmlUT::szStorageNames); ++ii)
	{
		CComPtr<IStorage> spStorage;
		DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
		hr = pParentStorage->CreateStorage(
			NS_ComplexXmlUT::szStorageNames[ii],//[in]                 const WCHAR* pwcsName,
			grfMode,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved1,
			0,//[in]                 DWORD reserved2,
			&spStorage//[out]                IStorage** ppstg
			);

		if (0 == ii)
		{
			hr = m_InnerSimpleXml.WriteToStorage(spStorage);
		}
		else if (1 == ii)
		{
			hr = m_InnerSimpleXml2.WriteToStorage(spStorage);
		}
	}

	return hr;
}

HRESULT CComplexXmlUT::ReadFromStorage( IStorage * pParentStorage )
{
	HRESULT hr = S_OK;

	GUID StgGUID = {0};
	hr = ::ReadClassStg(
		pParentStorage,//__in          IStorage* pStg,
		&StgGUID//__in          REFCLSID rclsid
		);
	BOOL bEqual = IsEqualGUID(StgGUID, NS_ComplexXmlUT::StgGUID);

	for (size_t ii=0; ii<_countof(NS_ComplexXmlUT::szStreamNames); ++ii)
	{
		CComPtr<IStream> spIStream;
		DWORD grfMode = STGM_READ | STGM_SHARE_EXCLUSIVE;
		hr = pParentStorage->OpenStream(
			NS_ComplexXmlUT::szStreamNames[ii],//[in]                 const WCHAR* pwcsName,
			NULL,//[in]                 void* reserved1,
			grfMode,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);

		CComVariant var;
		hr = var.ReadFromStream(spIStream);

		if (0 == ii)
		{
			m_Name = var.bstrVal;
		}
		else if (1 == ii)
		{
			m_Weight = var.ulVal;
		}
		else if (2 == ii)
		{
			m_Price = var.dblVal;
		}
	}

	for (size_t ii=0; ii<_countof(NS_ComplexXmlUT::szStorageNames); ++ii)
	{
		CComPtr<IStorage> spStorage;
		DWORD grfMode = STGM_READ | STGM_SHARE_EXCLUSIVE;
		hr = pParentStorage->OpenStorage(
			NS_ComplexXmlUT::szStorageNames[ii],//[in]                 const WCHAR* pwcsName,
			NULL,//[in]                 IStorage* pstgPriority,
			grfMode,//[in]                 DWORD grfMode,
			0,//[in]                 SNB snbExclude,
			0,//[in]                 DWORD reserved,
			&spStorage//[out]                IStorage** ppstg
			);

		if (0 == ii)
		{
			hr = m_InnerSimpleXml.ReadFromStorage(spStorage);
		}
		else if (1 == ii)
		{
			hr = m_InnerSimpleXml2.ReadFromStorage(spStorage);
		}
	}

	return hr;
}

HRESULT CComplexXmlUT::StorageHelperUT( LPCTSTR pszXmlFile )
{
	HRESULT hr = S_OK;

	do 
	{
		hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

		CComPtr<IXMLDOMDocument> spDocument;
		hr = spDocument.CoCreateInstance(__uuidof(DOMDocument));

		{
			CComplexXmlUT myXml;
			myXml.m_Name = TEXT("Mike");	//0
			myXml.m_Weight = 99;	//1
			myXml.m_Price = 88.88;	//2

			myXml.m_InnerSimpleXml.m_Name = TEXT("Ivan");	//0
			myXml.m_InnerSimpleXml.m_Weight = 88;	//1
			myXml.m_InnerSimpleXml.m_Price = 77.77;	//2

			myXml.m_InnerSimpleXml2.m_Name = TEXT("John");	//0
			myXml.m_InnerSimpleXml2.m_Weight = 425;	//1
			myXml.m_InnerSimpleXml2.m_Price = 9.88;	//2

			CComPtr<IStorage> spIStorage;
			DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
			CMyStgHelper::CreateFileStorage(pszXmlFile, &spIStorage, grfMode);

			hr = myXml.WriteToStorage(spIStorage);
		}

		{
			CComplexXmlUT myXml;

			CComPtr<IStorage> spIStorage;
			DWORD grfMode = STGM_READ | STGM_SHARE_DENY_WRITE;
			CMyStgHelper::OpenFileStorage(pszXmlFile, &spIStorage, grfMode);

			hr = myXml.ReadFromStorage(spIStorage);
		}

	} while (FALSE);

	CoUninitialize();

	return hr;
}