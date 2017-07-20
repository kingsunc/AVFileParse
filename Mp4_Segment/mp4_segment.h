#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include <string.h>
#include <string>

#define IN	
#define OUT	

struct mp4Buffer
{
	unsigned int		m_iSize;
	unsigned char*		m_pBuff;
};

/************************************************************************************************************
*	@brief		: 分割mp4文件(截取iStartTime~iEndTime秒内的音视频数据并重新组装成小的mp4文件)
*	@author		: sunc
*	@date		: 2017/7/17 17:33
*	@version	: ver 1.0
*	@param		: 
				outMp4		:		输出的mp4buffer
				inMp4		:		输入的mp4buffer
				fStartTime	:		截取的开始时间;
				fEndTime	:		截取的结束时间;

*	@return	:
				成功返回true, 失败返回false;

************************************************************************************************************/
bool mp4_segment(OUT mp4Buffer& outMp4, IN mp4Buffer& inMp4, IN float fStartTime, IN float fEndTime);
