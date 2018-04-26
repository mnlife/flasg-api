// FlashUpdateMain.cpp: 实现文件
//

#include "stdafx.h"
#include "flash_api.h"
#include "FlashUpdateMain.h"
#include "afxdialogex.h"
#include "flash_apiDlg.h"
#include "ControlCAN.h"
#include "CAN_FLASHupdateMsgHandle.h"
#include <afxdb.h>
// FlashUpdateMain 对话框

CString	STATE_MACHINE_MESSAGE_GET_WHAT[]{

{ _T("初始无效状态") },
{ _T("选择节点升级") },
{ _T("对指定节点开始升级过程") },
{ _T("等待握手信号状态") },
{ _T("等待芯片解密应答信号") },
{ _T("等待API版本确认信息") },
{ _T("API 版本确认ok") },
{ _T("FLASH擦除中") },
{ _T("擦除结束") },
{ _T("等待编程允许") },
{ _T("编程允许") },
{ _T("传输BLOCK头") },
{ _T("传输BLOCK头结束") },
{ _T("传输下一个BLOCK") },
{ _T("等待DSP允许传输BLOCK数据") },
{ _T("BLOCK数据传输结束") },
{ _T("等待BLOCK校验和应答") },
{ _T("BLOCK校验OK") },
{ _T("编程状态等待") },
{ _T("BLOCK编程完成") },
{ _T("文件传输完成") },
{ _T("FLASH 校验中") },
{ _T("FLASH 校验完毕") },
{ _T("FLASH UPDATE 失败") }

};

IMPLEMENT_DYNAMIC(FlashUpdateMain, CDialogEx)


//CAN_FlashupdateMsgHandle * uniflash;
FlashUpdateMain::FlashUpdateMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FLASH_UPDATE_INV, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	time_out_cnt = 0;
	BlockCount = 0;
	for (UINT16 i = 0; i < 500; ++i) {
		EveryBlockDataNum[i] = 0;
		BlockCheckSum[i] = 0;
		BlockAddress[i] = 0;
		for (UINT16 j = 0; j < 1024; ++j) {

			BlockData[i][j] = 0;
		}

	}
	
	uniflash = new CAN_FlashupdateMsgHandle;

	msg_init.DataLen = 0;
	msg_init.ExternFlag = 0;
	msg_init.ID = 0;
	msg_init.RemoteFlag = 0;
	msg_init.Reserved[0] = 0;
	msg_init.Reserved[1] = 0;
	msg_init.Reserved[2] = 0;
	msg_init.SendType = 0;
	msg_init.TimeFlag = 0;
	msg_init.Data[0] = 0;
	msg_init.Data[1] = 0;
	msg_init.Data[2] = 0;
	msg_init.Data[3] = 0;
	msg_init.Data[4] = 0;
	msg_init.Data[5] = 0;
	msg_init.Data[6] = 0;
	msg_init.Data[7] = 0;

}

FlashUpdateMain::~FlashUpdateMain()
{
	delete uniflash;
	/*Cflash_apiDlg	MainMenu;
	MainMenu.DoModal();
	Cflash_apiDlg* dlg;
	dlg = new Cflash_apiDlg;
	dlg->Create(IDD_FLASH_API_DIALOG); //非模态对话框ID号
	dlg->ShowWindow(SW_SHOW);*/
}

void FlashUpdateMain::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(FlashUpdateMain, CDialogEx)
	ON_BN_CLICKED(ID_START_FLASH_UPDATE, &FlashUpdateMain::OnBnClickedStartFlashUpdate)
	ON_BN_CLICKED(ID_CLOSE_CAN_DEVICE, &FlashUpdateMain::OnBnClickedCloseCanDevice)
	ON_WM_DROPFILES()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RECORD_SAVED, &FlashUpdateMain::OnBnClickedRecordSaved)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CLEAR_MSG, &FlashUpdateMain::OnBnClickedClearMsg)
	ON_WM_CANCELMODE()
	ON_WM_CANCELMODE()
END_MESSAGE_MAP()


// FlashUpdateMain 消息处理程序


