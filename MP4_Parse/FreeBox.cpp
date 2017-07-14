#include "FreeBox.h"

FreeBox::FreeBox()
{
}

FreeBox::~FreeBox()
{
}

BaseBox* FreeBox::CreateObject()
{
	return new FreeBox();
}
