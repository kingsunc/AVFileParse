#include "AVIFile.h"

CAVIFile::CAVIFile()
{
}

CAVIFile::~CAVIFile()
{

}

bool CAVIFile::LoadFile(char* strFile)
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
	int iSize = ftell(pFile);
	byte_ptr pData = new byte[iSize];
	fseek(pFile, 0, SEEK_SET);
	if (iSize != fread(pData, sizeof(byte), iSize, pFile))
	{
		printf("read file is failed. \n");
		return false;
	}
	fclose(pFile);

	byte ch[1000];
	memcpy(ch, pData, 1000);

	bool bResult = ReadData(pData, iSize);
	delete[]pData;
	pData = NULL;
	return bResult;
}

bool CAVIFile::ReadData(byte_ptr pData, int iSize)
{
	byte_ptr pEnd = pData + iSize;

	ReadAviHeader(pData);
	while (pData < pEnd)
	{
		ReadList(pData);
	}

	return true;
}

int CAVIFile::ReadAviHeader(byte_ptr &pData)
{
	byte_ptr pBegin = pData;
	printf("******************************* AVI Header *******************************\n");
	printf("\t AVI Type   \t : %s \n", BytesToStr(pData, 4).c_str());
	printf("\t AVI Size   \t : %d \n", BytesToInt(pData, 4));
	printf("\t AVI Name   \t : %s \n", BytesToStr(pData, 4).c_str());
	return pData - pBegin;
}

int CAVIFile::ReadList(byte_ptr &pData)
{
	byte_ptr pBegin = pData;
	printf("******************************* List Header *******************************\n");
	std::string strType = BytesToStr(pData, 4);
	printf("\t List Type   \t : %s \n", strType.c_str());
	int iListSize = BytesToInt(pData, 4);
	printf("\t List Size   \t : %d \n", iListSize);
	std::string strName = BytesToStr(pData, 4);
	printf("\t List Name   \t : %s \n", strName.c_str());
	byte_ptr pEnd = pBegin + (iListSize+8);

	if (0 == strName.compare("hdrl"))
	{
		ReadHdrl(pData);
	}
	else if(0 == strName.compare("strl"))
	{
		ReadStrl(pData, pEnd);
	}
	else if(0 == strName.compare("movi"))
	{
		ReadMovi(pData, pEnd);
	}
	else
	{
		if (0 == strType.compare("idx1"))
		{
			// 回退四个字节;
			pData -= 4;
			ReadIdx1(pData, pEnd);
		}
		else
		{
			printf("\t find unknown name: %s \n", strName.c_str());
			pData = pEnd;
		}
	}

	return pData - pBegin;
}

int CAVIFile::ReadHdrl(byte_ptr &pData)
{
	byte_ptr pBegin = pData;
	// 读数据;
	{
		avih_data avihData = { 0 };
		int iAvihSize = sizeof(avihData);
		memcpy(&avihData, pData, iAvihSize);
		pData += iAvihSize;

		printf("******************************* avih *******************************\n");
		byte_ptr temp = avihData.strName;
		printf("\t Name          \t : %s \n", BytesToStr(temp, 4).c_str());
		printf("\t Size          \t : %d \n", avihData.dwSize);
		printf("\t InterTime     \t : %d \n", avihData.dwIntervalTime);
		printf("\t dwMaxBytes    \t : %d \n", avihData.dwMaxBytesPerSec);
		printf("\t dwPadding     \t : %d \n", avihData.dwPaddingGranularity);
		printf("\t dwFlags       \t : %d \n", avihData.dwFlags);
		printf("\t dwTotalFrames \t : %d \n", avihData.dwTotalFrames);
		printf("\t dwStreams     \t : %d \n", avihData.dwStreams);
		printf("\t dwSBuffSize   \t : %d \n", avihData.dwSBuffSize);
		printf("\t dwWidth       \t : %d \n", avihData.dwWidth);
		printf("\t dwHeight      \t : %d \n", avihData.dwHeight);
		temp = avihData.dwReserved;
		printf("\t dwReserved    \t : %s \n", BytesToStr(temp, 16).c_str());
	}
	// 遍历子节点;
	ReadList(pData);

	return pData - pBegin;
}

int CAVIFile::ReadStrl(byte_ptr &pData, byte_ptr pEnd)
{
	byte_ptr pBegin = pData;
	// 读数据;
	std::string strType;
	ReadStrh(pData, strType);
	ReadStrf(pData, strType);

	while (pData < pEnd)
	{
		byte ch[100];
		memcpy(ch, pData, 100);
		std::string strName = BytesToStr(pData, 4);
		int iSize = BytesToInt(pData, 4);
		pData += iSize;
		printf("\t '%s' : size %d bytes. \n", strName.c_str(), iSize);
	}

	return pData - pBegin;
}

