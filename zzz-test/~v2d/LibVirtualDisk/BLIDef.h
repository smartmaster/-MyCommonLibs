#pragma once

#define BLI_INDEX_FOLDER        L"Index"
#define BLI_TOPURGE_FOLDER        L"ToPurge"
#define BLI_SESSION_NAME_PATTERN    L"S%010u"
#define BLI_SESSION_INDEX_EXT     L".idx"
//#define BLI_SESSION_INDEX_EXT_TOPURGE     L".idx_topurge"
#define BLI_SESSION_INDEX_EXT_STEP1  L".idx_step1"
#define BLI_SESSION_INDEX_EXT_STEP2  L".idx_step2"
#define BLI_DISK_PRIMERY_NAME_PATTERN L"disk%010u"
#define BLI_DISK_NAME_PATTERN     BLI_DISK_PRIMERY_NAME_PATTERN L".vhd"
#define BLI_BLOCK_NAME_PATTERN   L"block%010u.ctf"

#define BLI_PURGE_STOP_EVENT_NAME  L"AB_BLI_PURGE_STOP_CUR_SESS"

#define BLI_INVALID_SESSION_NUMBER     (DWORD)(-1)