BOOL FlashUpdateMain::OnInitDialog()
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

	CComboBox *pCmob = (CComboBox*)GetDlgItem(IDC_DEVICE_SELECT);
	pCmob->SetCurSel(0);
	pCmob = (CComboBox*)GetDlgItem(IDC_BAUD_RATE_SELECT);
	pCmob->SetCurSel(7);
	pCmob = (CComboBox*)GetDlgItem(IDC_FLASH_UPDATE_TYPE);
	pCmob->SetCurSel(1);
	pCmob = (CComboBox*)GetDlgItem(IDC_DSP_SELECT);
	pCmob->SetCurSel(2);
	pCmob = (CComboBox*)GetDlgItem(IDC_SOFTWARE_SELECT);
	pCmob->SetCurSel(0);
	pCmob = (CComboBox*)GetDlgItem(IDC_module_select);
	pCmob->SetCurSel(0);
	/*
	ShowWindow(SW_MAXIMIZE);
	CWnd *pButtom1 = NULL;
	pButtom1 = GetDlgItem(IDD_FLASH_UPDATE_MAIN);
	CRect rs;
	//GetClientRect(rs);
	GetClientRect(&rs);
	//if(pButtom1)pButtom1-> MoveWindow(0, 0,50,50); 
	if (pButtom1)pButtom1->MoveWindow((rs.right - rs.left) / 2, (rs.bottom - rs.top) / 2, 50, 50);*/

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
int FlashUpdateMain::OnBnClickedOpenCanDevice()
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


	VCI_INIT_CONFIG	vic;
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
		return FALSE;
	}
	vic.Reserved = 0;

	if (VCI_OpenDevice(device_type, device_ind, can_ind) != STATUS_OK) {
		MessageBox(_T("Open Device Failed!!!"), _T("Caution"), MB_OK | MB_ICONQUESTION);
		SetWindowText(_T("Open Device Failed！"));
		return FALSE;
	}

	if (VCI_InitCAN(device_type, device_ind, can_ind, &vic) == STATUS_ERR) {
		VCI_CloseDevice(device_type, device_ind);
		MessageBox(_T("Init Device Failed！"), _T("Caution"), MB_OK | MB_ICONQUESTION);
		return FALSE;
	}

	if (VCI_StartCAN(device_type, device_ind, can_ind) == STATUS_ERR) {
		VCI_CloseDevice(device_type, device_ind);
		MessageBox(_T("Start Device Failed！"), _T("Caution"), MB_OK | MB_ICONQUESTION);
		return FALSE;
	}
	else {
		SetWindowText(_T("Open Device Success！"));
		return TRUE;
	}
}

void FlashUpdateMain::OnBnClickedStartFlashUpdate() {

	ParameterRefresh();
	TryCloseDevice();
	if (OnBnClickedOpenCanDevice() == FALSE) return;
	if (Hex_file_resolve() == FALSE) {

		//AfxMessageBox(_T("请检查待升级文件!!!"));
		return;
	}

	FlashUpdateGetNode();
	SendFlashUpdateOrder();
	uniflash->m_pHostModuleItc->u16FlashupdateStatus = STATUS_FLASH_START;
	if (uniflash->m_u16UpdaingNodeAdd == 0x3F) {

		SetTimer(2, 100, NULL);
	}
	else {

		SetTimer(2, 3, NULL);
	}
	SetTimer(3, 1000, NULL);
}
	/*
	UINT16 recore_type_04_line[10];
	for (UINT16 i = 0; i < 10; ++i) {

		recore_type_04_line[i] = 0;
	}
	UINT16 recore_type_04 = 0;
	for (DWORD i = 0; i < line_count; ++i) {

		if (every_line_recore_type[i] == 4) {

			recore_type_04_line[recore_type_04] = i;
			++recore_type_04;
		}
	}

	UINT32 now_address = 0;
	UINT32 backup = 0;
	UINT32	high32_address = 0;
	for (UINT16 i = 0; i < recore_type_04; ++i) {

		for (DWORD j = 0; j < line_count; ++j) {
			if (every_line_recore_type[j] == 4) {

				high32_address = ((((UINT32)every_line_data[recore_type_04_line[i++]][0]) << 8) & 0x00FF0000L);


			}
			else {

				every_line_address[now_address] = high32_address + every_line_address[j];
				++now_address;
			}
		}
	}
	*/

