
// FLV_UI_Parse.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CFLV_UI_ParseApp: 
// 有关此类的实现，请参阅 FLV_UI_Parse.cpp
//

class CFLV_UI_ParseApp : public CWinApp
{
public:
	CFLV_UI_ParseApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CFLV_UI_ParseApp theApp;