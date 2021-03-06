// ParameterWatching.cpp: 实现文件
//

#include "stdafx.h"
#include "flash_api.h"
#include "ParameterWatching.h"
#include "afxdialogex.h"


// ParameterWatching 对话框

IMPLEMENT_DYNAMIC(ParameterWatching, CDialogEx)

ParameterWatching::ParameterWatching(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARAMETER_WATCHING, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

ParameterWatching::~ParameterWatching()
{
}

void ParameterWatching::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ParameterWatching, CDialogEx)

END_MESSAGE_MAP()


// ParameterWatching 消息处理程序


BOOL ParameterWatching::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	ShowWindow(SW_MAXIMIZE);
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//CSize	a;
	//a.SetSize(100, 200);
	//pList->ApproximateViewRect(a);
	pList->InsertColumn(0, _T("ModuleID"), LVCFMT_LEFT, 66);
	pList->InsertColumn(1, _T("Module FlashUpdate Message"), LVCFMT_LEFT, 200);


	pList = (CListCtrl*)GetDlgItem(IDC_LIST3);
	//CSize	a;
	//a.SetSize(100, 200);
	//pList->ApproximateViewRect(a);
	pList->InsertColumn(0, _T("ModuleID"), LVCFMT_LEFT, 66);
	pList->InsertColumn(1, _T("Module FlashUpdate Message"), LVCFMT_LEFT, 200);

	pList = (CListCtrl*)GetDlgItem(IDC_LIST4);
	//CSize	a;
	//a.SetSize(100, 200);
	//pList->ApproximateViewRect(a);
	pList->InsertColumn(0, _T("ModuleID"), LVCFMT_LEFT, 66);
	pList->InsertColumn(1, _T("Module FlashUpdate Message"), LVCFMT_LEFT, 200);


	pList = (CListCtrl*)GetDlgItem(IDC_LIST5);
	//CSize	a;
	//a.SetSize(100, 200);
	//pList->ApproximateViewRect(a);
	pList->InsertColumn(0, _T("ModuleID"), LVCFMT_LEFT, 66);
	pList->InsertColumn(1, _T("Module FlashUpdate Message"), LVCFMT_LEFT, 200);

	pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//CSize	a;
	//a.SetSize(100, 200);
	//pList->ApproximateViewRect(a);
	pList->InsertColumn(0, _T("ModuleID"), LVCFMT_LEFT, 66);
	pList->InsertColumn(1, _T("Module FlashUpdate Message"), LVCFMT_LEFT, 200);


	pList = (CListCtrl*)GetDlgItem(IDC_LIST6);
	//CSize	a;
	//a.SetSize(100, 200);
	//pList->ApproximateViewRect(a);
	pList->InsertColumn(0, _T("ModuleID"), LVCFMT_LEFT, 66);
	pList->InsertColumn(1, _T("Module FlashUpdate Message"), LVCFMT_LEFT, 200);

	pList = (CListCtrl*)GetDlgItem(IDC_LIST7);
	//CSize	a;
	//a.SetSize(100, 200);
	//pList->ApproximateViewRect(a);
	pList->InsertColumn(0, _T("ModuleID"), LVCFMT_LEFT, 66);
	pList->InsertColumn(1, _T("Module FlashUpdate Message"), LVCFMT_LEFT, 200);


	pList = (CListCtrl*)GetDlgItem(IDC_LIST8);
	//CSize	a;
	//a.SetSize(100, 200);
	//pList->ApproximateViewRect(a);
	pList->InsertColumn(0, _T("ModuleID"), LVCFMT_LEFT, 66);
	pList->InsertColumn(1, _T("Module FlashUpdate Message"), LVCFMT_LEFT, 200);

	pList = (CListCtrl*)GetDlgItem(IDC_LIST9);
	//CSize	a;
	//a.SetSize(100, 200);
	//pList->ApproximateViewRect(a);
	pList->InsertColumn(0, _T("ModuleID"), LVCFMT_LEFT, 66);
	pList->InsertColumn(1, _T("Module FlashUpdate Message"), LVCFMT_LEFT, 200);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


