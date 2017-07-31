#include "BMPFile.h"

CBMPFile::CBMPFile()
{
	m_pData = NULL;
}

CBMPFile::~CBMPFile()
{
	if (m_pData)
	{
		delete []m_pData;
		m_pData = NULL;
	}
}

bool CBMPFile::LoadFile(const char* strFile)
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
	m_iFileSize = ftell(pFile);
	byte_ptr pData = new byte[m_iFileSize];
	fseek(pFile, 0, SEEK_SET);
	if (m_iFileSize != fread(pData, sizeof(byte), m_iFileSize, pFile))
	{
		printf("read file is failed. \n");
		return false;
	}
	fclose(pFile);

	bool bResult = ReadData(pData, m_iFileSize);
	delete[]pData;
	pData = NULL;
	return bResult;
}

bool CBMPFile::SaveFile(const char* strFile, RECT rt)
{
	return true;
}

void CBMPFile::Display()
{
	printf("************************ BITMAP FILE HEADER ************************\n");
	printf("\t bfType           \t : %d \n", m_bmpFileHeader.bfType);
	printf("\t bfSize           \t : %d \n", m_bmpFileHeader.bfSize);
	printf("\t bfReserved1      \t : %d \n", m_bmpFileHeader.bfReserved1);
	printf("\t bfReserved2      \t : %d \n", m_bmpFileHeader.bfReserved2);
	printf("\t bfOffBits        \t : %d \n", m_bmpFileHeader.bfOffBits);

	printf("************************ BITMAP INFO HEADER ************************\n");
	printf("\t biSize           \t : %d \n", m_bmpInfoHeader.biSize);
	printf("\t biWidth          \t : %d \n", m_bmpInfoHeader.biWidth);
	printf("\t biHeight         \t : %d \n", m_bmpInfoHeader.biHeight);
	printf("\t biPlanes         \t : %d \n", m_bmpInfoHeader.biPlanes);
	printf("\t biBitCount       \t : %d \n", m_bmpInfoHeader.biBitCount);
	printf("\t biCompression    \t : %d \n", m_bmpInfoHeader.biCompression);
	printf("\t biSizeImage      \t : %d \n", m_bmpInfoHeader.biSizeImage);
	printf("\t biXPelsPerMeter  \t : %d \n", m_bmpInfoHeader.biXPelsPerMeter);
	printf("\t biYPelsPerMeter  \t : %d \n", m_bmpInfoHeader.biYPelsPerMeter);
	printf("\t biClrUsed        \t : %d \n", m_bmpInfoHeader.biClrUsed);
	printf("\t biClrImportant   \t : %d \n", m_bmpInfoHeader.biClrImportant);

	printf("************************ BITMAP DATA ************************\n");
	int iDataSize = min(100, m_bmpInfoHeader.biSizeImage);
	for (int i = 0; i < iDataSize; i++)
	{
		printf(" %02X ", m_pData[i]);
	}
	printf(" ...");
}

bool CBMPFile::ReadData(byte_ptr pData, int iSize)
{
	byte ch[1000];
	memcpy(ch, pData, 1000);
	// BITMAPFILEHEADER;
	int iFileHeaderSize = sizeof(m_bmpFileHeader);
	memcpy(&m_bmpFileHeader, pData, iFileHeaderSize);
	pData += iFileHeaderSize;
	// BITMAPINFOHEADER;
	int iInfoHeaderSize = sizeof(m_bmpInfoHeader);
	memcpy(&m_bmpInfoHeader, pData, iInfoHeaderSize);
	pData += iInfoHeaderSize;

	switch (m_bmpInfoHeader.biBitCount)
	{
	case 24:
		{
			// ÎÞµ÷É«°å;
			int iImageSize = m_bmpInfoHeader.biSizeImage;
			if (m_pData)
			{
				delete []m_pData;
			}
			m_pData = new byte[iImageSize];
			memcpy(m_pData, pData, iImageSize);
		}
		break;
	case 32:
		{
		}
		break;
	default:
		break;
	}

	return true;
}

bool CBMPFile::WriteData(byte_ptr pData, int &iSize, double dStart, double dEnd)
{
	return true;
}