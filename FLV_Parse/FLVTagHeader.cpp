#include "FLVTagHeader.h"

CFLVTagHeader::CFLVTagHeader()
{
}

CFLVTagHeader::~CFLVTagHeader()
{

}

int CFLVTagHeader::ReadData(byte_ptr &pData)
{
	// 往后错四个字节读取header;
	pData += 4;
	int iTagHeaderSize = sizeof(flvtagheader_data);
	memcpy(&m_data, pData, iTagHeaderSize);
	pData += iTagHeaderSize;

	return iTagHeaderSize + 4;
}

int CFLVTagHeader::WriteData(byte_ptr &pData)
{
	memset(pData, 0, 4);
	pData += 4;
	int iTagHeaderSize = sizeof(flvtagheader_data);
	memcpy(pData, &m_data, iTagHeaderSize);
	pData += iTagHeaderSize;

	return iTagHeaderSize + 4;
}

void CFLVTagHeader::Display()
{
	printf("\n      ************************Tag Header************************      \n");
	printf("\t Tag Type   \t : %d \n", m_data.TagType);
	printf("\t DataSize   \t : %d \n", BytesToInt(m_data.DataSize, sizeof(m_data.DataSize)));
	printf("\t Timestamp  \t : %d \n", BytesToInt(m_data.Timestamp, sizeof(m_data.Timestamp)));
	printf("\t TimeExtend \t : %d \n", m_data.TimeExtend);
	printf("\t StreamsID  \t : %d \n\n", BytesToInt(m_data.StreamsID, sizeof(m_data.StreamsID)));
}

int CFLVTagHeader::GetTagType()
{
	int iTagType = BytesToInt(&m_data.TagType, sizeof(m_data.TagType));
	return iTagType;
}

int CFLVTagHeader::GetTagSize()
{
	int iTagType = BytesToInt(m_data.DataSize, sizeof(m_data.DataSize));
	return iTagType;
}
