
#include "stdafx.h"
#include "resource.h"
#include "Cabinet.hpp"


using namespace Cabinet;

int main_cabinet(int argc, TCHAR **argv)
{
#if _TraceExtract || _TraceCompress
	Cabinet::CTrace::TraceW(L"Entering main()");
#endif

	try
	{
		CMain i_Main;
		i_Main.RunDemo();
	}
	catch (const char* s8_Msg)
	{
		CStrW sw_Msg;
		sw_Msg.ToUnicode(1252, s8_Msg); // ANSII -> Unicode
		CMain::PrintW(RED, L"Exception: %s", (WCHAR*)sw_Msg);
		return E_FAIL;
	}

#if _TraceExtract || _TraceCompress
	Cabinet::CTrace::TraceW(L"Exiting main()");
#endif

	return 0;
}

void CMain::RunDemo()
{
	// This is an ID which is written (invisible) into the CAB file
	UINT u32_CabID = 12345;

	// Store filetimes as UTC in CAB files while compression
	BOOL b_StoreUtcTime = TRUE;

	// TRUE  -> If a filename has characters > 0x7F, encode it using UTF8
	// FALSE -> Store ANSI filenames unchanged, read documentation!
	BOOL b_EncodeUtf = TRUE;

	// Change this to split the archive into multiple files (200000 --> CAB split size = 200kB)
	// ATTENTION: In this case Parameter 1 of CreateFCIContext MUST contain "%d" or "%03u"...
	UINT u32_SplitSize = 0x7FFFFFFF;

	// Compression algorithm = NONE, MSZIP, LZX
	CCompress::eCompress e_Compress = CCompress::E_ComprLZX;

	// TRUE -> don't write any file to disk, all files are passed in OnAfterCopyFile()
	BOOL b_ExtractToMemory = FALSE;

	// You can specify your own key for CAB encryption here (the longer the more secure, up to 72 characters or bytes)
	// Set empty EncryptionKey ("") to get a CAB file without encryption
	char* s8_EncryptionKey = "";
	//	char* s8_EncryptionKey = "AH%KJ/76?KßHÖ\\Äüdghf7(ZTbjsf82iz<sx87qc5ba&m;-@^l#jeL9/)$D2@rTbZ<?´}2&";

	// ########################## Initialization ############################

	WCHAR u16_WinDir[MAX_PATH];
	GetWindowsDirectoryW(u16_WinDir, MAX_PATH);

	WCHAR u16_Explorer[MAX_PATH];
	WCHAR u16_Notepad[MAX_PATH];
	swprintf_s(u16_Explorer, L"%s\\Explorer.exe", u16_WinDir);
	swprintf_s(u16_Notepad, L"%s\\System32\\Notepad.exe", u16_WinDir);

	WCHAR u16_WorkDir[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, u16_WorkDir);

	WCHAR u16_CompressDir[MAX_PATH];
	WCHAR u16_ExtractFile[MAX_PATH];
	WCHAR u16_ExtractRsrc[MAX_PATH];
	WCHAR u16_ExtractUrl[MAX_PATH];
	swprintf_s(u16_CompressDir, L"%s\\__Compressed", u16_WorkDir);
	swprintf_s(u16_ExtractFile, L"%s\\__ExtractFile", u16_WorkDir);
	swprintf_s(u16_ExtractRsrc, L"%s\\__ExtractResource", u16_WorkDir);
	swprintf_s(u16_ExtractUrl, L"%s\\__ExtractUrl", u16_WorkDir);

	WCHAR  u16_CompressFile[MAX_PATH];
	wcscpy_s(u16_CompressFile, u16_CompressDir);
	wcscat_s(u16_CompressFile, L"\\Packed_%02d.cab"); // "Packed_01.cab", "Packed_02.cab", "Packed_03.cab",...

	CCompress        i_Compress;
	CExtract         i_ExtrFile;
	CExtractResource i_ExtrResource;
	CExtractUrl      i_ExtrUrl;

	i_Compress.SetEncryptionKey(s8_EncryptionKey, (DWORD)strlen(s8_EncryptionKey));
	i_ExtrFile.SetDecryptionKey(s8_EncryptionKey, (DWORD)strlen(s8_EncryptionKey));

	// set the optional extraction callbacks (only static functions)
	CExtract::kCallbacks k_ExtrCallbacks;
	k_ExtrCallbacks.f_OnBeforeCopyFile = &CMain::OnBeforeCopyFile;
	k_ExtrCallbacks.f_OnAfterCopyFile = &CMain::OnAfterCopyFile;
	k_ExtrCallbacks.f_OnProgressInfo = &CMain::OnProgressInfo;
	k_ExtrCallbacks.f_OnCabinetInfo = &CMain::OnCabinetInfo;
	k_ExtrCallbacks.f_OnNextCabinet = &CMain::OnNextCabinet;

	i_ExtrFile.SetCallbacks(&k_ExtrCallbacks);
	i_ExtrResource.SetCallbacks(&k_ExtrCallbacks);
	i_ExtrUrl.SetCallbacks(&k_ExtrCallbacks);

	// set the optional compression callbacks (only static functions)
	CCompress::kCallbacks k_ComprCallbacks;
	k_ComprCallbacks.f_FilePlaced = &CMain::OnFilePlaced;
	k_ComprCallbacks.f_UpdateStatus = &CMain::OnUpdateStatus;
	k_ComprCallbacks.f_GetNextCabinet = &CMain::OnGetNextCabinet;

	i_Compress.SetCallbacks(&k_ComprCallbacks);

	CStrW s_Mode = L"Compiled as: ";
#ifdef UNICODE
	s_Mode += L"UNICODE, ";
#else 
	s_Mode += L"MBCS, ";
#endif
#ifdef _WIN64
	s_Mode += L"64 Bit";
#else 
	s_Mode += L"32 Bit";
#endif

	PrintW(CYAN, (WCHAR*)s_Mode);

	// ########################## Compress demo ############################

	// This will pack Explorer.exe and Notepad.exe into a CAB file with subfolders

	PrintW(WHITE, L"\n----------------------------------------------------------------");
	PrintW(YELLOW, L"\n                   CAB FILE COMPRESSION\n");

	if (!i_Compress.CreateFCIContextW(u16_CompressFile, b_StoreUtcTime, b_EncodeUtf, u32_SplitSize, u32_CabID))
	{
		PrintW(RED, L"Compress ERROR: Could not create FCI context:\n%s", i_Compress.LastErrorW());
		goto _RESOURCE;
	}

	if (!i_Compress.AddFileW(u16_Explorer, L"FileManager\\Explorer.exe", e_Compress))
	{
		PrintW(RED, L"Compress ERROR: Could not add Explorer.exe to cabinet:\n%s", i_Compress.LastErrorW());
		goto _RESOURCE;
	}

	if (!i_Compress.AddFileW(u16_Notepad, L"TextManager\\Notepad.exe", e_Compress))
	{
		PrintW(RED, L"Compress ERROR: Could not add Notepad.exe to cabinet:\n%s", i_Compress.LastErrorW());
		goto _RESOURCE;
	}

	if (!i_Compress.DestroyFCIContext())
	{
		PrintW(RED, L"Compress ERROR: Could not flush Cabinet:\n%s", i_Compress.LastErrorW());
		goto _RESOURCE;
	}

	PrintW(GREEN, L"\nSUCCESS: Compressed Explorer.exe and Notepad.exe into cabinet in\n\"%s\"", u16_CompressDir);


	// ########################## Extract file demo ############################

	// This will decrypt the CAB file if it was encrypted above

	PrintW(WHITE, L"\n----------------------------------------------------------------");
	PrintW(YELLOW, L"\n                   CAB FILE EXTRACTION\n");

	if (b_ExtractToMemory)
		wcscpy(u16_ExtractFile, L"MEMORY");

	if (!i_ExtrFile.CreateFDIContext())
	{
		PrintW(RED, L"Extract ERROR: Could not create FDI context:\n%s", i_ExtrFile.LastErrorW());
		goto _RESOURCE;
	}

	// replace "%02d" -> "01"
	swprintf_s(u16_CompressFile, u16_CompressFile, 1);

	FDICABINETINFO k_Info;
	if (i_ExtrFile.IsCabinetW(u16_CompressFile, &k_Info))
	{
		PrintW(WHITE, L"CAB filesize:        %d Bytes", k_Info.cbCabinet);
		PrintW(WHITE, L"Folder count in CAB: %d", k_Info.cFolders);
		PrintW(WHITE, L"File   count in CAB: %d", k_Info.cFiles);
		PrintW(WHITE, L"Set ID:              %d", k_Info.setID);
		PrintW(WHITE, L"CAB Index:           %d", k_Info.iCabinet);
		PrintW(WHITE, L"Has additional header data:          %s", k_Info.fReserve ? L"true" : L"false");
		PrintW(WHITE, L"Has predecessor in splitted Cabinet: %s", k_Info.hasprev ? L"true" : L"false");
		PrintW(WHITE, L"Has successor   in splitted Cabinet: %s\n", k_Info.hasnext ? L"true" : L"false");
	}
	else
	{
		PrintW(RED, L"Extract ERROR: The file is not a valid Cabinet: %s", u16_CompressFile);
	}

	// Now extract into subdirectory "_ExtractFile" and the corresponding subdirectories in the CAB file
	if (!i_ExtrFile.ExtractFileW(u16_CompressFile, u16_ExtractFile, this))
	{
		PrintW(RED, L"Extract ERROR: Not all files could be extracted:\n%s", i_ExtrFile.LastErrorW());
		goto _RESOURCE;
	}

	PrintW(GREEN, L"\nSUCCESS: Extracted all files from the above compressed cabinet into\n\"%s\"", u16_ExtractFile);

_RESOURCE:
	// You MUST call CleanUp() whenever you want to use another instance of CExtract, CExtractUrl or CExtractResource
	// in the same thread before the destructor of the current instance has been executed!
	if (!i_ExtrFile.CleanUp())
	{
		PrintW(RED, L"CleanUp ERROR:\n%s", i_ExtrFile.LastErrorW());
	}

	// ########################## Extract resource demo ############################

	// This will extract the file "Test.cab" in the embedded resources into a directory

	PrintW(WHITE, L"\n----------------------------------------------------------------");
	PrintW(YELLOW, L"\n                   CAB RESOURCE EXTRACTION\n");

	if (b_ExtractToMemory)
		wcscpy(u16_ExtractRsrc, L"MEMORY");

	if (!i_ExtrResource.CreateFDIContext())
	{
		PrintW(RED, L"Extract ERROR: Could not create FDI context:\n%s", i_ExtrResource.LastErrorW());
		goto _URL;
	}

	// Now extract into subdirectory "_ExtractResource" and the corresponding subdirectories in the CAB file
	if (!i_ExtrResource.ExtractResourceW(L"", ID_CAB_TEST, L"CABFILE", u16_ExtractRsrc, this))
	{
		PrintW(RED, L"Extract ERROR: Not all files could be extracted:\n%s", i_ExtrResource.LastErrorW());
		goto _URL;
	}

	PrintW(GREEN, L"\nSUCCESS: Extracted all files from cabinet resource Test.cab into\n\"%s\"", u16_ExtractRsrc);

_URL:
	// You MUST call CleanUp() whenever you want to use another instance of CExtract, CExtractUrl or CExtractResource
	// in the same thread before the destructor of the current instance has been executed!
	if (!i_ExtrResource.CleanUp())
	{
		PrintW(RED, L"CleanUp ERROR:\n%s", i_ExtrResource.LastErrorW());
	}

	// ########################## Extract URL demo ############################

	// This will extract a CAB file loacted on a HTTP server

	PrintW(WHITE, L"\n----------------------------------------------------------------");
	PrintW(YELLOW, L"\n                   CAB URL EXTRACTION\n");

	// !!!! IMPORTANT: Read in the documentation about the 100 possible ways to build the URL !!!!
	WCHAR* u16_Url = L"http://home.arcor.de/starwalker22/Test/UrlExtractDemo.cab";
	//	WCHAR* u16_Url =L"https://company.com/Downloads/Installer_1.35.cab";
	//	WCHAR* u16_Url =  L"ftp://user:password@company.com/Downloads/Installer_1.35.cab";

	// Size of blocks to be loaded to memory (HTTP Server must run a script, FTP Server must support "REST")
	// Set zero if you want to download an entire file to disk
	// !!!! IMPORTANT: Read in the documentation about the 100 possible types of downloading !!!!
	DWORD u32_BlockSize = 0;

	// If u16_LocalFile is an empty string -> create a temporary CAB file, extract it, then delete it.
	// Otherwise download into the specified file, extract it and do NOT delete it.
	// !!!! IMPORTANT: Read the documentation for more details !!!!
	WCHAR* u16_LocalFile = L"";
	//	WCHAR* u16_LocalFile = L"C:\\Downloaded.cab"; // Ignored if blocksize > 0

	// Passive mode for FTP downloads
	i_ExtrUrl.SetPassiveFtpMode(TRUE);

	// Proxy Format= "http=http://Proxy1.com:8000 https=https://Proxy2.com:443"
	// Proxy = "" --> Use Internet Explorer default settings
	i_ExtrUrl.SetProxyW(L"");

	// Modifies the HTTP headers which are sent to the server. (separated by pipe)
	// e.g. "Referer: http://www.test.com|Accept-Language:en"  (no space before and after the pipe!!)
	// If the value of a header is empty, the header is removed.
	i_ExtrUrl.SetHttpHeadersW(L"");

	if (b_ExtractToMemory)
		wcscpy(u16_ExtractUrl, L"MEMORY");

	if (!i_ExtrUrl.CreateFDIContext())
	{
		PrintW(RED, L"Extract ERROR: Could not create FDI context:\n%s", i_ExtrUrl.LastErrorW());
		goto _WAIT;
	}

	// !!!! IMPORTANT: Read the documentation about the 100 possible modes of downloading !!!!
	PrintW(GREY, L"Extracting: %s", u16_Url);
	if (u32_BlockSize)    PrintW(GREY, L"Download to memory, Blocksize: %d Bytes\n", u32_BlockSize);
	else if (u16_LocalFile[0]) PrintW(GREY, L"Download to : %s\n", u16_LocalFile);
	else                       PrintW(GREY, L"Download to temporary file\n");

	// Now extract into subdirectory "_ExtractUrl"
	if (!i_ExtrUrl.ExtractUrlW(u16_Url, u32_BlockSize, u16_LocalFile, u16_ExtractUrl, this))
	{
		PrintW(RED, L"Extract ERROR: Not all files could be extracted:\n%s", i_ExtrUrl.LastErrorW());
		goto _WAIT;
	}

	// You can reutilize the downloaded CAB data for further extractions:
	//	if (!i_ExtrUrl.ExtractMoreUrlW("C:\\", this)) ... Error handling

	PrintW(GREEN, L"\nSUCCESS: Extracted all files from the URL into\n\"%s\"", u16_ExtractUrl);

_WAIT:
	// IMPORTANT: Close all handles and free memory after every URL extraction!!
	if (!i_ExtrUrl.CleanUp())
	{
		PrintW(RED, L"CleanUp ERROR:\n%s", i_ExtrUrl.LastErrorW());
	}

	// Under some cirumstances the DOS window closes immediately, so you don't see the output!
	//PrintW(GREY, L"\n\nHit any key to exit");
	//getch();
}

