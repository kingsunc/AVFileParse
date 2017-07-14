#include "SttsBox.h"

SttsBox::SttsBox()
{
}

SttsBox::~SttsBox()
{
}

int SttsBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	m_iEntryCount = BytesToIntEx(pData, STTS_ENTRYCOUNT_SIZE);
	for (int i = 0; i < m_iEntryCount; i++)
	{
		SampleData tempSample;
		tempSample.iSampleCount = BytesToIntEx(pData, STTS_SAMPLECOUNT_SIZE);
		tempSample.iSampleDelta = BytesToIntEx(pData, STTS_SAMPLEDELTA_SIZE);
		m_vecSampleDatas.push_back(tempSample);
	}

	// ´òÓ¡ÏÔÊ¾;
	printf("\t EntryCount  \t : %d \n", m_iEntryCount);
	int iPrintSizes = min(m_vecSampleDatas.size(), 20);
	for (int i = 0; i < iPrintSizes; i++)
	{
		printf(" \t SampleCount : %9d,   SampleDelta : %9d \n", m_vecSampleDatas[i].iSampleCount, m_vecSampleDatas[i].iSampleDelta);
	}
	printf("  \t ... \n");

	return pData - pBegin;
}

BaseBox* SttsBox::CreateObject()
{
	return new SttsBox();
}