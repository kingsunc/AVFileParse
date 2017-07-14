#pragma once
#include "BaseBox.h"

#define STSS_ENTRYCOUNT_SIZE		8 
#define STSS_SAMPLENUM_SIZE			4

typedef vector<int> VecSampleNums;

class StssBox : public BaseBox
{
public:
	StssBox();
	~StssBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:

	int					m_iEntryCount;
	VecSampleNums		m_vecSampleNums;
};
