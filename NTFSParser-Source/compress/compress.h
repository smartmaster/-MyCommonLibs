

#ifndef  _COMPRESS_H_
#define  _COMPRESS_H_

#if 0
#define ASSERT(exp)        if (!(exp)) { Log(_T("%s:%d %s\n"),__FILE__,__LINE__,#exp); }
#define ASSERTMSG(msg,exp) if (!(exp)) { Log(_T("%s:%d %s %s\n"),__FILE__,__LINE__,msg,#exp); }
#else
#define ASSERT(exp)        ATLASSERT(exp)
#define ASSERTMSG(msg,exp) ATLASSERT(exp)
#endif


//
// The following macros store and retrieve USHORTS and ULONGS from potentially
// unaligned addresses, avoiding alignment faults.  they should probably be
// rewritten in assembler
//

#define SHORT_SIZE  (sizeof(USHORT))
#define SHORT_MASK  (SHORT_SIZE - 1)
#define LONG_SIZE       (sizeof(LONG))
#define LONG_MASK       (LONG_SIZE - 1)
#define LOWBYTE_MASK 0x00FF

#define FIRSTBYTE(VALUE)  (VALUE & LOWBYTE_MASK)
#define SECONDBYTE(VALUE) ((VALUE >> 8) & LOWBYTE_MASK)
#define THIRDBYTE(VALUE)  ((VALUE >> 16) & LOWBYTE_MASK)
#define FOURTHBYTE(VALUE) ((VALUE >> 24) & LOWBYTE_MASK)

//
// if MIPS Big Endian, order of bytes is reversed.
//

#define SHORT_LEAST_SIGNIFICANT_BIT  0
#define SHORT_MOST_SIGNIFICANT_BIT   1

//++
//
// VOID
// RtlRetrieveUshort (
//     PUSHORT DESTINATION_ADDRESS
//     PUSHORT SOURCE_ADDRESS
//     )
//
// Routine Description:
//
// This macro retrieves a USHORT value from the SOURCE address, avoiding
// alignment faults.  The DESTINATION address is assumed to be aligned.
//
// Arguments:
//
//     DESTINATION_ADDRESS - where to store USHORT value
//     SOURCE_ADDRESS - where to retrieve USHORT value from
//
// Return Value:
//
//     none.
//
//--

#define RtlRetrieveUshort(DEST_ADDRESS,SRC_ADDRESS)                   \
         if ((ULONG)SRC_ADDRESS & SHORT_MASK) {                       \
             ((PUCHAR) DEST_ADDRESS)[0] = ((PUCHAR) SRC_ADDRESS)[0];  \
             ((PUCHAR) DEST_ADDRESS)[1] = ((PUCHAR) SRC_ADDRESS)[1];  \
         }                                                            \
         else {                                                       \
             *((PUSHORT) DEST_ADDRESS) = *((PUSHORT) SRC_ADDRESS);    \
         }                                                            \

//++
//
// VOID
// RtlStoreUshort (
//     PUSHORT ADDRESS
//     USHORT VALUE
//     )
//
// Routine Description:
//
// This macro stores a USHORT value in at a particular address, avoiding
// alignment faults.
//
// Arguments:
//
//     ADDRESS - where to store USHORT value
//     VALUE - USHORT to store
//
// Return Value:
//
//     none.
//
//--

#define RtlStoreUshort(ADDRESS,VALUE)                     \
         if ((ULONG)ADDRESS & SHORT_MASK) {               \
             ((PUCHAR) ADDRESS)[SHORT_LEAST_SIGNIFICANT_BIT] = (UCHAR)(FIRSTBYTE(VALUE));    \
             ((PUCHAR) ADDRESS)[SHORT_MOST_SIGNIFICANT_BIT ] = (UCHAR)(SECONDBYTE(VALUE));   \
         }                                                \
         else {                                           \
             *((PUSHORT) ADDRESS) = (USHORT) VALUE;       \
         }



//------------------------------------------------------------------------//
// Following constants are copied from NTSTATUS.H
//------------------------------------------------------------------------//


//
// The success status codes 0 - 63 are reserved for wait completion status.
//
#define STATUS_SUCCESS                          ((NTSTATUS)0x00000000L)

//
// MessageId: STATUS_INVALID_PARAMETER
//
// MessageText:
//
//  An invalid parameter was passed to the a service or function.
//
#define STATUS_INVALID_PARAMETER         ((NTSTATUS)0xC000000DL)


//
// MessageId: STATUS_UNSUPPORTED_COMPRESSION
//
// MessageText:
//
//  The specified compression format is unsupported.
//
#define STATUS_UNSUPPORTED_COMPRESSION   ((NTSTATUS)0xC000025FL)


//
// MessageId: STATUS_BAD_COMPRESSION_BUFFER
//
// MessageText:
//
//  The specified buffer contains ill-formed data.
//
#define STATUS_BAD_COMPRESSION_BUFFER    ((NTSTATUS)0xC0000242L)


