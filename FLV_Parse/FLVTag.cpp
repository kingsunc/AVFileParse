#include "FLVTag.h"


CFLVTag::CFLVTag()
{
}

CFLVTag::~CFLVTag()
{
}

int CFLVTag::ReadData(byte_ptr &pData)
{
	int iTagHeaderSize = m_tagHeader.ReadData(pData);
	int iTagBodySize = m_tagBody.ReadData(pData, m_tagHeader.GetTagSize(), m_tagHeader.GetTagType());
	if (iTagBodySize < 0)
	{
		// 出现异常;
		return -1;
	}
	return iTagHeaderSize + iTagBodySize;
}

int CFLVTag::WriteData(byte_ptr &pData)
{
	int iTagHeaderSize = m_tagHeader.WriteData(pData);
	int iTagBodySize = m_tagBody.WriteData(pData);
	if (iTagBodySize < 0)
	{
		// 出现异常;
		return -1;
	}
	return iTagHeaderSize + iTagBodySize;
}

void CFLVTag::Display()
{
	m_tagHeader.Display();
	m_tagBody.Display();
}

bool CFLVTag::IsKeyFrame()
{
	return m_tagBody.IsKeyFrame();
}
