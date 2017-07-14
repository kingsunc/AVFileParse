#include "StszBox.h"

StszBox::StszBox()
{
}

StszBox::~StszBox()
{
}

BaseBox* StszBox::CreateObject()
{
	return new StszBox();
}

int StszBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	m_iVersion = BytesToIntEx(pData, STSZ_VERSION_SIZE);
	m_iFlag = BytesToIntEx(pData, STSZ_Flag_SIZE);
	m_iSampleSize = BytesToIntEx(pData, STSZ_SAMPLESIZE_SIZE);
	m_iSampleCount = BytesToIntEx(pData, STSZ_SAMPLECOUNT_SIZE);
	for (int i = 0; i < m_iSampleCount; i++)
	{
		int iTemp = BytesToIntEx(pData, STSZ_VECSAMPLESIZE_SIZE);
		m_vecSampleSize.push_back(iTemp);
	}

	// ´òÓ¡ÏÔÊ¾;
	printf("\t Version     \t : %d \n", m_iVersion);
	printf("\t Flag        \t : %d \n", m_iFlag);
	printf("\t SampleSize  \t : %d \n", m_iSampleSize);
	printf("\t SampleCount \t : %d \n", m_iSampleCount);
	int iPrintfSize = min(m_iSampleCount, 10);
	for (int j = 0; j < iPrintfSize; j++)
	{
		printf("\t SampleSize   \t : %9d  \n", m_vecSampleSize[j]);
	}
	printf("\t ... \n");

	return pData - pBegin;
}