// MessageId: STATUS_BUFFER_ALL_ZEROS
//
// MessageText:
//
//  Specified buffer contains all zeros.
//
#define STATUS_BUFFER_ALL_ZEROS          ((NTSTATUS)0x00000117L)

// MessageId: STATUS_BUFFER_TOO_SMALL
//
// MessageText:
//
//  {Buffer Too Small}
//  The buffer is too small to contain the entry.  No information has been
//  written to the buffer.
//
#define STATUS_BUFFER_TOO_SMALL          ((NTSTATUS)0xC0000023L)

//
// MessageId: STATUS_NOT_SUPPORTED
//
// MessageText:
//
//  The network request is not supported.
//
#define STATUS_NOT_SUPPORTED             ((NTSTATUS)0xC00000BBL)

//
// MessageId: STATUS_NO_MORE_ENTRIES
//
// MessageText:
//
//  {No More Entries}
//  No more entries are available from an enumeration operation.
//
#define STATUS_NO_MORE_ENTRIES           ((NTSTATUS)0x8000001AL)

//------------------------------------------------------------------------//
//          Following constants are copied from NTDEF.H                   //
//------------------------------------------------------------------------//

//
// NTSTATUS
//

typedef LONG NTSTATUS;
/*lint -e624 */  // Don't complain about different typedefs.   // winnt
typedef NTSTATUS *PNTSTATUS;
/*lint +e624 */  // Resume checking for different typedefs.    // winnt

//
//  Status values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-------------------------+-------------------------------+
//  |Sev|C|       Facility          |               Code            |
//  +---+-+-------------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//

//
// Generic test for success on any status value (non-negative numbers
// indicate success).
//

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)


typedef struct _LZNT1_STANDARD_WORKSPACE {

    PUCHAR UncompressedBuffer;
    PUCHAR EndOfUncompressedBufferPlus1;
    ULONG  MaxLength;
    PUCHAR MatchedString;

    PUCHAR IndexPTable[4096][2];

} LZNT1_STANDARD_WORKSPACE, *PLZNT1_STANDARD_WORKSPACE;

typedef struct _LZNT1_MAXIMUM_WORKSPACE {

    PUCHAR UncompressedBuffer;
    PUCHAR EndOfUncompressedBufferPlus1;
    ULONG  MaxLength;
    PUCHAR MatchedString;

} LZNT1_MAXIMUM_WORKSPACE, *PLZNT1_MAXIMUM_WORKSPACE;

typedef struct _LZNT1_FRAGMENT_WORKSPACE {

    UCHAR Buffer[0x1000];

} LZNT1_FRAGMENT_WORKSPACE, *PLZNT1_FRAGMENT_WORKSPACE;

//
//  Now define the local procedure prototypes.
//

typedef ULONG (*PLZNT1_MATCH_FUNCTION) (PUCHAR ,PVOID);


typedef NTSTATUS (*PRTL_COMPRESS_WORKSPACE_SIZE)(IN USHORT Engine, OUT PULONG CompressBufferWorkSpaceSize,OUT PULONG CompressFragmentWorkSpaceSize);
typedef NTSTATUS (*PRTL_COMPRESS_BUFFER)(IN USHORT Engine, IN PUCHAR UncompressedBuffer,
    IN ULONG UncompressedBufferSize,
    OUT PUCHAR CompressedBuffer,
    IN ULONG CompressedBufferSize,
    IN ULONG UncompressedChunkSize,
    OUT PULONG FinalCompressedSize,
    IN PVOID WorkSpace
    );

typedef NTSTATUS (*PRTL_DECOMPRESS_BUFFER)( OUT PUCHAR UncompressedBuffer,
    IN ULONG UncompressedBufferSize,
    IN PUCHAR CompressedBuffer,
    IN ULONG CompressedBufferSize,
    OUT PULONG FinalUncompressedSize
    );

typedef NTSTATUS (*PRTL_DECOMPRESS_FRAGMENT)(
    OUT PUCHAR UncompressedFragment,
    IN ULONG UncompressedFragmentSize,
    IN PUCHAR CompressedBuffer,
    IN ULONG CompressedBufferSize,
    IN ULONG FragmentOffset,
    OUT PULONG FinalUncompressedSize,
    IN PLZNT1_FRAGMENT_WORKSPACE WorkSpace
    );

typedef NTSTATUS(*PRTL_DESCRIBE_CHUNK) (
    IN OUT PUCHAR *CompressedBuffer,
    IN PUCHAR EndOfCompressedBufferPlus1,
    OUT PUCHAR *ChunkBuffer,
    OUT PULONG ChunkSize
    );

typedef NTSTATUS (*PRTL_RESERVE_CHUNK) (
    IN OUT PUCHAR *CompressedBuffer,
    IN PUCHAR EndOfCompressedBufferPlus1,
    OUT PUCHAR *ChunkBuffer,
    IN ULONG ChunkSize
    );

//
//  Compressed Data Information structure.  This structure is
//  used to describe the state of a compressed data buffer,
//  whose uncompressed size is known.  All compressed chunks
//  described by this structure must be compressed with the
//  same format.  On compressed reads, this entire structure
//  is an output, and on compressed writes the entire structure
//  is an input.
//

