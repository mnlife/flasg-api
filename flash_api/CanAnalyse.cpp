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
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	pList->InsertColumn(0, _T("Number"), LVCFMT_LEFT, 150);
	pList->InsertColumn(1, _T("Number"), LVCFMT_LEFT, 200);
	pList->InsertColumn(2, _T("Number"), LVCFMT_LEFT, 150);
	pList->InsertColumn(3, _T("Number"), LVCFMT_LEFT, 80);
	pList->InsertColumn(4, _T("Number"), LVCFMT_LEFT, 400);
	pList->SetBkColor(RGB(100, 200, 300));
	pList->SetTextBkColor(RGB(206, 226, 52));

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
END_MESSAGE_MAP()


// CanAnalyse 消息处理程序


void CanAnalyse::OnBnClickedStartCanDevice()
{
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


void CanAnalyse::OnBnClickedStartReceived()
{
	int device_type = 4;	// CANalyst-II
	int device_ind = 0;		// first device
	int can_ind = 0;		// CAN channel 0
	VCI_INIT_CONFIG	vic;
	DWORD	msg_num;
	VCI_CAN_OBJ	rx_msg[100000];
	vic.AccCode = 0x00000000;
	vic.AccMask = 0xFFFFFFFF;
	vic.Filter = 0;
	vic.Mode = 0;
	vic.Timing0 = 0x03;
	vic.Timing1 = 0x1C;
	vic.Reserved = 0;
	DWORD count;
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	msg_num = VCI_GetReceiveNum(device_type, device_ind, can_ind);

	VCI_Receive(device_type, device_ind, can_ind, rx_msg, msg_num, 500);
	for (DWORD i = 0; i < msg_num; ++i) {
		CString	a, b, c, d, e, f;
		a.Format(_T("0x%08x"), (int)(rx_msg[i].ID));
		
		b.Format(_T("%02x"), (int)(rx_msg[i].DataLen));
	

		for (int j = 0; i < rx_msg[i].DataLen; ++i) {

			d.Format(_T("%02x  "), rx_msg[i].Data[i]);
			c += d;
		}
		count = pList->GetItemCount();
		e.Format(_T("%d"), count);
		f.Format(_T("d"), rx_msg[i].TimeStamp);
		pList->InsertItem(0, e);
		pList->SetItemText(0, 1, f);
		pList->SetItemText(0, 2, a);
		pList->SetItemText(0, 3, b);
		pList->SetItemText(0, 4, c);

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
		MessageBox(_T("关闭设备失败！"), _T("警告"), MB_OK | MB_ICONQUESTION);
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
