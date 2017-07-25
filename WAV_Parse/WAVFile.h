#pragma once
#include "bytes.h"
#include <windows.h>

struct WAVE_FORMAT
{
	WORD	wFormatTag;
	WORD	wChannels;
	DWORD	dwSamplesPerSec;
	DWORD	dwAvgBytesPerSec;
	WORD	wBlockAlign;
	WORD	wBitsPerSample;
};

struct WAVE_HEADER
{
	// RIFF
	byte		szRiffID[4];		// 'R','I','F','F'
	DWORD		dwRiffSize;
	byte		szRiffFormat[4];	// 'W','A','V','E'
	// fmt
	byte		szFmtID[4];			// 'f','m','t',' '
	DWORD		dwFmtSize;
	WAVE_FORMAT wavFormat;
	// data
	byte		szDataID[4];		// 'd','a','t','a '
	DWORD		dwDataSize;
};

class CWAVFile
{
public:
	CWAVFile();
	~CWAVFile();

public:
	// 加载文件,读取数据;
	bool LoadFile(const char* strFile);

	bool SaveFile(const char* strFile, RECT rt = { 0 });

	// 显示;
	void Display();

protected:
	// 解析数据;
	bool ReadData(byte_ptr pData, int iSize);
	// 打包数据;
	bool WriteData(byte_ptr pData, int &iSize, double dStart, double dEnd);

public:
	WAVE_HEADER			m_WaveHeader;
	byte*				m_pData;
	int					m_iSize;
};