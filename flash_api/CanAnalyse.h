#pragma once


// CanAnalyse 对话框

class CanAnalyse : public CDialogEx
{
	DECLARE_DYNAMIC(CanAnalyse)

public:
	CanAnalyse(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CanAnalyse();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAN_ANALYSE };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartCanDevice();
	afx_msg void OnBnClickedStartReceived();
	afx_msg void OnBnClickedCloseCanDevice();
	afx_msg void OnBnClickedClearReceived();

private:
	int mnlife;
public:
	afx_msg void OnBnClickedStopReceived();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
