#pragma once
#include "AVFile.h"

/*      PACKET_TYPE_...                0x00 */
#define PACKET_TYPE_CHUNK_SIZE         0x01
/*      PACKET_TYPE_...                0x02 */
#define PACKET_TYPE_BYTES_READ_REPORT  0x03
#define PACKET_TYPE_CONTROL            0x04
#define PACKET_TYPE_SERVER_BW          0x05
#define PACKET_TYPE_CLIENT_BW          0x06
/*      PACKET_TYPE_...                0x07 */
#define PACKET_TYPE_AUDIO              0x08
#define PACKET_TYPE_VIDEO              0x09
/*      PACKET_TYPE_...                0x0A */
/*      PACKET_TYPE_...                0x0B */
/*      PACKET_TYPE_...                0x0C */
/*      PACKET_TYPE_...                0x0D */
/*      PACKET_TYPE_...                0x0E */
#define PACKET_TYPE_FLEX_STREAM_SEND   0x0F
#define PACKET_TYPE_FLEX_SHARED_OBJECT 0x10
#define PACKET_TYPE_FLEX_MESSAGE       0x11
#define PACKET_TYPE_SCRIPT             0x12
#define PACKET_TYPE_SHARED_OBJECT      0x13
#define PACKET_TYPE_INVOKE             0x14
/*      PACKET_TYPE_...                0x15 */
#define PACKET_TYPE_FLASH_VIDEO        0x16

/************************************************************************************************************
header部分记录了flv的类型、版本等信息，是flv的开头，一般都差不多，占9bytes.

具体格式如下;
文件类型	3 bytes		"FLV";
版本		1 byte		一般为0x01;
流信息		1 byte		倒数第一位是1表示有视频，倒数第三位是1表示有音频，倒数第二、四位必须为0;
header长度	4 bytes		整个header的长度，一般为9；大于9表示下面还有扩展信息;
************************************************************************************************************/
struct FlvHeader
{
	byte Type[3];
	byte Version;
	byte StreamInfo;
	byte HeaderSize[4];
};

/************************************************************************************************************
每个Tag由也是由两部分组成的：Tag Header和Tag Data;
Tag Header里存放的是当前Tag的类型、数据区（Tag Data）长度等信息;
具体如下;

名称			长度					介绍;
Tag类型			1 bytes					8：音频 \ 9：视频 \ 18：脚本 \  其他：保留;
数据区长度		3 bytes					在数据区的长度;
时间戳			3 bytes					整数，单位是毫秒 对于脚本型的tag总是0;
时间戳扩展		1 bytes					将时间戳扩展为4bytes,代表高8位 很少用到;
StreamsID		3 bytes					总是0;
数据区(data)	由数据区长度决定		数据实体;
************************************************************************************************************/
struct TagHeader
{
	byte TagType;
	byte DataSize[3];
	byte Timestamp[3];
	byte TimeExtend;
	byte StreamsID[3];
};

class CFLVFile: public CAVFile
{
public:
	CFLVFile();
	virtual ~CFLVFile();

	virtual bool ParseFile();

protected:
	bool ParseFLVHeader();
	bool ParseFLVBody();

	bool ParseAudioData(int iDataSize);
	bool ParseVideoData(int iDataSize);
	bool ParseScriptData(int iDataSize);
};