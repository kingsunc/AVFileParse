#pragma once
#include "BaseBox.h"

#define STSZ_VERSION_SIZE				1
#define STSZ_Flag_SIZE					3
#define STSZ_SAMPLESIZE_SIZE			4
#define STSZ_SAMPLECOUNT_SIZE			4
#define STSZ_VECSAMPLESIZE_SIZE			4

typedef vector<int> VecSampleSize;

class StszBox : public BaseBox
{
public:
	StszBox();
	~StszBox();

	static BaseBox* CreateObject();

	virtual int ParseAttrs(byteptr &pData);

public:
	int				m_iVersion;
	int				m_iFlag;
	int				m_iSampleSize;
	int				m_iSampleCount;
	VecSampleSize	m_vecSampleSize;
};