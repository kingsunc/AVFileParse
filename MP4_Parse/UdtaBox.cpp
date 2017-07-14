#include "UdtaBox.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UdataBox
UdtaBox::UdtaBox()
{
}

UdtaBox::~UdtaBox()
{
}

int UdtaBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	printf(" UdtaBox::ParseAttrs ??? \n");
	pData += m_iBoxSize - BOXHEADER_SIZE;
	//fseek(pFile, iAttrsSize, SEEK_CUR);
	//char ch[100];
	//fread(ch, 1, 100, pFile);
	return pData - pBegin;
}

BaseBox* UdtaBox::CreateObject()
{
	return new UdtaBox();
}
