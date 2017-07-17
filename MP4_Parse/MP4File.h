#pragma once
#include "BaseBox.h"

class CMP4File
{
public:
	CMP4File();
	~CMP4File();

	// 加载文件;
	bool LoadFile(std::string strFile);
	bool LoadFile(byte* pData, int iDataSize);

	// 保存文件;
	bool SaveFile(std::string strFile);
	bool SaveFile();

private:
	VecBoxs m_vecBoxs;
};