#include "stdafx.h"
#include "CryptoCommon.h"
#include "CXMLFileMgr.h"
#include "D2DErrorCode.h"
//#include "ICatalogMgrInterface.h"

CXMLFileMgr::CXMLFileMgr(const WCHAR* pwzXMLNameOrContent /* = NULL */, int iCreateMode /* = EXMLCM_WRITE | EXMLCM_CONTENT */)
: m_pXMLDomPtr(NULL),
  m_wsXmlFileName(L""),
  m_wsXmlContent(L""),
  m_bCoInitialized(false),
  m_bIsMSXNLCreated(false),
  m_iCreateMode(iCreateMode)
{
    if(!m_bCoInitialized)
    {
        HRESULT hrRet = ::CoInitialize(NULL); 
        if (SUCCEEDED(hrRet))
            m_bCoInitialized = true;
    }

    if(pwzXMLNameOrContent && (wcslen(pwzXMLNameOrContent) != 0))
    {
        if(iCreateMode & EXMLCM_FILE)
        {
            if(iCreateMode & EXMLCM_CACHE_CONTENT)
                CacheXMLContent(pwzXMLNameOrContent, m_wsXmlContent);

            m_wsXmlFileName = wstring(pwzXMLNameOrContent);
        }
        else if(iCreateMode & EXMLCM_CONTENT)
            m_wsXmlContent = wstring(pwzXMLNameOrContent);

        CreateXML(pwzXMLNameOrContent, iCreateMode);
    }
}

CXMLFileMgr::~CXMLFileMgr()
{
    SaveXML(m_wsXmlFileName.c_str(), m_iCreateMode);
    ReleaseXML();

    if(m_bCoInitialized)
        ::CoUninitialize();
}

