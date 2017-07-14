#pragma once
#include "BaseBox.h"

/************************************************************************************************************
								Movie Box (moov)

************************************************************************************************************/
class MoovBox : public BaseBox
{
public:
	MoovBox();
	virtual ~MoovBox();

	static BaseBox* CreateObject();
};
