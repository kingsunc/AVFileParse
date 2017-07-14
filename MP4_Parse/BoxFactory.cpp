#include "BoxFactory.h"
#include "FtypBox.h"
#include "MoovBox.h"
#include "MvhdBox.h"
#include "TrakBox.h"
#include "TkhdBox.h"
#include "EdtsBox.h"
#include "ElstBox.h"
#include "MdiaBox.h"
#include "MdhdBox.h"
#include "HdlrBox.h"
#include "MinfBox.h"
#include "SmhdBox.h"
#include "DinfBox.h"
#include "DrefBox.h"
#include "UrlBox.h"
#include "StblBox.h"
#include "UdtaBox.h"
#include "FreeBox.h"
#include "StsdBox.h"
#include "SttsBox.h"
#include "StscBox.h"
#include "StszBox.h"
#include "StcoBox.h"
#include "MdatBox.h"
#include "VmhdBox.h"
#include "Avc1Box.h"
#include "AvcCBox.h"
#include "StssBox.h"
#include "Mp4aBox.h"
#include "EsdsBox.h"

#define FTYP_NAME		"ftyp"
#define MOOV_NAME		"moov"
#define MVHD_NAME		"mvhd"
#define TRAK_NAME		"trak"
#define TKHD_NAME		"tkhd"
#define EDTS_NAME		"edts"
#define ELST_NAME		"elst"
#define MDIA_NAME		"mdia"
#define MDHD_NAME		"mdhd"
#define HDLR_NAME		"hdlr"
#define MINF_NAME		"minf"
#define SMHD_NAME		"smhd"
#define VMHD_NAME		"vmhd"
#define DINF_NAME		"dinf"
#define DREF_NAME		"dref"
#define URL_NAME		"url "
#define URN_NAME		"urn "
#define STBL_NAME		"stbl"
#define UDTA_NAME		"udta"
#define STSD_NAME		"stsd"
#define STTS_NAME		"stts"
#define STSC_NAME		"stsc"
#define STSS_NAME		"stss"
#define STSZ_NAME		"stsz"
#define STCO_NAME		"stco"
#define AVC1_NAME		"avc1"
#define AVCC_NAME		"avcC"
#define MP4A_NAME		"mp4a"
#define ESDS_NAME		"esds"
#define FREE_NAME		"free"
#define MDAT_NAME		"mdat"

BoxFactory::BoxFactory()
{
	m_mapCreateObj[FTYP_NAME] = FtypBox::CreateObject;
	m_mapCreateObj[MOOV_NAME] = MoovBox::CreateObject;
	m_mapCreateObj[MVHD_NAME] = MvhdBox::CreateObject;
	m_mapCreateObj[TRAK_NAME] = TrakBox::CreateObject;
	m_mapCreateObj[TKHD_NAME] = TkhdBox::CreateObject;
	m_mapCreateObj[EDTS_NAME] = EdtsBox::CreateObject;
	m_mapCreateObj[ELST_NAME] = ElstBox::CreateObject;
	m_mapCreateObj[MDIA_NAME] = MdiaBox::CreateObject;
	m_mapCreateObj[MDHD_NAME] = MdhdBox::CreateObject;
	m_mapCreateObj[HDLR_NAME] = HdlrBox::CreateObject;
	m_mapCreateObj[MINF_NAME] = MinfBox::CreateObject;
	m_mapCreateObj[SMHD_NAME] = SmhdBox::CreateObject;
	m_mapCreateObj[VMHD_NAME] = VmhdBox::CreateObject;
	m_mapCreateObj[DINF_NAME] = DinfBox::CreateObject;
	m_mapCreateObj[DREF_NAME] = DrefBox::CreateObject;
	m_mapCreateObj[URL_NAME]  = UrlBox::CreateObject;
	m_mapCreateObj[URN_NAME]  = UrlBox::CreateObject;
	m_mapCreateObj[STBL_NAME] = StblBox::CreateObject;
	m_mapCreateObj[UDTA_NAME] = UdtaBox::CreateObject;
	m_mapCreateObj[STSD_NAME] = StsdBox::CreateObject;
	m_mapCreateObj[AVC1_NAME] = Avc1Box::CreateObject;
	m_mapCreateObj[AVCC_NAME] = AvcCBox::CreateObject;
	m_mapCreateObj[STTS_NAME] = SttsBox::CreateObject;
	m_mapCreateObj[STSS_NAME] = StssBox::CreateObject;
	m_mapCreateObj[STSC_NAME] = StscBox::CreateObject;
	m_mapCreateObj[STSZ_NAME] = StszBox::CreateObject;
	m_mapCreateObj[STCO_NAME] = StcoBox::CreateObject;
	m_mapCreateObj[MP4A_NAME] = Mp4aBox::CreateObject;
	m_mapCreateObj[ESDS_NAME] = EsdsBox::CreateObject;

	m_mapCreateObj[FREE_NAME] = FreeBox::CreateObject;
	m_mapCreateObj[MDAT_NAME] = MdatBox::CreateObject;
}

BoxFactory::~BoxFactory()
{
}

BoxFactory* BoxFactory::GetInstance()
{
	static BoxFactory boxFactory;
	return &boxFactory;
}

BaseBox* BoxFactory::CreateBox(string strType)
{
	BaseBox* pBox = NULL;
	MAP_BOX_CREATEOBJECT::iterator it = m_mapCreateObj.find(strType);
	if (it != m_mapCreateObj.end())
	{
		return (BaseBox*)(it->second());
	}

	return NULL;
}