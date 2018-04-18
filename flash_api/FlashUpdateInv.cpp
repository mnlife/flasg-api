// FlashUpdateInv.cpp: 实现文件
//

#include "stdafx.h"
#include "flash_api.h"
#include "FlashUpdateInv.h"
#include "afxdialogex.h"
#include "flash_apiDlg.h"
#include "ControlCAN.h"
// FlashUpdateInv 对话框

IMPLEMENT_DYNAMIC(FlashUpdateInv, CDialogEx)

FlashUpdateInv::FlashUpdateInv(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FLASH_UPDATE_INV, pParent)
{

}

FlashUpdateInv::~FlashUpdateInv()
{
	/*Cflash_apiDlg	MainMenu;
	MainMenu.DoModal();
	Cflash_apiDlg* dlg;
	dlg = new Cflash_apiDlg;
	dlg->Create(IDD_FLASH_API_DIALOG); //非模态对话框ID号
	dlg->ShowWindow(SW_SHOW);*/
}

void FlashUpdateInv::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_INV_MODILE1, FlashUpdateProgressCtrl[0]);
	DDX_Control(pDX, IDC_INV_MODILE2, FlashUpdateProgressCtrl[1]);
	DDX_Control(pDX, IDC_INV_MODILE3, FlashUpdateProgressCtrl[2]);
	DDX_Control(pDX, IDC_INV_MODILE4, FlashUpdateProgressCtrl[3]);
	DDX_Control(pDX, IDC_INV_MODILE5, FlashUpdateProgressCtrl[4]);
	DDX_Control(pDX, IDC_INV_MODILE6, FlashUpdateProgressCtrl[5]);
	DDX_Control(pDX, IDC_INV_MODILE7, FlashUpdateProgressCtrl[6]);
	DDX_Control(pDX, IDC_INV_MODILE8, FlashUpdateProgressCtrl[7]);
	DDX_Control(pDX, IDC_INV_MODILE9, FlashUpdateProgressCtrl[8]);
	DDX_Control(pDX, IDC_INV_MODILE10, FlashUpdateProgressCtrl[9]);
	DDX_Control(pDX, IDC_INV_MODILE11, FlashUpdateProgressCtrl[10]);
	DDX_Control(pDX, IDC_INV_MODILE12, FlashUpdateProgressCtrl[11]);
	DDX_Control(pDX, IDC_INV_MODILE13, FlashUpdateProgressCtrl[12]);
	DDX_Control(pDX, IDC_INV_MODILE14, FlashUpdateProgressCtrl[13]);
	DDX_Control(pDX, IDC_INV_MODILE15, FlashUpdateProgressCtrl[14]);
	DDX_Control(pDX, IDC_INV_MODILE16, FlashUpdateProgressCtrl[15]);
}


BEGIN_MESSAGE_MAP(FlashUpdateInv, CDialogEx)
	ON_BN_CLICKED(ID_START_FLASH_UPDATE, &FlashUpdateInv::OnBnClickedStartFlashUpdate)
	ON_BN_CLICKED(ID_OPEN_CAN_DEVICE, &FlashUpdateInv::OnBnClickedOpenCanDevice)
	ON_BN_CLICKED(ID_CLOSE_CAN_DEVICE, &FlashUpdateInv::OnBnClickedCloseCanDevice)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// FlashUpdateInv 消息处理程序


