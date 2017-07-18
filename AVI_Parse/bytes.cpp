#include "bytes.h"

int BytesToInt(byte_ptr &pData, int iCount, bool bLitte)
{
	int iResult = 0;
	if (bLitte)
	{
		for (int i = 0; i < iCount; i++)
		{
			iResult += pData[i] << (i * 8);
		}
	}
	else
	{
		for (int i = 0; i < iCount; i++)
		{
			iResult += pData[i] << ((iCount - 1 - i) * 8);
		}
	}
	
	pData += iCount;
	return iResult;
}

std::string BytesToStr(byte_ptr &pData, int iCount)
{
	std::string strResult((char*)pData, iCount);
	pData += iCount;
	return strResult;
}