int CAVIFile::ReadStrh(byte_ptr &pData, std::string& strType)
{
	byte_ptr pBegin = pData;
	// 读数据;
	{
		strh_data strhData = {0};
		int iStrhSize = sizeof(strhData);
		memcpy(&strhData, pData, iStrhSize);
		pData += iStrhSize;

		printf("******************************* strh *******************************\n");
		byte_ptr temp = strhData.strName;
		printf("\t Name          \t : %s \n", BytesToStr(temp, 4).c_str());
		printf("\t Size          \t : %d \n", strhData.dwSize);
		temp = strhData.strType;
		strType = BytesToStr(temp, 4).c_str();
		printf("\t Type          \t : %s \n", strType.c_str());
		temp = strhData.strHandler;
		printf("\t Handler       \t : %d \n", BytesToInt(temp, 4));
		printf("\t dwFlags       \t : %d \n", strhData.dwFlags);
		printf("\t wPriority     \t : %d \n", strhData.wPriority);
		printf("\t wLanguage     \t : %d \n", strhData.wLanguage);
		printf("\t dwIniFrames   \t : %d \n", strhData.dwIniFrames);
		printf("\t dwScale       \t : %d \n", strhData.dwScale);
		printf("\t dwRate        \t : %d \n", strhData.dwRate);
		printf("\t dwStart       \t : %d \n", strhData.dwStart);
		printf("\t dwLength      \t : %d \n", strhData.dwLength);
		printf("\t dwSBuffSize   \t : %d \n", strhData.dwSBuffSize);
		printf("\t dwQuality     \t : %d \n", strhData.dwQuality);
		printf("\t dwSampleSize  \t : %d \n", strhData.dwSampleSize);
		printf("\t rcFrame_l     \t : %d \n", strhData.rcFrame.left);
		printf("\t rcFrame_t     \t : %d \n", strhData.rcFrame.top);
		printf("\t rcFrame_r     \t : %d \n", strhData.rcFrame.right);
		printf("\t rcFrame_b     \t : %d \n", strhData.rcFrame.bottom);
	}
	
	return pData - pBegin;
}

int CAVIFile::ReadStrf(byte_ptr &pData, std::string strType)
{
	std::string strName = BytesToStr(pData, 4);
	if (0 != strName.compare("strf"))
	{
		printf("\t strName(%s) is not strf", strName.c_str());
		pData -= 4;
		return 0;
	}

	byte_ptr pBegin = pData;
	// 读数据;
	if (0 == strType.compare("vids"))
	{
		BITMAPINFO strfData = { 0 };
		int iStrfSize = sizeof(strfData);
		memcpy(&strfData, pData, iStrfSize);
		pData += iStrfSize;

		printf("******************************* strf *******************************\n");
		printf("\t Size         \t : %d \n", strfData.bmiHeader.biSize);
		printf("\t Width        \t : %d \n", strfData.bmiHeader.biWidth);
		printf("\t Height       \t : %d \n", strfData.bmiHeader.biHeight);
		printf("\t Planes       \t : %d \n", strfData.bmiHeader.biPlanes);
		printf("\t BitCount     \t : %d \n", strfData.bmiHeader.biBitCount);
		printf("\t Compression  \t : 0x%X \n", strfData.bmiHeader.biCompression);
		printf("\t SizeImage    \t : %d \n", strfData.bmiHeader.biSizeImage);
		printf("\t XPels        \t : %d \n", strfData.bmiHeader.biXPelsPerMeter);
		printf("\t YPels        \t : %d \n", strfData.bmiHeader.biYPelsPerMeter);
		printf("\t ClrUsed      \t : %d \n", strfData.bmiHeader.biClrUsed);
		printf("\t SampleSize   \t : %d \n", strfData.bmiHeader.biClrImportant);
		printf("\t rgbBlue      \t : %d \n", strfData.bmiColors[0].rgbBlue);
		printf("\t rgbGreen     \t : %d \n", strfData.bmiColors[0].rgbGreen);
		printf("\t rgbRed       \t : %d \n", strfData.bmiColors[0].rgbRed);
		printf("\t rgbReserved  \t : %c \n", strfData.bmiColors[0].rgbReserved);
	}
	else if (0 == strType.compare("auds"))
	{
		printf("******************************* strf *******************************\n");
		int iSize = BytesToInt(pData, 4);
		printf("\t Size     \t : %d \n", iSize);

		WAVEFORMATEX strfData = { 0 };
		int iStrfSize = sizeof(strfData);
		memcpy(&strfData, pData, iStrfSize);
		pData += iStrfSize;
		printf("\t FormatTag     \t : %d \n", strfData.wFormatTag);
		printf("\t Channels      \t : %d \n", strfData.nChannels);
		printf("\t SamplesPerSec \t : %d \n", strfData.nSamplesPerSec);
		printf("\t AvgBytesPerSec\t : %d \n", strfData.nAvgBytesPerSec);
		printf("\t BlockAlign    \t : %d \n", strfData.nBlockAlign);
		printf("\t BitsPerSample \t : %d \n", strfData.wBitsPerSample);
		printf("\t cbSize        \t : %d \n", strfData.cbSize);
		pData += strfData.cbSize;	// 后面有额外信息 需要后移;
	}

	return pData - pBegin;
}

int CAVIFile::ReadMovi(byte_ptr &pData, byte_ptr pEnd)
{
	byte_ptr pBegin = pData;
	// 
	while (pData < pEnd)
	{
		std::string strType = BytesToStr(pData, 4);
		int iSize = BytesToInt(pData, 4);
		int iOffset = iSize;
		if ( 0 != (iSize % 2) )
		{
			// 此处有字节对齐原则, 需判断奇偶性;
			iOffset += 1;
		} 
		pData += iOffset;

		printf("\t '%s' : %d bytes. \n", strType.c_str(), iSize);
	}

	return pData - pBegin;
}

int CAVIFile::ReadIdx1(byte_ptr &pData, byte_ptr pEnd)
{
	byte_ptr pBegin = pData;

	while (pData < pEnd)
	{
		std::string strTrackID = BytesToStr(pData, 4);
		int iFlag = BytesToInt(pData, 4);
		int iOffset = BytesToInt(pData, 4);
		int iLength = BytesToInt(pData, 4);

		printf("\t TrackID: '%s',  Flag : %6d,  Offset : %6d,  Length : %6d \n", strTrackID.c_str(), iFlag, iOffset, iLength);
	}

	return pData - pBegin;
}