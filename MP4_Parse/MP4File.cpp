#include "MP4File.h"


CMP4File::CMP4File()
{
	m_vecBoxs.clear();
}

CMP4File::~CMP4File()
{
	for (VecBoxs::iterator itor = m_vecBoxs.begin(); itor != m_vecBoxs.end(); itor++)
	{
		delete *itor;
	}
	m_vecBoxs.clear();
}

bool CMP4File::LoadFile(std::string strFile)
{
	FILE* pFile = fopen(strFile.c_str(), "rb");
	if (!pFile)
	{
		printf("CMP4File::LoadFile(%s): fopen failed. \n", strFile.c_str());
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	int iSize = ftell(pFile);
	byte* pData = new byte[iSize];
	fseek(pFile, 0, SEEK_SET);
	if (iSize != fread(pData, 1, iSize, pFile))
	{
		printf("read file failed. \n");
		return false;
	}
	fclose(pFile);

	bool bRes = LoadFile(pData, iSize);
	delete[]pData;
	pData = NULL;

	return bRes;
}

bool CMP4File::LoadFile(byte* pData, int iDataSize)
{
	byteptr pCur = pData;
	byteptr pEnd = pCur + iDataSize;
	while (pCur < pEnd)
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

bool CMP4File::SaveFile(std::string strFile)
{
	/*CMP4File::SaveFile();

	FILE* pFile = fopen(strFile.c_str(), "rb");
	if (!pFile)
	{
		printf("CMP4File::LoadFile(%s): fopen failed. \n", strFile.c_str());
		return false;
	}
	fwrite(m_pSaveData,)
	int iFileSize = m_pLoadEnd - m_pLoadData;
	byte* pData = new byte[iFileSize];*/

	return true;
}

bool CMP4File::SaveFile()
{
	return true;
}

