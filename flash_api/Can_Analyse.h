#pragma once


// Can_Analyse 对话框

class Can_Analyse : public CDialogEx
{
	DECLARE_DYNAMIC(Can_Analyse)

public:
	Can_Analyse(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Can_Analyse();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = CAN_ANALYSE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
