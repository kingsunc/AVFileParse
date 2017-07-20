#pragma once
#include "BaseBox.h"

#define FTYP_MAJORBRAND_SIZE		4
#define FTYP_MINORVERSION_SIZE		4
#define FTYP_COMPBRAND_SIZE			4

/********************************************************************************************
**							File Type Box (ftyp)
**
--------------------------------------------------------------------------------------------
**		字段名称			|	长度(bytes)	|		有关描述
--------------------------------------------------------------------------------------------
**		boxsize				|	4			|		box的长度
**		boxtype				|	4			|		box的类型
**		major_brand			|	4			|
**		minor_version		|	4			|		版本号
**		compatible_brands	|	4 * N		|		本文件遵从的多种协议（ismo, iso2, mp41）
********************************************************************************************/
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