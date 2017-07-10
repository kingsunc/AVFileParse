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

int CAVFile::ByteToInt(byte Type[], int iCount)
{
	int iRes = 0;
	int iOffset = 0;
	for (int i = iCount - 1; i >= 0; i--)
	{
		iRes += Type[i] << (iOffset * 8);
		iOffset++;
	}

	return iRes;
}
