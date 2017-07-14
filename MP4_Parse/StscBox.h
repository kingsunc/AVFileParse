#pragma once
#include "BaseBox.h"

#define STSC_VERSION_SIZE					1
#define STSC_FLAG_SIZE						3
#define STSC_ENTRYCOUNT_SIZE				4
#define STSC_FIRSTCHUNK_SIZE				4
#define STSC_SAMPPERCHUNK_SIZE				4
#define STSC_SAMPDESINDEX_SIZE				4

struct StscData
{
	int iFirstChunk;
	int iSampPerChunk;
	int iSampDesIndex;
};
typedef vector<StscData> VecStscData;

class StscBox : public BaseBox
{
public:
	StscBox();
	~StscBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int				m_iVersion;
	int				m_iFlag;
	int				m_iEntryCount;
	VecStscData		m_vecStscData;
};