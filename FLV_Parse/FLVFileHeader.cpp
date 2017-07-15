#include "FLVFileHeader.h"

CFLVFileHeader::CFLVFileHeader()
{

}

CFLVFileHeader::~CFLVFileHeader()
{

}

int CFLVFileHeader::ReadData(byte_ptr &pData)
{
	int iSize = sizeof(m_data);
	memcpy((char*)&m_data, pData, iSize);
	pData += iSize;  // 读完数据 后移;
	return iSize;
}

int CFLVFileHeader::WriteData(byte_ptr &pData)
{
	int iSize = sizeof(m_data);
	memcpy(pData, (char*)&m_data, iSize);
	pData += iSize;  // 写完数据 后移;
	return iSize;
}

void CFLVFileHeader::Display()
{
	printf("******************************FLV Header******************************\n");
	// FLV Header;
	{
		printf("\t File Type     \t : %c %c %c \n", m_data.Type[0], m_data.Type[1], m_data.Type[2]);
		printf("\t Version       \t : %d \n", m_data.Version);
		printf("\t Stream Info   \t : %d \n", m_data.StreamInfo);
		printf("\t Header Length \t : %d \n\n", BytesToInt((byte_ptr)m_data.HeaderSize, sizeof(m_data.HeaderSize)));
	}
}