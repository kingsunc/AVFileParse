#include "WAVEncoder.h"
#include "WAVDecoder.h"

typedef unsigned char byte;

struct WAVE_FORMAT
{
	WORD	wFormatTag;
	WORD	wChannels;
	DWORD	dwSamplesPerSec;
	DWORD	dwAvgBytesPerSec;
	WORD	wBlockAlign;
	WORD	wBitsPerSample;
};

struct WAVE_HEADER
{
	// RIFF
	byte		szRiffID[4];		// 'R','I','F','F'
	DWORD		dwRiffSize;
	char		szRiffFormat[4];	// 'W','A','V','E'
	// fmt
	byte		szFmtID[4];			// 'f','m','t',' '
	DWORD		dwFmtSize;
	WAVE_FORMAT wavFormat;
	// data
	byte		szDataID[4];		// 'd','a','t','a '
	DWORD		dwDataSize;
};


int Main_OnGetAudioBuff(const char* pBuff, const int& iBuffSize);
int Main_OnSetAudioBuff(char* pBuff, int iBuffSize);

FILE*		g_pInFile = NULL;
FILE*		g_pOutFile = NULL;
AudioInfo	g_audioInfo;


static DWORD MyThread(DWORD wPame)
{
	Sleep(10 * 1000);

	return 0;
}

// WAV采集编码保存;
// 采集PCM原始数据并保存文件;
int WAV_Encoder(char* strPCMFile)
{
	g_pInFile = fopen(strPCMFile, "wb");
	if (!g_pInFile)
	{
		printf("fopen is failed. \n");
		return 0;
	}

	CWAVEncoder wavEncoder;
	wavEncoder.SetAudioBack(Main_OnGetAudioBuff);

	g_audioInfo.iAvgBytesPerSec = 96000;
	g_audioInfo.iChannels = 2;
	g_audioInfo.iBitsPerSample = 16;
	g_audioInfo.iSamplesPerSec = 44100;
	wavEncoder.SetAudioInfo(g_audioInfo);

	wavEncoder.Start();
	HANDLE thWait = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MyThread, NULL, NULL, NULL);
	WaitForSingleObject(thWait, INFINITE);	//用线程只是为了等上几秒，否则会直接结束;
	wavEncoder.Stop();

	Sleep(3000); // 等上几秒，否则容易卡，界面编程时经常遇到;
	wavEncoder.Clear();

	fclose(g_pInFile);
	g_pInFile = NULL;

	return 1;
}

// 将PCM原始数据打开并播放;
int WAV_Decoder(char* strPCMFile)
{
	g_pOutFile = fopen(strPCMFile, "rb");
	if (!g_pOutFile)
	{
		printf("fopen is failed. \n");
		return 0;
	}

	CWAVDecoder wavDecoder;
	wavDecoder.SetAudioBack(Main_OnSetAudioBuff);

	g_audioInfo.iAvgBytesPerSec = 96000;
	g_audioInfo.iChannels = 2;
	g_audioInfo.iBitsPerSample = 16;
	g_audioInfo.iSamplesPerSec = 44100;
	wavDecoder.SetAudioInfo(g_audioInfo);

	wavDecoder.Start();
	HANDLE thWait = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MyThread, NULL, NULL, NULL);
	WaitForSingleObject(thWait, INFINITE);	//用线程只是为了等上几秒，否则会直接结束;
	wavDecoder.Stop();

	Sleep(3000); // 等上几秒，否则容易卡;
	wavDecoder.Clear();

	fclose(g_pOutFile);
	g_pOutFile = NULL;

	return 1;
}

// PCM转WAV 只需添加44个字节的头部就可以啦;
int PCM2WAV(char* strPCMFile, char* strWAVFile)
{
	FILE* pPcmFile = fopen(strPCMFile, "rb");
	if (!pPcmFile)
	{
		printf("fopen pcm file is failed. \n");
		return 0;
	}
	fseek(pPcmFile, 0, SEEK_END);
	int iDataSize = ftell(pPcmFile);
	byte* pData = new byte[iDataSize];
	fseek(pPcmFile, 0, SEEK_SET);
	if (iDataSize != fread(pData, sizeof(byte), iDataSize, pPcmFile))
	{
		printf("read pcm file is failed. \n");
		return false;
	}
	fclose(pPcmFile);


	FILE* pWAVFile = fopen(strWAVFile, "wb");
	if (!pWAVFile)
	{
		printf("fopen wav file is failed. \n");
		return 0;
	}

	WAVE_HEADER waveHeader;
	memcpy(waveHeader.szRiffID, "RIFF", 4);
	waveHeader.dwRiffSize = iDataSize + 36;
	memcpy(waveHeader.szRiffFormat, "WAVE", 4);
	memcpy(waveHeader.szFmtID, "fmt ", 4);
	waveHeader.dwFmtSize = 16;
	int iChannels = g_audioInfo.iChannels;
	int iBitsPerSample = g_audioInfo.iBitsPerSample;
	waveHeader.wavFormat.wFormatTag = WAVE_FORMAT_PCM;						// 声音格式为PCM;
	waveHeader.wavFormat.wChannels = g_audioInfo.iChannels;					// 采样声道数，2声道;  
	waveHeader.wavFormat.dwSamplesPerSec = g_audioInfo.iSamplesPerSec;		// 采样率;
	waveHeader.wavFormat.dwAvgBytesPerSec = g_audioInfo.iAvgBytesPerSec;	// 每秒的数据率，就是每秒能采集多少字节的数据;  
	waveHeader.wavFormat.wBlockAlign = (iChannels * iBitsPerSample) / 8;	// 一个块的大小，采样bit的bits数乘以声道数/8;
	waveHeader.wavFormat.wBitsPerSample = g_audioInfo.iBitsPerSample;		// 采样位数，16bits/次;
	memcpy(waveHeader.szDataID, "data", 4);
	waveHeader.dwDataSize = iDataSize;
	fwrite(&waveHeader, 1, sizeof(waveHeader), pWAVFile);
	fwrite(pData, 1, iDataSize, pWAVFile);
	fclose(pWAVFile);

	delete []pData;

	return 1;
}

int main()
{
	WAV_Encoder("../testfile/pcm_out.pcm");

	//PCM2WAV("../testfile/pcm_out.pcm", "../testfile/wav_out.wav");

	WAV_Decoder("../testfile/pcm_out.pcm");

	return 1;
}

int Main_OnGetAudioBuff(const char* pBuff, const int& iBuffSize)
{
	int iWriteSize = fwrite(pBuff, 1, iBuffSize, g_pInFile);

	return iWriteSize;
}

int Main_OnSetAudioBuff(char* pBuff, int iBuffSize)
{
	int iReadSize = fread(pBuff, 1, iBuffSize, g_pOutFile);

	return iReadSize;
}