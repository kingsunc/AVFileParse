#pragma once
#include "TagType.h"

class CTagVideo : public CTagType
{
public:
	CTagVideo();
	~CTagVideo();

	// 显示;
	virtual void Display();

	// 是否视频的关键帧;
	virtual bool IsKeyFrame();
};