// Can_Analyse.cpp: 实现文件
//

#include "stdafx.h"
#include "flash_api.h"
#include "Can_Analyse.h"
#include "afxdialogex.h"


// Can_Analyse 对话框

IMPLEMENT_DYNAMIC(Can_Analyse, CDialogEx)

Can_Analyse::Can_Analyse(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CAN_ANALYSE, pParent)
{

}

Can_Analyse::~Can_Analyse()
{
}

void Can_Analyse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Can_Analyse, CDialogEx)
END_MESSAGE_MAP()


// Can_Analyse 消息处理程序
