#include "bytes.h"

int BytesToInt(byte_ptr pData, int iCount)
{
	int iResult = 0;
	for (int i = 0; i < iCount; i++)
	{
		iResult += pData[i] << ((iCount - 1- i)* 8);
	}
	return iResult;
}

std::string BytesToStr(byte_ptr pData, int iCount)
{
	std::string strResult((char*)pData, iCount);
	return strResult;
}