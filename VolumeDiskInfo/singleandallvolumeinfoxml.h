#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

//////////////////////////////////////////////////////////////////////////
#include "..\PublicHeader\PublicDTraceLib.h"


#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"

#include "..\PublicHeader\PublicSimpleXml.h"
#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicValueXmlElementConvert.h"
#include "..\PublicHeader\PublicParameterStreamDef.h"
//////////////////////////////////////////////////////////////////////////

#include "SingleAndAllVolumeInfo.h"

struct CGetVolumeInformationXml
{
	SERIALIZE_BEGIN_STRUCT(CGetVolumeInformationXml, CGetVolumeInformationXml);
	CString m_VolumeNameBuffer;				SERIALIZE_BASIC(m_VolumeNameBuffer);
	DWORD m_VolumeSerialNumber;				SERIALIZE_BASIC(m_VolumeSerialNumber);
	DWORD m_MaximumComponentLength;			SERIALIZE_BASIC(m_MaximumComponentLength);
	DWORD m_FileSystemFlags;				SERIALIZE_BASIC(m_FileSystemFlags);
	CString m_FileSystemNameBuffer;			SERIALIZE_BASIC(m_FileSystemNameBuffer);
	SERIALIZE_END_STRUCT(CGetVolumeInformationXml, CGetVolumeInformationXml);
	STRUCT_XML_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	CGetVolumeInformationXml()
	{
		m_VolumeSerialNumber = 0;
		m_MaximumComponentLength = 0;
		m_FileSystemFlags = 0;
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST CGetVolumeInformation & src, CGetVolumeInformationXml & target)
	{
		target.m_VolumeNameBuffer = src.m_VolumeNameBuffer;
		target.m_VolumeSerialNumber = src.m_VolumeSerialNumber;
		target.m_MaximumComponentLength = src.m_MaximumComponentLength;
		target.m_FileSystemFlags = src.m_FileSystemFlags;
		target.m_FileSystemNameBuffer = src.m_FileSystemNameBuffer;
	}

	static void Xml2Bin(CONST CGetVolumeInformationXml& src, CGetVolumeInformation & target)
	{
		_tcscpy_s(target.m_VolumeNameBuffer, src.m_VolumeNameBuffer.GetString());
		target.m_VolumeSerialNumber = src.m_VolumeSerialNumber;
		target.m_MaximumComponentLength = src.m_MaximumComponentLength;
		target.m_FileSystemFlags = src.m_FileSystemFlags;
		_tcscpy_s(target.m_FileSystemNameBuffer, src.m_FileSystemNameBuffer.GetString());
	}
}


//////////////////////////////////////////////////////////////////////////

struct NTFS_VOLUME_DATA_BUFFER_Xml
{
	SERIALIZE_BEGIN_STRUCT(NTFS_VOLUME_DATA_BUFFER_Xml, NTFS_VOLUME_DATA_BUFFER_Xml);
	LONGLONG VolumeSerialNumber;			SERIALIZE_BASIC(VolumeSerialNumber);
	LONGLONG NumberSectors;					SERIALIZE_BASIC(NumberSectors);
	LONGLONG TotalClusters;					SERIALIZE_BASIC(TotalClusters);
	LONGLONG FreeClusters;					SERIALIZE_BASIC(FreeClusters);
	LONGLONG TotalReserved;					SERIALIZE_BASIC(TotalReserved);
	DWORD BytesPerSector;					SERIALIZE_BASIC(BytesPerSector);
	DWORD BytesPerCluster;					SERIALIZE_BASIC(BytesPerCluster);
	DWORD BytesPerFileRecordSegment;		SERIALIZE_BASIC(BytesPerFileRecordSegment);
	DWORD ClustersPerFileRecordSegment;		SERIALIZE_BASIC(ClustersPerFileRecordSegment);
	LONGLONG MftValidDataLength;			SERIALIZE_BASIC(MftValidDataLength);
	LONGLONG MftStartLcn;					SERIALIZE_BASIC(MftStartLcn);
	LONGLONG Mft2StartLcn;					SERIALIZE_BASIC(Mft2StartLcn);
	LONGLONG MftZoneStart;					SERIALIZE_BASIC(MftZoneStart);
	LONGLONG MftZoneEnd;					SERIALIZE_BASIC(MftZoneEnd);
	SERIALIZE_END_STRUCT(NTFS_VOLUME_DATA_BUFFER_Xml, NTFS_VOLUME_DATA_BUFFER_Xml);
	STRUCT_XML_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	NTFS_VOLUME_DATA_BUFFER_Xml()
	{
		ZeroMemory(this, sizeof(NTFS_VOLUME_DATA_BUFFER_Xml));
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST NTFS_VOLUME_DATA_BUFFER & src, NTFS_VOLUME_DATA_BUFFER_Xml & target)
	{
		target.VolumeSerialNumber = src.VolumeSerialNumber.QuadPart;
		target.NumberSectors = src.NumberSectors.QuadPart;
		target.TotalClusters = src.TotalClusters.QuadPart;
		target.FreeClusters = src.FreeClusters.QuadPart;
		target.TotalReserved = src.TotalReserved.QuadPart;
		target.BytesPerSector = src.BytesPerSector;
		target.BytesPerCluster = src.BytesPerCluster;
		target.BytesPerFileRecordSegment = src.BytesPerFileRecordSegment;
		target.ClustersPerFileRecordSegment = src.ClustersPerFileRecordSegment;
		target.MftValidDataLength = src.MftValidDataLength.QuadPart;
		target.MftStartLcn = src.MftStartLcn.QuadPart;
		target.Mft2StartLcn = src.Mft2StartLcn.QuadPart;
		target.MftZoneStart = src.MftZoneStart.QuadPart;
		target.MftZoneEnd = src.MftZoneEnd.QuadPart;
	}