long 
CXMLFileMgr::CreateXML(const WCHAR* pwzXMLNameOrContent, int iCreateMode, bool bWriteHeader /* = true */)
{
    ReleaseXML();

    if(iCreateMode != m_iCreateMode)
        m_iCreateMode = iCreateMode;

    if(pwzXMLNameOrContent && (wcslen(pwzXMLNameOrContent) != 0))
    {
        if((iCreateMode & EXMLCM_FILE) && (_wcsicmp(pwzXMLNameOrContent, m_wsXmlFileName.c_str()) != 0))
        {
            if (iCreateMode & EXMLCM_CACHE_CONTENT)
                CacheXMLContent(pwzXMLNameOrContent, m_wsXmlContent);
            
            m_wsXmlFileName = wstring(pwzXMLNameOrContent);
        }
        else if((iCreateMode & EXMLCM_CONTENT) && (_wcsicmp(pwzXMLNameOrContent, m_wsXmlContent.c_str()) != 0))
            m_wsXmlContent = wstring(pwzXMLNameOrContent);
    }

    do 
    {
        HRESULT hrXml = m_pXMLDomPtr.CreateInstance(__uuidof(MSXML2::DOMDocument30/*DOMDocument30*/));
        if (SUCCEEDED(hrXml))
            LogFunc(ELL_DET, 0, L"Succeed to create MSXNL 6 instance.");
        else
        {
            LogFunc(ELL_DET, hrXml, L"Failed to create MSXML 6 instance.");
#if 0
            if(hrXml != REGDB_E_CLASSNOTREG)
            {
                m_LastXMLErr.Clear();
                m_LastXMLErr.m_lErrCode = (long)hrXml;
                return hrXml;
            }

            //ZZ: We will not try msxml4.dll, some times this will cause CreateInstance return 0x80010005
            hrXml = m_pXMLDomPtr.CreateInstance(__uuidof(MSXML2::DOMDocument30));
            if(SUCCEEDED(hrXml))
                break;

            if(hrXml != REGDB_E_CLASSNOTREG)
            {
                m_LastXMLErr.Clear();
                m_LastXMLErr.m_lErrCode = (long)hrXml;
                return hrXml;
            }
#endif
            hrXml = m_pXMLDomPtr.CreateInstance(__uuidof(MSXML2::DOMDocument30));
            if(FAILED(hrXml))
            {
                m_LastXMLErr.Clear();
                m_LastXMLErr.m_lErrCode = (long)hrXml;
                LogFunc(ELL_ERR, hrXml, L"Failed to create MSXML 3 & 6 instance.");
                return hrXml;
            }
            LogFunc(ELL_DET, 0, L"Succeed to create MSXML 3 instance.");
        }

        // Make XML file source has the correct format.
        m_pXMLDomPtr->preserveWhiteSpace = VARIANT_TRUE;

        m_pXMLDomPtr->put_async(VARIANT_FALSE);

        m_pXMLDomPtr->put_validateOnParse(VARIANT_FALSE);

        m_pXMLDomPtr->put_resolveExternals(VARIANT_FALSE);

        if((m_iCreateMode & EXMLCM_WRITE) || (m_iCreateMode & EXMLCM_APPEND))
        {
            m_pXMLDomPtr->put_preserveWhiteSpace(VARIANT_TRUE);
            if(bWriteHeader)
                WriteXMLHeader();
        }
        if(m_iCreateMode & EXMLCM_READ)
        {
            if(m_iCreateMode & EXMLCM_FILE)
            {
                if(m_iCreateMode & EXMLCM_CACHE_CONTENT)
                { 
                    RemoveUnicodeFlag(m_wsXmlContent, m_wsXmlContent.c_str());
                    VARIANT_BOOL varbRet = m_pXMLDomPtr->loadXML(m_wsXmlContent.c_str());
                    if(varbRet == VARIANT_FALSE)
                    {
                        GatherErrorInformation();
                        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, L"Failed to load XML content. (%s)", WS_S(m_LastXMLErr.m_wsErrReason));
                        return m_LastXMLErr.m_lErrCode;
                    }
                }
                else
                {
                    XMLVARIANT varFileName(m_wsXmlFileName.c_str());
                    VARIANT_BOOL varbRet = m_pXMLDomPtr->load(varFileName.at());
                    if(varbRet == VARIANT_FALSE)
                    {
                        GatherErrorInformation();
                        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, L"Failed to load XML file. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WS_S(m_wsXmlFileName));
                        return m_LastXMLErr.m_lErrCode;
                    }
                }
            }
            else if(m_iCreateMode & EXMLCM_CONTENT)
            {
                RemoveUnicodeFlag(m_wsXmlContent, m_wsXmlContent.c_str());
                VARIANT_BOOL varbRet = m_pXMLDomPtr->loadXML(m_wsXmlContent.c_str());
                if(varbRet == VARIANT_FALSE)
                {
                    GatherErrorInformation();
                    LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, L"Failed to load XML content. (%s)", WS_S(m_LastXMLErr.m_wsErrReason));
                    return m_LastXMLErr.m_lErrCode;
                }
            }
        }

        m_bIsMSXNLCreated = true;
        m_LastXMLErr.Clear();
    } while(0);

    return 0;
}

void
CXMLFileMgr::ReleaseXML()
{
    if (!IsBadCodePtr((FARPROC)(MSXML2::IXMLDOMDocument2*)m_pXMLDomPtr.GetInterfacePtr()))
        m_pXMLDomPtr = NULL;
    else
        m_pXMLDomPtr.Detach();

    m_bIsMSXNLCreated = false;
}

