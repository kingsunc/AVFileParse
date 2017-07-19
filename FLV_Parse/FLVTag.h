#pragma once
#include "bytes.h"
#include "FLVTagHeader.h"
#include "FLVTagBody.h"

class CFLVTag
{
public:
	CFLVTag();
	~CFLVTag();

	// 解析数据,返回解析字节数;
	int ReadData(byte_ptr &pData);
	// 打包数据;
	int WriteData(byte_ptr &pData);

	// 显示;
	void Display();

	bool IsKeyFrame();

public:
	CFLVTagHeader	m_tagHeader;
	CFLVTagBody		m_tagBody;
};

