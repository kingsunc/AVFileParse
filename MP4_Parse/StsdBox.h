#pragma once
#include "BaseBox.h"

#define STSD_VERSION_SIZE				1
#define STSD_FLAGS_SIZE					3
#define STSD_ENTRYCOUNT_SIZE			4

/************************************************************************************************************
**										stsd
**
-------------------------------------------------------------------------------------------------------------
**		字段名称			|	长度(bytes)	|		有关描述
-------------------------------------------------------------------------------------------------------------
**		boxsize				|	4			|		box的长度
**		boxtype				|	4			|		box的类型
**		version				|	1			|		box版本0或1 一般为0 (以下字节数均按version=0)
**		flags				|	3			|
**		entry_count			|	4			|		
************************************************************************************************************/
class StsdBox : public BaseBox
{
public:
	StsdBox();
	~StsdBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int				m_iVersion;
	int				m_iFlags;
	int				m_iEntryCount;
};