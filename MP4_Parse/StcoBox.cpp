#include "StcoBox.h"

StcoBox::StcoBox()
{
}

StcoBox::~StcoBox()
{
}

BaseBox* StcoBox::CreateObject()
{
	return new StcoBox();
}

int StcoBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	printf(" StcoBox::ParseAttrs ??? \n");
	pData += m_iBoxSize - BOXHEADER_SIZE;

	return pData - pBegin;
}
