========================================================================
    CONSOLE APPLICATION : AutoInputKey Project Overview
========================================================================

AppWizard has created this AutoInputKey application for you.

This file contains a summary of what you will find in each of the files that
make up your AutoInputKey application.


AutoInputKey.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

AutoInputKey.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

AutoInputKey.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named AutoInputKey.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
HWND WINAPI FindWindowEx(
  _In_opt_ HWND    hwndParent,
  _In_opt_ HWND    hwndChildAfter,
  _In_opt_ LPCTSTR lpszClass,
  _In_opt_ LPCTSTR lpszWindow
);



Ñ¸À×7µÇÂ¼
XLUEModalHostWnd


BOOL WINAPI ShowWindow(
  _In_ HWND hWnd,
  _In_ int  nCmdShow
);




BOOL WINAPI BringWindowToTop(
  _In_ HWND hWnd
);




BOOL WINAPI SetForegroundWindow(
  _In_ HWND hWnd
);



UINT WINAPI SendInput(
  _In_ UINT    nInputs,
  _In_ LPINPUT pInputs,
  _In_ int     cbSize
);

