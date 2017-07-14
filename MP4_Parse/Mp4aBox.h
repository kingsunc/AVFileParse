#pragma once
#include "BaseBox.h"

#define MP4A_RESERVED1_SIZE					6
#define MP4A_DATAREFINDEX_SIZE				2
#define MP4A_RESERVED2_SIZE					8
#define MP4A_CHANNELCOUNT_SIZE				2
#define MP4A_SAMPLESIZE_SIZE				2
#define MP4A_PREDEFINED_SIZE				2
#define MP4A_RESERVED3_SIZE					2
#define MP4A_SAMPLERATE_SIZE				4

/************************************************************************************************************
**									mp4a Box

const unsigned int(8)[6] reserved = 0;
unsigned int(16) data_reference_index;
const unsigned int(32)[2] reserved = 0;
template unsigned int(16) channelcount = 2;
template unsigned int(16) samplesize = 16;
unsigned int(16) pre_defined = 0;
const unsigned int(16) reserved = 0 ;
template unsigned int(32) samplerate = {timescale of media}<<16;
************************************************************************************************************/
class Mp4aBox : public BaseBox
{
public:
	Mp4aBox();
	~Mp4aBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	string				m_strReserved1;
	int					m_iDataRefIndex;
	string				m_strReserved2;
	int					m_iChannelCount;
	int					m_iSampleSize;
	string				m_strPreDefined;
	string				m_strReserved3;
	double				m_dSampleRate;
};