long 
CXMLFileMgr::GetXMLContent(WCHAR** ppwzXMLContentBuf, DWORD* pdwXMLContentBufSize)
{
    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    _bstr_t bstrXMLContent;
    HRESULT hrXML = m_pXMLDomPtr->get_xml(bstrXMLContent.GetAddress());
    if(FAILED(hrXML))
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to get XML content. (%s)", WS_S(m_LastXMLErr.m_wsErrReason));
        return m_LastXMLErr.m_lErrCode;
    }
    *pdwXMLContentBufSize = bstrXMLContent.length();
    *ppwzXMLContentBuf = new wchar_t[bstrXMLContent.length() + 1];

    wcsncpy_s(*ppwzXMLContentBuf, bstrXMLContent.length() + 1, bstrXMLContent, _TRUNCATE);
    m_LastXMLErr.Clear();

    return 0;
}

long
CXMLFileMgr::GetXMLContent(wstring& wsXMLContent)
{
    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    _bstr_t bstrXMLContent;
    HRESULT hrXML = m_pXMLDomPtr->get_xml(bstrXMLContent.GetAddress());
    if(FAILED(hrXML))
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to get XML content. (%s)", WS_S(m_LastXMLErr.m_wsErrReason));
        return m_LastXMLErr.m_lErrCode;
    }
    wsXMLContent = bstrXMLContent;
    m_LastXMLErr.Clear();

    return 0;
}

long 
CXMLFileMgr::WriteComment(MSXML2::IXMLDOMElementPtr pNodePtr, const WCHAR* pwzComment)
{
    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    if(NULL == pwzComment)
        return -1;

    MSXML2::IXMLDOMCommentPtr pNodeCommentPtr = m_pXMLDomPtr->createComment((_bstr_t)pwzComment);
    if(NULL == pNodeCommentPtr)
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to create comment. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzComment));
        return m_LastXMLErr.m_lErrCode;
    }

    MSXML2::IXMLDOMNodePtr pXMLRetPtr = NULL;
    pXMLRetPtr = pNodePtr->appendChild(pNodeCommentPtr);
    if(NULL == pXMLRetPtr)
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to get add comment child. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzComment));
        return m_LastXMLErr.m_lErrCode;
    }

    m_LastXMLErr.Clear();

    return 0;
}

long 
CXMLFileMgr::WriteNode(MSXML2::IXMLDOMElementPtr pNodePtr, const WCHAR* pwzNodeTag, MSXML2::IXMLDOMElementPtr& pNewNodePtr)
{
    if(NULL == pwzNodeTag)
        return D2DCOMM_E_INVALID_PARAM;

    pNewNodePtr = m_pXMLDomPtr->createElement((_bstr_t)pwzNodeTag);
    if(NULL == pNewNodePtr)
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to create element. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzNodeTag));
        return m_LastXMLErr.m_lErrCode;
    }

    MSXML2::IXMLDOMNodePtr pXMLRetPtr = NULL;
    if(NULL == pNodePtr)
        pXMLRetPtr = m_pXMLDomPtr->appendChild(pNewNodePtr);
    else
        pXMLRetPtr = pNodePtr->appendChild(pNewNodePtr);
    if(NULL == pXMLRetPtr)
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to add child element. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzNodeTag));
        return m_LastXMLErr.m_lErrCode;
    }

    m_LastXMLErr.Clear();

    return 0;
}

long
CXMLFileMgr::WriterNodeText(MSXML2::IXMLDOMElementPtr pNodePtr, const WCHAR* pwzNodeContent)
{
    if(NULL == pwzNodeContent)
        return D2DCOMM_E_INVALID_PARAM;

    HRESULT hr = 0;
    IXMLDOMText *pText = NULL;
    IXMLDOMNode *pNode2 = NULL;

    MSXML2::IXMLDOMTextPtr pNodeTextPtr = m_pXMLDomPtr->createTextNode((_bstr_t)pwzNodeContent);
    if(NULL == pNodeTextPtr)
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to create text node. (%s)", WS_S(m_LastXMLErr.m_wsErrReason));
        return m_LastXMLErr.m_lErrCode;
    }

    MSXML2::IXMLDOMNodePtr pXMLRetPtr = NULL;
    pXMLRetPtr = pNodePtr->appendChild(pNodeTextPtr);
    if(NULL == pXMLRetPtr)
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to append child node. (%s)", WS_S(m_LastXMLErr.m_wsErrReason));
        return m_LastXMLErr.m_lErrCode;
    }

    m_LastXMLErr.Clear();

    return 0;
}

