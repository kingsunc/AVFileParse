#include "EsdsBox.h"

EsdsBox::EsdsBox()
{
}

EsdsBox::~EsdsBox()
{
}

BaseBox* EsdsBox::CreateObject()
{
	return new EsdsBox();
}

int EsdsBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	printf(" EsdsBox::ParseAttrs ??? \n");
	pData += m_iBoxSize - BOXHEADER_SIZE;

	//byte ch[100];
	//memcpy(ch, pData, 100);

	//m_iVersion = BytesToIntEx(pData, ESDS_VERSION_SIZE);
	//m_iFlags = BytesToIntEx(pData, ESDS_FLAGS_SIZE);
	//m_iTag = BytesToIntEx(pData, ESDS_TAG_SIZE);
	//m_iLengthField1 = BytesToIntEx(pData, ESDS_LENGTHFIELD1_SIZE);
	//m_iEsID = BytesToIntEx(pData, ESDS_ESID_SIZE);
	//// 解析下个单字节;
	//{
	//	byte tempByte = pData[0];
	//	pData += 1;

	//	m_iStreamFlag = (tempByte & (0x80)) ? 1 : 0;
	//	m_iURLFlag = (tempByte & (0x40)) ? 1 : 0;
	//	m_iOCRStreamFlag = (tempByte & (0x20)) ? 1 : 0;
	//	m_iStreamPriority = tempByte & (0x1F);

	//	if (m_iStreamFlag)
	//	{
	//		m_iStreamFlagID = BytesToIntEx(pData, ESDS_STREAMFLAGLEN_SIZE);
	//	}
	//	if (m_iURLFlag)
	//	{
	//		m_iURLFlagLen = BytesToIntEx(pData, ESDS_URLFLAGLEN_SIZE);
	//		m_strURLFlagStr = BytesToIntEx(pData, m_iURLFlagLen);
	//	}
	//	if (m_iOCRStreamFlag)
	//	{
	//		//m_iOCRStreamFlagID = BytesToIntEx(pData, ESDS_OCRFLAGLEN_SIZE);
	//	}
	//}
	//m_iDcdTag = BytesToIntEx(pData, ESDS_DCDTAG_SIZE);
	//m_iLengthField2 = BytesToIntEx(pData, ESDS_LENGTHFIELD2_SIZE);
	//m_iObjectTypeIn = BytesToIntEx(pData, ESDS_OBJECTTYPEIN_SIZE);
	//// 解析下个单字节;
	//{
	//	byte tempByte = pData[0];
	//	pData += 1;
	//	m_iStreamType = tempByte & (0xFC);		// 6 bit
	//	m_iUpStream = tempByte & (0x02);		// 1 bit
	//	// m_strReserved1;						// 1bit
	//}
	//m_iBufferSizeDB = BytesToIntEx(pData, ESDS_BUFFERSIZEDB_SIZE);
	//m_iMaxBitrate = BytesToIntEx(pData, ESDS_MAXBITRATE_SIZE);
	//m_iAvgBitrate = BytesToIntEx(pData, ESDS_AVGBITRATE_SIZE);
	//m_iDecInfoTag = BytesToIntEx(pData, ESDS_DECINFOTAG_SIZE);
	//m_iLengthField3 = BytesToIntEx(pData, ESDS_LENGTHFIELD3_SIZE);

	//printf("\t Version     \t : %d \n", m_iVersion);
	//printf("\t Flags	   \t : %d \n", m_iFlags);
	//printf("\t Tag         \t : %d \n", m_iTag);
	//printf("\t LengthField \t : %d \n", m_iLengthField1);
	//printf("\t EsID        \t : %d \n", m_iEsID);
	//if (m_iStreamFlag)
	//{
	//	printf("\t StreamFlag  \t : %d,  %d \n", m_iStreamFlag, m_iStreamFlagID);
	//} 
	//else
	//{
	//	printf("\t StreamFlag  \t : %d \n", m_iStreamFlag);
	//}
	//if (m_iURLFlag)
	//{
	//	printf("\t URLFlag  \t : %d,  %d  ,  %s \n", m_iURLFlag, m_iURLFlagLen, m_strURLFlagStr.c_str());
	//}
	//else
	//{
	//	printf("\t URLFlag  \t : %d \n", m_iURLFlag);
	//}
	//if (m_iOCRStreamFlag)
	//{
	//	printf("\t OCRStreamFlag\t : %d ,  %d \n", m_iOCRStreamFlag, m_iOCRStreamFlagID);
	//}
	//else
	//{
	//	printf("\t OCRStreamFlag\t : %d \n", m_iOCRStreamFlag);
	//}
	//printf("\t StreamPriority\t : %d \n", m_iStreamPriority);
	//printf("\t DcdTag       \t : %d \n", m_iDcdTag);
	//printf("\t LengthField2 \t : %d \n", m_iLengthField2);
	//printf("\t ObjectTypeIn \t : %d \n", m_iObjectTypeIn);
	//printf("\t StreamType   \t : %d \n", m_iStreamType);
	//printf("\t UpStream     \t : %d \n", m_iUpStream);
	//printf("\t BufferSizeDB \t : %d \n", m_iBufferSizeDB);
	//printf("\t MaxBitrate   \t : %d \n", m_iMaxBitrate);
	//printf("\t AvgBitrate   \t : %d \n", m_iAvgBitrate);
	//printf("\t DecInfoTag   \t : %d \n", m_iDecInfoTag);
	//printf("\t LengthField3 \t : %d \n", m_iLengthField3);

	return pData - pBegin;
}
