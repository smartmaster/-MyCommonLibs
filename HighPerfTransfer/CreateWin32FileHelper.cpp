#include "stdafx.h"

#include "CreateWin32FileHelper.h"


HRESULT CreateWin32FileHelper(CONST BODY_FILE_DESCRITION & FileDesc, IN ULONG ulDesiredAccess, IN ULONG ulShareMode, IN ULONG ulCreationDisposition, IN ULONG ulFlagsAndAttributes, IFileDevice ** ppIFileDevice)
{
	return CreateInstanceFileDeviceWin32(
		FileDesc.m_FileName.GetString(),
		ulDesiredAccess,
		ulShareMode,
		ulCreationDisposition,
		ulFlagsAndAttributes,
		ppIFileDevice,
		TRUE);
}