/*
void FlashUpdateMain::OnBnClickedStartFlashUpdate()
{

#define RECORD_TYPE_OFFSET	3
#define	DATA_OFFSET	4
#define	HIGH_16BIT_ADDRESS_RECORD_TYPE	0x04
#define	DATA_DEFINICATION	0x00
#define DATA_END_RECORD_TYPE	0x01
	if (IDNO == AfxMessageBox(_T("Are you sure Flashupdate?"), MB_YESNO))return;
	//WinExec("hex2000.exe --memwidth=16 --romwidth=16 --intel -o G:/CCSV7workspace/28377_UPS/28377D_INV/FLASH_RUN/28377D_INV.hex  G:/CCSV7workspace/28377_UPS/28377D_INV/FLASH_RUN/28377D_INV.out", SW_NORMAL);
	//system("hex2000.exe --memwidth=16 --romwidth=16 --intel -o G:/CCSV7workspace/28377_UPS/28377D_INV/FLASH_RUN/28377D_INV.hex  G:/CCSV7workspace/28377_UPS/28377D_INV/FLASH_RUN/28377D_INV.out");
	//system("del 28377D_INV.hex");

	CFile	file;
	CFile	wfile;
	CFileException ex;
	CString filepath;
	CString	file_read;
	UINT16	check_every_block_size = 0;
	char	check_low16_address = 0;

	GetDlgItemText(IDC_MFCEDITBROWSE1, filepath);
	if (!file.Open(filepath, CFile::modeRead | CFile::shareDenyWrite, &ex)) {

		MessageBox(_T("请检查待升级文件！"), _T("警告"), MB_OK | MB_ICONQUESTION);
		return;
	}
	DWORD file_length = (DWORD)file.GetLength();
	char *p = new char[file_length];
	char my_file[500000];
	UINT16 OddorEven = 0;
	filepath = "Cell50KInvLib.hex";
	file.Read(p, file_length);
	file.Close();

	DWORD	char_num = 0;
	for (UINT32 k = 0; k < file_length; ++k) {
		if ((p[k] == ':') || (p[k] == '\r') || (p[k] == '\n')) {
			my_file[char_num] = p[k];
			char_num++;
			if (p[k] == ':') {
				OddorEven = k % 2;
			}
		}
		else {
			if (OddorEven) {
				if ((p[k] == 'A') || (p[k] == 'B') || (p[k] == 'C') ||
					(p[k] == 'D') || (p[k] == 'E') || (p[k] == 'F'))
				{
					if (k % 2) {
						my_file[char_num] |= (p[k] - 55);
						char_num++;
					}
					else {
						my_file[char_num] = (p[k] - 55) << 4;
						
					}
				}
				else {
					if (k % 2) {
						my_file[char_num] |= (p[k] - 48);
						char_num++;
					}
					else {
						my_file[char_num] = (p[k] - 48) << 4;
						
					}
				}
			}
			else if(OddorEven==0){
				if ((p[k] == 'A') || (p[k] == 'B') || (p[k] == 'C') ||
					(p[k] == 'D') || (p[k] == 'E') || (p[k] == 'F'))
				{
					if (k % 2) {
						my_file[char_num] = (p[k] - 55) << 4;

					}
					else {
						my_file[char_num] |= (p[k] - 55);
						char_num++;
					}
				}
				else {
					if (k % 2) {
						my_file[char_num] = (p[k] - 48) << 4;

					}
					else {
						my_file[char_num] |= (p[k] - 48);
						char_num++;
					}
				}
			}
		}
	}
	
	while (transfer_data.next_data_head < char_num) {
		for (DWORD i = transfer_data.next_data_head; i < char_num; ++i) {

			if (my_file[i] == ':') {
				transfer_data.data_head = i + 1;
			}
			if ((my_file[i] == '\r')&&(my_file[i+1] == '\n')&&(my_file[i + 2] == ':')) {
				transfer_data.data_trail = i - 1;
				transfer_data.next_data_head = i + 2;
				transfer_data.data_count = transfer_data.data_trail - transfer_data.data_head + 1;
				break;
			}

		}
		// Record Type( 00, 04, 01)
		if (my_file[transfer_data.data_head + RECORD_TYPE_OFFSET] == HIGH_16BIT_ADDRESS_RECORD_TYPE) {

			BlockAddress[BlockCount][1] = (((UINT16)(my_file[transfer_data.data_head + 4])) << 8) |
												(((UINT16)(my_file[transfer_data.data_head + 5]))&0xFF);
			continue;
		}
		else if (my_file[transfer_data.data_head + RECORD_TYPE_OFFSET] == DATA_END_RECORD_TYPE) {

			break;

		}
		else {

			check_every_block_size = EveryBlockDataNum[BlockCount] + (((UINT16)(my_file[transfer_data.data_head])) & 0xFF);
			if (EveryBlockDataNum[0] == 0) {

				BlockAddress[BlockCount][0] = (((UINT16)(my_file[transfer_data.data_head + 1])) << 8) |
												(((UINT16)(my_file[transfer_data.data_head + 2])) & 0xFF);
			}
			if (check_every_block_size > 2048){
				++BlockCount;
				BlockAddress[BlockCount][0] = (((UINT16)(my_file[transfer_data.data_head + 1])) << 8) |
												(((UINT16)(my_file[transfer_data.data_head + 2])) & 0xFF);
			}
			else if (check_low16_address != my_file[transfer_data.data_head + 2]) {

				/*++BlockCount;
				BlockAddress[BlockCount][0] = (((UINT16)(my_file[transfer_data.data_head + 1])) << 8) |
												(((UINT16)(my_file[transfer_data.data_head + 2])) & 0xFF)
			}
			check_low16_address = (my_file[transfer_data.data_head] / 2) + my_file[transfer_data.data_head + 2];
			for (UINT32 j = transfer_data.data_head + DATA_OFFSET; j < transfer_data.data_trail; ++j) {

				if (j % 2) {

					BlockData[BlockCount][EveryBlockDataNum[BlockCount]] = (((UINT16)(my_file[j])) << 8);
					
				}
				else {

					BlockData[BlockCount][EveryBlockDataNum[BlockCount]] |= (((UINT16)(my_file[j]))&0x00FF);
					++EveryBlockDataNum[BlockCount];
				}
			}

		}

		
	}

	delete[]p;
}*/



