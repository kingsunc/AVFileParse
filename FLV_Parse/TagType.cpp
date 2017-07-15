#include "TagType.h"


CTagType::CTagType()
{
	m_iSize = 0;
	m_pData = NULL;
}

CTagType::~CTagType()
{
	if (m_pData)
	{
		delete m_pData;
		m_pData = NULL;
	}
}

int CTagType::ReadData(byte_ptr &pData, int iTagSize)
{
	if (m_pData)
	{
		delete m_pData;
	}
	m_iSize = iTagSize;
	m_pData = new byte[m_iSize];
	memcpy(m_pData, pData, m_iSize);
	pData += m_iSize;	// 读完数据，后移操作;

	return m_iSize;
}

int CTagType::WriteData(byte_ptr &pData)
{
	memcpy(pData, m_pData, m_iSize);
	pData += m_iSize;	// 写完数据，后移操作;

	return m_iSize;
}

bool CTagType::IsKeyFrame()
{
	return false;
}
