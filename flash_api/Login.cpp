// Login.cpp: 实现文件
//

#include "stdafx.h"
#include "flash_api.h"
#include "Login.h"
#include "afxdialogex.h"


// Login 对话框

IMPLEMENT_DYNAMIC(Login, CDialogEx)

Login::Login(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Login, pParent)
{

}

Login::~Login()
{
}

void Login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Login, CDialogEx)
	ON_BN_CLICKED(IDOK, &Login::OnBnClickedOk)
END_MESSAGE_MAP()


// Login 消息处理程序


void Login::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


