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
	printf("\t RiffID           \t : %s \n", BytesToStr(m_WaveHeader.szRiffID, 4).c_str());
	printf("\t RiffSize         \t : %d \n", m_WaveHeader.dwRiffSize);
	printf("\t RiffFormat       \t : %s \n", BytesToStr(m_WaveHeader.szRiffFormat, 4).c_str());

	printf("************************ FMT BLOCK ************************\n");
	printf("\t szFmtID           \t : %s \n", BytesToStr(m_WaveHeader.szFmtID, 4).c_str());
	printf("\t dwFmtSize         \t : %d \n", m_WaveHeader.dwFmtSize);
	printf("\t wFormatTag        \t : %d \n", m_WaveHeader.wavFormat.wFormatTag);
	printf("\t wChannels         \t : %d \n", m_WaveHeader.wavFormat.wChannels);
	printf("\t dwSamplesPerSec   \t : %d \n", m_WaveHeader.wavFormat.dwSamplesPerSec);
	printf("\t dwAvgBytesPerSec  \t : %d \n", m_WaveHeader.wavFormat.dwAvgBytesPerSec);
	printf("\t wBlockAlign       \t : %d \n", m_WaveHeader.wavFormat.wBlockAlign);
	printf("\t wBitsPerSample    \t : %d \n", m_WaveHeader.wavFormat.wBitsPerSample);

	printf("************************ DATA HEADER ************************\n");
	printf("\t szDataID           \t : %s \n", BytesToStr(m_WaveHeader.szDataID, 4).c_str());
	printf("\t dwDataSize         \t : %d \n", m_WaveHeader.dwDataSize);

	printf("************************ WAVE DATA ************************\n");
	int iSize = min(100, m_WaveHeader.dwDataSize);
	for (int i = 0; i < iSize; i++)
	{
		printf(" %02X ", m_pData[i]);
	}
	printf(" ...");
}

bool CWAVFile::ReadData(byte_ptr pData, int iSize)
{
	// WAVE_HEADER;
	int iRiffHeaderSize = sizeof(m_WaveHeader);
	memcpy(&m_WaveHeader, pData, iRiffHeaderSize);
	pData += iRiffHeaderSize;

	if (m_pData)
	{
		delete []m_pData;
	}
	int iDataSize = m_WaveHeader.dwDataSize;
	m_pData = new byte[iDataSize];
	memcpy(m_pData, pData, iDataSize);

	return true;
}

bool CWAVFile::WriteData(byte_ptr pData, int &iSize, double dStart, double dEnd)
{
	return true;
}