	static void Xml2Bin(CONST NTFS_VOLUME_DATA_BUFFER_Xml& src, NTFS_VOLUME_DATA_BUFFER & target)
	{
		target.VolumeSerialNumber.QuadPart = src.VolumeSerialNumber;
		target.NumberSectors.QuadPart = src.NumberSectors;
		target.TotalClusters.QuadPart = src.TotalClusters;
		target.FreeClusters.QuadPart = src.FreeClusters;
		target.TotalReserved.QuadPart = src.TotalReserved;
		target.BytesPerSector = src.BytesPerSector;
		target.BytesPerCluster = src.BytesPerCluster;
		target.BytesPerFileRecordSegment = src.BytesPerFileRecordSegment;
		target.ClustersPerFileRecordSegment = src.ClustersPerFileRecordSegment;
		target.MftValidDataLength.QuadPart = src.MftValidDataLength;
		target.MftStartLcn.QuadPart = src.MftStartLcn;
		target.Mft2StartLcn.QuadPart = src.Mft2StartLcn;
		target.MftZoneStart.QuadPart = src.MftZoneStart;
		target.MftZoneEnd.QuadPart = src.MftZoneEnd;
	}
}

struct DISK_EXTENT_Xml
{
	SERIALIZE_BEGIN_STRUCT(DISK_EXTENT_Xml, DISK_EXTENT_Xml);
	DWORD DiskNumber;				SERIALIZE_BASIC(DiskNumber);
	LONGLONG StartingOffset;		SERIALIZE_BASIC(StartingOffset);
	LONGLONG ExtentLength;			SERIALIZE_BASIC(ExtentLength);
	SERIALIZE_END_STRUCT(DISK_EXTENT_Xml, DISK_EXTENT_Xml);
	STRUCT_XML_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	DISK_EXTENT_Xml()
	{
		ZeroMemory(this, sizeof(DISK_EXTENT_Xml));
	}
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST DISK_EXTENT & src, DISK_EXTENT_Xml & target)
	{
		target.DiskNumber = src.DiskNumber;
		target.StartingOffset = src.StartingOffset.QuadPart;
		target.ExtentLength = src.ExtentLength.QuadPart;

	}

	static void Xml2Bin(CONST DISK_EXTENT_Xml& src, DISK_EXTENT & target)
	{
		target.DiskNumber = src.DiskNumber;
		target.StartingOffset.QuadPart = src.StartingOffset;
		target.ExtentLength.QuadPart = src.ExtentLength;
	}
}

struct VOLUME_DISK_EXTENTS_Xml
{
	SERIALIZE_BEGIN_STRUCT(VOLUME_DISK_EXTENTS_Xml, VOLUME_DISK_EXTENTS_Xml);
	DWORD NumberOfDiskExtents;			SERIALIZE_BASIC(NumberOfDiskExtents);
	vector<DISK_EXTENT_Xml> Extents;	SERIALIZE_COMPLEX_VECTOR(Extents);
	SERIALIZE_END_STRUCT(VOLUME_DISK_EXTENTS_Xml, VOLUME_DISK_EXTENTS_Xml);
	STRUCT_XML_SERIALIZER;

