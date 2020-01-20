/******************************************************************************
*
*        filename  :      HAVhdUtility.h
*        created   :      2009/09/24
*        Written by:      baide02
*        comment   :      A VHD utility for HA.
*
******************************************************************************/
#ifndef _VHD_CONVERTER_INTERFACE_
#define _VHD_CONVERTER_INTERFACE_

#include "HaVhdUtilErrorcode.h"
#include "HAVmIoIntf.h"

#ifdef VHDCONV_EXPORTS
#define VHDCONV_API extern "C" __declspec(dllexport)
#else
#define VHDCONV_API extern "C" __declspec(dllimport)
#endif

namespace HaVhdUtility
{
	/*Help function*/
	inline bool VHDIsPlain(const VHD_TYPE& type)
	{
		return (type == VHD_Fixed || type == VHD_Dynamic || type == VHD_Diff);
	}

	inline bool VHDIsCompressed(const VHD_TYPE& type)
	{
		return (type == VHD_Dynamic2 || type == VHD_Diff2);
	}

	inline bool VHDCreatorIsD2D(const VHD_CREATOR& creator)
	{
		return (creator == D2D);
	}

	/*
	Create a IVHDFile interface, this will has GENERIC_READ and FILE_SHARE_READ | FILE_SHARE_WRITE.
	If is plain VHD, input pEncInfo as NULL.
	return value:
	0: success, others:fail
	if success, *ppVHDFile will NEVER be NULL.
	*/
	VHDCONV_API int CreateVHDFile(const wchar_t* pwszFilePath, D2D_ENCRYPTION_INFO* pEncInfo, 
		IVHDFile** ppVHDFile);

	typedef int (*PFN_CreateVHDFile)(const wchar_t* pwszFilePath, D2D_ENCRYPTION_INFO* pEncInfo, 
		IVHDFile** ppVHDFile);


	/*
	Create a IVHDFile interface, this will has ulShareMode(such as FILE_SHARE_READ) and ulDesireAccess
	@ulShareMode - FILE_SHARE_READ, FILE_SHARE_WRITE
	@ulDesiredAccess - GENERIC_READ, GENERIC_WRITE
	return value:
	0: success, others:fail
	if success, *ppVHDFile will NEVER be NULL.
	*/
	//VHDCONV_API int CreateVHDFileEx(const wchar_t* pwszFilePath, unsigned long ulDesiredAccess,
	//                                unsigned long ulShareMode, D2D_ENCRYPTION_INFO* pEncInfo, 
	//                                IVHDFile** ppVHDFile);

	typedef int (*PFN_CreateVHDFileEx)(const wchar_t* pwszFilePath, unsigned long ulDesiredAccess,
		unsigned long ulShareMode, D2D_ENCRYPTION_INFO* pEncInfo, 
		IVHDFile** ppVHDFile);


	/**
	* Create IVDiskDiff by a single VHD file (FULL or incremental VHD)
	* @pwszFilePath [in]    - the VHD file path
	* @pEncInfo     [in]    - the crypt info of this VHD file. if is plain VHD, input NULL is OK.
	* @ppDiskDiff   [out]   - the IVDiskDiff returned
	* return value: 
	* 0 success, others fail
	*/
	VHDCONV_API int CreateVDiskDiff(const wchar_t* pwszFilePath, D2D_ENCRYPTION_INFO* pEncInfo, 
		IVDiskDiff** ppDiskDiff);
	typedef int (*PFN_CreateVDiskDiff)(const wchar_t* pwszFilePath, D2D_ENCRYPTION_INFO* pEncInfo, 
		IVDiskDiff** ppDiskDiff);

	/**
	* Create IVDiskDiff by a set of VHD files. (the difference between pwszVHDOld and pwszVHDNew)
	* pwszVHDOld < Data <= pwszVHDNew
	* If pwszVHDOld is NULL or is empty string, means the whole snapshot from the initial parent
	* to pwszVHDNew.
	* Notes: pwszVHDOld is parent, and pwszVHDNew is child.
	* return value:
	* 0 success, others fail
	*/
	VHDCONV_API int CreateVDiskDiffEx(const wchar_t* pwszVHDOld, const wchar_t* pwszVHDNew, 
		D2D_ENCRYPTION_INFO* pEncInfo, IVDiskDiff** ppDiskDiff);
	typedef int (*PFN_CreateVDiskDiffEx)(const wchar_t* pwszVHDOld, const wchar_t* pwszVHDNew, 
		D2D_ENCRYPTION_INFO* pEncInfo, IVDiskDiff** ppDiskDiff);

