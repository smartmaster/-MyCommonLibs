#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "EnumElement.h"

class  CVCProjectDisableOptimazationHandler : public IEnumElementHandler
{
public:
	//struct IEnumElementHandler
	//{
	virtual BOOL Compare(CONST XmlElement & xmlElement);
	virtual HRESULT Operate(XmlElement & xmlElement);
	//};
};

HRESULT VcxprojDisableOpt(LPCTSTR pSourceFile, LPCTSTR pNewFile);
HRESULT VcxprojDisableOptRecurively(LPCTSTR pRootFolder);