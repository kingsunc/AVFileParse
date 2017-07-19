#pragma once
#include "FLVFileHeader.h"
#include "FLVFileBody.h"

class CFLVFile
{
public:
	CFLVFile();
	~CFLVFile();

public:

	// 加载文件,读取数据;
	bool LoadFile(const char* strFile);

	// 起始dStart~dEnd的音视频保存文件;
	// 要求0~1, dEnd > dStart 否则返回 false;
	bool SaveFile(const char* strFile, double dStart, double dEnd);

	// 显示;
	void Display();

protected:
	// 解析数据;
	bool ReadData(byte_ptr pData, int iSize);
	// 打包数据;
	bool WriteData(byte_ptr pData, int &iSize, double dStart, double dEnd);

public:
	CFLVFileHeader		m_flvHeader;
	CFLVFileBody		m_flvBody;
	int					m_iSize;
};