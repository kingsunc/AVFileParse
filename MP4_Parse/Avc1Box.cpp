#include "Avc1Box.h"

Avc1Box::Avc1Box()
{
}

Avc1Box::~Avc1Box()
{
}

BaseBox* Avc1Box::CreateObject()
{
	return new Avc1Box();
}

int Avc1Box::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	m_strReserved1 = BytesToStrEx(pData, AVC1_RESERVED1_SIZE);
	m_iDataRefIndex = BytesToIntEx(pData, AVC1_DATAREFINDEX_SIZE);
	m_strPredefined1 = BytesToStrEx(pData, AVC1_PREDEFINED1_SIZE);
	m_strReserved2 = BytesToStrEx(pData, AVC1_RESERVED2_SIZE);
	m_strPredefined2 = BytesToStrEx(pData, AVC1_PREDEFINED2_SIZE);
	m_iWidth = BytesToIntEx(pData, AVC1_WIDTH_SIZE);
	m_iHeight = BytesToIntEx(pData, AVC1_HEIGHT_SIZE);
	m_dHorRes = BytesToDoubleEx(pData, AVC1_HORRES_SIZE / 2, AVC1_HORRES_SIZE / 2);
	m_dVerRes = BytesToDoubleEx(pData, AVC1_VERRES_SIZE / 2, AVC1_VERRES_SIZE / 2);
	m_strReserved3 = BytesToStrEx(pData, AVC1_RESERVED3_SIZE);
	m_iFrameCount = BytesToIntEx(pData, AVC1_FRAMECOUNT_SIZE);
	m_strCompressName = BytesToStrEx(pData, AVC1_COMPRESSNAME_SIZE);
	m_iDepth = BytesToIntEx(pData, AVC1_DEPTH_SIZE);
	m_strPredefined3 = BytesToStrEx(pData, AVC1_PREDEFINED3_SIZE);

	printf("\t Reserved1     \t : %s \n", m_strReserved1.c_str());
	printf("\t DataRefIndex  \t : %d \n", m_iDataRefIndex);
	printf("\t Predefined1   \t : %s \n", m_strPredefined1.c_str());
	printf("\t Reserved2     \t : %s \n", m_strReserved2.c_str());
	printf("\t Predefined2   \t : %s \n", m_strPredefined2.c_str());
	printf("\t Width         \t : %d \n", m_iWidth);
	printf("\t Height        \t : %d \n", m_iHeight);
	printf("\t HorRes        \t : %f \n", m_dHorRes);
	printf("\t VerRes        \t : %f \n", m_dVerRes);
	printf("\t Reserved3     \t : %s \n", m_strReserved3.c_str());
	printf("\t FrameCount    \t : %d \n", m_iFrameCount);
	printf("\t CompressName  \t : %s \n", m_strCompressName.c_str());
	printf("\t Depth         \t : %d \n", m_iDepth);
	printf("\t Predefined3   \t : %s \n", m_strPredefined3.c_str());

	return pData - pBegin;
}