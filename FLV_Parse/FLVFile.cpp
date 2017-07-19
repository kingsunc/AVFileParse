#include "FLVFile.h"

CFLVFile::CFLVFile()
{
}

CFLVFile::~CFLVFile()
{

}

bool CFLVFile::LoadFile(const char* strFile)
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

// 起始dStart~dEnd的音视频保存文件;
// 要求0~1, dEnd > dStart 否则返回 false;
bool CFLVFile::SaveFile(const char* strFile, double dStart, double dEnd)
{
	if (!strFile)
	{
		printf("strFile is null. \n");
		return false;
	}
	if (dStart < 0.0 || dStart > 1.0)
	{
		printf("dStart should in : 0.0 ~ 1.0. \n");
		return false;
	}
	if (dEnd < 0.0 || dEnd > 1.0)
	{
		printf("dEnd should in : 0.0 ~ 1.0. \n");
		return false;
	}
	if (dStart >= dEnd)
	{
		printf("dStart should less than dEnd. \n");
		return false;
	}

	byte_ptr pData = new byte[m_iSize];
	memset(pData, sizeof(byte), m_iSize);
	int iSize = 0;
	bool bResult = WriteData(pData, iSize, dStart, dEnd);
	if (bResult)
	{
		FILE* pFile = fopen(strFile, "wb");
		if (!pFile)
		{
			printf("fopen is failed. \n");
			return false;
		}
		fwrite(pData, 1, iSize, pFile);
		fclose(pFile);
	}
	delete []pData;
	pData = NULL;

	return true;
}

bool CFLVFile::ReadData(byte_ptr pData, int iSize)
{
	int iHeaderSize = m_flvHeader.ReadData(pData);
	return m_flvBody.ReadData(pData, iSize - iHeaderSize);
}

bool CFLVFile::WriteData(byte_ptr pData, int &iSize, double dStart, double dEnd)
{
	iSize = 0;
	byte_ptr pBegin = pData;
	int iHeaderSize = m_flvHeader.WriteData(pData);
	int iBodySize = m_flvBody.WriteData(pData, dStart, dEnd);
	if (iHeaderSize < 0 || iBodySize < 0)
	{
		return false;
	}
	iSize = iHeaderSize + iBodySize;

	return true;
}

void CFLVFile::Display()
{
	m_flvHeader.Display();
	m_flvBody.Display();
}
