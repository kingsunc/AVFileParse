#include "Mp4aBox.h"

Mp4aBox::Mp4aBox()
{
}

Mp4aBox::~Mp4aBox()
{
}

BaseBox* Mp4aBox::CreateObject()
{
	return new Mp4aBox();
}

int Mp4aBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	m_strReserved1 = BytesToStrEx(pData, MP4A_RESERVED1_SIZE);
	m_iDataRefIndex = BytesToIntEx(pData, MP4A_DATAREFINDEX_SIZE);
	m_iChannelCount = BytesToIntEx(pData, MP4A_CHANNELCOUNT_SIZE);
	m_strReserved2 = BytesToStrEx(pData, MP4A_RESERVED2_SIZE);
	m_iSampleSize = BytesToIntEx(pData, MP4A_SAMPLESIZE_SIZE);
	m_strPreDefined = BytesToStrEx(pData, MP4A_PREDEFINED_SIZE);
	m_strReserved3 = BytesToStrEx(pData, MP4A_RESERVED3_SIZE);
	m_dSampleRate = BytesToDoubleEx(pData, 2, 2);

	printf("\t Reserved1     \t : %s \n", m_strReserved1.c_str());
	printf("\t DataRefIndex  \t : %d \n", m_iDataRefIndex);
	printf("\t ChannelCount  \t : %d \n", m_iChannelCount);
	printf("\t Reserved2     \t : %s \n", m_strReserved2.c_str());
	printf("\t SampleSize    \t : %d \n", m_iSampleSize);
	printf("\t PreDefined    \t : %s \n", m_strPreDefined.c_str());
	printf("\t Reserved3     \t : %s \n", m_strReserved3.c_str());
	printf("\t SampleRate    \t : %f \n", m_dSampleRate);

	return pData - pBegin;
}