void FlashUpdateMain::OnBnClickedCloseCanDevice()
{
	KillTimer(2);
	KillTimer(3);

	VCI_BOARD_INFO	vbi;
	if (!VCI_ReadBoardInfo(device_type, device_ind, &vbi)) {

		AfxMessageBox(_T("没有设备，关闭个毛，擦擦擦!!!"));
		return;
	}
	if (VCI_CloseDevice(device_type, device_ind) == STATUS_ERR) {
		MessageBox(_T("Close Device Failed！"), _T("Caution!!!"), MB_OK | MB_ICONQUESTION);
	}
	else {
		SetWindowText(_T("Close Device Success!"));
	}

}
void FlashUpdateMain::TryCloseDevice() {

	VCI_BOARD_INFO	vbi;
	if (!VCI_ReadBoardInfo(device_type, device_ind, &vbi)) {

		//AfxMessageBox(_T("没有设备，关闭个毛，擦擦擦!!!"));
		return;
	}
	if (VCI_CloseDevice(device_type, device_ind) == STATUS_ERR) {
		MessageBox(_T("Close Device Failed！"), _T("Caution!!!"), MB_OK | MB_ICONQUESTION);
	}
	else {
		SetWindowText(_T("Close Device Success!"));
	}
}


int FlashUpdateMain::FlashUpdateProgressSetPos(){
	
	CString module_num_display, module_msg_display;
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST2);
	for (int i = 0; i < 16; ++i) {

		FlashUpdateProgressCtrl[i].SetPos(int((uniflash->FlashUpdateProgress[i+uniflash->NodeOffset]*100)/(BlockCount-1)));
		module_num_display.Format(_T("%02d"), i + 1);
		if (uniflash->m_pHostModuleItcBak.u16FlashupdateStatus != uniflash->m_pHostModuleItc->u16FlashupdateStatus) {

			if (uniflash->FlashUpdateErrorMsg[i + uniflash->NodeOffset].ereor_cnt == 123) {
				
				pList->InsertItem(0, module_num_display);
				module_msg_display.Format(_T("模块%02d：")+
					STATE_MACHINE_MESSAGE_GET_WHAT[int(uniflash->FlashUpdateErrorMsg[i + uniflash->NodeOffset].error_state_saved)] +
					_T("超时!!!		!!!		!!!		!!!		!!!"),
					i + 1);
				pList->SetItemText(0, 1, module_msg_display);
			}
			else {

				module_num_display.Format(_T("%02d"), i + 1);
				pList->InsertItem(0, module_num_display);
				module_msg_display.Format(_T("模块%02d：") +
					
					STATE_MACHINE_MESSAGE_GET_WHAT[int(uniflash->m_pHostModuleItc->u16FlashupdateStatus)]
					, i + 1);
				pList->SetItemText(0, 1, module_msg_display);
			}
		}
		else {

			time_out_cnt++;
		}
		if ((time_out_cnt > (30 << 4)) && (uniflash->m_pHostModuleItc->u16FlashupdateStatus != STATUS_FILE_TRANS_END)) {

			pList->InsertItem(0, module_num_display);
			module_msg_display.Format(_T("模块%02d：确定EPO或者有插模块或者模块有程序吗吗?"), i+1);
			pList->SetItemText(0, 1, module_msg_display);
		}
		uniflash->FlashUpdateErrorMsgBak[i + uniflash->NodeOffset] = uniflash->FlashUpdateErrorMsg[i + uniflash->NodeOffset];
		
	}

	uniflash->m_pHostModuleItcBak.u16FlashupdateStatus = uniflash->m_pHostModuleItc->u16FlashupdateStatus;
	return 0;
}


