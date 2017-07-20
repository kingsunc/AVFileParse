#include "StscBox.h"

StscBox::StscBox()
{
}

StscBox::~StscBox()
{
}

BaseBox* StscBox::CreateObject()
{
	return new StscBox();
}

int StscBox::ParseAttrs(byteptr &pData)
{
	byteptr pBeign = pData;

	m_iVersion = BytesToIntEx(pData, STSC_VERSION_SIZE);
	m_iFlag = BytesToIntEx(pData, STSC_FLAG_SIZE);
	m_iEntryCount = BytesToIntEx(pData, STSC_ENTRYCOUNT_SIZE);
	printf("\t Version   \t : %d \n", m_iVersion);
	printf("\t Flag   \t : %d \n", m_iFlag);
	printf("\t EntryCount   \t : %d \n", m_iEntryCount);

	for (int i = 0; i < m_iEntryCount; i++)
	{
		StscData tempStscData;
		tempStscData.iFirstChunk = BytesToIntEx(pData, STSC_FIRSTCHUNK_SIZE);
		tempStscData.iSampPerChunk = BytesToIntEx(pData, STSC_SAMPPERCHUNK_SIZE);
		tempStscData.iSampDesIndex = BytesToIntEx(pData, STSC_SAMPDESINDEX_SIZE);
		m_vecStscData.push_back(tempStscData);
	}

	int iPrintf = min(m_vecStscData.size(), 10);
	for (int j = 0; j < iPrintf; j++)
	{
		printf("\t FirstChunk : %3d, SampPerChunk: %3d, SampDesIndex : %3d \n",
			m_vecStscData[j].iFirstChunk, m_vecStscData[j].iSampPerChunk, m_vecStscData[j].iSampDesIndex);
	}
	printf("\t ... \n");

	return pData - pBeign;
}
