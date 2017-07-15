#pragma once
#include "TagType.h"
#include "amf.h"
#include <vector>

class CTagScript : public CTagType
{
public:
	CTagScript();
	~CTagScript();

	// œ‘ æ;
	virtual void Display();
};