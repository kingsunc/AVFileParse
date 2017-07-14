#include "VmhdBox.h"

VmhdBox::VmhdBox()
{
}

VmhdBox::~VmhdBox()
{
}

int VmhdBox::ParseAttrs(byteptr &pData)
{
	byteptr pBeigin = pData;

	m_iVersion = BytesToIntEx(pData, VMHD_VERSION_SIZE);
	m_iFlags = BytesToIntEx(pData, VMHD_FLAGS_SIZE);
	m_iGraphicsMode = BytesToIntEx(pData, VMHD_GRAPHICSMODE_SIZE);
	byteptr pOpcolor = pData;
	pData += VMHD_OPCOLOR_SIZE;

	printf("\t Version       \t : %d \n", m_iVersion);
	printf("\t Flags         \t : %d \n", m_iFlags);
	printf("\t GraphicsMode  \t : %f \n", m_iGraphicsMode);
	printf("\t Opcolor       \t : %02X %02X %02X %02X  \n", pOpcolor[0], pOpcolor[1], pOpcolor[2], pOpcolor[3]);

	return pData - pBeigin;
}

BaseBox* VmhdBox::CreateObject()
{
	return new VmhdBox();
}
