#pragma once
#include <windows.h>
#include <stdio.h>
#include "AudioInfo.h"

// 获取AudioBuff的回调函数 原始PCM数据;
typedef int(*OnGetAudioBuff)(const char* pBuff, const int& iBuffSize);

class CWAVEncoder
{
public:
	CWAVEncoder();
	~CWAVEncoder();

	void SetAudioBack(OnGetAudioBuff pCallBack);
	void SetAudioInfo(const AudioInfo& info);

	int Start();
	int Stop();
	int Clear();

	// 回调函数，当缓冲区录满就会调用该函数;
	static DWORD CALLBACK WaveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

private:
	bool			m_bStop;
	AudioInfo		m_audioInfo;

	HWAVEIN			hWaveIn;			// 输入设备;  
	WAVEFORMATEX	waveform;			// 采集音频的格式，结构体;
	WAVEHDR			wHdr[AUDIO_BUFF_MAXCOUNT];	// 采集音频时包含数据缓存的结构体;
	char			chBuff[AUDIO_BUFF_MAXCOUNT][AUDIO_BUFF_MAXSIZE];
	OnGetAudioBuff	m_pCallBack;
};