	/**
	* Get incremental disks from a full disk by the given incrementals' bitmap.
	* @pwszFullVhd   [in]    - the full VHD file path.
	* @pDiffBitmap   [in]    - the bitmap of inc disks we want to read from FULL VHD.
	* @ppDiskDiff    [out]   - the IVDiskDiff returned.
	* return 0 success, others fail.
	* Attention: Caller need to handle the release of pDiffBitmap.
	*/
	VHDCONV_API int GetIncDisksFromFull(/*in*/const wchar_t* pwszFullVhd, /*in*/IDiskBmpReader* pDiffBitmap,
		/*in*/D2D_ENCRYPTION_INFO* pEncInfo, /*out*/IVDiskDiff** ppDiskDiff);
	typedef int (*PFN_GetIncDisksFromFull)(const wchar_t* pwszFullVhd, IDiskBmpReader* pDiffBitmap,
		/*in*/D2D_ENCRYPTION_INFO* pEncInfo, /*out*/IVDiskDiff** ppDiskDiff);

	/*
	* Get incremental disks from a full disk and the following incremental disk.
	* suppose there has S03(full), S04, S05. With this API, can read the data of S02 (inc), s03(inc), S04, S05.
	* @pwszFullVhd   [in]    - the full VHD file path. Such as S03.
	* @pDiffBitmap   [in]    - the bitmap of inc disks we want to read from FULL VHD.
	* @pwszIncVhd    [in]    - the incremental VHD file path. Such as S05.
	* @ppDiskDiff    [out]   - the IVDiskDiff returned.
	* return 0 success, others fail.
	*/
	VHDCONV_API int GetIncDisks_MixMode(const wchar_t* pwszFullVhd, IDiskBmpReader* pDiffBitmap,
		const wchar_t* pwszIncVhd, D2D_ENCRYPTION_INFO* pEncInfo, 
		IVDiskDiff** ppDiskDiff);
	VHDCONV_API int (*PFN_GetIncDisks_MixMode)(const wchar_t* pwszFullVhd, IDiskBmpReader* pDiffBitmap,
		const wchar_t* pwszIncVhd, D2D_ENCRYPTION_INFO* pEncInfo, 
		IVDiskDiff** ppDiskDiff);
#if 0
	/*
	Get the VHD type and creator
	@pwszFilePath [in] - the file path
	@type [out] - return the VHD type, if is NULL, will never get it's type
	@creator[out] - return the VHD's creator, if is NULL, will never get it's creator
	return value:
	0 success
	others fail
	*/
	VHDCONV_API int GetVHDProp(const wchar_t* pwszFilePath, VHD_TYPE* type, VHD_CREATOR* creator);

	typedef int (*PFN_GetVHDProp)(const wchar_t* pwszFilePath, VHD_TYPE* type, VHD_CREATOR* creator);

	/*
	Convert a D2D file to a mountable plain VHD file, and will NOT rename it.
	This function will convert the origin D2D to VHD, caller need to first copy the src path to a des path and/or rename it.
	pwszVHDRelP and pwszVHDAbsP can NOT both be NULL.

	@pwszD2D - the source D2D file path to convert.
	@bSetRelP - if will set relative parent path,
	@pwszVHDRelP - the destination VHD file's relative parent path, if bSetRelP but pwszVHDRelP is NULL, we remove the path
	@bSetAbsP - if will set absolute parent path
	@pwszVHDAbsP - the destination VHD file's absolute parent path, if bSetAbsP but pwszVHDAbsP is NULL, we remove the path

	return value:
	0 - success
	others - fail
	*/
	VHDCONV_API int ConvertD2DToVHD(const wchar_t* pwszD2D, bool bSetRelP, const wchar_t* pwszVHDRelP, 
		bool bSetAbsP, const wchar_t* pwszVHDAbsP);

