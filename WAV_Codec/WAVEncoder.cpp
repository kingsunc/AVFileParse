#include "WAVEncoder.h"

CWAVEncoder::CWAVEncoder()
{
	m_bStop = true;
	m_pCallBack = NULL;
}

CWAVEncoder::~CWAVEncoder()
{
}

void CWAVEncoder::SetAudioBack(OnGetAudioBuff pCallBack)
{
	m_pCallBack = pCallBack;
}

void CWAVEncoder::SetAudioInfo(const AudioInfo& info)
{
	m_audioInfo = info;
}

int CWAVEncoder::Start()
{
	memset(&waveform, 0, sizeof(WAVEFORMATEX));
	int iChannels = m_audioInfo.iChannels;
	int iBitsPerSample = m_audioInfo.iBitsPerSample;
	waveform.wFormatTag = WAVE_FORMAT_PCM;						// 声音格式为PCM;
	waveform.nSamplesPerSec = m_audioInfo.iSamplesPerSec;		// 采样率;
	waveform.wBitsPerSample = iBitsPerSample;					// 采样比特，16bits/次;
	waveform.nChannels = iChannels;								// 采样声道数，2声道;  
	waveform.nAvgBytesPerSec = m_audioInfo.iAvgBytesPerSec;		// 每秒的数据率，就是每秒能采集多少字节的数据;  
	waveform.nBlockAlign = (iChannels * iBitsPerSample) / 8;	// 一个块的大小，采样bit的bits数乘以声道数/8;
	waveform.cbSize = 0;										// 额外外信息大小, 一般为0;

	// 打开设备;
	MMRESULT iRes = waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform, (DWORD)(WaveInProc), (DWORD)this, CALLBACK_FUNCTION);
	if (iRes != 0)
	{
		printf("open waveIn err!\n");
		exit(0);
	}
	unsigned int iWaveID = -1;
	waveInGetID(hWaveIn, &iWaveID);	// 你所使用的输入设备ID，-1为默认;
	printf("you use waveid: %d \n", iWaveID);

	for (int i = 0; i < AUDIO_BUFF_MAXCOUNT; i++)			// 设置内存块格式;
	{
		wHdr[i].lpData = chBuff[i];
		wHdr[i].dwBufferLength = AUDIO_BUFF_MAXSIZE;
		wHdr[i].dwBytesRecorded = 0;
		wHdr[i].dwUser = i;
		wHdr[i].dwFlags = 0;
		waveInPrepareHeader(hWaveIn, &wHdr[i], sizeof(WAVEHDR));	// 准备内存块录音;
		waveInAddBuffer(hWaveIn, &wHdr[i], sizeof(WAVEHDR));		// 增加内存块;
	}

	m_bStop = false;
	waveInStart(hWaveIn);	//开始录音;

	return 0;
}

int CWAVEncoder::Stop()
{
	// 此处不能释放, 去线程结束时释放;
	m_bStop = true;

	return 0;
}

DWORD CALLBACK CWAVEncoder::WaveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	CWAVEncoder* pThis = reinterpret_cast<CWAVEncoder*>(dwInstance);
	assert(pThis);

	if (uMsg == WIM_DATA)
	{
		WAVEHDR* p = (WAVEHDR*)dwParam1;	// dwParam1指向WAVEHDR的地址;
		printf("getdate id: %d size:%d timestemp:%d\n", p->dwUser, p->dwBytesRecorded, clock());
		int iIndex = p->dwUser;
		if (pThis->m_bStop) //0表示停止了
		{
			waveInUnprepareHeader(pThis->hWaveIn, p, sizeof(WAVEHDR)); // 释放;
			return 0;
		}
		else
		{
			if (pThis->m_pCallBack)
			{
				pThis->m_pCallBack(pThis->chBuff[iIndex], p->dwBytesRecorded);
			}
			waveInUnprepareHeader(pThis->hWaveIn, p, sizeof(WAVEHDR)); // 释放;
		}
		p->lpData = pThis->chBuff[iIndex];
		p->dwBufferLength = AUDIO_BUFF_MAXSIZE;
		p->dwBytesRecorded = 0;
		p->dwUser = iIndex;
		p->dwFlags = 0;
		waveInPrepareHeader(pThis->hWaveIn, p, sizeof(WAVEHDR));	// 准备内存块录音;
		waveInAddBuffer(pThis->hWaveIn, p, sizeof(WAVEHDR));		// 增加内存块;
	}
	if (uMsg == WIM_OPEN)
	{
		printf("open\n");
	}
	if (uMsg == WIM_CLOSE)
	{
		printf("close\n");
	}
	return 0;
}

int CWAVEncoder::Clear()
{
	printf("Start Stop\n");
	waveInStop(hWaveIn);
	printf("Start Reset\n");
	waveInReset(hWaveIn);
	printf("Start Close\n");
	waveInClose(hWaveIn);

	return 1;
}