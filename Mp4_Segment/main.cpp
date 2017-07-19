#include "mp4_segment.h"

int main()
{
	std::string strFile = "./mp4_test.mp4";
	FILE* pFile = fopen(strFile.c_str(), "rb");
	if (!pFile)
	{
		printf("CMP4File::LoadFile(%s): fopen failed. \n", strFile.c_str());
		return 0;
	}
	fseek(pFile, 0, SEEK_END);
	int iSize = ftell(pFile);
	unsigned char* pData = new unsigned char[iSize];
	fseek(pFile, 0, SEEK_SET);
	if (iSize != fread(pData, 1, iSize, pFile))
	{
		printf("read file failed. \n");
		return 0;
	}
	fclose(pFile);

	mp4Buffer inBuffer;
	inBuffer.m_pBuff = pData;
	inBuffer.m_iSize = iSize;
	mp4Buffer outBuffer;

	// ∑÷∏Ó120.0~200√Î
	mp4_segment(outBuffer, inBuffer, 120.0, 200.0);

	{
		std::string strFile = "./mp4_output.mp4";
		FILE* pOutFile = fopen(strFile.c_str(), "wb");
		if (!pOutFile)
		{
			printf("CMP4File::LoadFile(%s): fopen failed. \n", strFile.c_str());
			return 0;
		}

		fwrite(outBuffer.m_pBuff, 1, outBuffer.m_iSize, pOutFile);

		fclose(pOutFile);
	}

	delete[]inBuffer.m_pBuff;
	delete[]outBuffer.m_pBuff;
}