#pragma once
#include "BaseBox.h"

#define STSD_VERSION_SIZE				1
#define STSD_FLAGS_SIZE					3
#define STSD_ENTRYCOUNT_SIZE			4

/************************************************************************************************************
**					sample description box (stsd);
**   
**	"stsd"必不可少,且至少包含一个条目 该box包含了data reference box进行sample数据检索的信息;
**	没有“stsd”就无法计算media sample的存储位置. "stsd"包含了编码的信息，其存储的信息随媒体类型不同而不同;
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