long 
CXMLFileMgr::WriteNodeAttribute(MSXML2::IXMLDOMElementPtr pNodePtr, const WCHAR* pwzAttriName, const WCHAR* pwzAttriStr)
{
    if ((NULL == pwzAttriName) || (NULL == pNodePtr))
        return D2DCOMM_E_INVALID_PARAM;

    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    HRESULT hrXML = 0;
    hrXML = pNodePtr->setAttribute((_bstr_t)pwzAttriName, (NULL == pwzAttriStr) ? (_bstr_t)"" : (_bstr_t)pwzAttriStr);
    if(FAILED(hrXML))
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to set attribute. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzAttriName));
        return m_LastXMLErr.m_lErrCode;
    }

    m_LastXMLErr.Clear();

    return 0;
}

long 
CXMLFileMgr::WriteNodeAttribute(MSXML2::IXMLDOMElementPtr pNodePtr, const WCHAR* pwzAttriName, DWORD dwAttriValue)
{
    if ((NULL == pwzAttriName) || (NULL == pNodePtr))
        return D2DCOMM_E_INVALID_PARAM;

    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    XMLVARIANT varAttri(dwAttriValue);
    HRESULT hrXML = 0;
    hrXML = pNodePtr->setAttribute((_bstr_t)pwzAttriName, varAttri.at());
    if(FAILED(hrXML))
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to set attribute. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzAttriName));
        return m_LastXMLErr.m_lErrCode;
    }

    m_LastXMLErr.Clear();

    return 0;
}

long
CXMLFileMgr::WriteNodeAttribute(MSXML2::IXMLDOMElementPtr pNodePtr, const WCHAR* pwzAttriName, ULONGLONG ullAttriValue)
{
    if ((NULL == pwzAttriName) || (NULL == pNodePtr))
        return D2DCOMM_E_INVALID_PARAM;

    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    XMLVARIANT varAttri(ullAttriValue);
    HRESULT hrXML = 0;
    hrXML = pNodePtr->setAttribute((_bstr_t)pwzAttriName, varAttri.at());
    if(FAILED(hrXML))
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to set attribute. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzAttriName));
        return m_LastXMLErr.m_lErrCode;
    }

    m_LastXMLErr.Clear();

    return 0;
}

long 
CXMLFileMgr::WriteNodeEnd(MSXML2::IXMLDOMElementPtr pNodePtr)
{
    m_LastXMLErr.Clear();
    return 0;
}

long 
CXMLFileMgr::WriteXMLHeader()
{
    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    MSXML2::IXMLDOMProcessingInstructionPtr pInstructionPtr;
    pInstructionPtr = m_pXMLDomPtr->createProcessingInstruction(L"xml", L"version='1.0' encoding='UTF-8'");
    if(NULL == pInstructionPtr)
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to create process instruction. (%s)", WS_S(m_LastXMLErr.m_wsErrReason));
        return m_LastXMLErr.m_lErrCode;
    }

    MSXML2::IXMLDOMNodePtr pXMLRetPtr = NULL;
    pXMLRetPtr = m_pXMLDomPtr->appendChild(pInstructionPtr);

    if(NULL == pXMLRetPtr)
    {
        GatherErrorInformation();
        return m_LastXMLErr.m_lErrCode;
    }	

    m_LastXMLErr.Clear();

    return 0;
}