	VOLUME_DISK_EXTENTS_Xml()
	{
		NumberOfDiskExtents = 0;
	}
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST VOLUME_DISK_EXTENTS & src, VOLUME_DISK_EXTENTS_Xml & target)
	{
		target.NumberOfDiskExtents = src.NumberOfDiskExtents;
		target.Extents.resize(target.NumberOfDiskExtents);
		for (size_t ii = 0; ii < target.NumberOfDiskExtents; ++ ii)
		{
			Bin2Xml(src.Extents[ii], target.Extents[ii]);
		}
	}

	static void Xml2Bin(CONST VOLUME_DISK_EXTENTS_Xml & src, VOLUME_DISK_EXTENTS & target)
	{
		target.NumberOfDiskExtents = src.NumberOfDiskExtents;
		for (size_t ii = 0; ii < target.NumberOfDiskExtents; ++ii)
		{
			Xml2Bin(src.Extents[ii], target.Extents[ii]);
		}
	}
}


struct VOLUME_BITMAP_BUFFER_Xml
{
	SERIALIZE_BEGIN_STRUCT(VOLUME_BITMAP_BUFFER_Xml, VOLUME_BITMAP_BUFFER_Xml);
	LONGLONG StartingLcn;			SERIALIZE_BASIC(StartingLcn);
	LONGLONG BitmapSize;			SERIALIZE_BASIC(BitmapSize);
	VectorBYTE  Buffer;			SERIALIZE_BASIC(Buffer);
	SERIALIZE_END_STRUCT(VOLUME_BITMAP_BUFFER_Xml, VOLUME_BITMAP_BUFFER_Xml);
	STRUCT_XML_SERIALIZER;

	VOLUME_BITMAP_BUFFER_Xml()
	{
		StartingLcn = 0;
		BitmapSize = 0;
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST VOLUME_BITMAP_BUFFER & src, VOLUME_BITMAP_BUFFER_Xml & target)
	{
		target.StartingLcn = src.StartingLcn.QuadPart;
		target.BitmapSize = src.BitmapSize.QuadPart;
		BufferToVectorHelper(src.Buffer, AtlAlignUp(src.BitmapSize.QuadPart, 8) / 8, target.Buffer/*.Buffer*/);
	}

	static void Xml2Bin(CONST VOLUME_BITMAP_BUFFER_Xml & src, VOLUME_BITMAP_BUFFER & target)
	{
		target.StartingLcn.QuadPart = src.StartingLcn;
		target.BitmapSize.QuadPart = src.BitmapSize;
		VectorToBufferHelper(target.Buffer, AtlAlignUp(target.BitmapSize.QuadPart, 8) / 8, src.Buffer/*.Buffer*/);
	}
}


struct GET_LENGTH_INFORMATION_Xml
{
	SERIALIZE_BEGIN_STRUCT(GET_LENGTH_INFORMATION_Xml, GET_LENGTH_INFORMATION_Xml);
	LONGLONG Length;		SERIALIZE_BASIC(Length);
	SERIALIZE_END_STRUCT(GET_LENGTH_INFORMATION_Xml, GET_LENGTH_INFORMATION_Xml);
	STRUCT_XML_SERIALIZER;

	GET_LENGTH_INFORMATION_Xml()
	{
		Length = 0;
	}
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST GET_LENGTH_INFORMATION & src, GET_LENGTH_INFORMATION_Xml & target)
	{
		target.Length = src.Length.QuadPart;
	}

	static void Xml2Bin(CONST GET_LENGTH_INFORMATION_Xml & src, GET_LENGTH_INFORMATION & target)
	{
		target.Length.QuadPart = src.Length;
	}
}



