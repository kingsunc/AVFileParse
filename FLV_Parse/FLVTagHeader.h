#pragma once
#include "bytes.h"

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
struct flvtagheader_data
{
	byte TagType;
	byte DataSize[3];
	byte Timestamp[3];
	byte TimeExtend;
	byte StreamsID[3];
};

class CFLVTagHeader
{
public:
	CFLVTagHeader();
	~CFLVTagHeader();

	// 解析数据,返回解析字节数;
	int ReadData(byte_ptr &pData);
	// 打包数据;
	int WriteData(byte_ptr &pData);

	// 显示;
	void Display();

	// 获取Tag类型;
	int GetTagType();
	// 获取Tag大小;
	int GetTagSize();

public:
	flvtagheader_data m_data;
};