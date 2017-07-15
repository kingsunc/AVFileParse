#include "FLVFileBody.h"


CFLVFileBody::CFLVFileBody()
{
	m_vecFlvTags.clear();
}

CFLVFileBody::~CFLVFileBody()
{
	for (VecFlvTag::iterator it = m_vecFlvTags.begin(); it != m_vecFlvTags.end(); it++)
	{
		delete *it;
	}
	m_vecFlvTags.clear();
}

bool CFLVFileBody::ReadData(byte_ptr &pData, int iSize)
{
	int iReadSize = 0;
	while (iReadSize < iSize)
	{
		CFLVTag* pTag = new CFLVTag();
		assert(pTag);
		int iTagSize = pTag->ReadData(pData);
		if (iTagSize > 0 )
		{
			iReadSize += iTagSize;
			m_vecFlvTags.push_back(pTag);
		}
		else
		{
			delete pTag;
			return true;
			// return false;
		}
	}
	
	return true;
}

int CFLVFileBody::WriteData(byte_ptr &pData, double dStart, double dEnd)
{
	int iWriteSize = 0;
	int iTagCount = m_vecFlvTags.size();
	int iBegin = (int)(iTagCount * dStart);
	int iEnd = (int)(iTagCount * dEnd);
	if (iBegin > 0 && m_vecFlvTags[0])
	{
		// 尽量把第一帧的数据加上吧 很多是自定义脚本数据;
		m_vecFlvTags[0]->WriteData(pData);
		m_vecFlvTags[1]->WriteData(pData);
		m_vecFlvTags[2]->WriteData(pData);
	}

	bool bFindKeyFrame = false;
	for (int i = iBegin; i < iEnd; i++)
	{
		if (m_vecFlvTags[i])
		{
			if (!bFindKeyFrame)
			{
				//先找关键帧 再输出;
				if (m_vecFlvTags[i]->IsKeyFrame())
				{
					bFindKeyFrame = true;
				}
				else
				{
					continue;
				}
			}
			int iTagSize = m_vecFlvTags[i]->WriteData(pData);
			if (iTagSize > 0)
			{
				iWriteSize += iTagSize;
			}
		}
	}
	return iWriteSize;
}

void CFLVFileBody::Display()
{
	int iTagCount = m_vecFlvTags.size();
	for (int i = 0; i < iTagCount; i++)
	{
		if (m_vecFlvTags[i])
		{
			m_vecFlvTags[i]->Display();
		}
	}
}
