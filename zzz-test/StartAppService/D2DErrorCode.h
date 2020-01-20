/***********************************************************************************************************************
                                                    For HRESULT
 ***********************************************************************************************************************
  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 +-+-+-+-+-+---------------------+-------------------------------+
 |S|R|C|N|X|     Facility        |               Code            |
 +-+-+-+-+-+---------------------+-------------------------------+
 S (1 bit): 
    Severity. If set, indicates a failure result. If clear, indicates a success result.
 R (1 bit): 
    Reserved. If the N bit is clear, this bit MUST be set to 0. If the N bit is set, this bit is defined by the NTSTATUS 
    numbering space (as specified in section 2.3).
 C (1 bit): 
    Customer. This bit specifies if the value is customer-defined or Microsoft-defined. The bit is set for customer-defined
    values and clear for Microsoft-defined values.<1>
 N (1 bit):
    If set, indicates that the error code is an NTSTATUS value (as specified in section 2.3), except that this bit is set.
 X (1 bit): 
    Reserved. SHOULD be set to 0. <2>
 Facility (11 bits): 
    An indicator of the source of the error. New facilities are occasionally added by Microsoft.
 Code (2 bytes): 
    The remainder of the error code.
 ***********************************************************************************************************************
                                                    For NTSTATUS
 ***********************************************************************************************************************
  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 +---+-+-+-----------------------+-------------------------------+
 |Sev|C|R|     Facility          |               Code            |
 +---+-+-+-----------------------+-------------------------------+

 Sev (2 bits): Severity. Severity codes are as follows. 
    STATUS_SEVERITY_SUCCESS(0x0): -------- Success
    STATUS_SEVERITY_INFORMATIONAL(0x1): -- Informational
    STATUS_SEVERITY_WARNING(0x2): -------- Warning
    STATUS_SEVERITY_ERROR(0x3): ---------- Error
 C (1 bit): 
    Customer. This specifies if the value is customer- or Microsoft-defined. This bit is set for customer-defined values 
    and clear for Microsoft-defined values.
 N (1 bit): 
    Reserved. MUST be set to 0 so that it is possible to map an NTSTATUS value to an equivalent HRESULT value, as specified
    in section 2.1, by setting this bit.
 Facility (12 bits): 
    A value that, together with the C bit, indicates the numbering space to use for the Code field.
 Code (2 bytes): 
    The remainder of the error code. Vendors SHOULD reuse the values in the following table with their indicated meaning or 
    define their own values with the C bit set. Choosing any other value with the C bit clear runs the risk of a collision 
    in the future.
 ************************************************************************************************************************
                                                    For D2D HRESULT
 ************************************************************************************************************************
  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 +-+-+-+-+-+-----+---------------+-------------------------------+
 |S|R|C|N|X| D2D |   Facility    |               Code            |
 +-+-+-+-+-+-----+---------------+-------------------------------+
 D2D(3 bit): Indicate this is error code is D2D encryption internal error.
 ************************************************************************************************************************/
#pragma once

#define MAKE_D2DRETCODE(sev, fac, code) \
    ((HRESULT) (((unsigned long)(sev)<<31) | (0x1<<29) | ((unsigned long)((fac)<<16)) | ((unsigned long)(code))) )

#define D2DRC_SUCCEED(hr)                         ((HRESULT)(hr) >= 0)
#define D2DRC_FAILED(hr)                          ((HRESULT)(hr) < 0)

#define D2DHRESULT_CODE(hr)                       ((hr) & 0xFFFF)
#define D2DHRESULT_FACILITY(hr)                   (((hr) >> 16) & 0x1fff) 
#define D2DHRESULT_SEVERITY(hr)                   (((hr) >> 31) & 0x1)

#define ERROR2D2D(fac, hr)                        MAKE_D2DRETCODE(1, (fac), (hr))
#define HRESULT2D2D(fac, hr)                      MAKE_D2DRETCODE(0, (fac), (hr))

#define D2DERRCODEBASE(fac, base)                 ERROR2D2D((fac), (base))
#define D2DERRCODEGEN(fac, base, index)           (D2DERRCODEBASE((fac), (base)) + (index))
#define D2DERRCODEGENBYIDX(base, index)           ((base) + (index))

#define D2D_FACILITY_COMMON                       0x100
#define D2D_FACILITY_INTERFACE                    0x200
#define D2D_FACILITY_CRYPTO                       0x700
#define D2D_FACILITY_VSSWRAP                      0x800

////////////////////////////////////////////////////////////////////
//ZZ: Some common error codes.

#define COMM_ERRD2D(hr)                           ERROR2D2D(D2D_FACILITY_COMMON, (hr))
#define COMM_HRD2D(hr)                            HRESULT2D2D(D2D_FACILITY_COMMON, (hr))

#define D2DCOMM_ERROR_CODE_BASE                   D2DERRCODEBASE(D2D_FACILITY_COMMON, 0)

//
// MessageId: D2DCOMM_S_OK
//
// MessageText:
//
//  Everything is fine.
//
#define D2DCOMM_S_OK     0x0

//
// MessageId: D2DCOMM_E_MORE_DATA
//
// MessageText:
//
//  The input buffer is insufficient to store all data.
//
#define D2DCOMM_E_MORE_DATA                       ERROR2D2D(D2D_FACILITY_COMMON, ERROR_MORE_DATA)

