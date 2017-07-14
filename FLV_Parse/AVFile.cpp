#include "AVFile.h"

CAVFile::CAVFile()
{
	m_pFile = NULL;
}

CAVFile::~CAVFile()
{
}

bool CAVFile::LoadFile(std::string strFile)
{
	m_pFile = fopen(strFile.c_str(), "rb");
	if (!m_pFile)
	{
		printf("m_pFile is null.");
		return false;
	}

	bool bResult = ParseFile();
	fclose(m_pFile);
	m_pFile = NULL;
	return bResult;
}

int CAVFile::ByteToInt(byte arrByte[], int iCount)
{
	int iRes = 0;
	int iOffset = 0;
	for (int i = iCount - 1; i >= 0; i--)
	{
		iRes += arrByte[i] << (iOffset * 8);
		iOffset++;
	}

	return iRes;
}

std::string CAVFile::ByteToStr(byte arrByte[], int iCount)
{
	char* pByte = (char*)arrByte;
	return ByteToStr(pByte, iCount);
}

std::string CAVFile::ByteToStr(char* pByte, int iCount)
{
	std::string strResult(pByte, iCount);
	return strResult;
}