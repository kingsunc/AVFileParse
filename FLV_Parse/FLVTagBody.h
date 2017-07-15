#pragma once
#include "bytes.h"
#include "TagType.h"

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

private:
	CTagType*	m_pTag;
};