#pragma once
#include "BaseBox.h"
#include <map>

typedef BaseBox* (*CreateObject)();
typedef std::map<std::string, CreateObject> MAP_BOX_CREATEOBJECT;

class BoxFactory
{
private:
	BoxFactory();
	virtual ~BoxFactory();

public:

	static BoxFactory* GetInstance();
	BaseBox* CreateBox(string strType);

	MAP_BOX_CREATEOBJECT	m_mapCreateObj;
};

#define AfxGetBoxFactory	BoxFactory::GetInstance
#define AfxCreateBox		AfxGetBoxFactory()->CreateBox