void CMain::PrintA(WORD u16_Color, char* s8_Format, ...)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), u16_Color);
	va_list  args;
	va_start(args, s8_Format);
	vprintf(s8_Format, args);
	printf("\n");
}
void CMain::PrintW(WORD u16_Color, WCHAR* u16_Format, ...)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), u16_Color);
	va_list  args;
	va_start(args, u16_Format);
	vwprintf(u16_Format, args);
	wprintf(L"\n");
}

// ########################### EXTRACTION CALLBACKS #######################

// This function will be called for each file in a cabinet at the moment it is about to be extracted.
// Return TRUE to extract the file to the path specified by the second parameter or FALSE to skip.
BOOL CMain::OnBeforeCopyFile(CExtract::kCabinetFileInfo *pk_Info, void* p_Param)
{
	CStrW sw_FileName = pk_Info->u16_File;
	if (!sw_FileName.IsAscii())
		sw_FileName = L"Console cannot display Unicode filename";

	PrintW(GREY, L"Callback OnBeforeCopyFile: %s", (WCHAR*)sw_FileName);
	return TRUE;
}

// This function will be called when a file is successfully extracted.
// If u16_TargetDirectory = "MEMORY" the file will not be written to disk, it will be stored in pi_ExtractMem instead.
// You must copy the content of pi_ExtractMem to another buffer as it becomes invalid when this function exits.
void CMain::OnAfterCopyFile(WCHAR* u16_Path, CMemory* pi_ExtractMem, void* p_Param)
{
	CStrW sw_FileName;
	CFile::SplitPathW(u16_Path, 0, &sw_FileName);

	if (!sw_FileName.IsAscii())
		sw_FileName = L"Console cannot display Unicode filename";

	PrintW(GREY, L"Callback OnAfterCopyFile:  %s", (WCHAR*)sw_FileName);

	if (pi_ExtractMem)
	{
		int s32_Len; // get the content of the file
		BYTE* u8_Data = pi_ExtractMem->GetData(&s32_Len);
	}
}