long 
CXMLFileMgr::ReadNode(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzNodeTag, MSXML2::IXMLDOMNodePtr& pNodeFoundPtr)
{
    if ((NULL == pwzNodeTag) || (NULL == pNodePtr))
        return D2DCOMM_E_INVALID_PARAM;

    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    pNodeFoundPtr = NULL;

    if (NULL == pNodePtr)
        pNodeFoundPtr = m_pXMLDomPtr->selectSingleNode(pwzNodeTag);
    else
        pNodeFoundPtr = pNodePtr->selectSingleNode(pwzNodeTag);

    if (NULL == pNodeFoundPtr)
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to select single node. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzNodeTag));
        return m_LastXMLErr.m_lErrCode;
    }
    return 0;
}

long 
CXMLFileMgr::ReadNode(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzNodeTag, NodePtrVector& vecNodeFound)
{
    if ((NULL == pwzNodeTag) || (NULL == pNodePtr))
        return D2DCOMM_E_INVALID_PARAM;

    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    MSXML2::IXMLDOMNodeListPtr pNodeListPtr = NULL;
    if (NULL == pNodePtr)
        pNodeListPtr = m_pXMLDomPtr->selectNodes(pwzNodeTag);
    else
        pNodeListPtr = pNodePtr->selectNodes(pwzNodeTag);
    if (NULL == pNodeListPtr)
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to select multiple node. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzNodeTag));
        return m_LastXMLErr.m_lErrCode;
    }

    long lNodeCnt = pNodeListPtr->Getlength();
    if (0 != lNodeCnt)
    {
        for (long lIdx = 0; lIdx < lNodeCnt; lIdx++)
        {
            MSXML2::IXMLDOMNodePtr pCurNodePtr = pNodeListPtr->Getitem(lIdx);
            if (pCurNodePtr)
                vecNodeFound.push_back(pCurNodePtr);
        }
    }

    return 0;
}

long
CXMLFileMgr::ReadNodeText(MSXML2::IXMLDOMNodePtr pNodePtr, wstring& wsNodeContent)
{
    if ((NULL == pNodePtr) || (NULL == pNodePtr))
        return D2DCOMM_E_INVALID_PARAM;

    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    WCHAR* pwzNodeContent = pNodePtr->Gettext();
    if (pwzNodeContent)
        wsNodeContent = pwzNodeContent;

    return 0;
}

long
CXMLFileMgr::ReadNodeAttribute(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzAttriName, wstring& wsAttriStr)
{
    if((NULL == pNodePtr) || (NULL == pwzAttriName))
        return D2DCOMM_E_INVALID_PARAM;

    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    MSXML2::IXMLDOMNamedNodeMapPtr pAttributeMap = pNodePtr->Getattributes();
    if(NULL == pAttributeMap)
    {
        GatherErrorInformation();
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to get attribute. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzAttriName));
        return m_LastXMLErr.m_lErrCode;
    }

    m_LastXMLErr.Clear();

    MSXML2::IXMLDOMNodePtr pAttribNodePtr = pAttributeMap->getNamedItem(pwzAttriName);
    if(pAttribNodePtr)
    {
        VARIANT& varAttribVal = pAttribNodePtr->GetnodeValue();
        WCHAR* pwzAttribVal = (WCHAR*)varAttribVal.pbstrVal;
        if(pwzAttribVal)
            wsAttriStr = pwzAttribVal;
        else
            wsAttriStr.clear();
    }
    else
    {
        m_LastXMLErr.m_lErrCode = D2DCOMM_E_ITEM_NOT_FOUND;
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Not found attribute. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzAttriName));
        return D2DCOMM_E_ITEM_NOT_FOUND;
    }

    return 0;
}

long 
CXMLFileMgr::ReadNodeAttribute(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzAttriName, DWORD& dwAttriValue)
{
    wstring wsAttribVal;
    long lRetCode = ReadNodeAttribute(pNodePtr, pwzAttriName, wsAttribVal);
    if(lRetCode != 0)
        return lRetCode;

    dwAttriValue = wcstoul(wsAttribVal.c_str(), NULL, 10);

    m_LastXMLErr.Clear();

    return 0;
}

