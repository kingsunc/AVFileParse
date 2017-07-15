#include "TagVideo.h"


CTagVideo::CTagVideo()
{
}

CTagVideo::~CTagVideo()
{

}

/************************************************************************************************************
第一个字节是视频信息格式,格式如下;

	名称			长度					介绍;

	帧类型			4 bits 
											1: keyframe(for AVC, a seekable frame)
											2 : inter frame(for AVC, a non - seekable frame)
											3 : disposable inter frame(H.263 only)
											4 : generated keyframe(reserved for server use only)
											5 : video info / command frame

	编码ID			4 bits					
											1 : JPEG(currently unused)
											2 : Sorenson H.263
											3 : Screen video
											4 : On2 VP6
											5 : On2 VP6 with alpha channel
											6 : Screen video version 2
											7 : AVC

第二个字节开始就是视频流数据了;
************************************************************************************************************/
void CTagVideo::Display()
{
	// 解析音频格式;
	byte videoHeader = m_pData[0];
	char strVideoInfo[100] = { 0 };
	{
		byte frameType = videoHeader >> 4;
		switch (frameType)
		{
		case 1:strcat(strVideoInfo, "key frame  "); break;
		case 2:strcat(strVideoInfo, "inter frame"); break;
		case 3:strcat(strVideoInfo, "disposable inter frame"); break;
		case 4:strcat(strVideoInfo, "generated keyframe"); break;
		case 5:strcat(strVideoInfo, "video info/command frame"); break;
		default:strcat(strVideoInfo, "UNKNOWN"); break;
		}
		strcat(strVideoInfo, "| ");
	}

	{
		byte sampBits = videoHeader << 4;
		sampBits = sampBits >> 4;
		switch (sampBits)
		{
		case 1:strcat(strVideoInfo, "JPEG (currently unused)"); break;
		case 2:strcat(strVideoInfo, "Sorenson H.263"); break;
		case 3:strcat(strVideoInfo, "Screen video"); break;
		case 4:strcat(strVideoInfo, "On2 VP6"); break;
		case 5:strcat(strVideoInfo, "On2 VP6 with alpha channel"); break;
		case 6:strcat(strVideoInfo, "Screen video version 2"); break;
		case 7:strcat(strVideoInfo, "AVC"); break;
		default:strcat(strVideoInfo, "UNKNOWN"); break;
		}
		strcat(strVideoInfo, "| ");
	}

	printf("\t %s video data: %d bytes \n", strVideoInfo, m_iSize - 1);
}

bool CTagVideo::IsKeyFrame()
{
	bool bIsKeyFrame = (1 == (m_pData[0] >> 4)) ? true : false;
	return true;
}
