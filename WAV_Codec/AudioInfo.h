#pragma once
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")

#define AUDIO_AVGBYTESPERSEC		96000
#define AUDIO_SAMPLESPERSEC			44100
#define AUDIO_CHANNELS				2
#define AUDIO_BITSPERSAMPLE			16

#define  AUDIO_BUFF_MAXCOUNT		3			// 3个缓冲区;
#define  AUDIO_BUFF_MAXSIZE			176400		// 每个缓冲区的最大录音数据(最大值);

class AudioInfo
{
public:
	AudioInfo()
	{
		iAvgBytesPerSec = AUDIO_AVGBYTESPERSEC;
		iChannels = AUDIO_CHANNELS;
		iBitsPerSample = AUDIO_BITSPERSAMPLE;
		iSamplesPerSec = AUDIO_SAMPLESPERSEC;
	}

	int		iAvgBytesPerSec;	// 码率;
	int		iChannels;			// 通道数;
	int		iBitsPerSample;		// 采样位数;
	int		iSamplesPerSec;		// 采样频率;
};