#pragma once
#include "BaseBox.h"

#define DREF_VERSION_SIZE				1
#define DREF_FLAGS_SIZE					3
#define DREF_ENTRYCOUNT_SIZE			4

/************************************************************************************************************
							Dref Box

version					1					box版本 0或1 一般为0.（以下字节数均按version=0）
flags					3					
entry count				4					"url"或"urn"表的元素个数
"url"或"urn"列表		不定
************************************************************************************************************/
class DrefBox : public BaseBox
{
public:
	DrefBox();
	~DrefBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int				m_iVersion;
	int				m_iFlags;
	int				m_iEntryCount;
};