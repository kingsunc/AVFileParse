#pragma once
#include <string>

typedef unsigned char byte;

// ÒôÊÓÆµÎÄ¼ş;
class CAVFile
{
public:
	CAVFile();
	virtual ~CAVFile();

	bool LoadFile(std::string strFile);

	virtual bool ParseFile() = 0;

	int ByteToInt(byte Type[], int iCount);

protected:
	FILE*	m_pFile;
};