#include "StssBox.h"

StssBox::StssBox()
{
}

StssBox::~StssBox()
{
}

BaseBox* StssBox::CreateObject()
{
	return new StssBox();
}

int StssBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	m_iEntryCount = BytesToIntEx(pData, STSS_ENTRYCOUNT_SIZE);
	for (int i = 0; i < m_iEntryCount; i++)
	{
		int iSampleNum = BytesToIntEx(pData, STSS_SAMPLENUM_SIZE);
		m_vecSampleNums.push_back(iSampleNum);
	}

	// ´òÓ¡ÏÔÊ¾;
	printf("\t EntryCount  \t : %d \n", m_iEntryCount);
	int iPrintSizes = min(m_vecSampleNums.size(), 20);
	for (int i = 0; i < iPrintSizes; i++)
	{
		printf(" \t SampleNum    \t : %9d \n", m_vecSampleNums[i]);
	}
	printf("  \t ... \n");

	return pData - pBegin;
}
