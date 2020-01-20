#pragma once


// General includes
#include <windows.h>
#include <winbase.h>

#include <iostream>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

// ATL includes
#pragma warning( disable: 4189 )    // disable local variable is initialized but not referenced
#include <atlbase.h>

// STL includes
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <fstream>
using namespace std;   

// Used for safe string manipulation
#include <strsafe.h>

#include "..\PublicHeader\PublicDTraceLib.h"

// _VSHADOW_ASSERTE declaration (used by ATL) and other macros
#include "macros.h"
#include "shadow.h"


