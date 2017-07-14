#include "StsdBox.h"

StsdBox::StsdBox()
{
}

StsdBox::~StsdBox()
{
}

int StsdBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	m_iVersion = BytesToIntEx(pData, STSD_VERSION_SIZE);
	m_iFlags = BytesToIntEx(pData, STSD_FLAGS_SIZE);
	m_iEntryCount = BytesToIntEx(pData, STSD_ENTRYCOUNT_SIZE);

	printf("\t Version       \t : %d \n", m_iVersion);
	printf("\t Flags         \t : %d \n", m_iFlags);
	printf("\t EntryCount    \t : %d \n", m_iEntryCount);

	return pData - pBegin;
}

BaseBox* StsdBox::CreateObject()
{
	return new StsdBox();
}