struct CSingleVolumeInfoXml
{
	SERIALIZE_BEGIN_STRUCT(CSingleVolumeInfoXml, CSingleVolumeInfoXml);
	CString m_strVolumeGUID;									SERIALIZE_BASIC(m_strVolumeGUID);
	vector<CString> m_Paths;									SERIALIZE_VECTOR(m_Paths);
	vector<CString> m_MountPoint;								SERIALIZE_VECTOR(m_MountPoint);
	CGetVolumeInformationXml m_GetVolumeInformation;			SERIALIZE_COMPLEX(m_GetVolumeInformation);
	NTFS_VOLUME_DATA_BUFFER_Xml m_NtfsVolumeDataBuffer;			SERIALIZE_COMPLEX(m_NtfsVolumeDataBuffer);
	VOLUME_DISK_EXTENTS_Xml m_VolumeDiskExtents;				SERIALIZE_COMPLEX(m_VolumeDiskExtents);
	VOLUME_BITMAP_BUFFER_Xml m_VolumeBitmapBuffer;				SERIALIZE_COMPLEX(m_VolumeBitmapBuffer);
	GET_LENGTH_INFORMATION_Xml m_GetLengthInformation;			SERIALIZE_COMPLEX(m_GetLengthInformation);
	SERIALIZE_END_STRUCT(CSingleVolumeInfoXml, CSingleVolumeInfoXml);
	STRUCT_XML_SERIALIZER;
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST CSingleVolumeInfo & src, CSingleVolumeInfoXml & target)
	{
		target.m_strVolumeGUID = src.m_strVolumeGUID;
		target.m_Paths = src.m_Paths;
		target.m_MountPoint = src.m_MountPoint;
		Bin2Xml(src.m_GetVolumeInformation, target.m_GetVolumeInformation);
		Bin2Xml(src.m_NtfsVolumeDataBuffer, target.m_NtfsVolumeDataBuffer);

		if (src.m_pVolumeDiskExtents)
		{
			Bin2Xml(*src.m_pVolumeDiskExtents, target.m_VolumeDiskExtents);
		}
		
		if (src.m_pVolumeBitmapBuffer)
		{
			Bin2Xml(*src.m_pVolumeBitmapBuffer, target.m_VolumeBitmapBuffer);
		}
		
		Bin2Xml(src.m_GetLengthInformation, target.m_GetLengthInformation);
	}

	static void Xml2Bin(CONST CSingleVolumeInfoXml & src, CSingleVolumeInfo & target)
	{
		target.m_strVolumeGUID = src.m_strVolumeGUID;
		target.m_Paths = src.m_Paths;
		target.m_MountPoint = src.m_MountPoint;
		Xml2Bin(src.m_GetVolumeInformation, target.m_GetVolumeInformation);
		Xml2Bin(src.m_NtfsVolumeDataBuffer, target.m_NtfsVolumeDataBuffer);

		{
			CHeapPtr<BYTE> apBuffer;
			size_t allocSize = sizeof(VOLUME_DISK_EXTENTS) + src.m_VolumeDiskExtents.Extents.size() * sizeof(DISK_EXTENT);
			apBuffer.Allocate(allocSize);
			ZeroMemory(apBuffer.m_pData, allocSize);
			target.m_pVolumeDiskExtents = (VOLUME_DISK_EXTENTS*)apBuffer.Detach();
		}
		Xml2Bin(src.m_VolumeDiskExtents, *target.m_pVolumeDiskExtents);


		{
			CHeapPtr<BYTE> apBuffer;
			size_t allocSize = sizeof(VOLUME_BITMAP_BUFFER) + src.m_VolumeBitmapBuffer.Buffer/*.Buffer*/.size();
			apBuffer.Allocate(allocSize);
			ZeroMemory(apBuffer.m_pData, allocSize);
			target.m_pVolumeBitmapBuffer = (VOLUME_BITMAP_BUFFER*)apBuffer.Detach();
		}
		Xml2Bin(src.m_VolumeBitmapBuffer, *target.m_pVolumeBitmapBuffer);

		Xml2Bin(src.m_GetLengthInformation, target.m_GetLengthInformation);
	}
}



struct CAllVolumeInfoXml
{
	SERIALIZE_BEGIN_STRUCT(CAllVolumeInfoXml, CAllVolumeInfoXml);
	vector<CSingleVolumeInfoXml> m_pVolumeInfos; SERIALIZE_COMPLEX_VECTOR(m_pVolumeInfos);
	SERIALIZE_END_STRUCT(CAllVolumeInfoXml, CAllVolumeInfoXml);
	STRUCT_XML_SERIALIZER;
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST CAllVolumeInfo & src, CAllVolumeInfoXml & target)
	{
		target.m_pVolumeInfos.resize(src.m_pVolumeInfos.size());
		for (size_t ii = 0; ii < target.m_pVolumeInfos.size(); ++ ii)
		{
			if (src.m_pVolumeInfos[ii])
			{
				Bin2Xml(*src.m_pVolumeInfos[ii], target.m_pVolumeInfos[ii]);
			}
		}
	}

	static void Xml2Bin(CONST CAllVolumeInfoXml & src, CAllVolumeInfo & target)
	{
		target.m_pVolumeInfos.resize(src.m_pVolumeInfos.size());
		for (size_t ii = 0; ii < target.m_pVolumeInfos.size(); ++ii)
		{
			CAutoPtr<CSingleVolumeInfo> apTemp(new CSingleVolumeInfo);
			Xml2Bin(src.m_pVolumeInfos[ii], *apTemp.m_p);
			target.m_pVolumeInfos[ii] = apTemp.Detach();
		}
	}
}