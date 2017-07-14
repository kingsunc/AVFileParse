#pragma once
#include "BaseBox.h"

/************************************************************************************************************
**								Elst Box;
**
************************************************************************************************************/
class ElstBox : public BaseBox
{
public:
	ElstBox();
	~ElstBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();
};