#pragma once
#include "BaseBox.h"

#define MDHD_VERSION_SIZE				1
#define MDHD_FLAGS_SIZE					3
#define MDHD_CREATETIME_SIZE			4
#define MDHD_MODIFYTIME_SIZE			4
#define MDHD_TIMESCALE_SIZE				4
#define MDHD_DURATION_SIZE				4
#define MDHD_LANGUAGE_SIZE				2
#define MDHD_PREDEFINED_SIZE			2

/************************************************************************************************************
**										tkhd
**
-------------------------------------------------------------------------------------------------------------
**		字段名称			|	长度(bytes)	|		有关描述
-------------------------------------------------------------------------------------------------------------
**		boxsize				|	4			|		box的长度
**		boxtype				|	4			|		box的类型
**		version				|	1			|		box版本0或1 一般为0 (以下字节数均按version=0)
**		flags				|	3			|		
**		creation_time		|	4			|		创建时间（相对于UTC时间1904 - 01 - 01零点的秒数）
**		modification_time	|	4			|		修改时间
**		time_scale			|	4			|		
**		duration			|	4			|		track的时间长度
**		language			|	2			|		媒体语言码,最高位为0 后面15位为3个字符[见ISO 639-2/T标准中定义]
**		pre-defined			|	2			|		保留位
************************************************************************************************************/
class MdhdBox : public BaseBox
{
public:
	MdhdBox();
	~MdhdBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int				m_iVersion;
	int				m_iFlags;
	int				m_iCreateTime;
	int				m_iModifyTime;
	int				m_iTimeScale;
	int				m_iDuration;
	int				m_iLanguage;
	int				m_iPreDefined;
};