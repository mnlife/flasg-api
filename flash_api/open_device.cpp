// open_device.cpp: 实现文件
//

#include "stdafx.h"
#include "flash_api.h"
#include "open_device.h"
#include "afxdialogex.h"


// open_device 对话框

IMPLEMENT_DYNAMIC(open_device, CDialogEx)

open_device::open_device(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Login, pParent)
{

}

open_device::~open_device()
{
}

void open_device::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(open_device, CDialogEx)
END_MESSAGE_MAP()


// open_device 消息处理程序
