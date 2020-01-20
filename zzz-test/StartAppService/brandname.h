#ifndef _BRANDNAME_H
#define _BRANDNAME_H

#ifndef NETWARE

#include <tchar.h>

#endif //NETWARE

#ifndef CST_PRODUCT
#define CST_PRODUCT

#define	CST_PRODUCT_REG_ROOT_A		"Arcserve\\Unified Data Protection\\Engine"
#define	CST_PRODUCT_REG_ROOT_T		_T( "Arcserve\\Unified Data Protection\\Engine" )
#define	CST_PRODUCT_REG_ROOT_L	 	L"Arcserve\\Unified Data Protection\\Engine"

#define	CST_PRODUCT_NAME_A	"arcserve Unified Data Protection"
#define	CST_PRODUCT_NAME_T	_T( "arcserve Unified Data Protection")
#define	CST_PRODUCT_NAME_L	L"arcserve Unified Data Protection"

#define CST_PRODUCT_NAME_AGENT_A "arcserve Unified Data Protection Agent"
#define CST_PRODUCT_NAME_AGENT_T _T("arcserve Unified Data Protection Agent")
#define CST_PRODUCT_NAME_AGENT_L L"arcserve Unified Data Protection Agent"

#define	CST_PRODUCT_REG_NAME_A	"Unified Data Protection\\Engine"
#define	CST_PRODUCT_REG_NAME_T	_T( "Unified Data Protection\\Engine")
#define	CST_PRODUCT_REG_NAME_L	L"Unified Data Protection\\Engine"

#define	CST_PRODUCT_FAMILY_NAME_A	"CA ARCserve D2D"
#define	CST_PRODUCT_FAMILY_NAME_T	_T( "CA ARCserve D2D" )
#define	CST_PRODUCT_FAMILY_NAME_L	L"CA ARCserve D2D"

#define CST_BRAND_NAME_A	"CA ARCserve D2D"
#define CST_BRAND_NAME_T	_T( "CA ARCserve D2D" )
#define CST_BRAND_NAME_L	L"CA ARCserve D2D"

#define CST_SHAREDCOMPONENT_NAME_A	"CA ARCserve D2D"
#define CST_SHAREDCOMPONENT_NAME_T	_T( "CA ARCserve D2D" )
#define CST_SHAREDCOMPONENT_NAME_L	L"CA ARCserve D2D"

#define CST_PRODUCTMGR_A	CST_BRAND_NAME_A "Mgr"
#define CST_PRODUCTMGR_T	CST_BRAND_NAME_T _T( "Mgr" )
#define CST_PRODUCTMGR_L	CST_BRAND_NAME_L L"Mgr"
#define CST_PRODUCTLOG_A	CST_BRAND_NAME_A ".log"
#define CST_PRODUCTLOG_T	CST_BRAND_NAME_T _T( ".log" )
#define CST_PRODUCTLOG_L	CST_BRAND_NAME_L L".log"
#define CST_PRODUCTHELP_A	CST_BRAND_NAME_A "Help"
#define CST_PRODUCTHELP_T	CST_BRAND_NAME_T _T( "Help" )
#define CST_PRODUCTHELP_L	CST_BRAND_NAME_L L"Help"

#define CST_REG_HEADER_A		"SOFTWARE\\Arcserve"
#define CST_REG_HEADER_T		_T( "SOFTWARE\\Arcserve" )
#define CST_REG_HEADER_L		L"SOFTWARE\\Arcserve"

#define CST_REG_HEADER_WOW6432_A	   "SOFTWARE\\Wow6432Node\\Arcserve"
#define CST_REG_HEADER_WOW6432_T		_T( "SOFTWARE\\Wow6432Node\\Arcserve" )
#define CST_REG_HEADER_WOW6432_L		L"SOFTWARE\\Wow6432Node\\Arcserve"

#define CST_REG_ROOT_A	CST_REG_HEADER_A "\\" CST_PRODUCT_REG_NAME_A
#define CST_REG_ROOT_T	CST_REG_HEADER_T _T("\\") CST_PRODUCT_REG_NAME_T
#define CST_REG_ROOT_L	CST_REG_HEADER_L L"\\" CST_PRODUCT_REG_NAME_L

#define	CST_BRAND_REG_ROOT_A		CST_REG_HEADER_A	"\\Unified Data Protection"
#define	CST_BRAND_REG_ROOT_T		CST_REG_HEADER_T	_T( "\\Unified Data Protection" )
#define	CST_BRAND_REG_ROOT_L	 	CST_REG_HEADER_L	L"\\Unified Data Protection"

#define	CST_PRODUCT_REG_NAME_CENTRAL_A	"Unified Data Protection\\Management"
#define	CST_PRODUCT_REG_NAME_CENTRAL_T	_T( "Unified Data Protection\\Management")
#define	CST_PRODUCT_REG_NAME_CENTRAL_L	L"Unified Data Protection\\Management"

