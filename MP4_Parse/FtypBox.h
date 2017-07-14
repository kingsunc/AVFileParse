#pragma once
#include "BaseBox.h"

#define FTYP_MAJORBRAND_SIZE		4
#define FTYP_MINORVERSION_SIZE		4
#define FTYP_COMPBRAND_SIZE			4

/************************************************************************************************************
				File Type Box (ftyp);
该box有且只有1个，并且只能被包含在文件层，而不能被其他box包含;
该box应该被放在文件的最开始，指示该MP4文件应用的相关信息;

major_brand			4字节		0x69736f6d:		"isom"的ASCII码;
minor_version		4字节		0x00000200:		 ismo的版本号;
compatible_brands	N个4字节	说明本文件遵从ismo, iso2, mp41三种协议;
************************************************************************************************************/

class FtypBox : public BaseBox
{
public:
	FtypBox();
	virtual ~FtypBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	string				m_strMajorBrand;
	int					m_iMinorVersion;
	vector<string>		m_vecCompBrands;
};