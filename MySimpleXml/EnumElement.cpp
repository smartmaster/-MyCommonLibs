#include "stdafx.h"

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "EnumElement.h"


VOID EnumXmlElement(XmlElement & RootElement, IEnumElementHandler * pHandler)
{
	HRESULT hr = S_OK;

	if (pHandler->Compare(RootElement))
	{
		pHandler->Operate(RootElement);
	}

	for (size_t ii = 0; ii < RootElement.m_Children.size(); ++ii)
	{
		EnumXmlElement(RootElement.m_Children[ii], pHandler);
	}
}




