#pragma once

#include "AllVdsDiskVolumeInfo.h"

#include "SingleVdsVolumeInfoXml.h"
#include "SingleVdsDiskInfoXml.h"


//m_pVdsDiskInfos
//m_pVdsVolumeInfos




struct CAllVdsDiskVolumeInfoXml
{
	SERIALIZE_BEGIN_STRUCT(CAllVdsDiskVolumeInfoXml, CAllVdsDiskVolumeInfoXml);
	vector<CSingleVdsDiskInfoXml> m_pVdsDiskInfos;			SERIALIZE_COMPLEX_VECTOR(m_pVdsDiskInfos);
	vector<CSingleVdsVolumeInfoXml> m_pVdsVolumeInfos;		SERIALIZE_COMPLEX_VECTOR(m_pVdsVolumeInfos);
	SERIALIZE_END_STRUCT(CAllVdsDiskVolumeInfoXml, CAllVdsDiskVolumeInfoXml);
	STRUCT_XML_SERIALIZER;


	CAllVdsDiskVolumeInfoXml()
	{
	}
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST CAllVdsDiskVolumeInfo & src, CAllVdsDiskVolumeInfoXml & target)
	{
		target.m_pVdsDiskInfos.resize(src.m_pVdsDiskInfos.size());
		for (size_t ii = 0; ii < target.m_pVdsDiskInfos.size(); ++ii)
		{
			if (src.m_pVdsDiskInfos[ii])
			{
				Bin2Xml(*src.m_pVdsDiskInfos[ii], target.m_pVdsDiskInfos[ii]);
			}
		}


		target.m_pVdsVolumeInfos.resize(src.m_pVdsVolumeInfos.size());
		for (size_t ii = 0; ii < target.m_pVdsVolumeInfos.size(); ++ii)
		{
			if (src.m_pVdsVolumeInfos[ii])
			{
				Bin2Xml(*src.m_pVdsVolumeInfos[ii], target.m_pVdsVolumeInfos[ii]);
			}
		}
	}

	static void Xml2Bin(CONST CAllVdsDiskVolumeInfoXml & src, CAllVdsDiskVolumeInfo & target)
	{
		target.m_pVdsDiskInfos.resize(src.m_pVdsDiskInfos.size());
		for (size_t ii = 0; ii < target.m_pVdsDiskInfos.size(); ++ii)
		{
			target.m_pVdsDiskInfos[ii] = new CSingleVdsDiskInfo;
			Xml2Bin(src.m_pVdsDiskInfos[ii], *target.m_pVdsDiskInfos[ii]);
		}


		target.m_pVdsVolumeInfos.resize(src.m_pVdsVolumeInfos.size());
		for (size_t ii = 0; ii < target.m_pVdsVolumeInfos.size(); ++ii)
		{
			target.m_pVdsVolumeInfos[ii] = new CSingleVdsVolumeInfo;
			Xml2Bin(src.m_pVdsVolumeInfos[ii], *target.m_pVdsVolumeInfos[ii]);
		}
	}
}