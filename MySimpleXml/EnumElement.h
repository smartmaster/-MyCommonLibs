#pragma once

#include "SimpleXml.h"

//////////////////////////////////////////////////////////////////////////
struct IEnumElementHandler 
{
	virtual BOOL Compare(CONST XmlElement & xmlElement) = 0;
	virtual HRESULT Operate(XmlElement & xmlElement) = 0;
};


VOID EnumXmlElement(XmlElement & RootElement, IEnumElementHandler * pHandler);