typedef struct _COMPRESSED_DATA_INFO {

    //
    //  Code for the compression format (and engine) as
    //  defined in ntrtl.h.  Note that COMPRESSION_FORMAT_NONE
    //  and COMPRESSION_FORMAT_DEFAULT are invalid if
    //  any of the described chunks are compressed.
    //

    USHORT CompressionFormatAndEngine;

    //
    //  Since chunks and compression units are expected to be
    //  powers of 2 in size, we express then log2.  So, for
    //  example (1 << ChunkShift) == ChunkSizeInBytes.  The
    //  ClusterShift indicates how much space must be saved
    //  to successfully compress a compression unit - each
    //  successfully compressed compression unit must occupy
    //  at least one cluster less in bytes than an uncompressed
    //  compression unit.
    //

    UCHAR CompressionUnitShift;
    UCHAR ChunkShift;
    UCHAR ClusterShift;
    UCHAR Reserved;

    //
    //  This is the number of entries in the CompressedChunkSizes
    //  array.
    //

    USHORT NumberOfChunks;

    //
    //  This is an array of the sizes of all chunks resident
    //  in the compressed data buffer.  There must be one entry
    //  in this array for each chunk possible in the uncompressed
    //  buffer size.  A size of FSRTL_CHUNK_SIZE indicates the
    //  corresponding chunk is uncompressed and occupies exactly
    //  that size.  A size of 0 indicates that the corresponding
    //  chunk contains nothing but binary 0's, and occupies no
    //  space in the compressed data.  All other sizes must be
    //  less than FSRTL_CHUNK_SIZE, and indicate the exact size
    //  of the compressed data in bytes.
    //

    ULONG CompressedChunkSizes[ANYSIZE_ARRAY];

} COMPRESSED_DATA_INFO;
typedef COMPRESSED_DATA_INFO *PCOMPRESSED_DATA_INFO;


//--------------------------------------------------------------------//
// Prototypes of functions in LZNT1.C
//--------------------------------------------------------------------//

NTSTATUS
RtlCompressWorkSpaceSizeLZNT1 (
    IN USHORT Engine,
    OUT PULONG CompressBufferWorkSpaceSize,
    OUT PULONG CompressFragmentWorkSpaceSize
    );

NTSTATUS
RtlCompressBufferLZNT1 (
    IN USHORT Engine,
    IN PUCHAR UncompressedBuffer,
    IN ULONG UncompressedBufferSize,
    OUT PUCHAR CompressedBuffer,
    IN ULONG CompressedBufferSize,
    IN ULONG UncompressedChunkSize,
    OUT PULONG FinalCompressedSize,
    IN PVOID WorkSpace
    );
NTSTATUS
RtlDecompressBufferLZNT1 (
    OUT PUCHAR UncompressedBuffer,
    IN ULONG UncompressedBufferSize,
    IN PUCHAR CompressedBuffer,
    IN ULONG CompressedBufferSize,
    OUT PULONG FinalUncompressedSize
    );
NTSTATUS
RtlDecompressFragmentLZNT1 (
    OUT PUCHAR UncompressedFragment,
    IN ULONG UncompressedFragmentSize,
    IN PUCHAR CompressedBuffer,
    IN ULONG CompressedBufferSize,
    IN ULONG FragmentOffset,
    OUT PULONG FinalUncompressedSize,
    IN PLZNT1_FRAGMENT_WORKSPACE WorkSpace
    );

NTSTATUS
RtlDescribeChunkLZNT1 (
    IN OUT PUCHAR *CompressedBuffer,
    IN PUCHAR EndOfCompressedBufferPlus1,
    OUT PUCHAR *ChunkBuffer,
    OUT PULONG ChunkSize
    );

NTSTATUS
RtlReserveChunkLZNT1 (
    IN OUT PUCHAR *CompressedBuffer,
    IN PUCHAR EndOfCompressedBufferPlus1,
    OUT PUCHAR *ChunkBuffer,
    IN ULONG ChunkSize
    );


//--------------------------------------------------------------------//
// Prototypes of functions in COMPRESS.C
//--------------------------------------------------------------------//

NTSTATUS
RtlCompressBuffer (
    IN USHORT CompressionFormatAndEngine,
    IN PUCHAR UncompressedBuffer,
    IN ULONG UncompressedBufferSize,
    OUT PUCHAR CompressedBuffer,
    IN ULONG CompressedBufferSize,
    IN ULONG UncompressedChunkSize,
    OUT PULONG FinalCompressedSize,
    IN PVOID WorkSpace
    );


NTSTATUS
RtlDecompressBuffer (
    IN USHORT CompressionFormat,
    OUT PUCHAR UncompressedBuffer,
    IN ULONG UncompressedBufferSize,
    IN PUCHAR CompressedBuffer,
    IN ULONG CompressedBufferSize,
    OUT PULONG FinalUncompressedSize
    );

#endif



