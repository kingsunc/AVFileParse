#include "BaseBox.h"
#include "BoxFactory.h"

#define DOUBLE_MAX_SIZE		40

BaseBox::BaseBox()
{
}

BaseBox::~BaseBox()
{
}

BaseBox* BaseBox::GetBoxFromFile(byteptr &pData)
{
	int iBoxSize = BytesToIntEx(pData, BOXSIZE_SIZE);
	string strBoxType = BytesToStrEx(pData, BOXTYPE_SIZE);

	BaseBox* pBox = AfxCreateBox(strBoxType);
	if (pBox)
	{
		// 多读了8个字节 撤回去;
		pData -= BOXHEADER_SIZE;
	}
	else
	{
		// 不识别的类型跳过;
		pData += iBoxSize - BOXHEADER_SIZE;
		printf(" find unknown type : %s, %d bytes. \n", strBoxType.c_str(), iBoxSize);

#ifdef _DEBUG
		//getchar();
#endif
	}

	return pBox;
}

int BaseBox::BytesToIntEx(byteptr &pData, int iCount, bool bOffset, bool bBigEndian)
{
	int iRes = 0;
	if (bBigEndian)
	{
		int iOffset = 0;
		for (int i = iCount - 1; i >= 0; i--)
		{
			iRes += pData[i] << (iOffset * 8);
			iOffset++;
		}
	}
	else
	{
		printf("Little Endian is not supported. \n");
	}
	if (bOffset)
	{
		pData += iCount;
	}

	return iRes;
}

int BaseBox::BytesToInt(byte arrByte[], int iCount)
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

std::string BaseBox::BytesToStrEx(byteptr &pData, int iCount, bool bOffset)
{
	std::string strResult((char*)pData, iCount);
	if (bOffset)
	{
		pData += iCount;
	}
	return strResult;
}

std::string BaseBox::BytesToStr(byte arrByte[], int iCount)
{
	char* pByte = (char*)arrByte;
	return BytesToStr(pByte, iCount);
}

double BaseBox::BytesToDoubleEx(byteptr &pData, int iM, int iN, bool bOffset)
{
	char chTemp[DOUBLE_MAX_SIZE] = { 0 };
	sprintf_s(chTemp, "%d.%d", BytesToInt(&pData[0], iM), BytesToInt(&pData[iM], iN));
	if (bOffset)
	{
		pData += iM + iN;
	}

	return atof(chTemp);
}

double BaseBox::BytesToDouble(byte arrByte[], int iM, int iN)
{
	char chTemp[DOUBLE_MAX_SIZE] = { 0 };
	sprintf_s(chTemp, "%d.%d", BytesToInt(&arrByte[0], iM), BytesToInt(&arrByte[iM], iN));

	return atof(chTemp);
}

int BaseBox::BitsToIntEx(byteptr &pData, int iPos, int iCount, bool bOffset)
{
	byte tempByte = pData[0] << iPos;
	tempByte = tempByte >> (8 - iCount);
	if (bOffset)
	{
		pData += 1;
	}
	return tempByte;
}

std::string BaseBox::BytesToStr(char* pByte, int iCount)
{
	std::string strResult(pByte, iCount);
	return strResult;
}

int BaseBox::ParseBox(byteptr &pData)
{
	byteptr pBegin = pData;
	int iHeaderSize = ParseHeader(pData);
	byteptr pEnd = pData + (m_iBoxSize - iHeaderSize);

	int iAttrsSize = ParseAttrs(pData);
	while (pData < pEnd)
	{
		int iChildSize = ParseChild(pData);

		if ((iAttrsSize <= 0) && (iChildSize <= 0))
		{
			// 若iAttrsSize和iChildSize同时为0;
			// 避免死循环应该过滤;
			pData = pEnd;
		}
	}

	return pEnd - pBegin;
}

int BaseBox::ParseHeader(byteptr &pData)
{
	byteptr pBeign = pData;
	printf("******************************* box *******************************\n");
	m_iBoxSize = BytesToIntEx(pData, BOXSIZE_SIZE);
	m_strBoxType = BytesToStrEx(pData, BOXTYPE_SIZE);
	printf("\t BoxSize       \t : %d \n", m_iBoxSize);
	printf("\t BoxType       \t : %s \n", m_strBoxType.c_str());

	return pData - pBeign;
}

int BaseBox::ParseAttrs(byteptr &pData)
{
	return 0;
}

int BaseBox::ParseChild(byteptr &pData)
{
	byteptr pBeign = pData;
	BaseBox* pBox = BaseBox::GetBoxFromFile(pData);
	if (pBox)
	{
		pBox->ParseBox(pData);
	}

	return pData - pBeign;
}

BaseBox* BaseBox::CreateObject()
{
	return NULL;
}