int FlashUpdateMain::Hex_file_resolve()
{
	system("del target_file.hex");
	
	//WinExec("hex2000.exe --memwidth=16 --romwidth=16 --intel -o G:/CCSV7workspace/28377_UPS/28377D_INV/FLASH_RUN/28377D_INV.hex  G:/CCSV7workspace/28377_UPS/28377D_INV/FLASH_RUN/28377D_INV.out", SW_NORMAL);
	//system("hex2000.exe --memwidth=16 --romwidth=16 --intel -o G:/CCSV7workspace/28377_UPS/28377D_INV/FLASH_RUN/28377D_INV.hex  G:/CCSV7workspace/28377_UPS/28377D_INV/FLASH_RUN/28377D_INV.out");
	//system("del 28377D_INV.hex");
	//system("cmd");
	CFile	file;
	CFileException ex;
	CString outfilepath;
	CString target_file_path;
	target_file_path = _T("hex2000.exe --memwidth=16 --romwidth=16 --intel -o ./target_file.hex ");
	GetDlgItemText(IDC_MFCEDITBROWSE1, outfilepath);
	target_file_path += outfilepath;
	//char abcd[] = T2A(target_file_path.GetBuffer(target_file_path.GetLength()));
	USES_CONVERSION;
	//MessageBox(target_file_path, _T("警告"), MB_OK | MB_ICONQUESTION);
	//CHAR *abcd = T2A(target_file_path.GetBuffer(0));
	//T2A(m_strCMD.GetBuffer(m_strCMD.GetLength()))
	WinExec(T2A(target_file_path.GetBuffer(target_file_path.GetLength())), SW_HIDE);

	if (IDNO == AfxMessageBox(_T("Are you sure Flashupdate?"), MB_YESNO)) {
	
		system("del target_file.hex");
		return FALSE;
	}
	if (!file.Open(_T("./target_file.hex"), CFile::modeRead | CFile::shareDenyWrite, &ex)) {

		MessageBox(_T("请检查待升级文件！"), _T("警告"), MB_OK | MB_ICONQUESTION);
		return FALSE;
	}
	
	DWORD file_length = (DWORD)file.GetLength();
	if (file.GetLength() > 1000000L) return FALSE;

	char *p = new char[file_length];
	file.Read(p, file_length);
	file.Close();
	system("del target_file.hex");



	// Initialization
#define LINE_COUNT 5000
#define EVERY_LINE_CHAR_NUMBER	74

	char resolve_hex_file[LINE_COUNT][EVERY_LINE_CHAR_NUMBER];
	char char_to_number[LINE_COUNT][EVERY_LINE_CHAR_NUMBER / 2];
	for (DWORD i = 0; i < LINE_COUNT; ++i) {
		for (DWORD j = 0; j < EVERY_LINE_CHAR_NUMBER; ++j) {

			resolve_hex_file[i][j] = 0;
		}
		for (DWORD k = 0; k < EVERY_LINE_CHAR_NUMBER / 2; ++k) {

			char_to_number[i][k] = 0;
		}
	}
	char every_line_count[LINE_COUNT];
	for (DWORD i = 0; i < LINE_COUNT; ++i) {

		every_line_count[i] = 0;
	}





	DWORD	line_count = 0;

	// Resolve Hex File, // Except ':', '/r', '/n',
	// Place in resolve_hex_file[LINE_COUNT][EVERY_LINE_CHAR_NUMBER]
	for (DWORD i = 0; i < file_length; ++i) {

		if ((p[i] == ':') || (p[i] == '\r') || (p[i] == '\n')) {}
		else {
			
			resolve_hex_file[line_count][every_line_count[line_count]] = p[i];
			every_line_count[line_count]++;
			if (every_line_count[line_count] > EVERY_LINE_CHAR_NUMBER) return FALSE;
		}
		if (p[i] == '\n') {

			line_count++;
			if (every_line_count[line_count] > LINE_COUNT) return FALSE;
		}
	}





	// ASCII to char number
	for (DWORD i = 0; i < line_count; ++i) {

		for (char j = 0; j < every_line_count[i]; ++j) {

			if ((resolve_hex_file[i][j] == 'A') || (resolve_hex_file[i][j] == 'B') || (resolve_hex_file[i][j] == 'C')
				|| (resolve_hex_file[i][j] == 'D') || (resolve_hex_file[i][j] == 'E') || (resolve_hex_file[i][j] == 'F')) {

				if (j % 2) {

					char_to_number[i][j / 2] |= resolve_hex_file[i][j] - 55;
				}
				else {

					char_to_number[i][j / 2] = ((resolve_hex_file[i][j] - 55) << 4) & 0xF0;
				}
			}
			else {

				if (j % 2) {

					char_to_number[i][j / 2] |= resolve_hex_file[i][j] - 48;
				}
				else {

					char_to_number[i][j / 2] = ((resolve_hex_file[i][j] - 48) << 4) & 0xF0;
				}
			}
		}
	}






	// line count from ASCII to char number
	for (DWORD i = 0; i < line_count; ++i) {

		every_line_count[i] = every_line_count[i] / 2;
	}
	// char number to UINT16, address resolve to 32 bit
	UINT16	every_line_data[LINE_COUNT][16];
	UINT32	every_line_address[LINE_COUNT];
	UINT16	every_line_data_count[LINE_COUNT];
	UINT16	every_line_recore_type[LINE_COUNT];
	for (DWORD i = 0; i < LINE_COUNT; ++i) {

		for (int j = 0; j < 16; ++j) {

			every_line_data[i][j] = 0;
		}
		every_line_address[i] = 0x00000000;
		every_line_data_count[i] = 0;
		every_line_recore_type[i] = 0;
	}
	for (DWORD i = 0; i < line_count; ++i) {

		every_line_recore_type[i] = (((UINT16)char_to_number[i][3]) & 0x00FF);
		every_line_data_count[i] = char_to_number[i][0];
		every_line_address[i] = ((((UINT32)char_to_number[i][1]) << 8) & 0x0000FF00L) +
			(((UINT32)char_to_number[i][2]) & 0x000000FFL);

		for (char j = 4; j < every_line_count[i] - 1; ++j) {

			if (j % 2) {

				every_line_data[i][j / 2 - 2] |= (((UINT16)char_to_number[i][j]) & 0x00FF);
			}
			else {

				every_line_data[i][j / 2 - 2] = ((((UINT16)char_to_number[i][j]) << 8) & 0xFF00);
			}
		}
	}








	// line count from char to UINT16
	for (DWORD i = 0; i < line_count; ++i) {

		every_line_count[i] = (every_line_count[i] - 5) / 2;
	}
	UINT16	new_every_line_data[LINE_COUNT][16];
	UINT32	new_every_line_address[LINE_COUNT];
	UINT16	new_every_line_data_count[LINE_COUNT];
	UINT32	new_line_count = 0;
	UINT32	high32address = 0;
	for (DWORD i = 0; i < LINE_COUNT; ++i) {

		new_every_line_address[i] = 0;
		new_every_line_data_count[i] = 0;
		for (int j = 0; j < 16; ++j) {

			new_every_line_data[i][j] = 0;
		}
	}






	// remove recore type, adress from 16bit become 32 bit address, so line number decrease
	for (DWORD i = 0; i < line_count; i++) {
		if (every_line_recore_type[i] == 4) {

			high32address = (((UINT32)every_line_data[i][0]) << 16) & 0xFFFF0000;
		}
		else if (every_line_recore_type[i] == 1) {
			break;
		}
		else {

			new_every_line_address[new_line_count] = high32address + every_line_address[i];
			new_every_line_data_count[new_line_count] = every_line_data_count[i] / 2;
			for (UINT16 j = 0; j < 16; j++) {

				new_every_line_data[new_line_count][j] = every_line_data[i][j];
			}
			++new_line_count;
		}
	}




	// place message to block; waiting fir transmit, finally, resolved hex file successfully
	UINT16 BlockCountBak = 0;
	for (DWORD i = 0; i < new_line_count; ++i) {
		if (i != 0) {
			if (new_every_line_address[i] !=
				(new_every_line_address[i - 1] + new_every_line_data_count[i - 1]))
			{
				BlockCount++;

			}
		}
		if ((EveryBlockDataNum[BlockCount] + new_every_line_data_count[i]) > 1024) {

			BlockCount++;
		}
		if (BlockCount == BlockCountBak) {

			BlockAddress[BlockCount] = new_every_line_address[i];
			BlockCountBak++;
		}
		for (UINT16 j = 0; j < new_every_line_data_count[i]; ++j) {

			BlockData[BlockCount][EveryBlockDataNum[BlockCount]] = new_every_line_data[i][j];
			EveryBlockDataNum[BlockCount]++;
		}
	}
	BlockCount++;






	// calcluate check sum
	for (DWORD i = 0; i < BlockCount; ++i) {

		BlockCheckSum[i] += BlockAddress[i];
		BlockCheckSum[i] += EveryBlockDataNum[i];
		for (UINT16 j = 0; j < EveryBlockDataNum[i]; ++j) {

			BlockCheckSum[i] += BlockData[i][j];
		}
	}



	uniflash->BlockAmount = BlockCount;

	for (DWORD i = 0; i < 500; ++i) {

		uniflash->EveryBlockDataNum[i] = EveryBlockDataNum[i];
		uniflash->BlockCheckSum[i] = BlockCheckSum[i];
		uniflash->BlockAddress[i] = BlockAddress[i];
		for (DWORD j = 0; j < 1024; ++j) {

			uniflash->BlockData[i][j] = BlockData[i][j];
		}
	}

	delete[]p;
	return TRUE;
}


