#include "DrefBox.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DrefBox
DrefBox::DrefBox()
{
}

DrefBox::~DrefBox()
{
}

int DrefBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;

	m_iVersion = BytesToIntEx(pData, DREF_VERSION_SIZE);
	m_iFlags = BytesToIntEx(pData, DREF_FLAGS_SIZE);
	m_iEntryCount = BytesToIntEx(pData, DREF_ENTRYCOUNT_SIZE);

	printf("\t Version       \t : %d \n", m_iVersion);
	printf("\t Flags         \t : %d \n", m_iFlags);
	printf("\t EntryCount    \t : %d \n", m_iEntryCount);

	return pData - pBegin;
}

BaseBox* DrefBox::CreateObject()
{
	return new DrefBox();
}
