#include "FLVTagBody.h"
#include "TagAudio.h"
#include "TagVideo.h"
#include "TagScript.h"

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

CFLVTagBody::CFLVTagBody()
{
	m_pTag = NULL;
}

CFLVTagBody::~CFLVTagBody()
{
	if (m_pTag)
	{
		delete m_pTag;
		m_pTag = NULL;
	}
}

int CFLVTagBody::ReadData(byte_ptr &pData, int iTagSize, int iTagType)
{
	switch (iTagType)
	{
	case TAG_TYPE_AUDIO:	// 音频;
		{
			m_pTag = new CTagAudio();
		}
		break;
	case TAG_TYPE_VIDEO:	// 音频;
		{
			m_pTag = new CTagVideo();
		}
		break;
	case TAG_TYPE_SCRIPT:	// 脚本, 自定义数据;
		{
			m_pTag = new CTagScript();
		}
		break;
	default:
		{
			printf("\t unknown tagtype ： %d \n", iTagType);
			return -1;
		}
		break;
	}

	if (m_pTag)
	{
		int iParseSize = m_pTag->ReadData(pData, iTagSize);
		assert(iParseSize == iTagSize);
	}
	else
	{
		// 若不识别类型,则跳过;
		pData += iTagSize;
	}

	return iTagSize;
}

int CFLVTagBody::WriteData(byte_ptr &pData)
{
	int iSize = -1;
	if (m_pTag)
	{
		iSize = m_pTag->WriteData(pData);
	}
	return iSize;
}

void CFLVTagBody::Display()
{
	if (m_pTag)
	{
		m_pTag->Display();
	}
}

bool CFLVTagBody::IsKeyFrame()
{
	bool bIsKeyFrame = false;
	if (m_pTag)
	{
		bIsKeyFrame = m_pTag->IsKeyFrame();
	}
	return bIsKeyFrame;
}