void FlashUpdateMain::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 2) {
		uniflash->FlashUpdateRoutine();
		//SetTimer(2, 10, NULL);
	}
	else if (nIDEvent == 3) {

		FlashUpdateProgressSetPos();
	}
	CDialogEx::OnTimer(nIDEvent);
}


bool FlashUpdateMain::SendFlashUpdateOrder()
{
	CAN_PACKED_PROTOCOL_U *tx_msg = new CAN_PACKED_PROTOCOL_U;
	tx_msg->Frame = msg_init;
	tx_msg->PackedMsg.RemoteFlag = 0;// 数据帧
	tx_msg->PackedMsg.ExternFlag = 0;// 标准帧

	tx_msg->PackedMsg.b6DestinationMacId = uniflash->m_u16UpdaingNodeAdd;
	tx_msg->PackedMsg.b7ServiceCode = service_code;
	tx_msg->PackedMsg.b10MsgClass = CAN_CTRL_MSG_CLASS;
	tx_msg->PackedMsg.b1Fragment = NONFRAG_MSG;
	tx_msg->PackedMsg.b1RsRq = RQ_MSG;
	tx_msg->PackedMsg.b6SourceMacId = MAC_ID_MON;
	tx_msg->PackedMsg.DataLen = 6;
	tx_msg->PackedMsg.MsgData[0] = tx_fu_data[0];
	tx_msg->PackedMsg.MsgData[1] = tx_fu_data[1];
	tx_msg->PackedMsg.MsgData[2] = tx_fu_data[2];
	tx_msg->PackedMsg.MsgData[3] = tx_fu_data[3];


	VCI_Transmit(device_type, device_ind, can_ind, &tx_msg->Frame, 1);
	int a = 0;
	delete tx_msg;
	return TRUE;
}


