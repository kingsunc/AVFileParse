#pragma once
#include "BaseBox.h"

#define MVHD_VERSION_SIZE			1
#define MVHD_FLAGS_SIZE				3
#define MVHD_CREATETIME_SIZE		4
#define MVHD_MODIFYTIME_SIZE		4
#define MVHD_TIMESCALE_SIZE			4
#define MVHD_DURATION_SIZE			4
#define MVHD_RATE_SIZE				4
#define MVHD_VOLUME_SIZE			2
#define MVHD_RESERVED_SIZE			10
#define MVHD_MATRIX_SIZE			36
#define MVHD_PREDEFINED_SIZE		24
#define MVHD_NEXTTRACKID_SIZE		4

/************************************************************************************************************
**											mvhd
**
--------------------------------------------------------------------------------------------
**		字段名称			|	长度(bytes)	|		有关描述
--------------------------------------------------------------------------------------------
**		boxsize				|	4			|		box的长度
**		boxtype				|	4			|		box的类型
**		version				|	1			|		box版本，0或1，一般为0（以下字节数均按version = 0）
**		flags				|	3			|		
**		creation time		|	4			|		创建时间（相对于UTC时间1904 - 01 - 01零点的秒数）
**		modification time	|	4			|		修改时间
**		time scale			|	4			|		文件媒体在1秒时间内的刻度值，可以理解为1秒长度的时间单元数
**		duration			|	4			|		该track的时间长度，用duration和time scale值可以计算track时长
**		rate				|	4			|		推荐播放速率，高16位和低16位分别为小数点整数部分和小数部分，即[16.16] 格式.该值为1.0（0x00010000）表示正常前向播放
**		volume				|	2			|		与rate类似，[8.8] 格式，1.0（0x0100）表示最大音量
**		reserved			|	10			|		保留位
**		matrix				|	36			|		视频变换矩阵
**		pre-defined			|	24			|		
**		next track id		|	4			|		下一个track使用的id号
************************************************************************************************************/
class MvhdBox : public BaseBox
{
public:
	MvhdBox();
	virtual ~MvhdBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int				m_iVersion;
	int				m_iFlags;
	int				m_iCreateTime;
	int				m_iModifyTime;
	int				m_iTimeScale;
	int				m_iDuration;
	double			m_dRate;
	double			m_dVolume;
	string			m_strReserved;
	string			m_strMatrix;
	string			m_strPreDefined;
	int				m_iNextTrackID;
};