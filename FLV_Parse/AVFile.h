#pragma once
#include <string>

typedef unsigned char byte;

// ÒôÊÓÆµÎÄ¼þ;
class CAVFile
{
public:
	CAVFile();
	virtual ~CAVFile();

	bool LoadFile(std::string strFile);

	virtual bool ParseFile() = 0;

	static int ByteToInt(byte arrByte[], int iCount);

	static std::string ByteToStr(byte arrByte[], int iCount);

	static std::string ByteToStr(char* pByte, int iCount);

protected:
	FILE*	m_pFile;
};