#pragma once

#define SML_STRING_ANY(sml_any) # sml_any
#define SML_EXPAND_MACRO(sml_macro) SML_STRING_ANY(sml_macro)


#define SML_STRING_ANY_W(sml_any) L ## # sml_any
#define SML_EXPAND_MACRO_W(sml_macro) SML_STRING_ANY_W(sml_macro)





#if 0
#define SML_MAKE_STRING(x) L ## # x
#define SML_EXPAND_MACRO(x) SML_MAKE_STRING(x)
#endif



#if 0
#define STR_MACRO_BARE(x) # x
#define SAME_MACRO_BARE(x) x
#define EXPAND_MACRO_BARE(macro) SAME_MACRO_BARE(STR_MACRO_BARE(macro))

#define STR_MACRO(x) (# x)
#define SAME_MACRO(x) (x)
#define EXPAND_MACRO(macro) (SAME_MACRO(STR_MACRO(macro)))
#endif


#if 0
#define AFX_INTERFACEMAP AFX_INTERFACEMAP_ZZZZZZZZZZZZZZ
#define AFX_INTERFACEMAP_ENTRY AFX_INTERFACEMAP_ENTRY_ZZZZZZZZZZZZZZ

#define HELLO_BEGIN_INTERFACE_MAP(theClass, theBase) \
	const AFX_INTERFACEMAP* PASCAL theClass::GetThisInterfaceMap() \
		{ return &theClass::interfaceMap; } \
	const AFX_INTERFACEMAP* theClass::GetInterfaceMap() const \
		{ return &theClass::interfaceMap; } \
	AFX_COMDAT const AFX_INTERFACEMAP theClass::interfaceMap = \
		{ &theBase::GetThisInterfaceMap, &theClass::_interfaceEntries[0], }; \
	AFX_COMDAT const AFX_INTERFACEMAP_ENTRY theClass::_interfaceEntries[] = \
	{ \


int _tmain_test_expand_macro()
{
	CStringA str = EXPAND_MACRO(HELLO_BEGIN_INTERFACE_MAP(theClass, theBase));
	str += "\r\n ////";
	const char * str1 = EXPAND_MACRO_BARE(HELLO_BEGIN_INTERFACE_MAP(theClass, theBase)) "\r\n ////"; //illegal if using EXPAND_MACRO
	ATLASSERT(0 == _stricmp(str.GetString(), str1));
	printf_s("%s", str.GetString());
}
#endif