// This callback is called every 200 ms when extracting to disk. (only when huge files are extracted)
// It shows the progress of the current file in percent ("82.4%")
// On extraction to memory it will not be called. (It would be nonsene to extract huge files to memory)
// This callback will not be called for all files that extract faster than 200 ms
void CMain::OnProgressInfo(CExtract::kProgressInfo* pk_Info, void* p_Param)
{
	PrintA(GREY, "Callback OnProgressInfo: %.1f%%", pk_Info->fl_Percent);
}

// This function will be called exactly once for each cabinet opened by Copy, including continuation
// cabinets opened due to files spanning cabinet boundaries.
void CMain::OnCabinetInfo(CExtract::kCabinetInfo *pk_Info, void* p_Param)
{
	PrintA(GREY, "Callback OnCabinetInfo: Next CAB: '%s'", pk_Info->s8_NextCabinet);
}

// This function will be called when the next cabinet file in the sequence needs to be opened. The path
// has to be verified and can be changed if necessary. If the cabinet file cannot be opened this function
// will be called again with the second parameter set to an error that describes the problem.
void CMain::OnNextCabinet(CExtract::kCabinetInfo *pk_Info, int s32_Error, void* p_Param)
{
	PrintA(GREY, "Callback OnNextCabinet: Please insert '%s'", pk_Info->s8_NextDisk);
}