long 
CXMLFileMgr::ReadNodeAttribute(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzAttriName, ULONGLONG& ullAttriValue)
{
    wstring wsAttribVal;
    long lRetCode = ReadNodeAttribute(pNodePtr, pwzAttriName, wsAttribVal);
    if(lRetCode != 0)
        return lRetCode;

    ullAttriValue = _wcstoui64(wsAttribVal.c_str(), NULL, 10);

    m_LastXMLErr.Clear();

    return 0;
}

long 
CXMLFileMgr::ReadNodeAttribute(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzAttriName, LONGLONG& llAttriValue)
{
    wstring wsAttribVal;
    long lRetCode = ReadNodeAttribute(pNodePtr, pwzAttriName, wsAttribVal);
    if(lRetCode != 0)
        return lRetCode;

    llAttriValue = _wcstoi64(wsAttribVal.c_str(), NULL, 10);

    m_LastXMLErr.Clear();

    return 0;
}

long
CXMLFileMgr::ReadNodeAttribute(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzAttriName, bool& bAttriValue)
{
    DWORD dwAttribVal = 0;
    long lRetCode = ReadNodeAttribute(pNodePtr, pwzAttriName, dwAttribVal);
    if(lRetCode != 0)
        return lRetCode;

    bAttriValue = dwAttribVal ? true : false;

    m_LastXMLErr.Clear();

    return 0;
}

void
CXMLFileMgr::ReleaseXMLContent(const WCHAR* pwzXMLContent)
{
    if(pwzXMLContent)
        delete []pwzXMLContent;

    m_LastXMLErr.Clear();
}

long 
CXMLFileMgr::SaveXML(const WCHAR* pwzXMLFilelName /* = NULL */, int iCreateMode /* = EXMLCM_WRITE | EXMLCM_FILE */)
{
    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    if(m_pXMLDomPtr && (m_iCreateMode & EXMLCM_WRITE) && (m_iCreateMode & EXMLCM_FILE))
    {
        XMLVARIANT varFileName(pwzXMLFilelName);
        HRESULT hrXML = m_pXMLDomPtr->save(varFileName.at());
        if(FAILED(hrXML))
        {
            GatherErrorInformation();
            LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to save XML content. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzXMLFilelName));
            return m_LastXMLErr.m_lErrCode;
        }
    }

    m_LastXMLErr.Clear();

    return 0;
}

long
CXMLFileMgr::GetLastXMLError(MSXML2::IXMLDOMDocumentPtr pDocPtr, CXMLError& XMLErr)
{
    if(NULL == pDocPtr)
        return D2DCOMM_E_INVALID_PARAM;

    XMLErr.Clear();

    MSXML2::IXMLDOMParseErrorPtr pParseErrorPtr = NULL;
    HRESULT hrRet = pDocPtr->get_parseError(&pParseErrorPtr);
    if(FAILED(hrRet))
    {    
        return hrRet;
    }

    XMLErr.m_wsErrReason = pParseErrorPtr->Getreason();
    XMLErr.m_wsErrUrl = pParseErrorPtr->Geturl();
    XMLErr.m_wsSrcText = pParseErrorPtr->GetsrcText();
    XMLErr.m_lErrCode = pParseErrorPtr->GeterrorCode();
    XMLErr.m_lErrLine = pParseErrorPtr->Getline();
    XMLErr.m_lLinePos = pParseErrorPtr->Getlinepos();
    XMLErr.m_lFilePos = pParseErrorPtr->Getfilepos();

    return 0;
}

