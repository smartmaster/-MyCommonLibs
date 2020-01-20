#pragma once

#import <msxml3.dll>

#include <Windows.h>
#include <atlbase.h>
#include <string>
#include <vector>
using namespace std;

typedef enum
{
    ELL_ERR = 0,
    ELL_WAR,
    ELL_INF,
    ELL_DET,
    ELL_DBG
}E_LOG_LVL;

enum _eXMLCreateMode
{
    EXMLCM_READ = 0x00000001,
    EXMLCM_WRITE = 0x00000010,
    EXMLCM_APPEND = 0x00000100,
    EXMLCM_FILE = 0x00001000,
    EXMLCM_CONTENT = 0x00010000,
    EXMLCM_CACHE_CONTENT = 0x00100000  // Used for Unicode xml only. Read XMl file content to memory and then operate it in memory.
};

class XMLVARIANT
{
public:
    XMLVARIANT(const WCHAR* pwzString)
    {
        VariantInit(&m_varObj);
        V_BSTR(&m_varObj) = SysAllocString(pwzString);
        V_VT(&m_varObj) = VT_BSTR;
    }
    XMLVARIANT(unsigned long ulValue)
    {
        VariantInit(&m_varObj);
        V_VT(&m_varObj) = VT_UI4;
        V_UI4(&m_varObj) = ulValue;
    }
    XMLVARIANT(ULONGLONG ullValue)
    {
        VariantInit(&m_varObj);
        V_VT(&m_varObj) = VT_UI8;
        V_UI8(&m_varObj) = ullValue;
    }
    ~XMLVARIANT()
    {
        VariantClear(&m_varObj);
    }

    VARIANT& at()    { return m_varObj; }
    void     clear() { VariantClear(&m_varObj); }

private:
    VARIANT m_varObj;
};

class CXMLError
{
public:
    void Clear()
    {
        m_lErrCode = 0;
        m_lErrLine = 0;
        m_lLinePos = 0;
        m_lFilePos = 0;
        m_wsErrUrl.clear();
        m_wsErrReason.clear();
        m_wsSrcText.clear();    
    }

    long m_lErrCode;
    long m_lErrLine;
    long m_lLinePos;
    long m_lFilePos;
    wstring m_wsErrUrl;
    wstring m_wsErrReason;
    wstring m_wsSrcText;
};

typedef std::vector<MSXML2::IXMLDOMNodePtr> NodePtrVector;

class CXMLFileMgr
{
public:
    CXMLFileMgr(const WCHAR* pwzXMLNameOrContent = NULL, int iCreateMode = EXMLCM_WRITE | EXMLCM_CONTENT);
    virtual ~CXMLFileMgr();

public:
    long         CreateXML(const WCHAR* pwzXMLNameOrContent, int iCreateMode, bool bWriteHeader = true);
    void         ReleaseXML();
    virtual void LogFunc(DWORD dwLoglevel, DWORD dwRetCode, const WCHAR* pwzLogFormat, ...) {}

public:
    long         WriteComment(MSXML2::IXMLDOMElementPtr pNodePtr, const WCHAR* pwzComment);
    long         WriteNode(MSXML2::IXMLDOMElementPtr pNodePtr, const WCHAR* pwzNodeTag, MSXML2::IXMLDOMElementPtr& pNewNodePtr);
    long         WriterNodeText(MSXML2::IXMLDOMElementPtr pNodePtr, const WCHAR* pwzNodeContent);
    virtual long WriteNodeAttribute(MSXML2::IXMLDOMElementPtr pNodePtr, const WCHAR* pwzAttriName, const WCHAR* pwzAttriStr);
    virtual long WriteNodeAttribute(MSXML2::IXMLDOMElementPtr pNodePtr, const WCHAR* pwzAttriName, DWORD dwAttriValue);
    virtual long WriteNodeAttribute(MSXML2::IXMLDOMElementPtr pNodePtr, const WCHAR* pwzAttriName, ULONGLONG ullAttriValue);
    long         WriteNodeEnd(MSXML2::IXMLDOMElementPtr pNodePtr);
    long         WriteXMLHeader();

    virtual long ReadNode(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzNodeTag, MSXML2::IXMLDOMNodePtr& pNodeFoundPtr);
    virtual long ReadNode(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzNodeTag, NodePtrVector& vecNodeFound);
    long         ReadNodeText(MSXML2::IXMLDOMNodePtr pNodePtr, wstring& wsNodeContent);
    virtual long ReadNodeAttribute(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzAttriName, wstring& wsAttriStr);
    virtual long ReadNodeAttribute(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzAttriName, DWORD& dwAttriValue);
    virtual long ReadNodeAttribute(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzAttriName, ULONGLONG& ullAttriValue);
    virtual long ReadNodeAttribute(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzAttriName, LONGLONG& llAttriValue);
    virtual long ReadNodeAttribute(MSXML2::IXMLDOMNodePtr pNodePtr, const WCHAR* pwzAttriName, bool& bAttriValue);

    void         ReleaseXMLContent(const WCHAR* pwzXMLContent);
    long         SaveXML(const WCHAR* pwzXMLFilelName = NULL, int iCreateMode = EXMLCM_WRITE | EXMLCM_FILE);

    wstring&     XMLContent() { return m_wsXmlContent; }
    MSXML2::IXMLDOMDocument2Ptr& XMLDomPtr() { return m_pXMLDomPtr; }

public:
    CXMLError&   GetLastXMLError() { return m_LastXMLErr; }
    virtual long GetXMLContent(WCHAR** ppwzXMLContentBuf, DWORD* pdwXMLContentBufSize);
    virtual long GetXMLContent(wstring& wsXMLContent);

    static long  GetLastXMLError(MSXML2::IXMLDOMDocumentPtr pDocPtr, CXMLError& XMLErr);

protected:
    long         GatherErrorInformation();
    long         CacheXMLContent(const WCHAR* pwzXMLFilePath, wstring& wsXMLContent);
    long         RemoveUnicodeFlag(wstring& wsContent, const WCHAR* pwzContent);

protected:
    MSXML2::IXMLDOMDocument2Ptr m_pXMLDomPtr;
    wstring                     m_wsXmlFileName;
    wstring                     m_wsXmlContent;
    bool                        m_bCoInitialized;
    bool                        m_bIsMSXNLCreated;
    int                         m_iCreateMode;
    CXMLError                   m_LastXMLErr;
};