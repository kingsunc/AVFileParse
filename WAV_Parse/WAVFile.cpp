#include "WAVFile.h"

CWAVFile::CWAVFile()
{
	m_pData = NULL;
}

CWAVFile::~CWAVFile()
{
	if (m_pData)
	{
		delete[]m_pData;
		m_pData = NULL;
	}
}

bool CWAVFile::LoadFile(const char* strFile)
{
	if (!strFile)
	{
		printf("strFile is null. \n");
		return false;
	}
	FILE* pFile = fopen(strFile, "rb");
	if (!pFile)
	{
		printf("fopen is failed. \n");
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	m_iSize = ftell(pFile);
	byte_ptr pData = new byte[m_iSize];
	fseek(pFile, 0, SEEK_SET);
	if (m_iSize != fread(pData, sizeof(byte), m_iSize, pFile))
	{
		printf("read file is failed. \n");
		return false;
	}
	fclose(pFile);

	bool bResult = ReadData(pData, m_iSize);
	delete[]pData;
	pData = NULL;
	return bResult;
}

bool CWAVFile::SaveFile(const char* strFile, RECT rt)
{
	return true;
}

void CWAVFile::Display()
{
	printf("************************ RIFF HEADER ************************\n");
	printf("\t RiffID           \t : %s \n", BytesToStr(m_RiffHeader.szRiffID, 4).c_str());
	printf("\t RiffSize         \t : %d \n", m_RiffHeader.dwRiffSize);
	printf("\t RiffFormat       \t : %s \n", BytesToStr(m_RiffHeader.szRiffFormat, 4).c_str());

	printf("************************ FMT BLOCK ************************\n");
	printf("\t szFmtID           \t : %s \n", BytesToStr(m_FmtBlock.szFmtID, 4).c_str());
	printf("\t dwFmtSize         \t : %d \n", m_FmtBlock.dwFmtSize);
	printf("\t wFormatTag        \t : %d \n", m_FmtBlock.wavFormat.wFormatTag);
	printf("\t wChannels         \t : %d \n", m_FmtBlock.wavFormat.wChannels);
	printf("\t dwSamplesPerSec   \t : %d \n", m_FmtBlock.wavFormat.dwSamplesPerSec);
	printf("\t dwAvgBytesPerSec  \t : %d \n", m_FmtBlock.wavFormat.dwAvgBytesPerSec);
	printf("\t wBlockAlign       \t : %d \n", m_FmtBlock.wavFormat.wBlockAlign);
	printf("\t wBitsPerSample    \t : %d \n", m_FmtBlock.wavFormat.wBitsPerSample);

	printf("************************ DATA HEADER ************************\n");
	printf("\t szDataID           \t : %s \n", BytesToStr(m_DataHeader.szDataID, 4).c_str());
	printf("\t dwDataSize         \t : %d \n", m_DataHeader.dwDataSize);

	printf("************************ WAVE DATA ************************\n");
	int iSize = min(100, m_DataHeader.dwDataSize);
	for (int i = 0; i < iSize; i++)
	{
		printf(" %02X ", m_pData[i]);
	}
	printf(" ...");
}

bool CWAVFile::ReadData(byte_ptr pData, int iSize)
{
	// RIFF_HEADER;
	int iRiffHeaderSize = sizeof(m_RiffHeader);
	memcpy(&m_RiffHeader, pData, iRiffHeaderSize);
	pData += iRiffHeaderSize;
	// FMT_BLOCK;
	int iFmtBlockSize = sizeof(m_FmtBlock);
	memcpy(&m_FmtBlock, pData, iFmtBlockSize);
	pData += iFmtBlockSize;
	// DATA_HEADER;
	int iDataHeaderSize = sizeof(m_DataHeader);
	memcpy(&m_DataHeader, pData, iDataHeaderSize);
	pData += iDataHeaderSize;

	if (m_pData)
	{
		delete []m_pData;
	}
	int iDataSize = m_DataHeader.dwDataSize;
	m_pData = new byte[iDataSize];
	memcpy(m_pData, pData, iDataSize);

	return true;
}

bool CWAVFile::WriteData(byte_ptr pData, int &iSize, double dStart, double dEnd)
{
	return true;
}