long
CXMLFileMgr::GatherErrorInformation()
{
    if (!m_bIsMSXNLCreated)
        return D2DCOMM_E_POINTER_OBJECT_NOT_INIT;

    MSXML2::IXMLDOMParseErrorPtr pParseErrorPtr = NULL;
    HRESULT hrRet = m_pXMLDomPtr->get_parseError(&pParseErrorPtr);
    if(FAILED(hrRet))
    {
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to get XML error.");
        return hrRet;
    }

    if (pParseErrorPtr)
    {
        m_LastXMLErr.Clear();

        if (pParseErrorPtr->Getreason().GetBSTR())
            m_LastXMLErr.m_wsErrReason = pParseErrorPtr->Getreason();
        if (pParseErrorPtr->Geturl().GetBSTR())
            m_LastXMLErr.m_wsErrUrl = pParseErrorPtr->Geturl();
        if (pParseErrorPtr->GetsrcText().GetBSTR())
            m_LastXMLErr.m_wsSrcText = pParseErrorPtr->GetsrcText();
        m_LastXMLErr.m_lErrCode = pParseErrorPtr->GeterrorCode();
        m_LastXMLErr.m_lErrLine = pParseErrorPtr->Getline();
        m_LastXMLErr.m_lLinePos = pParseErrorPtr->Getlinepos();
        m_LastXMLErr.m_lFilePos = pParseErrorPtr->Getfilepos();
    }

    return 0;
}

long
CXMLFileMgr::CacheXMLContent(const WCHAR* pwzXMLFilePath, wstring& wsXMLContent)
{
    if(NULL == pwzXMLFilePath)
    {
        m_LastXMLErr.Clear();
        m_LastXMLErr.m_lErrCode = D2DCOMM_E_INVALID_PARAM;
        return D2DCOMM_E_INVALID_PARAM;
    }

    long lRetCode = 0;
    FILE* hXMLFile = NULL;
    errno_t errXMLFile = _wfopen_s(&hXMLFile, pwzXMLFilePath, L"r");
    if(0 != errXMLFile)
    {
        m_LastXMLErr.Clear();
        m_LastXMLErr.m_lErrCode = errXMLFile;
        LogFunc(ELL_ERR, m_LastXMLErr.m_lErrCode, TEXT(__FUNCTION__) L"Failed to read XML content from file. (%s|%s)", WS_S(m_LastXMLErr.m_wsErrReason), WZ_S(pwzXMLFilePath));
        return errXMLFile;
    }

    fseek(hXMLFile, 0, SEEK_END);
    long lFileSize = ftell(hXMLFile);
    rewind(hXMLFile);
    long lWCharCnt = lFileSize / sizeof(WCHAR) + 1;
    WCHAR* pwzXMLContent = new WCHAR[lWCharCnt];
    memset(pwzXMLContent, 0, lWCharCnt * sizeof(WCHAR));
    if(0 != fread_s(pwzXMLContent, lFileSize, 1, lFileSize, hXMLFile))
        wsXMLContent = pwzXMLContent;
    else
        lRetCode = GetLastError();
    delete []pwzXMLContent;
    fclose(hXMLFile);

    m_LastXMLErr.Clear();
    if (0 != lRetCode)
        m_LastXMLErr.m_lErrCode = lRetCode;

    return lRetCode;
}

long
CXMLFileMgr::RemoveUnicodeFlag(wstring& wsContent, const WCHAR* pwzContent)
{
    if (pwzContent)
    {
        DWORD dwContentLen = (DWORD)wcslen(pwzContent);
        DWORD dwContentSize = (DWORD)((dwContentLen + 1) * sizeof(WCHAR));

        if (dwContentSize <= 4)
            wsContent = (WCHAR*)pwzContent;
        else
        {
            USHORT usUnicodeFlag = 0xFEFF;
            USHORT usUtf8Flag = 0xFEFF;
            BYTE* pbContent = (BYTE*)(const_cast<WCHAR*>(pwzContent));
            if (0 == memcmp(pbContent, &usUnicodeFlag, sizeof(usUnicodeFlag)))   
            {
                pbContent += sizeof(usUnicodeFlag);
                wsContent = (WCHAR*)pbContent;
            }
            else
                wsContent = (WCHAR*)pbContent;
        }
    }
    return 0;
}
