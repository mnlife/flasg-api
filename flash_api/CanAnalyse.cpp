// CanAnalyse.cpp: 实现文件
//

#include "stdafx.h"
#include "flash_api.h"
#include "CanAnalyse.h"
#include "afxdialogex.h"
#include "ControlCAN.h"
#include "flash_apiDlg.h"
// CanAnalyse 对话框

IMPLEMENT_DYNAMIC(CanAnalyse, CDialogEx)

CanAnalyse::CanAnalyse(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAN_ANALYSE, pParent)
{

}

CanAnalyse::~CanAnalyse()
{
	Cflash_apiDlg Reopen_Main;
	Reopen_Main.DoModal();
}

BOOL CanAnalyse::OnInitDialog() {
	mnlife = 1;
	SetDlgItemText(IDC_ID_FILTER, _T("0x1FFFFFFF"));


	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	pList->InsertColumn(0, _T("Number of Message"), LVCFMT_LEFT, 80);
	pList->InsertColumn(1, _T("Time"), LVCFMT_LEFT, 150);
	pList->InsertColumn(2, _T("ID"), LVCFMT_LEFT, 150);
	pList->InsertColumn(3, _T("DLC"), LVCFMT_LEFT, 80);
	pList->InsertColumn(4, _T("DATA"), LVCFMT_LEFT, 350);
	pList->SetBkColor(RGB(137, 207, 240));
	pList->SetTextBkColor(RGB(135, 206, 235));

	return TRUE;
}
void CanAnalyse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CanAnalyse, CDialogEx)
	ON_BN_CLICKED(IDC_START_CAN_DEVICE, &CanAnalyse::OnBnClickedStartCanDevice)
	ON_BN_CLICKED(IDC_START_RECEIVED, &CanAnalyse::OnBnClickedStartReceived)
	ON_BN_CLICKED(IDC_CLOSE_CAN_DEVICE, &CanAnalyse::OnBnClickedCloseCanDevice)
	ON_BN_CLICKED(IDC_CLEAR_RECEIVED, &CanAnalyse::OnBnClickedClearReceived)
	ON_BN_CLICKED(IDC_STOP_RECEIVED, &CanAnalyse::OnBnClickedStopReceived)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CanAnalyse 消息处理程序


void CanAnalyse::OnBnClickedStartCanDevice()
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
	GetDlgItemText(IDC_BAUD_RATE, BuadRateSelect);
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


void CanAnalyse::OnBnClickedStartReceived()
{
#define	max_msg_number	20000
	int device_type = 4;	// CANalyst-II
	int device_ind = 0;		// first device
	int can_ind = 0;		// CAN channel 0
	VCI_INIT_CONFIG	vic;
	DWORD	msg_num;
	VCI_CAN_OBJ	rx_msg[max_msg_number];
	vic.AccCode = 0x00000000;
	vic.AccMask = 0xFFFFFFFF;
	vic.Filter = 0;
	vic.Mode = 0;
	vic.Timing0 = 0x03;
	vic.Timing1 = 0x1C;
	vic.Reserved = 0;
	DWORD count;
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	SYSTEMTIME time;
	CString	get_id_from_dlg, msg_id;
	CString	a, b, c, d, e, f;
	//while (mnlife) 
	{
		//Sleep(2000);
		msg_num = VCI_GetReceiveNum(device_type, device_ind, can_ind);
		msg_num = (msg_num > max_msg_number) ? max_msg_number : msg_num;
		VCI_Receive(device_type, device_ind, can_ind, rx_msg, msg_num, 500);
		for (DWORD i = 0; i < msg_num; ++i) {
			
			msg_id.Format(_T("0X%x"), rx_msg[i].ID);
			GetDlgItemText(IDC_ID_FILTER, get_id_from_dlg);

			if ((get_id_from_dlg != msg_id)&(get_id_from_dlg != (_T("0X1FFFFFFF"))))	continue;
			
			a.Format(_T("0x%08x"), (int)(rx_msg[i].ID));

			b.Format(_T("%02x"), (int)(rx_msg[i].DataLen));
			c = _T("");
			for (int j = 0; j < rx_msg[i].DataLen; ++j) {

				d.Format(_T("%02x  "), rx_msg[i].Data[j]);
				c += d;
			}
			count = pList->GetItemCount();
			e.Format(_T("%d"), count);
			GetSystemTime(&time);
			f.Format(_T("%dh %dmin %ds %dms"), time.wHour + 8, time.wMinute, time.wSecond, time.wMilliseconds);
			pList->InsertItem(0, e);
			pList->SetItemText(0, 1, f);
			pList->SetItemText(0, 2, a);
			pList->SetItemText(0, 3, b);
			pList->SetItemText(0, 4, c);

		}

	}
}


void CanAnalyse::OnBnClickedCloseCanDevice()
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


void CanAnalyse::OnBnClickedClearReceived()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	pList->DeleteAllItems();
}


void CanAnalyse::OnBnClickedStopReceived()
{
	mnlife = 0;
}

void CanAnalyse::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	//AnimateWindow(1000, AW_CENTER);
}
