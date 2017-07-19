#pragma once
#include "bytes.h"
#include "FLVTag.h"
#include <vector>

typedef std::vector<CFLVTag*>	VecFlvTag;

class CFLVFileBody
{
public:
	CFLVFileBody();
	~CFLVFileBody();

	// 解析数据,返回解析字节数;
	bool ReadData(byte_ptr &pData, int iSize);
	// 打包数据;
	int WriteData(byte_ptr &pData, double dStart, double dEnd);

	// 显示;
	void Display();

public:
	VecFlvTag	m_vecFlvTags;
};