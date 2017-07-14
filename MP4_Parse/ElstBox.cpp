#include "ElstBox.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ElstBox
ElstBox::ElstBox()
{
}

ElstBox::~ElstBox()
{
}

int ElstBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;
	printf(" ElstBox::ParseAttrs ????? \n");
	pData += m_iBoxSize - BOXHEADER_SIZE;

	return pData - pBegin;
}

BaseBox* ElstBox::CreateObject()
{
	return new ElstBox();
}
