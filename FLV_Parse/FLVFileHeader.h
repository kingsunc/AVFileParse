#pragma once
#include "bytes.h"

/************************************************************************************************************
header部分记录了flv的类型、版本等信息，是flv的开头，一般都差不多，占9bytes.

具体格式如下;
文件类型	3 bytes		"FLV";
版本		1 byte		一般为0x01;
流信息		1 byte		倒数第一位是1表示有视频，倒数第三位是1表示有音频，倒数第二、四位必须为0;
header长度	4 bytes		整个header的长度，一般为9；大于9表示下面还有扩展信息;
************************************************************************************************************/
struct flvfileheader_data
{
	byte Type[3];
	byte Version;
	byte StreamInfo;
	byte HeaderSize[4];
};

class CFLVFileHeader
{
public:
	CFLVFileHeader();
	~CFLVFileHeader();

	// 解析数据,返回解析字节数;
	int ReadData(byte_ptr &pData);
	// 打包数据;
	int WriteData(byte_ptr &pData);

	// 显示;
	void Display();

public:
	flvfileheader_data		m_data;
};