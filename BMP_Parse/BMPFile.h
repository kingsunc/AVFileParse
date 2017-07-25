#pragma once
#include "bytes.h"
#include <windows.h>

class CBMPFile
{
public:
	CBMPFile();
	~CBMPFile();

public:
	// 加载文件,读取数据;
	bool LoadFile(const char* strFile);

	bool SaveFile(const char* strFile, RECT rt = {0});

	// 显示;
	void Display();

protected:
	// 解析数据;
	bool ReadData(byte_ptr pData, int iSize);
	// 打包数据;
	bool WriteData(byte_ptr pData, int &iSize, double dStart, double dEnd);

public:
	BITMAPFILEHEADER	m_bmpFileHeader;
	BITMAPINFOHEADER	m_bmpInfoHeader;
	byte*				m_pData;
	int					m_iFileSize;
};