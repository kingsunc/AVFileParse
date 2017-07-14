#pragma once
#include "BaseBox.h"

#define AVC1_RESERVED1_SIZE				6
#define AVC1_DATAREFINDEX_SIZE			2
#define AVC1_PREDEFINED1_SIZE			2
#define AVC1_RESERVED2_SIZE				2
#define AVC1_PREDEFINED2_SIZE			12
#define AVC1_WIDTH_SIZE					2
#define AVC1_HEIGHT_SIZE				2
#define AVC1_HORRES_SIZE				4
#define AVC1_VERRES_SIZE				4
#define AVC1_RESERVED3_SIZE				4
#define AVC1_FRAMECOUNT_SIZE			2
#define AVC1_COMPRESSNAME_SIZE			32
#define AVC1_DEPTH_SIZE					2
#define AVC1_PREDEFINED3_SIZE			2

class Avc1Box : public BaseBox
{
public:
	Avc1Box();
	~Avc1Box();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	string		m_strReserved1;
	int			m_iDataRefIndex;
	string		m_strPredefined1;
	string		m_strReserved2;
	string		m_strPredefined2;
	int			m_iWidth;
	int			m_iHeight;
	double		m_dHorRes;
	double		m_dVerRes;
	string		m_strReserved3;
	int			m_iFrameCount;
	string		m_strCompressName;
	int			m_iDepth;
	string		m_strPredefined3;
};
