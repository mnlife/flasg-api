#pragma once

#include "CAN_FlashupdateMsgHandle.h"
#include "ControlCAN.h"
#include "afxwin.h"
// FlashUpdateInv 对话框




class	CAN_FlashupdateMsgHandle;
class FlashUpdateMain : public CDialogEx
{
	DECLARE_DYNAMIC(FlashUpdateMain)
private:
	HICON m_hIcon;

	afx_msg int OnBnClickedOpenCanDevice();
	afx_msg void OnBnClickedCloseCanDevice();
	void TryCloseDevice();
	int FlashUpdateProgressSetPos();
	int Hex_file_resolve();
	int FlashUpdateProgress[16];
	bool SendFlashUpdateOrder();

	UINT16 time_out_cnt;
	int ParameterRefresh();
	VCI_CAN_OBJ msg_init;
	CAN_FlashupdateMsgHandle * uniflash;
	int FlashUpdateGetNode();
	int device_type = 4;	// CANalyst-II
	int device_ind = 0;		// first device
	int can_ind = 0;		// CAN channel 0
	BYTE service_code;
	BYTE tx_fu_data[4];

	CProgressCtrl FlashUpdateProgressCtrl[16];
	

	struct {
		DWORD	data_head;
		DWORD	data_trail;
		DWORD	data_count;
		DWORD	next_data_head;
	}transfer_data;

	UINT16	BlockCount;
	UINT16	EveryBlockDataNum[500];
	UINT16	BlockData[500][1024];
	UINT32	BlockCheckSum[500];
	UINT32	BlockAddress[500];


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()

public:
	FlashUpdateMain(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~FlashUpdateMain();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStartFlashUpdate();
	
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FLASH_UPDATE_MAIN };
#endif
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedRecordSaved();
	afx_msg void OnClose();

	
	afx_msg void OnBnClickedClearMsg();
	virtual void OnOK();
	virtual void OnCancel();

};

