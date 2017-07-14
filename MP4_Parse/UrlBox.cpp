#include "UrlBox.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UrlBox
UrlBox::UrlBox()
{
}

UrlBox::~UrlBox()
{
}

int UrlBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	m_iVersion = BytesToIntEx(pData, URL_VERSION_SIZE);
	m_iFlags = BytesToIntEx(pData, URL_FLAGS_SIZE);

	printf("\t Version       \t : %d \n", m_iVersion);
	printf("\t Flags         \t : %d \n", m_iFlags);

	return pData - pBegin;
}

BaseBox* UrlBox::CreateObject()
{
	return new UrlBox();
}
