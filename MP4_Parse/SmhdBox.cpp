#include "SmhdBox.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SmhdBox
SmhdBox::SmhdBox()
{
}

SmhdBox::~SmhdBox()
{
}

int SmhdBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	m_iVersion = BytesToIntEx(pData, SMHD_VERSION_SIZE);
	m_iFlags = BytesToIntEx(pData, SMHD_FLAGS_SIZE);
	m_dBalance = BytesToDoubleEx(pData, SMHD_BALANCE_SIZE / 2, SMHD_BALANCE_SIZE / 2);
	m_strReserved = BytesToStrEx(pData, SMHD_RESERVED_SIZE);

	printf("\t Version       \t : %d \n", m_iVersion);
	printf("\t Flags         \t : %d \n", m_iFlags);
	printf("\t Balance       \t : %f \n", m_dBalance);
	printf("\t Reserved      \t : %s \n", m_strReserved.c_str());

	return pData - pBegin;
}

BaseBox* SmhdBox::CreateObject()
{
	return new SmhdBox();
}
