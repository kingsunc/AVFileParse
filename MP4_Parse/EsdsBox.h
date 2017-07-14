#pragma once
#include "BaseBox.h"

#define ESDS_VERSION_SIZE					1
#define ESDS_FLAGS_SIZE						3
#define ESDS_TAG_SIZE						1
#define ESDS_LENGTHFIELD1_SIZE				1
#define ESDS_ESID_SIZE						2
#define ESDS_STREAMFLAGLEN_SIZE				2
#define ESDS_URLFLAGLEN_SIZE				1
#define ESDS_OCRFLAGLEN_SIZE				2
#define ESDS_DCDTAG_SIZE					1
#define ESDS_LENGTHFIELD2_SIZE				1
#define ESDS_OBJECTTYPEIN_SIZE				1
#define ESDS_BUFFERSIZEDB_SIZE				3
#define ESDS_MAXBITRATE_SIZE				4
#define ESDS_AVGBITRATE_SIZE				4
#define ESDS_DECINFOTAG_SIZE				1
#define ESDS_LENGTHFIELD3_SIZE				1


/************************************************************************************************************
**									mp4a Box

version, flag //32
unsigned char tag; //8
unsigned char Length_Field; //8   Length Field：25
unsigned int  ES_ID;		//bit(16)   ES_ID: 是0
unsigned char streamDependenceFlag; //bit(1)	steamDependenceFlag，如果为1，则有16bits的
unsigned char URL_Flag;				//bit(1)	URL_Flag, 如果为1，后边则有8bits URLlength, 和相应的URLstring(URLlength)
unsigned char OCRstreamFlag;		//bit(1)	如果为1，有16bits OCR_ES_id;
unsigned char streamPriority;		//bit(5)
unsigned char DecoderConfigDescriptor_tag; //8  = 0x04
unsigned char Length_Field_1;      //8 = 0x11
unsigned char objectTypeIndication; //8 = 0x40  14496-1 Table8, 0x40是Audio ISO/IEC 14496-3
unsigned char streamType ; //6   5是Audio Stream, 14496-1 Table9
unsigned char upStream;    //1
unsigned char reserved;    //1
unsigned int  bufferSizeDB;//24         //这里是调节比特率的
unsigned int  maxBitrate;  //32
unsigned int  avgBitrate; //32
unsigned char DecSpecificInfotag; //8 = 0x05
unsigned char Length_Field_2; //8  = 0x02
unsigned char audioObjectType_2_GASpecificConfig; //5         //编解码类型：AAC-LC = 0x02
unsigned char samplingFrequencyIndex; //4                     //采样率 44100 = 0x04
unsigned char channelConfiguration; //4                       //声道 = 2
unsigned char cpConfig; //2
unsigned char directMapping;// 1
unsigned char SLConfigDescrTag;//8  = 0x06
unsigned char Length_Field_3;//8    = 0x01
unsigned char predefined;//8  predefined 0x02 Reserved for use in MP4 files

************************************************************************************************************/
class EsdsBox : public BaseBox
{
public:
	EsdsBox();
	~EsdsBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int					m_iVersion;
	int					m_iFlags;
	int					m_iTag;
	int					m_iLengthField1;
	int					m_iEsID;
	int					m_iStreamFlag;
	int					m_iStreamFlagID;
	int					m_iURLFlag;
	int					m_iURLFlagLen;
	string				m_strURLFlagStr;
	int					m_iOCRStreamFlag;
	int					m_iOCRStreamFlagID;
	int					m_iStreamPriority;
	int					m_iDcdTag;
	int					m_iLengthField2;
	int					m_iObjectTypeIn;
	int					m_iStreamType;
	int					m_iUpStream;
	string				m_strReserved1;
	int					m_iBufferSizeDB;
	int					m_iMaxBitrate;
	int					m_iAvgBitrate;
	int					m_iDecInfoTag;
	int					m_iLengthField3;
	//int					m_iaudioObjectType_2_GASpecificConfig; //5         //编解码类型：AAC-LC = 0x02
	//int					m_isamplingFrequencyIndex; //4                     //采样率 44100 = 0x04      
	//unsigned char channelConfiguration; //4                       //声道 = 2
	//unsigned char cpConfig; //2                                  
	//unsigned char directMapping;// 1
	//unsigned char SLConfigDescrTag;//8  = 0x06
	//unsigned char Length_Field_3;//8    = 0x01
	//unsigned char predefined;//8  predefined 0x02 Reserved for use in MP4 files
};