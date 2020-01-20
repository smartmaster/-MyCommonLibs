#pragma once

#include "D2DErrorCode.h"

#define CRYPTO_ERR2D2D(hr)                ERROR2D2D(D2D_FACILITY_CRYPTO, (hr))
#define CRYPTO_HR2D2D(hr)                 HRESULT2D2D(D2D_FACILITY_CRYPTO, (hr))

//ZZ: Error code defined in D2D encryption is from E001
#define D2DCRYPTO_SYSTEM_WRAP_ERRCODE_BASE  CRYPTO_ERR2D2D(0)
//ZZ: Reserve 20000 error codes for system error.
#define D2DCRYPTO_INTERNALE_ERRCODE_BASE    CRYPTO_ERR2D2D(0x4E20)
//ZZ: Reserve 37344 error codes for internal error
#define D2DCRYPTO_ALGORITHM_ERRCODE_BASE    CRYPTO_ERR2D2D(0xE000)

//
// MessageId: D2DCRYPTO_S_OK
//
// MessageText:
//
//  Everything is fine.
//
#define D2DCRYPTO_S_OK 0x0

//
// MessageId: D2DCRYPTO_E_MORE_DATA
//
// MessageText:
//
//  The input buffer is insufficient to store all data.
//
#define D2DCRYPTO_E_MORE_DATA  CRYPTO_ERR2D2D(ERROR_MORE_DATA)

//
// MessageId: D2DCRYPTO_E_INVALID_PARAM
//
// MessageText:
//
//  The input parameter for function is invalid.
//
#define D2DCRYPTO_E_INVALID_PARAM  CRYPTO_ERR2D2D(ERROR_INVALID_PARAMETER)

//
// MessageId: D2DCRYPTO_E_INSTANCE_NOT_INIT
//
// MessageText:
//
//  The instance used in API has not been initialized correctly.
//
#define D2DCRYPTO_E_INSTANCE_NOT_INIT  (D2DCRYPTO_INTERNALE_ERRCODE_BASE + 1)

//
// MessageId: D2DCRYPTO_E_ENCLIB_NOT_SUPPORTED
//
// MessageText:
//
//  The encryption library type specified is not support
//
#define D2DCRYPTO_E_ENCLIB_NOT_SUPPORTED  (D2DCRYPTO_ALGORITHM_ERRCODE_BASE + 1)

//
// MessageId: D2DCRYPTO_E_ENCALG_NOT_SUPPORTED
//
// MessageText:
//
//  The encryption algorithm type specified is not support
//
#define D2DCRYPTO_E_ENCALG_NOT_SUPPORTED  (D2DCRYPTO_ALGORITHM_ERRCODE_BASE + 2)

//
// MessageId: D2DCRYPTO_E_HASHALG_NOT_SUPPORTED
//
// MessageText:
//
//  The encryption algorithm type specified is not support
//
#define D2DCRYPTO_E_HASHALG_NOT_SUPPORTED  (D2DCRYPTO_ALGORITHM_ERRCODE_BASE + 4)

//
// MessageId: D2DCRYPTO_E_ENCAPI_NOT_SUPPORTED
//
// MessageText:
//
//  The encryption cryptoAPI type specified is not support
//
#define D2DCRYPTO_E_ENCAPI_NOT_SUPPORTED        (D2DCRYPTO_ALGORITHM_ERRCODE_BASE + 8)

#define D2DCRYPTO_E_UNABLE_TO_CREATE_CONTAINER  (D2DCRYPTO_ALGORITHM_ERRCODE_BASE + 9)