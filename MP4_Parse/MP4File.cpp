#include "MP4File.h"


CMP4File::CMP4File()
{
	m_pData = NULL;
	m_vecBoxs.clear();
}

CMP4File::~CMP4File()
{
	for (VecBoxs::iterator itor = m_vecBoxs.begin(); itor != m_vecBoxs.end(); itor++)
	{
		delete *itor;
	}
	m_vecBoxs.clear();

	if (m_pData)
	{
		delete []m_pData;
		m_pData = NULL;
	}
}

bool CMP4File::LoadFile(std::string strFile)
{
	FILE* pFile = fopen(strFile.c_str(), "rb");
	fseek(pFile, 0, SEEK_END);
	int iSize = ftell(pFile);
	m_pData = new byte[iSize];
	m_pEnd = m_pData + iSize;
	fseek(pFile, 0, SEEK_SET);
	if (iSize != fread(m_pData, 1, iSize, pFile))
	{
		printf("read file failed. \n");
		return false;
	}
	fclose(pFile);

	return ParseFile();
}

bool CMP4File::ParseFile()
{
	byteptr pCur = m_pData;
	while (pCur < m_pEnd)
	{
		BaseBox* pBox = BaseBox::GetBoxFromFile(pCur);
		if (pBox)
		{
			int iParseSize = pBox->ParseBox(pCur);
			if (iParseSize != pBox->m_iBoxSize)
			{
				printf("CMP4File::ParseFile£º iParseSize != pBox->m_iBoxSize: %s \n", pBox->m_strBoxType.c_str());
				delete pBox;
				return false;
			}

			m_vecBoxs.push_back(pBox);
		}
	}

	return true;
}