//
// MessageId: D2DCOMM_E_INVALID_PARAM
//
// MessageText:
//
//  The input parameter for function is invalid.
//
#define D2DCOMM_E_INVALID_PARAM                   ERROR2D2D(D2D_FACILITY_COMMON, ERROR_INVALID_PARAMETER)

#define D2DCOMM_E_ITEM_NOT_FOUND                      D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 1)
#define D2DCOMM_E_ITEM_ALREADY_EXIST                  D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 2)
#define D2DCOMM_E_FAILED_TO_FINDFIRSTFILE             D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 3)
#define D2DCOMM_E_CATCH_UNKNOWN_EXCEPTION             D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 4)
#define D2DCOMM_E_STRING_LENGTH_OVERLENGTH            D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 5)
#define D2DCOMM_E_UNABLE_TO_LOADLIBRARY               D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 6)
#define D2DCOMM_E_UNABLE_TO_FREELIBRARY               D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 7)
#define D2DCOMM_E_UNABLE_TO_GETPROCADDRESS            D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 8)
#define D2DCOMM_E_FAILED_TO_CREATE_FILE               D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 9)
#define D2DCOMM_E_FAILED_TO_WRITE_DUMP_FILE           D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 10)
#define D2DCOMM_E_FAILED_TO_LOAD_XML_FILE             D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 11)
#define D2DCOMM_E_FAILED_TO_LOAD_XML_CONTENT          D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 12)
#define D2DCOMM_E_INVALID_XML_FORMAT                  D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 13)
#define D2DCOMM_E_FAILED_TO_GET_DISK_INFO             D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 14)
#define D2DCOMM_E_POINTER_OBJECT_NOT_INIT             D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 15)
#define D2DCOMM_E_UNABLE_TO_GET_MOUNT_POINT           D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 16)
#define D2DCOMM_E_UNABLE_TO_GET_VOLUME_GUID           D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 17)
#define D2DCOMM_E_SESS_MERGE_IS_NOT_ALLOWED           D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 18)
#define D2DCOMM_E_FAILED_TO_DO_INTERGRATE_LOCK        D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 19)
#define D2DCOMM_E_FAILED_TO_LOCK_SESS_FOR_MERGE       D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 20)
#define D2DCOMM_E_LOCK_USED_FOR_ANOTHER_SESS          D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 21)
#define D2DCOMM_E_OPERATION_NOT_SUPPORTED             D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 22)
#define D2DCOMM_E_SESSION_IS_INVALID                  D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 23)
#define D2DCOMM_E_SESSION_IS_PURGED                   D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 24)
#define D2DCOMM_E_NO_JOB_SCRIPT_FOR_CATALOG           D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 25)
#define D2DCOMM_E_NOT_ALL_CATALOG_CREATED             D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 26)
#define D2DCOMM_E_SKIP_ARCHIVE_NO_FULL_CATALOG        D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 27)
#define D2DCOMM_E_CATALOG_IS_NOT_CREATED              D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 28)
#define D2DCOMM_E_CATCH_UNHANDLED_EXCEPTION           D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 29)
#define D2DCOMM_E_CATCH_SEH_EXCEPTION                 D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 30)
#define D2DCOMM_E_CATCH_INVALID_PARAM_EXCEPTION       D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 31)
#define D2DCOMM_E_JOB_SCRIPT_IS_DUPLICATED            D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 32)
#define D2DCOMM_E_NOT_ALL_REQUIRED_DATA_READ          D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 33)
#define D2DCOMM_E_NOT_ALL_REQUIRED_DATA_WRITE         D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 34)
#define D2DCOMM_E_UNABLE_TO_GET_LICENSE_INFO          D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 35)
#define D2DCOMM_E_FAILED_TO_START_SERVICE             D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 36)
#define D2DCOMM_E_UNABLE_TO_REPLACE_DEST_FILE         D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 37)
#define D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR            D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 38)
#define D2DCOMM_E_SERVICE_IS_NOT_INSTALLED            D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 39)
#define D2DCOMM_E_UNABLE_TO_GET_SERVICE_STATUS        D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 40)
#define D2DCOMM_E_SERVICE_IS_NOT_RUNNING              D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 41)
#define D2DCOMM_E_FAILED_TO_STOP_SERVICE              D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 42)
#define D2DCOMM_E_FAILED_TO_ALLOCATE_MEMORY           D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 43)
#define D2DCOMM_E_UNABLE_TO_ACCESS_JOB_SCRIPT         D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 44)
#define D2DCOMM_E_SPECIFIED_INSTANCE_EXIST            D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 45)
#define D2DCOMM_E_SESSION_DOES_NOT_EXIST              D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 46)
#define D2DCOMM_E_CATALOG_JOB_IS_SKIPPED              D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 47)
#define D2DCOMM_E_INSUFFICIENT_FREESPACE_4_MERGE      D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 48)
#define D2DCOMM_E_UNABLE_TO_CHECK_FREESPACE_4_MERGE   D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 49)
#define D2DCOMM_E_CATALOG_JS_EXCEED_MAX_RETRY_TIMES   D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 50)
#define D2DCOMM_E_CATALOG_FAILED_DATA_CORRUPT         D2DERRCODEGENBYIDX(D2DCOMM_ERROR_CODE_BASE, 51)