// ########################### COMPRESSION CALLBACKS #######################

// Called whenever the location of a file or file segment on a particular cabinet has been finalized.
// This information is useful only when files are being stored across multiple cabinets
// fContinuation is a BOOL which signifies whether the file is a later segment 
// of a file which has been split across cabinets.  
int CMain::OnFilePlaced(PCCAB pccab, WCHAR* u16_File, int s32_FileSize, BOOL fContinuation, void* p_Param)
{
	PrintW(GREY, L"Callback OnFilePlaced: %s %", u16_File);
	return 0;
}

//  typeStatus == statusFile if compressing a block into a folder
//        - cb1 : Size of compressed block
//        - cb2 : Size of uncompressed block
//
//  typeStatus == statusFolder if adding a folder to a cabinet
//        - cb1 : Amount of folder copied to cabinet so far
//        - cb2 : Total size of folder
//
//  typeStatus == statusCabinet if writing out a complete cabinet
//        - cb1 : Estimated cabinet size that was previously passed to GetNextCabinet().
//        - cb2 : Actual cabinet size
// - p_Param:
//        your user defined parameter which you have passed to AddFile(..)
//
// The return value is desired client size for cabinet file.
// FCI updates the maximum cabinet size remaining using this value.
// This allows a to generate multiple cabinets per disk, and have FCI limit the size correctly.
// The client can do cluster size rounding on the cabinet size!
// The client should either return cb2, or round cb2 up to some larger value and return that.
// Exit-Failure: Return -1 to signal that FCI should abort
int CMain::OnUpdateStatus(UINT typeStatus, CCompress::kCurStatus *pk_CurStatus, void *p_Param)
{
	// Display the values in pk_CurStatus in your GUI 
	// or do whatever you like with them or ignore them !
	if (pk_CurStatus->FolderPercent > 0)
		PrintW(GREY, L"Callback OnUpdateStatus: %02d %% completed", pk_CurStatus->FolderPercent);

	return 0;
}