#define CST_REG_ROOT_CENTRAL_A	CST_REG_HEADER_A "\\" CST_PRODUCT_REG_NAME_CENTRAL_A
#define CST_REG_ROOT_CENTRAL_T	CST_REG_HEADER_T _T("\\") CST_PRODUCT_REG_NAME_CENTRAL_T
#define CST_REG_ROOT_CENTRAL_L	CST_REG_HEADER_L L"\\" CST_PRODUCT_REG_NAME_CENTRAL_L

#ifndef NETWARE

#if defined(UNICODE) || defined( _UNICODE )

#define	CST_PRODUCT_NAME		CST_PRODUCT_NAME_L
#define CST_PRODUCT_FAMILY_NAME		CST_PRODUCT_FAMILY_NAME_L
#define CST_SHAREDCOMPONENT_NAME	CST_SHAREDCOMPONENT_NAME_L
#define CST_BRAND_NAME			CST_BRAND_NAME_L
#define CST_PRODUCTMGR			CST_PRODUCTMGR_L
#define CST_PRODUCTLOG			CST_PRODUCTLOG_L
#define CST_PRODUCTHELP			CST_PRODUCTHELP_L
#define CST_REG_HEADER			CST_REG_HEADER_L
#define CST_REG_ROOT			CST_REG_ROOT_L
#define	CST_BRAND_REG_ROOT		CST_BRAND_REG_ROOT_L

#else

#define	CST_PRODUCT_NAME		CST_PRODUCT_NAME_A
#define CST_PRODUCT_FAMILY_NAME		CST_PRODUCT_FAMILY_NAME_A
#define CST_SHAREDCOMPONENT_NAME	CST_SHAREDCOMPONENT_NAME_A
#define CST_BRAND_NAME			CST_BRAND_NAME_A
#define CST_PRODUCTMGR			CST_PRODUCTMGR_A
#define CST_PRODUCTLOG			CST_PRODUCTLOG_A
#define CST_PRODUCTHELP			CST_PRODUCTHELP_A
#define CST_REG_HEADER			CST_REG_HEADER_A
#define CST_REG_ROOT			CST_REG_ROOT_A
#define	CST_BRAND_REG_ROOT		CST_BRAND_REG_ROOT_A

#endif
#endif // NETWARE
#endif

#ifndef NETWARE

#ifdef __cplusplus
  extern "C" {                     /* avoid name-mangling if used from C++ */
#endif /* __cplusplus */

#ifndef PTCHAR
typedef TCHAR *PTCHAR;
#endif

#define BRAND_LONG_NAME_STRING_ID			201
#define BRAND_MID_NAME_STRING_ID			202
#define BRAND_SHORT_NAME_STRING_ID			203
#define	BRAND_BASE_NAME_STRING_ID			204
#define	BRAND_MGREXE_NAME_STRING_ID			205
#define	BRAND_LOGFILE_NAME_STRING_ID		206

#ifdef _TBYTES
#undef _TBYTES
#endif
#ifdef _TCHARS
#undef _TCHARS
#endif

#if     defined(UNICODE)
#define _TBYTES(s)      (lstrlen(s) * sizeof(TCHAR))
#define _TCHARS(s)      (sizeof(s) / sizeof(TCHAR))
#else
#define _TBYTES(s)      strlen(s)
#define _TCHARS(s)      sizeof(s)
#endif

#if     defined(UNICODE)
#define GetBrandLongName			GetBrandLongNameW
#define GetBrandMidName				GetBrandMidNameW
#define GetBrandShortName			GetBrandShortNameW
#define GetBrandBaseName			GetBrandBaseNameW
#define GetBrandMgrExeName			GetBrandMgrExeNameW
#define	GetBrandLogFileName			GetBrandLogFileNameW
#else
#define GetBrandLongName			GetBrandLongNameA
#define GetBrandMidName				GetBrandMidNameA
#define GetBrandShortName			GetBrandShortNameA
#define GetBrandBaseName			GetBrandBaseNameA
#define	GetBrandMgrExeName			GetBrandMgrExeNameA
#define	GetBrandLogFileName			GetBrandLogFileNameA
#endif

USHORT WINAPI GetBrandLongName(PTCHAR name, USHORT limit);
USHORT WINAPI GetBrandMidName(PTCHAR name, USHORT limit);
USHORT WINAPI GetBrandShortName(PTCHAR name, USHORT limit);
USHORT WINAPI GetBrandBaseName(PTCHAR name, USHORT limit);
USHORT WINAPI GetBrandMgrExeName(PTCHAR name, USHORT limit);
USHORT WINAPI GetBrandLogFileName(PTCHAR name, USHORT limit);

DWORD	pdtGetStringData( UINT id, LPSTR type, LPSTR text, USHORT limit);

#ifdef __cplusplus
  }
#endif /* __cplusplus */

#endif //NETWARE

#endif //_BRANDNAME_H
