#pragma once
#include "bytes.h"

class CTagType
{
public:
	CTagType();
	virtual ~CTagType();

	// 解析数据,返回解析字节数;
	int ReadData(byte_ptr &pData, int iTagSize) ;
	// 打包数据;
	int WriteData(byte_ptr &pData);

	// 显示;
	virtual void Display() = 0;

	// 是否视频的关键帧;
	virtual bool IsKeyFrame();

public:
	byte_ptr		m_pData;
	int				m_iSize;
};