void FlashUpdateMain::OnBnClickedRecordSaved()
{
	

	if (AfxMessageBox(_T("Are you Sure Save Message?"), MB_YESNO) == IDNO)return;
	//// TODO: Add your control notification handler code here
	//首先判断列表框中是否有记录
	CListCtrl* m_list = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//// TODO: Add your control notification handler code here
	//首先判断列表框中是否有记录
	if (m_list->GetItemCount() <= 0)
	{
		AfxMessageBox(_T("没有升级记录需要保存！"));
		return;
	}
	//打开另存为对话框 ，需要包含 #include <Afxdlgs.h>
	CFileDialog dlg(FALSE,
		_T("xls"),
		_T("FileList"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Excel 文件(*.xls)|*.xls||"));
	dlg.m_ofn.lpstrTitle = _T("文件列表另存为");

	if (dlg.DoModal() != IDOK)return;
	CString strFilePath;
	//获得文件路径名
	strFilePath = dlg.GetPathName();
	//判断文件是否已经存在，存在则删除重建
	DWORD dwRe = GetFileAttributes(strFilePath);
	if (dwRe != (DWORD)-1)
	{
		DeleteFile(strFilePath);
	}
	CDatabase database;//数据库库需要包含头文件 #include <afxdb.h>
	CString sDriver = _T("MICROSOFT EXCEL DRIVER (*.XLS)"); // Excel驱动
	CString sSql, strInsert;
	TRY
	{
		// 创建进行存取的字符串
		sSql.Format(_T("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s"),sDriver, strFilePath, strFilePath);

	// 创建数据库 (既Excel表格文件)
	if (database.OpenEx(sSql,CDatabase::noOdbcDialog))
	{
		//获得列别框总列数
		int iColumnNum,iRowCount;
		LVCOLUMN lvCol;
		CString strColName; //用于保存列标题名称
		int i,j; //列、行循环参数

		iColumnNum = m_list->GetHeaderCtrl()->GetItemCount();
		iRowCount = m_list->GetItemCount();
		sSql = _T(" CREATE TABLE DSO_DX ( ");
		strInsert = _T(" INSERT INTO DSO_DX ( ");
		//获得列标题名称
		lvCol.mask = LVCF_TEXT; //必需设置，说明LVCOLUMN变量中pszText参数有效
		lvCol.cchTextMax = 32; //必设，pszText参数所指向的字符串的大小
		lvCol.pszText = strColName.GetBuffer(32); //必设，pszText 所指向的字符串的实际存储位置。
												  //以上三个参数设置后才能通过 GetColumn()函数获得列标题的名称
		for (i = 0; i< iColumnNum; i++)
		{
			if (!(m_list->GetColumn(i,&lvCol)))
				return;
			if (i<iColumnNum - 1)
			{
				sSql = sSql + lvCol.pszText + _T(" TEXT , ");
				strInsert = strInsert + lvCol.pszText + _T(" , ");
			}
			else
			{
				sSql = sSql + lvCol.pszText + _T(" TEXT ) ");
				strInsert = strInsert + lvCol.pszText + _T(" )  VALUES ( ");
			}
		}
		//创建Excel表格文件
		database.ExecuteSQL(sSql);
		//循环提取记录并插入到EXCEL中
		sSql = strInsert;
		CString chTemp;
		for (j = 0; j<iRowCount; j++)
		{

			for (i = 0; i<iColumnNum; i++)
			{
				chTemp = m_list->GetItemText(j, i);
				if (i < (iColumnNum - 1))
				{
					sSql = sSql + _T("'") + chTemp + _T("' , ");
				}
				else
				{
					sSql = sSql + _T("'") + chTemp + _T("' ) ");
				}
			}
			//将记录插入到表格中
			database.ExecuteSQL(sSql);
			sSql = strInsert;
		}
	}
	// 关闭Excel表格文件
	database.Close();
	AfxMessageBox(_T("保存查询结果为Excel文件成功！"));
	}
		CATCH_ALL(e)
	{
		//错误类型很多，根据需要进行报错。
		AfxMessageBox(_T("Excel文件保存失败。"));
	}
	END_CATCH_ALL;
}





int FlashUpdateMain::FlashUpdateGetNode()
{

	UINT16 module_id_uint = GetDlgItemInt(IDC_module_select, NULL, 0);;
	CString model_id_string;
	GetDlgItemText(IDC_module_select, model_id_string);
	CString software_type;
	GetDlgItemText(IDC_SOFTWARE_SELECT, software_type);
	if (software_type == _T("")) return FALSE;
	else if (software_type == _T("REC")) {

		service_code = 0x20;
		tx_fu_data[2] = 0x14;
		tx_fu_data[3] = 0x55;
		if (model_id_string == _T("")) return FALSE;
		else if (model_id_string == _T("all module")) {

			tx_fu_data[0] = 0xFF;
			tx_fu_data[1] = 0xFF;
			uniflash->m_u16UpdaingNodeAdd = 0x3F;
			uniflash->NodeOffset = 0x20;
		}
		else {

			uniflash->Module_number = module_id_uint;
			uniflash->NodeOffset = 0x20;
			uniflash->m_u16UpdaingNodeAdd = 0x1F + module_id_uint;
			*(UINT16*)tx_fu_data = 1 << module_id_uint;
		}
	}
	else if (software_type == _T("INV")) {

		service_code = 0x21;
		tx_fu_data[2] = 0x25;
		tx_fu_data[3] = 0x4A;
		if (model_id_string == _T("")) return FALSE;
		else if (model_id_string == _T("all module")) {

			tx_fu_data[0] = 0xFF;
			tx_fu_data[1] = 0xFF;
			uniflash->NodeOffset = 0x10;
			uniflash->m_u16UpdaingNodeAdd = 0x3F;
		}
		else {

			uniflash->Module_number = module_id_uint;
			uniflash->NodeOffset = 0x10;
			uniflash->m_u16UpdaingNodeAdd = 0x0F + module_id_uint;
			*(UINT16*)tx_fu_data = 1 << module_id_uint;
		}
	}
	else if (software_type == _T("BYP")) {

		service_code = 0x22;
		tx_fu_data[0] = 0x00;
		tx_fu_data[1] = 0x00;
		tx_fu_data[2] = 0xAA;
		tx_fu_data[3] = 0x55;
		uniflash->Module_number = module_id_uint;
		uniflash->NodeOffset = 0x00;
		uniflash->m_u16UpdaingNodeAdd = 0x0F;
	}

	
	return TRUE;
}

int FlashUpdateMain::ParameterRefresh()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST2);
	pList->DeleteAllItems();
	time_out_cnt = 0;
	BlockCount = 0;
	for (UINT16 i = 0; i < 500; ++i) {
		EveryBlockDataNum[i] = 0;
		BlockCheckSum[i] = 0;
		BlockAddress[i] = 0;
		for (UINT16 j = 0; j < 1024; ++j) {

			BlockData[i][j] = 0;
		}

	}

	

	msg_init.DataLen = 0;
	msg_init.ExternFlag = 0;
	msg_init.ID = 0;
	msg_init.RemoteFlag = 0;
	msg_init.Reserved[0] = 0;
	msg_init.Reserved[1] = 0;
	msg_init.Reserved[2] = 0;
	msg_init.SendType = 0;
	msg_init.TimeFlag = 0;
	msg_init.Data[0] = 0;
	msg_init.Data[1] = 0;
	msg_init.Data[2] = 0;
	msg_init.Data[3] = 0;
	msg_init.Data[4] = 0;
	msg_init.Data[5] = 0;
	msg_init.Data[6] = 0;
	msg_init.Data[7] = 0;
	return 0;
}


void FlashUpdateMain::OnBnClickedClearMsg()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST2);
	pList->DeleteAllItems();
}


void FlashUpdateMain::OnClose()
{
	if (AfxMessageBox(_T("Are you Sure Exit FlashUpdate?"), MB_YESNO) == IDNO)return;

	CDialogEx::OnClose();
	CDialogEx::OnCancel();
}


void FlashUpdateMain::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void FlashUpdateMain::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}

