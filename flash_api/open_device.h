#pragma once


// open_device 对话框

class open_device : public CDialogEx
{
	DECLARE_DYNAMIC(open_device)

public:
	open_device(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~open_device();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
