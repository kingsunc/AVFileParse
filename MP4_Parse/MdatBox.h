#pragma once
#include "BaseBox.h"

/************************************************************************************************************
**									Mdat Box
**
************************************************************************************************************/
class MdatBox : public BaseBox
{
public:
	MdatBox();
	~MdatBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	byteptr		m_pData;
};
