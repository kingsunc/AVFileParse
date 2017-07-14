#include "FtypBox.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FtypBox
FtypBox::FtypBox()
{
	m_iMinorVersion = 0;
	m_vecCompBrands.clear();
}

FtypBox::~FtypBox()
{
	m_vecCompBrands.clear();
}

int FtypBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;
	m_strMajorBrand = BytesToStrEx(pData, FTYP_MAJORBRAND_SIZE);
	m_iMinorVersion = BytesToIntEx(pData, FTYP_MINORVERSION_SIZE);
	printf("\t MajorBrand    \t : %s \n", m_strMajorBrand.c_str());
	printf("\t MinorVersion  \t : %d \n", m_iMinorVersion);

	const int iCompatibleSize = m_iBoxSize - (BOXHEADER_SIZE + FTYP_MAJORBRAND_SIZE + FTYP_MINORVERSION_SIZE);
	m_vecCompBrands.clear();
	printf("\t CompatibleBrand : ");
	for (int i = 0; i < iCompatibleSize; i = i + FTYP_COMPBRAND_SIZE)
	{
		string strTemp = BytesToStrEx(pData, FTYP_COMPBRAND_SIZE);
		m_vecCompBrands.push_back(strTemp);
		printf("%s ", strTemp.c_str());
	}
	printf("\n\n");

	return pData - pBegin;
}

BaseBox* FtypBox::CreateObject()
{
	return new FtypBox();
}
