#pragma once
#include "BaseBox.h"

#define AVCC_VERSION_SIZE					1
#define AVCC_PROFILE_SIZE					1
#define AVCC_COMPAT_SIZE					1
#define AVCC_LEVEL_SIZE						1
#define AVCC_LENSIZE_SIZE					1
#define AVCC_SPSNUM_SIZE					1
#define AVCC_SPSLEN_SIZE					2
#define AVCC_PPSNUM_SIZE					1
#define AVCC_PPSLEN_SIZE					2

/************************************************************************************************************
**									avvC Box

configurationVersion			1			版本;
AVCProfileIndication			1			10进制的77 对照profile的情况 知道是main profile;
profile_compatibility			1			一般0
AVCLevelIndication				1			10进制的31
lengthSizeMinusOne				1			1111 1111	前面6位为reserved，后面2位（0b11）为：lengthSizeMinusOne，表示3.
											那么用来表示size的字节就有3 + 1 = 4个

numOfSequenceParameterSets		1			1110 0001	前面3位是reserved, 后面5bit是numOfSequenceParameterSets，表示有1个sps.
sequenceParameterSetLength		2			
SPS								23

numOfPictureParameterSets		1			有1个pps
pictureParameterSetLength		2			pictureParameterSetLength
PPS								4			PPS的内容

************************************************************************************************************/
class AvcCBox : public BaseBox
{
public:
	AvcCBox();
	~AvcCBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int			m_iVersion;
	int			m_iProfile;
	int			m_iCompat;
	int			m_iLevel;
	int			m_iLenSize;
	int			m_iSpsNum;
	VecParameters		m_vecSpsContent;
	int			m_iPpsNum;
	VecParameters		m_vecPpsContent;
};