
// FLV_UI_ParseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FLV_UI_Parse.h"
#include "FLV_UI_ParseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据;
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CFLV_UI_ParseDlg 对话框
CFLV_UI_ParseDlg::CFLV_UI_ParseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFLV_UI_ParseDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pFlvFile = NULL;
}

CFLV_UI_ParseDlg::~CFLV_UI_ParseDlg()
{
	if (m_pFlvFile)
	{
		delete m_pFlvFile;
		m_pFlvFile = NULL;
	}
}

void CFLV_UI_ParseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PARSE, m_list);
	DDX_Control(pDX, IDC_EDIT_PATH, m_edtPath);
}

BEGIN_MESSAGE_MAP(CFLV_UI_ParseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBUTTON_OPENPATH, &CFLV_UI_ParseDlg::OnBnClickedMfcbuttonOpenpath)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CFLV_UI_ParseDlg::OnBnClickedButtonClear)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PARSE, OnCustomdrawList)
END_MESSAGE_MAP()


// CFLV_UI_ParseDlg 消息处理程序

BOOL CFLV_UI_ParseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// IDM_ABOUTBOX 必须在系统命令范围内;
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);		//获取当前窗口style;
	lStyle &= ~LVS_TYPEMASK;								// 清除显示方式位;
	lStyle |= LVS_REPORT;									// 设置style;
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);		// 设置style;

	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;						// 选中某行使整行高亮（只适用与report风格的listctrl）;
	dwStyle |= LVS_EX_GRIDLINES;							// 网格线（只适用与report风格的listctrl）;
	//dwStyle |= LVS_EX_CHECKBOXES;							// item前生成checkbox控件;
	m_list.SetExtendedStyle(dwStyle);						// 设置扩展风格;

	m_list.InsertColumn(COL_INDEX, _T("序号"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(COL_TYPE, _T("类型"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(COL_TIME, _T("时间戳"), LVCFMT_LEFT, 110);
	m_list.InsertColumn(COL_CODE, _T("编码"), LVCFMT_LEFT, 110);
	m_list.InsertColumn(COL_FRAME, _T("帧类型"), LVCFMT_LEFT, 110);
	m_list.InsertColumn(COL_SIZE, _T("帧大小(bytes)"), LVCFMT_LEFT, 110);

	return TRUE;
}

void CFLV_UI_ParseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFLV_UI_ParseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFLV_UI_ParseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFLV_UI_ParseDlg::OnBnClickedMfcbuttonOpenpath()
{
	BOOL bOpen = TRUE;
	CString strDefDir = _T(""); 
	CString strFileName = _T("");
	CString strFilter = _T("文件 (*.flv)|*.flv||");
	CFileDialog dlgOpenFile(bOpen, strDefDir, strFileName, OFN_HIDEREADONLY | OFN_READONLY, strFilter, NULL);
	INT_PTR result = dlgOpenFile.DoModal();
	CString strFilePath;
	if (IDOK != result)
	{
		return;
	}

	if (m_pFlvFile)
	{
		delete m_pFlvFile;
		m_pFlvFile = NULL;
	}

	strFilePath = dlgOpenFile.GetPathName();
	m_edtPath.SetWindowText(strFilePath);

	std::string strPathA = CT2CA(strFilePath);
	m_pFlvFile = new CFLVFile();
	bool bRes = m_pFlvFile->LoadFile(strPathA.c_str());
	assert(bRes);
	
	m_list.DeleteAllItems();
	VecFlvTag& vecFlags = m_pFlvFile->m_flvBody.m_vecFlvTags;
	int iCount = vecFlags.size();
	for (int i = 0; i < iCount; i++)
	{
		CString strTemp;
		strTemp.Format(_T("%d"), i+1);
		int iRow = m_list.InsertItem(m_list.GetItemCount(), strTemp);
	
		CFLVTag* pTag = vecFlags[i];
		if (!pTag)
		{
			continue;
		}

		CString strFrameType = _T(" - ");
		CString strFrameCode;
		flvtagheader_data & tagHeader = pTag->m_tagHeader.m_data;
		byte_ptr& pTagData = pTag->m_tagBody.m_pTag->m_pData;
		switch (tagHeader.TagType)
		{
		case TAG_TYPE_AUDIO:
			{
				strTemp = TAG_TYPE_AUDIO_NAME;

				// 解析音频格式;
				byte audioHeader = pTagData[0];
				{
					byte audioFormat = audioHeader >> 4;
					switch (audioFormat)
					{
					case 0:	 strFrameCode = _T("Linear PCM, platform endian"); break;
					case 1:	 strFrameCode = _T("ADPCM"); break;
					case 2:	 strFrameCode = _T("MP3"); break;
					case 3:	 strFrameCode = _T("Linear PCM, little endian"); break;
					case 4:	 strFrameCode = _T("Nellymoser 16-kHz mono"); break;
					case 5:  strFrameCode = _T("Nellymoser 8-kHz mono"); break;
					case 6:  strFrameCode = _T("Nellymoser"); break;
					case 7:  strFrameCode = _T("G.711 A-law logarithmic PCM"); break;
					case 8:  strFrameCode = _T("G.711 mu-law logarithmic PCM"); break;
					case 9:  strFrameCode = _T("reserved"); break;
					case 10: strFrameCode = _T("AAC"); break;
					case 11: strFrameCode = _T("Speex"); break;
					case 14: strFrameCode = _T("MP3 8-Khz"); break;
					case 15: strFrameCode = _T("Device-specific sound"); break;
					default: strFrameCode = _T("UNKNOWN"); break;
					}
				}
			}
			break;
		case TAG_TYPE_VIDEO:
			{
				strTemp = TAG_TYPE_VIDEO_NAME;

				// 解析视频格式;
				byte videoHeader = pTagData[0];
				{
					byte frameType = videoHeader >> 4;
					switch (frameType)
					{
					case 1:	strFrameType = _T("key frame"); break;
					case 2:	strFrameType = _T("inter frame"); break;
					case 3:	strFrameType = _T("disposable inter frame"); break;
					case 4:	strFrameType = _T("generated keyframe"); break;
					case 5:	strFrameType = _T("video info/command frame"); break;
					default:strFrameType = _T("UNKNOWN"); break;
					}
				}

				{
					byte sampBits = videoHeader << 4;
					sampBits = sampBits >> 4;
					switch (sampBits)
					{
					case 1:	strFrameCode = _T("JPEG (currently unused)"); break;
					case 2:	strFrameCode = _T("Sorenson H.263"); break;
					case 3:	strFrameCode = _T("Screen video"); break;
					case 4:	strFrameCode = _T("On2 VP6"); break;
					case 5:	strFrameCode = _T("On2 VP6 with alpha channel"); break;
					case 6:	strFrameCode = _T("Screen video version 2"); break;
					case 7:	strFrameCode = _T("H.264/AVC"); break;
					default:strFrameCode = _T("UNKNOWN"); break;
					}
				}
			}
			break;
		case TAG_TYPE_SCRIPT:
			{
				strTemp = TAG_TYPE_SCRIPT_NAME;
			}
			break;
		default:
			{
				strTemp = TAG_TYPE_UNKNOWN_NAME;
			}
			break;
		}
		m_list.SetItemText(iRow, COL_TYPE, strTemp);

		int iTimeStamp = BytesToInt(tagHeader.Timestamp, sizeof(tagHeader.Timestamp));
		int iMicSec = iTimeStamp % 1000;
		int iSec = (iTimeStamp / 1000) % 60;
		int iHour = ((iTimeStamp / 1000) / 60);
		strTemp.Format(_T("%02d:%02d:%03d"), iHour, iSec, iMicSec);
		m_list.SetItemText(iRow, COL_TIME, strTemp);

		m_list.SetItemText(iRow, COL_CODE, strFrameCode);
		m_list.SetItemText(iRow, COL_FRAME, strFrameType);

		strTemp.Format(_T("%d"), BytesToInt(tagHeader.DataSize, sizeof(tagHeader.DataSize)));
		m_list.SetItemText(iRow, COL_SIZE, strTemp);
	}
}


void CFLV_UI_ParseDlg::OnBnClickedButtonClear()
{
	m_list.DeleteAllItems();
	m_edtPath.SetWindowText(_T(""));

	if (m_pFlvFile)
	{
		delete m_pFlvFile;
		m_pFlvFile = NULL;
	}
}

void CFLV_UI_ParseDlg::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	//////////////////////////////////////////////////////
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	*pResult = CDRF_DODEFAULT;
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		COLORREF crText, crBk;

		CString strTagType;
		CString strFrameType;
		TCHAR szBuf[1024];
		LVITEM lvi;
		lvi.iItem = pLVCD->nmcd.dwItemSpec;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuf;
		lvi.cchTextMax = 1024;
		lvi.iSubItem = COL_TYPE;
		if (m_list.GetItem(&lvi))
		{
			strTagType = lvi.pszText;
		}
		lvi.iSubItem = COL_FRAME;
		if (m_list.GetItem(&lvi))
		{
			strFrameType = lvi.pszText;
		}

		if (0 == strTagType.CompareNoCase(TAG_TYPE_AUDIO_NAME))
		{
			crText = RGB(0, 0, 0);
			crBk = RGB(255, 255, 255);
		} 
		else if (0 == strTagType.CompareNoCase(TAG_TYPE_VIDEO_NAME))
		{
			crText = RGB(0, 0, 0);
			crBk = RGB(214, 219, 233);
			if (strFrameType.Find(_T("key")) > -1)
			{
				crText = RGB(255, 0, 0);
			}
		}
		else if (0 == strTagType.CompareNoCase(TAG_TYPE_SCRIPT_NAME))
		{
			crText = RGB(0, 0, 0);
			crBk = RGB(0, 255, 0);
		}
		else if (0 == strTagType.CompareNoCase(TAG_TYPE_UNKNOWN_NAME))
		{
			crText = RGB(0, 0, 0);
			crBk = RGB(255, 0, 0);
		}
		else
		{
		}

		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBk;

		if (m_list.GetItemState(pLVCD->nmcd.dwItemSpec, CDIS_SELECTED))
		{
			crBk = RGB(75, 149, 229);
			crText = RGB(255, 255, 255);
			pLVCD->clrText = crText;
			pLVCD->clrTextBk = crBk;
			*pResult = CDRF_NEWFONT;
		}

		if (LVIS_SELECTED == m_list.GetItemState(pLVCD->nmcd.dwItemSpec, LVIS_SELECTED))
		{
			//清除选择状态，如果不清除的话，还是会显示出蓝色的高亮条
			BOOL b = m_list.SetItemState(pLVCD->nmcd.dwItemSpec, 0, LVIS_SELECTED);
			pLVCD->clrText = crText;
			pLVCD->clrTextBk = crBk;

			*pResult = CDRF_NEWFONT;
			return;
		}
		*pResult = CDRF_NEWFONT;
	}
}
