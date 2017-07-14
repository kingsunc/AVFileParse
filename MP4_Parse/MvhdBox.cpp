#include "MvhdBox.h"


MvhdBox::MvhdBox()
{
}

MvhdBox::~MvhdBox()
{
}

int MvhdBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;
	m_iVersion = BytesToIntEx(pData, MVHD_VERSION_SIZE);
	m_iFlags = BytesToIntEx(pData, MVHD_FLAGS_SIZE);
	m_iCreateTime = BytesToIntEx(pData, MVHD_CREATETIME_SIZE);
	m_iModifyTime = BytesToIntEx(pData, MVHD_MODIFYTIME_SIZE);
	m_iTimeScale = BytesToIntEx(pData, MVHD_TIMESCALE_SIZE);
	m_iDuration = BytesToIntEx(pData, MVHD_DURATION_SIZE);
	m_dRate = BytesToDoubleEx(pData, MVHD_RATE_SIZE / 2, MVHD_RATE_SIZE / 2);
	m_dVolume = BytesToDoubleEx(pData, MVHD_VOLUME_SIZE / 2, MVHD_VOLUME_SIZE / 2);
	m_strReserved = BytesToStrEx(pData, MVHD_RESERVED_SIZE);
	m_strMatrix = BytesToStrEx(pData, MVHD_MATRIX_SIZE);
	m_strPreDefined = BytesToStrEx(pData, MVHD_PREDEFINED_SIZE);
	m_iNextTrackID = BytesToIntEx(pData, MVHD_NEXTTRACKID_SIZE);

	printf("\t Version       \t : %d \n", m_iVersion);
	printf("\t Flags         \t : %d \n", m_iFlags);
	printf("\t CreateTime    \t : %d \n", m_iCreateTime);
	printf("\t ModifyTime    \t : %d \n", m_iModifyTime);
	printf("\t TimeScale     \t : %d \n", m_iTimeScale);
	printf("\t Duration      \t : %d \n", m_iDuration);
	printf("\t Rate          \t : %f \n", m_dRate);
	printf("\t Volume        \t : %f \n", m_dVolume);
	printf("\t Reserved      \t : %s \n", m_strReserved.c_str());
	printf("\t Matrix        \t : %s \n", m_strMatrix.c_str());
	printf("\t PreDefined    \t : %s \n", m_strPreDefined.c_str());
	printf("\t NextTrackID   \t : %d \n", m_iNextTrackID);

	return pData - pBegin;
}

BaseBox* MvhdBox::CreateObject()
{
	return new MvhdBox();
}
