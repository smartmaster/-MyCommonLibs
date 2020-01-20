#pragma once
#include "LIB_VHD_dynamicvhd.h"

class CDiffVHD :
	public CDynamicVHD
{
protected:
	std::wstring m_strParent;
	INT32 m_nParentDiskType;

public:
	enum
	{
		VHDType = 4,
	};

	CDiffVHD(VOID);
	~CDiffVHD(VOID);

	INT32 Create(LPCTSTR pszFilename, DWORD dwCreationDisposition, PBATELEMENT pGlobalBAT, PVHD_CREATEPARAM pParam=NULL);

	INT32 Close();
	static INT CompareFilenames(LPCTSTR pszFilename1, LPCTSTR pszFilename2, INT* pnBackSlashCount);

protected:
	INT GenParentLocator(LPCTSTR pszImageFilename, LPCTSTR pszParentFilename, ULONGLONG ullFirstCodePos, BYTE* pCodeBuf4KB);
	static BOOL GetParentFilename(LPSTR pszParentName, LPCSTR pszChildName);
	static BOOL GetParentFilenameW(LPWSTR pszParentName, LPCTSTR pszChildName);
	static UINT32 ConvFileTimeToTimeStamp(FILETIME* pTime);

public:
	inline INT32 GetParentDiskType() CONST {return m_nParentDiskType;};
	inline LPCTSTR GetParentDiskName() CONST{return m_strParent.c_str();};
};
