#pragma once
#include "bytes.h"
#include <wtypes.h>

struct avih_data
{
	byte	strName[4];				// 必须为‘avih’
	DWORD	dwSize;					// 本数据结构的大小，不包括最初的8个字节（fcc和cb两个域）
	DWORD	dwIntervalTime;			// 视频帧间隔时间（以毫秒为单位）
	DWORD	dwMaxBytesPerSec;		// 这个AVI文件的最大数据率
	DWORD	dwPaddingGranularity;	// 数据填充的粒度
	DWORD	dwFlags;				// AVI文件的全局标记，比如是否含有索引块等
	DWORD	dwTotalFrames;			// 总帧数
	DWORD	dwInitialFrames;		// 为交互格式指定初始帧数（非交互格式应该指定为0）
	DWORD	dwStreams;				// 本文件包含的流的个数
	DWORD	dwSBuffSize;			// 建议读取本文件的缓存大小（应能容纳最大的块）
	DWORD	dwWidth;				// 视频图像的宽（以像素为单位）
	DWORD	dwHeight;				// 视频图像的高（以像素为单位）
	byte	dwReserved[16];			// 保留
};

// (视频流或文字流)在视频主窗口中的显示位置
// 视频主窗口由AVIMAINHEADER结构中的dwWidth和dwHeight决定
struct rectFrame
{
	short int left;
	short int top;
	short int right;
	short int bottom;
};

struct strh_data
{
	byte	strName[4];		// 必须为‘strh’
	DWORD	dwSize;			// 本数据结构的大小，不包括最初的8个字节（fcc和cb两个域）
	byte	strType[4];		// 流的类型：‘auds’（音频流）、‘vids’（视频流）、‘mids’（MIDI流）、‘txts’（文字流）
	byte	strHandler[4];	// 指定流的处理者，对于音视频来说就是解码器
	DWORD	dwFlags;		// 标记：是否允许这个流输出？调色板是否变化？
	WORD	wPriority;		// 流的优先级（当有多个相同类型的流时优先级最高的为默认流）
	WORD	wLanguage;
	DWORD	dwIniFrames;	// 为交互格式指定初始帧数
	DWORD	dwScale;		// 这个流使用的时间尺度
	DWORD	dwRate;
	DWORD	dwStart;		// 流的开始时间
	DWORD	dwLength;		// 流的长度（单位与dwScale和dwRate的定义有关）
	DWORD	dwSBuffSize;	// 读取这个流数据建议使用的缓存大小
	DWORD	dwQuality;		// 流数据的质量指标（0~10,000）
	DWORD	dwSampleSize;	// Sample的大小
	
	rectFrame rcFrame;
};