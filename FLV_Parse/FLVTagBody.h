#pragma once
#include "bytes.h"
#include "TagType.h"

/*      TAG_TYPE_...                0x00 */
#define TAG_TYPE_CHUNK_SIZE         0x01
/*      TAG_TYPE_...                0x02 */
#define TAG_TYPE_BYTES_READ_REPORT  0x03
#define TAG_TYPE_CONTROL            0x04
#define TAG_TYPE_SERVER_BW          0x05
#define TAG_TYPE_CLIENT_BW          0x06
/*      TAG_TYPE_...                0x07 */
#define TAG_TYPE_AUDIO              0x08
#define TAG_TYPE_VIDEO              0x09
/*      TAG_TYPE_...                0x0A */
/*      TAG_TYPE_...                0x0B */
/*      TAG_TYPE_...                0x0C */
/*      TAG_TYPE_...                0x0D */
/*      TAG_TYPE_...                0x0E */
#define TAG_TYPE_FLEX_STREAM_SEND   0x0F
#define TAG_TYPE_FLEX_SHARED_OBJECT 0x10
#define TAG_TYPE_FLEX_MESSAGE       0x11
#define TAG_TYPE_SCRIPT             0x12
#define TAG_TYPE_SHARED_OBJECT      0x13
#define TAG_TYPE_INVOKE             0x14
/*      TAG_TYPE_...                0x15 */
#define TAG_TYPE_FLASH_VIDEO        0x16

#define TAG_TYPE_AUDIO_NAME			_T("audio")
#define TAG_TYPE_VIDEO_NAME			_T("video")
#define TAG_TYPE_SCRIPT_NAME		_T("script")
#define TAG_TYPE_UNKNOWN_NAME		_T("unknown")

class CFLVTagBody
{
public:
	CFLVTagBody();
	~CFLVTagBody();

	// 解析数据,返回解析字节数;
	int ReadData(byte_ptr &pData, int iTagSize,int iTagType);
	// 打包数据;
	int WriteData(byte_ptr &pData);

	// 显示;
	void Display();

	bool IsKeyFrame();

public:
	CTagType*	m_pTag;
};