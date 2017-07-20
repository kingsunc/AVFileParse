#pragma once
#include "BaseBox.h"

#define TKHD_VERSION_SIZE			1
#define TKHD_FLAGS_SIZE				3
#define TKHD_CREATETIME_SIZE		4
#define TKHD_MODIFYTIME_SIZE		4
#define TKHD_TRACKID_SIZE			4
#define TKHD_RESERVED1_SIZE			4
#define TKHD_DURATION_SIZE			4
#define TKHD_RESERVED2_SIZE			8
#define TKHD_LAYER_SIZE				2
#define TKHD_ALTERGROUP_SIZE		2
#define TKHD_VOLUME_SIZE			2
#define TKHD_RESERVED3_SIZE			2
#define TKHD_MATRIX_SIZE			36
#define TKHD_WIDTH_SIZE				4
#define TKHD_HEIGHT_SIZE			4

/************************************************************************************************************
**										tkhd
**
-------------------------------------------------------------------------------------------------------------
**		字段名称			|	长度(bytes)	|		有关描述
-------------------------------------------------------------------------------------------------------------
**		boxsize				|	4			|		box的长度
**		boxtype				|	4			|		box的类型
**		version				|	1			|		box版本，0或1，一般为0。（以下字节数均按version = 0）
**		flags				|	3			|		按位或操作结果值，预定义如下;
													0x000001 track_enabled，否则该track不被播放；
													0x000002 track_in_movie，表示该track在播放中被引用；
													0x000004 track_in_preview，表示该track在预览时被引用。
													一般该值为7，如果一个媒体所有track均未设置track_in_movie和track_in_preview,将被理解为所有track均设置了这两项;
													对于hint track，该值为0;
**		creation_time		|	4			|		创建时间（相对于UTC时间1904 - 01 - 01零点的秒数）
**		modification_time	|	4			|		修改时间
**		track_id			|	4			|		id号 不能重复且不能为0
**		reserved			|	4			|		保留位
**		duration			|	4			|		track的时间长度
**		reserved			|	8			|		保留位
**		layer				|	2			|		视频层，默认为0，值小的在上层
**		alternate_group		|	2			|		track分组信息，默认为0表示该track未与其他track有群组关系
**		volume				|	2			|		[8.8] 格式，如果为音频track，1.0（0x0100）表示最大音量；否则为0
**		reserved			|	2			|		保留位
**		matrix				|	36			|		视频变换矩阵
**		width				|	4			|		宽
**		height				|	4			|		高，均为[16.16] 格式值 与sample描述中的实际画面大小比值，用于播放时的展示宽高
************************************************************************************************************/
class TkhdBox : public BaseBox
{
public:
	TkhdBox();
	~TkhdBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int				m_iVersion;
	int				m_iFlags;
	int				m_iCreateTime;
	int				m_iModifyTime;
	int				m_iTrackID;
	string			m_strReserved1;
	int				m_iDuration;
	string			m_strReserved2;
	int				m_iLayer;
	int				m_iAlterGroup;
	double			m_dVolume;
	string			m_strReserved3;
	string			m_strMatrix;
	double			m_dWidth;
	double			m_dHeight;
};