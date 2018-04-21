#pragma once


// FlashUpdateInv 对话框

class FlashUpdateMain : public CDialogEx
{
	DECLARE_DYNAMIC(FlashUpdateMain)
private:
	HICON m_hIcon;


public:
	FlashUpdateMain(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~FlashUpdateMain();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FLASH_UPDATE_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStartFlashUpdate();
	afx_msg void OnBnClickedOpenCanDevice();
private:
	CProgressCtrl FlashUpdateProgressCtrl[16];
	int	FlashUpdateProgress[16];
	int InvFlashUpdate();
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
	int i = 0;
public:
	afx_msg void OnBnClickedCloseCanDevice();

private:
	int FlashUpdateProgressSetPos();
	int Hex_file_resolve();
};
