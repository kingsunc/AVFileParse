#include "MdatBox.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MdatBox
MdatBox::MdatBox()
{
	m_pData = NULL;
}

MdatBox::~MdatBox()
{
	if (m_pData)
	{
		delete[]m_pData;
		m_pData = NULL;
	}
}

int MdatBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;
	int iDataSize = m_iBoxSize - BOXHEADER_SIZE;
	if (m_pData)
	{
		delete[]m_pData;
		m_pData = NULL;
	}
	m_pData = new byte[iDataSize];
	memcpy(m_pData, pData, iDataSize);
	pData += iDataSize;

	// 只打印100个做展示;
	printf("\n");
	int iPrintSize = min(iDataSize, 100);
	for (int i = 0; i < iPrintSize; i++)
	{
		printf(" %02X ", m_pData[i]);
	}
	printf(" ... \n");

	return pData - pBegin;
}

BaseBox* MdatBox::CreateObject()
{
	return new MdatBox();
}