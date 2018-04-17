
// flash_apiDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "flash_api.h"
#include "flash_apiDlg.h"
#include "afxdialogex.h"
#include "ControlCAN.h"
#include "CanAnalyse.h"
#include "FlashUpdateInv.h"
//#pragma comment(lib, "ControlCAN.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cflash_apiDlg 对话框



Cflash_apiDlg::Cflash_apiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FLASH_API_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cflash_apiDlg::DoDataExchange(CDataExchange* pDX)
{


	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, FlashUpdateProgress);
}

BEGIN_MESSAGE_MAP(Cflash_apiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Cflash_apiDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ADD, &Cflash_apiDlg::OnBnClickedAdd)
	ON_EN_CHANGE(IDC_LEFT, &Cflash_apiDlg::OnEnChangeLeft)
	ON_EN_CHANGE(IDC_RESULT, &Cflash_apiDlg::OnEnChangeResult)
	ON_BN_CLICKED(ID_START, &Cflash_apiDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_CLOSE, &Cflash_apiDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_ADD, &Cflash_apiDlg::OnBnClickedAdd)
	ON_BN_CLICKED(ID_OPENDEVICE, &Cflash_apiDlg::OnBnClickedOpendevice)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_START_CAN_ANALYSE, &Cflash_apiDlg::OnBnClickedStartCanAnalyse)
END_MESSAGE_MAP()


// Cflash_apiDlg 消息处理程序

BOOL Cflash_apiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cflash_apiDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cflash_apiDlg::OnPaint()
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
HCURSOR Cflash_apiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cflash_apiDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("let's go!"));
	//CDialogEx::OnOK();
}





void Cflash_apiDlg::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int left = GetDlgItemInt(IDC_LEFT);
	int right = GetDlgItemInt(IDC_RIGHT);
	SetDlgItemInt(IDC_RESULT,left+right);
	char hello[] = "come on!";
	SetDlgItemText(IDC_GOOGLE, _T("come on!"));
	//SetDlgItemText(IDC_RESULT, _T("hello"));/
	//char hello[] = "hello, world";
	//MessageBox(_T("let's go!"));
	//while (1);
}


void Cflash_apiDlg::OnEnChangeLeft()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void Cflash_apiDlg::OnEnChangeResult()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}




void Cflash_apiDlg::OnEnChangeGoogle()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

/****************************************************************************
pInitConfig->AccCode AccCode 对应SJA1000 中的四个寄存器ACR0，ACR1，ACR2，
ACR3，其中高字节对应ACR0，低字节对应ACR3；AccMask
对应SJA1000 中的四个寄存器AMR0，AMR1，AMR2，AMR3，
其中高字节对应AMR0，低字节对应AMR3。（请看表后说明）
pInitConfig->AccMask
pInitConfig->Reserve
d
保留
pInitConfig->Filter 滤波方式，1 表示单滤波，0 表示双滤波
pInitConfig->Timing0 波特率定时器0，详见VCI_INIT_CONFIG
pInitConfig->Timing1 波特率定时器1，详见VCI_INIT_CONFIG
pInitConfig->Mode 模式，0 表示正常模式，1 表示只听模式

CAN		波特率		定时器0		定时器1
5Kbps		0xBF		0xFF
10Kbps		0x31		0x1C
20Kbps		0x18		0x1C
40Kbps		0x87		0xFF
50Kbps		0x09		0x1C
80Kbps		0x83		0Xff
100Kbps		0x04		0x1C
125Kbps		0x03		0x1C
200Kbps		0x81		0xFA
250Kbps		0x01		0x1C
400Kbps		0x80		0xFA
500Kbps		0x00		0x1C
666Kbps		0x80		0xB6
800Kbps		0x00		0x16
1000Kbps	0x00		0x14
**************************************************************************/
void Cflash_apiDlg::OnBnClickedStart()
{
	OnOK();
	FlashUpdateInv	FlashUpdateInv;;
	FlashUpdateInv.DoModal();

}


void Cflash_apiDlg::OnBnClickedClose()
{
	FlashUpdateProgress.SetPos(75);
	// TODO: 在此添加控件通知处理程序代码
	int device_type = 4;	// CANalyst-II
	int device_ind = 0;		// first device
	int can_ind = 0;		// CAN channel 0
	DWORD	test;
	test = VCI_CloseDevice(device_type, device_ind);
	if (test == STATUS_ERR) {
		MessageBox(_T("关闭设备失败！"), _T("警告"), MB_OK | MB_ICONQUESTION);
	}
	else {
		SetWindowText(_T("Close Device Success!"));
	}
	SetDlgItemText(IDC_OUT_TXT, _T("go! go!! go!!!"));

}


void Cflash_apiDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	int left = GetDlgItemInt(IDC_LEFT);
	int right = GetDlgItemInt(IDC_RIGHT);
	SetDlgItemInt(IDC_RESULT, left + right);
	char hello[] = "come on!";
	SetDlgItemText(IDC_GOOGLE, _T("come on!"));
	//SetDlgItemText(IDC_RESULT, _T("hello"));/
	//char hello[] = "hello, world";
	//MessageBox(_T("let's go!"));
	//while (1);

	CString	b;
	b += "\nad离开时间发货不打了卡就卡就是不发链接哈埃里克森的减肥不打了卡时间分布拉斯贷款部分";
	b += "\nad离开时间发货不打了卡就是不服你就是不发链接哈埃里克森的减肥不打了卡时间分布拉斯贷款部分";
	SetDlgItemText(IDC_OUT_TXT, b);
}



void Cflash_apiDlg::OnBnClickedOpendevice()
{
	//Login login_in;
	//login_in.DoModal();
	FlashUpdateProgress.SetPos(25);
	int device_type = 4;	// CANalyst-II
	int device_ind = 0;		// first device
	int can_ind = 0;		// CAN channel 0
	VCI_INIT_CONFIG	vic;
	DWORD	test;
	vic.AccCode = 0x00000000;
	vic.AccMask = 0xFFFFFFFF;
	vic.Filter = 0;
	vic.Mode = 0;
	vic.Timing0 = 0x03;
	vic.Timing1 = 0x1C;
	vic.Reserved = 0;
	test = VCI_OpenDevice(device_type, device_ind, can_ind);

	if (test != STATUS_OK) {
		MessageBox(_T("Open Device Failed!!!"), _T("Caution"), MB_OK | MB_ICONQUESTION);
		return;
	}

	test = VCI_InitCAN(device_type, device_ind, can_ind, &vic);
	if (test == STATUS_ERR) {
		VCI_CloseDevice(device_type, device_ind);
		MessageBox(_T("Init Device Failed！"), _T("Caution"), MB_OK | MB_ICONQUESTION);
		return;
	}

	test = VCI_StartCAN(device_type, device_ind, can_ind);
	if (test == STATUS_ERR) {
		VCI_CloseDevice(device_type, device_ind);
		MessageBox(_T("Start Device Failed！"), _T("Caution"), MB_OK | MB_ICONQUESTION);
		return;
	}
	else {
		SetWindowText(_T("Open Device Success！"));
	}

}


void Cflash_apiDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	//AnimateWindow(1000, AW_CENTER);
}


void Cflash_apiDlg::OnBnClickedStartCanAnalyse()
{
	OnOK();
	CanAnalyse	OpenCanDevice;
	OpenCanDevice.DoModal();
}
