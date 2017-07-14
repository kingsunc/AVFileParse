#include "MdhdBox.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MdhdBox
MdhdBox::MdhdBox()
{
}

MdhdBox::~MdhdBox()
{
}

int MdhdBox::ParseAttrs(byteptr &pData)
{
	byteptr pBeign = pData;

	m_iVersion = BytesToIntEx(pData, MDHD_VERSION_SIZE);
	m_iFlags = BytesToIntEx(pData, MDHD_FLAGS_SIZE);
	m_iCreateTime = BytesToIntEx(pData, MDHD_CREATETIME_SIZE);
	m_iModifyTime = BytesToIntEx(pData, MDHD_MODIFYTIME_SIZE);
	m_iTimeScale = BytesToIntEx(pData, MDHD_TIMESCALE_SIZE);
	m_iDuration = BytesToIntEx(pData, MDHD_DURATION_SIZE);
	m_iLanguage = BytesToIntEx(pData, MDHD_LANGUAGE_SIZE);
	m_iPreDefined = BytesToIntEx(pData, MDHD_PREDEFINED_SIZE);

	printf("\t Version       \t : %d \n", m_iVersion);
	printf("\t Flags         \t : %d \n", m_iFlags);
	printf("\t CreateTime    \t : %d \n", m_iCreateTime);
	printf("\t ModifyTime    \t : %d \n", m_iModifyTime);
	printf("\t TimeScale     \t : %d \n", m_iTimeScale);
	printf("\t Duration      \t : %d \n", m_iDuration);
	printf("\t Language      \t : %d \n", m_iLanguage);
	printf("\t PreDefined    \t : %d \n", m_iPreDefined);

	return pData - pBeign;
}

BaseBox* MdhdBox::CreateObject()
{
	return new MdhdBox();
}
