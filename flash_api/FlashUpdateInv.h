#pragma once


// FlashUpdateInv 对话框

class FlashUpdateInv : public CDialogEx
{
	DECLARE_DYNAMIC(FlashUpdateInv)

public:
	FlashUpdateInv(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~FlashUpdateInv();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FLASH_UPDATE_INV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
