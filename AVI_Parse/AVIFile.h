#pragma once
#include "AVIData.h"

class CAVIFile
{
public:
	CAVIFile();
	~CAVIFile();

public:
	// 加载文件,读取数据;
	bool LoadFile(char* strFile);

protected:
	// 解析数据;
	bool ReadData(byte_ptr pData, int iSize);

	int ReadAviHeader(byte_ptr &pData);
	int ReadList(byte_ptr &pData);
	int ReadHdrl(byte_ptr &pData);
	int ReadStrl(byte_ptr &pData, byte_ptr pEnd);
	int ReadStrh(byte_ptr &pData, std::string& strType);
	int ReadStrf(byte_ptr &pData, const std::string strType);
	int ReadMovi(byte_ptr &pData, byte_ptr pEnd);
	int ReadIdx1(byte_ptr &pData, byte_ptr pEnd);
};