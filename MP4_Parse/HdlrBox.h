#pragma once
#include "BaseBox.h"

#define HDLR_VERSION_SIZE				1
#define HDLR_FLAGS_SIZE					3
#define HDLR_PREDEFINED_SIZE			4
#define HDLR_HANDLERTYPE_SIZE			4
#define HDLR_RESERVED_SIZE				12

/************************************************************************************************************
**										hdlr
**
-------------------------------------------------------------------------------------------------------------
**		字段名称			|	长度(bytes)	|		有关描述
-------------------------------------------------------------------------------------------------------------
**		boxsize				|	4			|		box的长度
**		boxtype				|	4			|		box的类型
**		version				|	1			|		box版本0或1 一般为0 (以下字节数均按version=0)
**		flags				|	3			|
**		pre-defined			|	4			|
**		handler type		|	4			|		在media box中，该值为4个字符
													"vide"— video track
													"soun"— audio track
													"hint"— hint track
**		reserved			|	12			|
**		name				|	不定		|		track type name，以‘\0’结尾的字符串
************************************************************************************************************/
class HdlrBox : public BaseBox
{
public:
	HdlrBox();
	~HdlrBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int				m_iVersion;
	int				m_iFlags;
	int				m_iPreDefined;
	string			m_strHandlerType;
	string			m_strReserved;
	string			m_strName;
};
