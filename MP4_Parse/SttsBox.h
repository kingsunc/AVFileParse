#pragma once
#include "BaseBox.h"

#define STTS_ENTRYCOUNT_SIZE		8 
#define STTS_SAMPLECOUNT_SIZE		4
#define STTS_SAMPLEDELTA_SIZE		4

struct SampleData
{
	int iSampleCount;
	int iSampleDelta;
};
typedef vector<SampleData> VecSampleData;

class SttsBox : public BaseBox
{
public:
	SttsBox();
	~SttsBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int					m_iEntryCount;
	VecSampleData		m_vecSampleDatas;
};