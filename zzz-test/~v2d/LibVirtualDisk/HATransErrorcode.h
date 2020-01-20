/******************************************************************************
*
*        filename  :      HATransErrorCode.h
*        created   :      2009/10/14
*        Written by:      baide02
*        comment   :      define the error code used in HA transfer project
*
******************************************************************************/
#ifndef _HA_TRANS_ERRORCODE_INCLUDE_
#define _HA_TRANS_ERRORCODE_INCLUDE_
//
//error code
//

#define E_HA_INVALIDSRCPATH                 0xA00F0001
#define E_HA_STACK_NOMOREFILES              0xA00F0002
#define E_HA_STACK_INVALIDPATH              0xA00F0003
#define E_HA_STACK_OTHERS                   0xA00F0004
#define E_HA_INVALID_CRYPT_ALG              0xA00F0005
#define E_HA_INVALID_CMPRS_ALG              0xA00F0006

//error code at socket client side
#define E_HA_SOCK_WRONGVERSION              0xA00F0007
#define E_HA_SOCK_ALREADYEXIST              0xA00F0008
#define E_HA_SOCK_FILEHEADERFAIL            0xA00F0009
#define E_HA_SOCK_INVALID_IP                0xA00F000A
#define E_HA_SOCK_SRV_STOPPING              0xA00F000B      // Server side is in stopping
#define E_HA_SRCSESS_GETPWDFAILED           0xA00F000C      // Failed to get the source session password


//error code at socket server side
#define E_HA_SVR_RECV_INVALID_DATALEN       0xA00F1001
#define E_HA_SRV_AUTHENTICATE_FAILED        0xA00F1002      // Failed to authenticate user
#define E_HA_SRV_VHD_CMPRSED_VOLUME         0xA00F1003      // Do not support create VHD on cmprs vol
#define E_HA_SRV_UNSERIALIZE_JOBHEADER      0xA00F1004

#define E_HA_SVR_INVALID_FILE_HANDLE        0xA00F100B
#define E_HA_SVR_INVALID_CMD                0xA00F100C
#define E_HA_SVR_INVALID_HEADERSIZE         0xA00F100D
#define E_HA_SVR_INVALID_FILETYPE           0xA00F100E
#define E_HA_SVR_CREATE_DIR_FAILED          0xA00F100F
#define E_HA_SVR_CREATE_FILE_FAILED         0xA00F1010
#define E_HA_SVR_WRITEFILE_ERROR            0xA00F1011
#define E_HA_SVR_CREATE_HART_FAILED         0xA00F1012
#define E_HA_SVR_CREATE_STATISTIC_FAILED    0xA00F1013
#define E_HA_SRV_SANDISK_WRITE_FAIL         0xA00F005D

#define E_HA_LOG_CANNOT_RENAMED             0xA00F1014 
#define E_HA_LOG_CREATE_FILE_FAILED         0xA00F1015

//error code for normal client code
#define E_HA_CLI_READ_INPUT_XML_FAILED      0xA00F0016
#define E_HA_CLI_CONVERT_XML2BINARY_FAILED  0xA00F0017
#define E_HA_CLI_CREATE_FILE_FAILED         0xA00F0018
#define E_HA_CLI_SEND_DATA_FAILED           0xA00F0019

//error code for HAStream
#define E_HA_STREAM_OPEN_READMODE           0xA00F001A
#define E_HA_STREAM_OPEN_WRITEMODE          0xA00F001B
#define E_HA_STREAM_CRYPTFAILED             0xA00F001C
#define E_HA_STREAM_ZLIBDE_INIT_FAILED      0xA00F001D
#define E_HA_STREAM_ZLIBIN_INIT_FAILED      0xA00F001E
#define E_HA_STREAM_INVALIDHANDLE           0xA00F001F
#define E_HA_STREAM_INVALIDFILE_TYPE        0xA00F0020
#define E_HA_STREAM_HAS_READ_FINAL          0xA00F0021               //has already read to the end one time


//error code for HA Client monitor
#define E_HA_CMONITOR_PARENTPROC_EXIT       0xA00F0040

#define E_HA_SHM_CREATEMAPPING              0xA00F0041
#define E_HA_SHM_OPENMAPPING                0xA00F0042
#define E_HA_SHM_MAPVIEW                    0xA00F0043
#define E_HA_SHM_CREATEMUTEX                0xA00F0044
#define E_HA_SHM_OPENMUTEX                  0xA00F0045
#define E_HA_SHM_ERRORMEMORY                0xA00F0046
#define E_HA_SHM_ERRORMUTEX                 0xA00F0047
#define E_HA_SHM_ERRORSIZE                  0xA00F0048    //too large input size or offset
#define E_HA_MSG_NOITEM                     0xA00F0049
#define E_HA_MSG_ITEMFULL                   0xA00F004A    //The item number has reach the max item number
#define E_HA_MSG_SPACEFULL                  0xA00F004B

//error code for HA Jobscript shm
#define E_HA_JSSHM_NEEDMORESPACE            0xA00F004C
#define E_HA_JSSHM_FAILED_TO_LOCK           0xA00F004D



//error code for HA DR
#define E_HA_DR_TOO_LARGE_RANGE             0xA00F4001




#endif //_HA_TRANS_ERRORCODE_INCLUDE_