BOOL FlashUpdateInv::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST2);
	//CSize	a;
	//a.SetSize(100, 200);
	//pList->ApproximateViewRect(a);
	pList->InsertColumn(0, _T("ModuleID"), LVCFMT_LEFT, 66);
	pList->InsertColumn(1, _T("Module FlashUpdate Message"), LVCFMT_LEFT, 666);
	pList->SetBkColor(RGB(137, 207, 240));
	pList->SetTextBkColor(RGB(135, 206, 235));

	CString	module_num_display, module_msg_display;
	
	for (int i = 0; i < 16; ++i) {

		module_num_display.Format(_T("%02d"), i+1);
		pList->InsertItem(0, module_num_display);
		module_msg_display.Format(_T("模块%02d：用CAN升级Ultra Fast!!!"), i+1);
		pList->SetItemText(0, 1, module_msg_display);

	}

	transfer_data.data_count = 0;
	transfer_data.data_head = 0;
	transfer_data.data_trail = 0;
	transfer_data.next_data_head = 0;
	SetDlgItemText(IDC_DSP_SELECT, _T("320F2808"));
	return TRUE;
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
void FlashUpdateInv::OnBnClickedOpenCanDevice()
{
#define	TIMING0_5Kbps	0xBF
#define	TIMING1_5Kbps	0xFF

#define	TIMING0_10Kbps	0x31
#define	TIMING1_10Kbps	0x1C

#define	TIMING0_20Kbps	0x18
#define	TIMING1_20Kbps	0x1C

#define	TIMING0_40Kbps	0x87
#define	TIMING1_40Kbps	0xFF

#define	TIMING0_50Kbps	0x09
#define	TIMING1_50Kbps	0x1C

#define	TIMING0_80Kbps	0x83
#define	TIMING1_80Kbps	0xFF

#define	TIMING0_100Kbps	0x04
#define	TIMING1_100Kbps	0x1C

#define	TIMING0_125Kbps	0x03
#define	TIMING1_125Kbps	0x1C

#define	TIMING0_200Kbps	0x81
#define	TIMING1_200Kbps	0xFA

#define	TIMING0_250Kbps	0x01
#define	TIMING1_250Kbps	0x1C

#define	TIMING0_400Kbps	0x80
#define	TIMING1_400Kbps	0xFA

#define	TIMING0_500Kbps	0x00
#define	TIMING1_500Kbps	0x1C

#define	TIMING0_666Kbps	0x80
#define	TIMING1_666Kbps	0xB6

#define	TIMING0_800Kbps	0x00
#define	TIMING1_800Kbps	0x16

#define	TIMING0_1000Kbps	0x00
#define	TIMING1_1000Kbps	0x14

	int device_type = 4;	// CANalyst-II
	int device_ind = 0;		// first device
	int can_ind = 0;		// CAN channel 0
	VCI_INIT_CONFIG	vic;
	DWORD	test;
	vic.AccCode = 0x00000000;
	vic.AccMask = 0xFFFFFFFF;
	vic.Filter = 0;
	vic.Mode = 0;
	CString BuadRateSelect;
	GetDlgItemText(IDC_BAUD_RATE_SELECT, BuadRateSelect);
	if (BuadRateSelect == _T("5kbps")) {
		vic.Timing0 = TIMING0_5Kbps;
		vic.Timing1 = TIMING1_5Kbps;
	}
	else if (BuadRateSelect == _T("10kbps")) {
		vic.Timing0 = TIMING0_10Kbps;
		vic.Timing1 = TIMING1_10Kbps;
	}
	else if (BuadRateSelect == _T("20kbps")) {
		vic.Timing0 = TIMING0_20Kbps;
		vic.Timing1 = TIMING1_20Kbps;
	}
	else if (BuadRateSelect == _T("40kbps")) {
		vic.Timing0 = TIMING0_40Kbps;
		vic.Timing1 = TIMING1_40Kbps;
	}
	else if (BuadRateSelect == _T("50kbps")) {
		vic.Timing0 = TIMING0_50Kbps;
		vic.Timing1 = TIMING1_50Kbps;
	}
	else if (BuadRateSelect == _T("80kbps")) {
		vic.Timing0 = TIMING0_80Kbps;
		vic.Timing1 = TIMING1_80Kbps;
	}
	else if (BuadRateSelect == _T("100kbps")) {
		vic.Timing0 = TIMING0_100Kbps;
		vic.Timing1 = TIMING1_100Kbps;
	}
	else if (BuadRateSelect == _T("125kbps")) {
		vic.Timing0 = TIMING0_125Kbps;
		vic.Timing1 = TIMING1_125Kbps;
	}
	else if (BuadRateSelect == _T("200kbps")) {
		vic.Timing0 = TIMING0_200Kbps;
		vic.Timing1 = TIMING1_200Kbps;
	}
	else if (BuadRateSelect == _T("250kbps")) {
		vic.Timing0 = TIMING0_250Kbps;
		vic.Timing1 = TIMING1_250Kbps;
	}
	else if (BuadRateSelect == _T("400kbps")) {
		vic.Timing0 = TIMING0_400Kbps;
		vic.Timing1 = TIMING1_400Kbps;
	}
	else if (BuadRateSelect == _T("500kbps")) {
		vic.Timing0 = TIMING0_500Kbps;
		vic.Timing1 = TIMING1_500Kbps;
	}
	else if (BuadRateSelect == _T("666kbps")) {
		vic.Timing0 = TIMING0_666Kbps;
		vic.Timing1 = TIMING1_666Kbps;
	}
	else if (BuadRateSelect == _T("800kbps")) {
		vic.Timing0 = TIMING0_800Kbps;
		vic.Timing1 = TIMING1_800Kbps;
	}
	else if (BuadRateSelect == _T("1000kbps")) {
		vic.Timing0 = TIMING0_1000Kbps;
		vic.Timing1 = TIMING1_1000Kbps;
	}
	else {
		MessageBox(_T("请选择波特率！"), _T("警告！！！"), MB_OK | MB_ICONQUESTION);
		return;
	}
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

void FlashUpdateInv::OnBnClickedStartFlashUpdate()
{
	//WinExec("hex2000.exe --memwidth=16 --romwidth=16 --intel -o G:/CCSV7workspace/28377_UPS/28377D_INV/FLASH_RUN/28377D_INV.hex  G:/CCSV7workspace/28377_UPS/28377D_INV/FLASH_RUN/28377D_INV.out", SW_NORMAL);
	//system("hex2000.exe --memwidth=16 --romwidth=16 --intel -o G:/CCSV7workspace/28377_UPS/28377D_INV/FLASH_RUN/28377D_INV.hex  G:/CCSV7workspace/28377_UPS/28377D_INV/FLASH_RUN/28377D_INV.out");
	//system("del 28377D_INV.hex");

	CFile	file;
	CFile	wfile;
	CFileException ex;
	CString filepath;

	GetDlgItemText(IDC_MFCEDITBROWSE1, filepath);
	if (!file.Open(filepath, CFile::modeRead | CFile::shareDenyWrite, &ex)) {

		MessageBox(_T("请检查待升级文件！"), _T("警告"), MB_OK | MB_ICONQUESTION);
		return;
	}
	DWORD file_length = (DWORD)file.GetLength();
	char *p = new char[file_length];
	file.Read(p, file_length);
	file.Close();
	while (transfer_data.next_data_head < file_length) {
		for (DWORD i = transfer_data.next_data_head; i < file_length; ++i) {

			if (p[i] == ':') {
				transfer_data.data_head = i + 1;
			}
			if (p[i] == '\r') {
				transfer_data.data_trail = i - 1;
				transfer_data.next_data_head = i + 2;
				transfer_data.data_count = transfer_data.data_trail - transfer_data.data_head + 1;
				break;
			}

		}
	}







	i += 10;
	if (i > 100)
	{
		i = 100;
		return;
	}
	for (int j = 0; j < 16; ++j) {
		FlashUpdateProgressCtrl[j].SetPos(i);
	}




	delete[]p;
}

int FlashUpdateInv::InvFlashUpdate()
{
	// TODO: 在此处添加实现代码.
	return 0;
}


void FlashUpdateInv::OnBnClickedCloseCanDevice()
{
	int device_type = 4;	// CANalyst-II
	int device_ind = 0;		// first device
	int can_ind = 0;		// CAN channel 0
	DWORD	test;
	test = VCI_CloseDevice(device_type, device_ind);
	if (test == STATUS_ERR) {
		MessageBox(_T("Close Device Failed！"), _T("Caution!!!"), MB_OK | MB_ICONQUESTION);
	}
	else {
		SetWindowText(_T("Close Device Success!"));
	}

}




int FlashUpdateInv::FlashUpdateProgressSetPos(){

	for (int i = 0; i < 16; ++i) {

		FlashUpdateProgress[i] = (FlashUpdateProgress[i] > 100) ? 100 : FlashUpdateProgress[i];
		FlashUpdateProgress[i] = (FlashUpdateProgress[i] < 0) ? 0 : FlashUpdateProgress[i];

		FlashUpdateProgressCtrl[i].SetPos(FlashUpdateProgress[i]);
	}
	return 0;
}
