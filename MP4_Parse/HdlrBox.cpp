#include "HdlrBox.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HdlrBox
HdlrBox::HdlrBox()
{
}

HdlrBox::~HdlrBox()
{
}

int HdlrBox::ParseAttrs(byteptr &pData)
{
	byteptr pBegin = pData;
	
	m_iVersion = BytesToIntEx(pData, HDLR_VERSION_SIZE);
	m_iFlags = BytesToIntEx(pData, HDLR_FLAGS_SIZE);
	m_iPreDefined = BytesToIntEx(pData, HDLR_PREDEFINED_SIZE);
	m_strHandlerType = BytesToStrEx(pData, HDLR_HANDLERTYPE_SIZE);
	m_strReserved = BytesToStrEx(pData, HDLR_RESERVED_SIZE);
	const int iNameSize = m_iBoxSize - (BOXHEADER_SIZE + (pData - pBegin));
	m_strName = BytesToStrEx(pData, iNameSize);

	printf("\t Version       \t : %d \n", m_iVersion);
	printf("\t Flags         \t : %d \n", m_iFlags);
	printf("\t PreDefined    \t : %d \n", m_iPreDefined);
	printf("\t HandlerType   \t : %s \n", m_strHandlerType.c_str());
	printf("\t Reserved      \t : %s \n", m_strReserved.c_str());
	printf("\t Name          \t : %s \n", m_strName.c_str());

	return pData - pBegin;
}

BaseBox* HdlrBox::CreateObject()
{
	return new HdlrBox();
}
