#pragma once
#include "BaseBox.h"

class StcoBox : public BaseBox
{
public:
	StcoBox();
	~StcoBox();

	static BaseBox* CreateObject();

	virtual int ParseAttrs(byteptr &pData);
};