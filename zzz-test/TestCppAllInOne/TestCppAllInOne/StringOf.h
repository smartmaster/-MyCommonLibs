#pragma once

#define SML_WSTRING_OF(x) L ## #x
#define SML_STRING_OF(x) #x

#define  SML_EXPANDED_MACRO_STRING(x) SML_STRING_OF(x)
#define  SML_EXPANDED_MACRO_WSTRING(x) SML_WSTRING_OF(x)


