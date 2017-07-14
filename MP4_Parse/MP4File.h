#pragma once
#include "BaseBox.h"

class CMP4File
{
public:
	CMP4File();
	~CMP4File();

	bool LoadFile(std::string strFile);

	virtual bool ParseFile();

private:
	byteptr	m_pData;
	byteptr	m_pEnd;
	VecBoxs m_vecBoxs;
};