	typedef int (*PFN_ConvertD2DToVHD)(const wchar_t* pwszD2D, bool bSetRelP, const wchar_t* pwszVHDRelP, 
		bool bSetAbsP, const wchar_t* pwszVHDAbsP);


	/*
	Convert a plain VHD file to a D2D file, and will NOT rename it.
	This function will convert the origin VHD to D2D, caller need to first copy the src path to a des path and/or rename it.
	pwszVHDRelP and pwszVHDAbsP can NOT both be NULL.

	@pwszVHD - the source D2D file path to convert.
	@bSetRelP - if will set relative parent path,
	@pwszD2DRelP - the destination VHD file's relative parent path, if bSetRelP but pwszVHDRelP is NULL, we remove the path
	@bSetAbsP - if will set absolute parent path
	@pwszD2DAbsP - the destination VHD file's absolute parent path, if bSetAbsP but pwszVHDAbsP is NULL, we remove the path

	return value:
	0 - success
	others - fail
	*/
	VHDCONV_API int ConvertVHDToD2D(const wchar_t* pwszVHD, bool bSetRelP, const wchar_t* pwszD2DRelP, 
		bool bSetAbsP, const wchar_t* pwszD2DAbsP);

	typedef int (*PFN_ConvertVHDToD2D)(const wchar_t* pwszVHD, bool bSetRelP, const wchar_t* pwszD2DRelP, 
		bool bSetAbsP, const wchar_t* pwszD2DAbsP);

#endif // if 0

	//
	// << add support for multi path >> 2009-06-11
	//
	enum VHD_CREATE_DISP
	{
		VHD_DISP_CREATE_NEW = 0,
		VHD_DISP_CREATE_ALWAYS = 1,
		VHD_DISP_OPEN_EXISTING = 2,
		VHD_DISP_OPEN_ALWAYS = 3
	};

	enum VHD_CREATE_FLAG
	{
		VHD_CREATE_FLAG_CUSTOM_GUID = 0x00000001, //use the input GUID in the ST_CREATE_PARMS
	};

	typedef struct _t_create_parms
	{
		int             nVer;
		VHD_TYPE        eType;
		__int64         llDiskSize;
		unsigned long   dwCreateDisp;
		unsigned long   ulFlags;
		GUID            uniqueID;
	} ST_CREATE_PARMS;


	/**
	* Represent a VHD file writer
	*/
	struct IVHDFileW
	{
		virtual int Release() = 0;

		//nMethod - VDISK_SEEK_BEGIN, VDISK_SEEK_CURR, VDISK_SEEK_END
		virtual int Seek(__int64 llOffset, __int64* pllNewPos, int nMethod) = 0;

		//@llStartOffset - If is -1, write at current offset
		virtual int Write(const char* pBuf, unsigned long cbToWrite,
			unsigned long* pcbWritten, __int64 llStartOffset) = 0;
	};

	VHDCONV_API int CreateVHDFileW(const wchar_t*      pwszFilePath, 
		const ST_CREATE_PARMS*    pstCreateParms,
		IVHDFileW** ppVhdW);
	typedef int (*PFN_CreateVHDFileW)(const wchar_t*      pwszFilePath, 
		const ST_CREATE_PARMS*    pstCreateParms,
		IVHDFileW** ppVhdW);

	VHDCONV_API int CreateChildVHDFileW(const wchar_t*      pwszFilePath, 
		const ST_CREATE_PARMS*    pstCreateParms,
		const wchar_t*      pwszParentPath,
		IVHDFileW** ppVhdW);
	typedef int (*PFN_CreateChildVHDFileW)(const wchar_t*      pwszFilePath, 
		const ST_CREATE_PARMS*    pstCreateParms,
		const wchar_t*      pwszParentPath,
		IVHDFileW** ppVhdW);


	/**
	* Add a API to create IVDisk. The input pwszPath should be a full path of plain VHD.
	*/
	VHDCONV_API int OpenVDisk_HyperV(const wchar_t* pwszPath, bool bROLast, IVDisk** ppDisk);
	typedef int (*PFN_OpenVDisk_HyperV)(const wchar_t* pwszPath, bool bROLast, IVDisk** ppDisk);

} //namespace HaVhdUtility

namespace VHDConv = HaVhdUtility;

#endif // _VHD_CONVERTER_INTERFACE_
