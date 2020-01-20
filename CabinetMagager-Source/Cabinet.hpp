
#pragma once

#include <windows.h>
#include <stdio.h>
#include <conio.h>         // getch()
//#include "resource.h"
//#include "Cabinet/ExtractResource.hpp"
//#include "Cabinet/ExtractUrl.hpp"
//#include "Cabinet/Compress.hpp"

#include "ExtractResource.hpp"
#include "ExtractUrl.hpp"
#include "Compress.hpp"


class CMain
{
#define CYAN   (FOREGROUND_GREEN | FOREGROUND_BLUE  | FOREGROUND_INTENSITY)
#define GREY   (FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define WHITE  (FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define GREEN  (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define RED    (FOREGROUND_RED   | FOREGROUND_INTENSITY)
#define YELLOW (FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_INTENSITY)

public:
	void RunDemo();
	static void PrintA(WORD u16_Color, char* s8_Format, ...);
	static void PrintW(WORD u16_Color, WCHAR* u16_Format, ...);

	// Extract callbacks
	static BOOL OnBeforeCopyFile(Cabinet::CExtract::kCabinetFileInfo *pk_Info, void* p_Param);
	static void OnAfterCopyFile(WCHAR* u16_Path, Cabinet::CMemory* pi_ExtractMem, void* p_Param);
	static void OnProgressInfo(Cabinet::CExtract::kProgressInfo* pk_Info, void* p_Param);
	static void OnCabinetInfo(Cabinet::CExtract::kCabinetInfo *pk_Info, void* p_Param);
	static void OnNextCabinet(Cabinet::CExtract::kCabinetInfo *pk_Info, int s32_Error, void* p_Param);

	// Compress callbacks
	static int  OnFilePlaced(PCCAB pccab, WCHAR* u16_File, int cbFile, BOOL fContinuation, void* p_Param);
	static BOOL OnGetNextCabinet(PCCAB pccab, ULONG cbPrevCab, char* s8_FormatString, void* p_Param);
	static int  OnUpdateStatus(UINT typeStatus, Cabinet::CCompress::kCurStatus *pk_CurStatus, void *p_Param);
};


int main_cabinet(int argc, TCHAR **argv);