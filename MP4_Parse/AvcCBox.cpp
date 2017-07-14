#include "AvcCBox.h"

AvcCBox::AvcCBox()
{
}

AvcCBox::~AvcCBox()
{
	{
		for (VecParameters::iterator it = m_vecSpsContent.begin(); it != m_vecSpsContent.end(); it++)
		{
			byteptr pTemp = (*it).pData;
			delete[]pTemp;
		}
		m_vecSpsContent.clear();
	}

	{
		for (VecParameters::iterator it = m_vecPpsContent.begin(); it != m_vecPpsContent.end(); it++)
		{
			byteptr pTemp = (*it).pData;
			delete[]pTemp;
		}
		m_vecPpsContent.clear();
	}
}

BaseBox* AvcCBox::CreateObject()
{
	return new AvcCBox();
}

int AvcCBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	m_iVersion = BytesToIntEx(pData, AVCC_VERSION_SIZE);
	m_iProfile = BytesToIntEx(pData, AVCC_PROFILE_SIZE);
	m_iCompat = BytesToIntEx(pData, AVCC_COMPAT_SIZE);
	m_iLevel = BytesToIntEx(pData, AVCC_LEVEL_SIZE);
	// 只有最后两位是size;
	m_iLenSize = BitsToIntEx(pData, 6, 2);

	printf("\t Version     \t : %d \n", m_iVersion);
	printf("\t Profile     \t : %d \n", m_iProfile);
	printf("\t Compat      \t : %d \n", m_iCompat);
	printf("\t Level       \t : %d \n", m_iLevel);
	printf("\t LenSize     \t : %d \n", m_iLenSize);

	{
		// Sps只有最后五位是size;
		int iSpsNum = BitsToIntEx(pData, 3, 5);
		printf("\t SpsNum     \t : %d \n", iSpsNum);
		for (int i = 0; i < iSpsNum; i++)
		{
			int iSpsLen = BytesToIntEx(pData, AVCC_SPSLEN_SIZE);
			// 显示输出;
			printf("\t %d : SysLen : %d \n \t SysContent: ", i+1, iSpsLen);
			for (int j = 0; j < iSpsLen; j++)
			{
				printf("%02X, ", pData[j]);
			}
			printf("\n");

			Parameter tempPara;
			tempPara.iSize = iSpsLen;
			tempPara.pData = pData;
			m_vecSpsContent.push_back(tempPara);
			pData += iSpsLen;
		}
	}
	
	{
		// Pps八位全是size;
		int iPpsNum = BytesToIntEx(pData, AVCC_PPSNUM_SIZE);
		printf("\t PpsNum     \t : %d \n", iPpsNum);
		for (int i = 0; i < iPpsNum; i++)
		{
			int iPpsLen = BytesToIntEx(pData, AVCC_PPSLEN_SIZE);
			// 显示输出;
			printf("\t %d : PpsLen : %d \n \t PpsContent: ", i+1, iPpsLen);
			for (int j = 0; j < iPpsLen; j++)
			{
				printf("%02X ", pData[j]);
			}
			printf("\n");

			Parameter tempPara;
			tempPara.iSize = iPpsLen;
			tempPara.pData = pData;
			m_vecPpsContent.push_back(tempPara);
			pData += iPpsLen;
		}
	}

	return pData - pBegin;
}
