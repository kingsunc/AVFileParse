#include "TkhdBox.h"

TkhdBox::TkhdBox()
{
}

TkhdBox::~TkhdBox()
{
}

int TkhdBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	m_iVersion = BytesToIntEx(pData, TKHD_VERSION_SIZE);
	m_iFlags = BytesToIntEx(pData, TKHD_FLAGS_SIZE);
	m_iCreateTime = BytesToIntEx(pData, TKHD_CREATETIME_SIZE);
	m_iModifyTime = BytesToIntEx(pData, TKHD_MODIFYTIME_SIZE);
	m_iTrackID = BytesToIntEx(pData, TKHD_TRACKID_SIZE);
	m_strReserved1 = BytesToStrEx(pData, TKHD_RESERVED1_SIZE);
	m_iDuration = BytesToIntEx(pData, TKHD_DURATION_SIZE);
	m_strReserved2 = BytesToStrEx(pData, TKHD_RESERVED2_SIZE);
	m_iLayer = BytesToIntEx(pData, TKHD_LAYER_SIZE);
	m_iAlterGroup = BytesToIntEx(pData, TKHD_ALTERGROUP_SIZE);
	m_dVolume = BytesToDoubleEx(pData, TKHD_VOLUME_SIZE / 2, TKHD_VOLUME_SIZE / 2);
	m_strReserved3 = BytesToStrEx(pData, TKHD_RESERVED3_SIZE);
	m_strMatrix = BytesToStrEx(pData, TKHD_MATRIX_SIZE);
	m_dWidth = BytesToDoubleEx(pData, TKHD_WIDTH_SIZE / 2, TKHD_WIDTH_SIZE / 2);
	m_dHeight = BytesToDoubleEx(pData, TKHD_HEIGHT_SIZE / 2, TKHD_HEIGHT_SIZE / 2);

	printf("\t Version       \t : %d \n", m_iVersion);
	printf("\t Flags         \t : %d \n", m_iFlags);
	printf("\t CreateTime    \t : %d \n", m_iCreateTime);
	printf("\t ModifyTime    \t : %d \n", m_iModifyTime);
	printf("\t TrackID       \t : %d \n", m_iTrackID);
	printf("\t Reserved1     \t : %s \n", m_strReserved1.c_str());
	printf("\t Duration      \t : %d \n", m_iDuration);
	printf("\t Reserved2     \t : %s \n", m_strReserved2.c_str());
	printf("\t Layer         \t : %d \n", m_iLayer);
	printf("\t AlterGroup    \t : %d \n", m_iAlterGroup);
	printf("\t Volume        \t : %f \n", m_dVolume);
	printf("\t Reserved3     \t : %s \n", m_strReserved3.c_str());
	printf("\t Matrix        \t : %s \n", m_strMatrix.c_str());
	printf("\t Width        \t : %f \n", m_dWidth);
	printf("\t Height       \t : %f \n", m_dHeight);

	return pData - pBegin;
}

BaseBox* TkhdBox::CreateObject()
{
	return new TkhdBox();
}
