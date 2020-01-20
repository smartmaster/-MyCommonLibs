//
// Error code for HAVhdUtility
//
#ifndef _HA_VHD_UTILITY_RC_INCLUDE_
#define _HA_VHD_UTILITY_RC_INCLUDE_

#define E_VHD_COV_SUCCESS                0
#define E_VHD_COV_FAILED                 0xFFFFFFFF

#define E_VHD_COV_RANGE_MOREDATA         0x00000050

#define E_VHD_COV_INVALID_ARG            0xA00E0001
#define E_VHD_COV_CREATE_FILE            0xA00E0002
#define E_VHD_COV_READ_FOOTER            0xA00E0003
#define E_VHD_COV_INVALID_TYPE           0xA00E0004
#define E_VHD_COV_READ_HEADER            0xA00E0005
#define E_VHD_COV_CHECKSUM               0xA00E0006
#define E_VHD_COV_READBAT                0xA00E0007
#define E_VHD_COV_READBITMAP             0xA00E0008
#define E_VHD_COV_ERROR_MAGIC            0xA00E0009
#define E_VHD_COV_CREATE_IDXFILE         0xA00E000A
#define E_VHD_COV_NEED_MORE_INPUTBUF     0xA00E000B

#define E_VHD_COV_SETPPATH_INVALID_TYPE  0xA00E000C //invalid parent path type
#define E_VHD_COV_GETPPATH_INVALID_TYPE  0xA00E000D
#define E_VHD_COV_GETPPATH_NOTEXIST      0xA00E000E //no parent path
#define E_VHD_COV_GETPPATH_NO_PARENT     0xA00E000F //base vhd, no need to get parent path
#define E_VHD_COV_SETPPATH_NO_PARENT     0xA00E0010 //base vhd, no need to set parent path
#define E_VHD_COV_SETPTIME_NO_PARENT     0xA00E0011 //base vhd, no need to set parent time
#define E_VHD_COV_GETPTIME_NO_PARENT     0xA00E0012 //base vhd, no need to get parent time

#define E_VHD_COV_VHD2_NO_PTIME          0xA00E0013

#define E_VHD_COV_WRITEHEADER_FAILED     0xA00E0014 //write file header failed
#define E_VHD_COV_SETFILEPOS             0xA00E0015 //set file pos failed
#define E_VHD_COV_WRITEFOOTER_FAILED     0xA00E0016 //write file footer failed

#define E_VHD_COV_UNUSED_BAT_ENTRY       0xA00E0017
#define E_VHD_COV_NO_ENOUGH_MEM          0xA00E0018 //not enough memory
#define E_VHD_COV_FILE_SIZE_LIMITED      0xA00E0019 //idx size limited
#define E_VHD_COV_INTERNAL_ERROR         0xA00E001A
#define E_VHD_COV_READ_DATA              0xA00E001B //read data failed


#endif //_HA_VHD_UTILITY_RC_INCLUDE_
