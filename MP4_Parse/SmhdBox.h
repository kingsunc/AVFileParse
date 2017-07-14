#pragma once
#include "BaseBox.h"

#define SMHD_VERSION_SIZE			1
#define SMHD_FLAGS_SIZE				3
#define SMHD_BALANCE_SIZE			2
#define SMHD_RESERVED_SIZE			2

/************************************************************************************************************
							Smhd Box

version					1					box版本，0或1，一般为0。（以下字节数均按version=0）
flags					3
balance					2					立体声平衡，[8.8] 格式值，一般为0，-1.0表示全部左声道，1.0表示全部右声道
reserved				2
************************************************************************************************************/
class SmhdBox : public BaseBox
{
public:
	SmhdBox();
	~SmhdBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int				m_iVersion;
	int				m_iFlags;
	double			m_dBalance;
	string			m_strReserved;
};