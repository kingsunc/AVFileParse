#pragma once
#include "BaseBox.h"

/************************************************************************************************************
Udta Box

************************************************************************************************************/
class UdtaBox : public BaseBox
{
public:
	UdtaBox();
	~UdtaBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();
};