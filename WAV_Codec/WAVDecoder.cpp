#include "WAVDecoder.h"

CWAVDecoder::CWAVDecoder()
{
	m_bStop = true;
	m_pCallBack = NULL;
}

CWAVDecoder::~CWAVDecoder()
{
}

void CWAVDecoder::SetAudioBack(OnSetAudioBuff pCallBack)
{
	m_pCallBack = pCallBack;
}

void CWAVDecoder::SetAudioInfo(const AudioInfo& info)
{
	m_audioInfo = info;
}

int CWAVDecoder::Start()
{
	memset(&m_waveform, 0, sizeof(WAVEFORMATEX));
	int iChannels = m_audioInfo.iChannels;
	int iBitsPerSample = m_audioInfo.iBitsPerSample;
	m_waveform.wFormatTag = WAVE_FORMAT_PCM;						// 声音格式为PCM;
	m_waveform.nSamplesPerSec = m_audioInfo.iSamplesPerSec;		// 采样率;
	m_waveform.wBitsPerSample = iBitsPerSample;					// 采样比特，16bits/次;
	m_waveform.nChannels = iChannels;								// 采样声道数，2声道;  
	m_waveform.nAvgBytesPerSec = m_audioInfo.iAvgBytesPerSec;		// 每秒的数据率，就是每秒能采集多少字节的数据;  
	m_waveform.nBlockAlign = (iChannels * iBitsPerSample) / 8;	// 一个块的大小，采样bit的bits数乘以声道数/8;
	m_waveform.cbSize = 0;										// 额外外信息大小, 一般为0;

	// 打开设备;
	MMRESULT iRes = waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &m_waveform, (DWORD)(WaveOutProc), (DWORD)this, CALLBACK_FUNCTION);
	if (iRes != 0)
	{
		printf("open waveIn err!\n");
		exit(0);
	}

	for (int i = 0; i < AUDIO_BUFF_MAXCOUNT; i++)			// 设置内存块格式;
	{
		m_arrHdr[i].lpData = m_BuffOut[i];
		m_arrHdr[i].dwBufferLength = AUDIO_BUFF_MAXSIZE;
		m_arrHdr[i].dwBytesRecorded = 0;
		m_arrHdr[i].dwUser = i;
		m_arrHdr[i].dwFlags = 0;
		waveOutPrepareHeader(m_hWaveOut, &m_arrHdr[i], sizeof(WAVEHDR));	// 准备内存块录音;
		waveOutWrite(m_hWaveOut, &m_arrHdr[i], sizeof(WAVEHDR));		// 增加内存块;
	}

	m_bStop = false;

	return 0;
}

int CWAVDecoder::Stop()
{
	// 此处不能释放, 去线程结束时释放;
	m_bStop = true;

	return 0;
}

DWORD CALLBACK CWAVDecoder::WaveOutProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	CWAVDecoder* pThis = reinterpret_cast<CWAVDecoder*>(dwInstance);
	assert(pThis);

	if (uMsg == WOM_DONE)
	{
		WAVEHDR*p = (WAVEHDR*)dwParam1;
		int i = p->dwUser;
		waveOutUnprepareHeader(pThis->m_hWaveOut, p, sizeof(WAVEHDR));

		int iDataSize = 0;
		if (pThis->m_pCallBack)
		{
			iDataSize = pThis->m_pCallBack(pThis->m_BuffOut[i], AUDIO_BUFF_MAXSIZE);
		}
		if (iDataSize < AUDIO_BUFF_MAXSIZE)
		{
			printf("play over!\n");
			pThis->m_bStop = true;
			return 0;
		}

		p->lpData = pThis->m_BuffOut[i];
		p->dwBufferLength = iDataSize;
		p->dwBytesRecorded = 0;
		p->dwUser = i;
		p->dwFlags = 0;

		waveOutPrepareHeader(pThis->m_hWaveOut, p, sizeof(WAVEHDR)); //准备内存块录音
		waveOutWrite(pThis->m_hWaveOut, p, sizeof(WAVEHDR));
	}
	if (uMsg == WOM_OPEN)
	{
		printf("wom open\n");
	}
	if (uMsg == WOM_CLOSE)
	{
		printf("wom close\n");
	}
	return 0;
}

int CWAVDecoder::Clear()
{
	waveOutRestart(m_hWaveOut);
	for (int i = 0; i < AUDIO_BUFF_MAXCOUNT; i++)
	{
		waveOutUnprepareHeader(m_hWaveOut, &m_arrHdr[i], sizeof(WAVEHDR));
	}
	printf("Start Close\n");
	waveOutClose(m_hWaveOut);

	return 1;
}