#pragma once
#include "AudioInfo.h"

// 获取AudioBuff的回调函数 原始PCM数据;
typedef int(*OnSetAudioBuff)(char* pBuff, int iBuffSize);

class CWAVDecoder
{
public:
	CWAVDecoder();
	~CWAVDecoder();

	void SetAudioBack(OnSetAudioBuff pCallBack);
	void SetAudioInfo(const AudioInfo& info);

	int Start();
	int Stop();
	int Clear();

	// 回调函数，需要时由外部传入数据;
	static DWORD CALLBACK WaveOutProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

private:
	bool			m_bStop;
	AudioInfo		m_audioInfo;

	HWAVEOUT		m_hWaveOut;			// 输出设备;  
	WAVEFORMATEX	m_waveform;			// 采集音频的格式，结构体;
	WAVEHDR			m_arrHdr[AUDIO_BUFF_MAXCOUNT];	// 采集音频时包含数据缓存的结构体;
	char			m_BuffOut[AUDIO_BUFF_MAXCOUNT][AUDIO_BUFF_MAXSIZE];
	OnSetAudioBuff	m_pCallBack;
};