// called whenever FCI wishes to create a new cabinet, which will happen whenever 
// the size of the cabinet is about to exceed the media size as specified in the cb field 
// of the CCAB structure passed to FCICreate.  
// - pccab:
//      pointer to a copy of the CCAB structure of the cabinet which has just been completed.
//      However, the iCab field will have been incremented by one.
//      When this function returns, the next cabinet will be created using the fields 
//      in this structure, so these fields should be modified as is necessary.
//      In particular, the szCab field (the cabinet name) should be changed.
//      If creating multiple cabinets, typically the iCab field is used to create the name;
//      Similarly, the disk name, media size, folder threshold, etc. parameters may 
//      also be modified.
// - cbPrevCab:
//      is an estimate of the size of the cabinet which has just been completed.
// - p_Param:
//      your user defined parameter which you have passed to AddFile(..)
// return TRUE for success, or FALSE to abort cabinet creation.
BOOL CMain::OnGetNextCabinet(PCCAB pccab, ULONG cbPrevCab, char* s8_FormatString, void* p_Param)
{
	// You can modify pccab->szCab and pccab->szDisk here:
	// Write the name for CAB files spanning over multiple files into pccab->szCab
	// Write the disk name into pccab->szDisk
	// (This will also be called if only one CAB file is created)
	// ATTENTION: All strings are UTF8 encoded!
	PrintA(GREY, "Callback OnGetNextCabinet: %s, %s", pccab->szCab, pccab->szDisk);

	// Modify the other elements if required:
	// pccab->cbFolderThresh = xyz;
	// pccab->setID = xyz;
	// pccab->cb    = xyz;
	return TRUE;
}

