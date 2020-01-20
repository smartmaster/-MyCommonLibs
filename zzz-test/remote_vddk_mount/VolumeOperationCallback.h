#pragma once


#include "..\..\PublicHeader\PublicDTraceLib.h"
#include "..\..\PublicHeader\PublicCodeBlock.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////////
struct IVolumeOperationCallback
{
	virtual HRESULT Do(CONST vector<CString> & mountPoints, CONST vector<CString> & symbolLinks) = 0;
	virtual ~IVolumeOperationCallback(){}
};

//////////////////////////////////////////////////////////////////////////
class CVolumeChkdsk : public IVolumeOperationCallback
{
public:
	//struct IVolumeOperationCallback
	//{
	//	virtual HRESULT Do(CONST vector<CString> & mountPoints, CONST vector<CString> & symbolLinks) = 0;
	//};
	virtual HRESULT Do(CONST vector<CString> & mountPoints, CONST vector<CString> & symbolLinks);

	//////////////////////////////////////////////////////////////////////////
	static HRESULT CHKDSK(LPCTSTR volumePath);
};