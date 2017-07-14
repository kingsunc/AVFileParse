#pragma once
#include "BaseBox.h"

#define URL_VERSION_SIZE				1
#define URL_FLAGS_SIZE					3

/************************************************************************************************************
**								Url Box
**
************************************************************************************************************/
class UrlBox : public BaseBox
{
public:
	UrlBox();
	~UrlBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int				m_iVersion;
	int				m_iFlags;
};