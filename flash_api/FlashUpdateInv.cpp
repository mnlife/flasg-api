// FlashUpdateInv.cpp: 实现文件
//

#include "stdafx.h"
#include "flash_api.h"
#include "FlashUpdateInv.h"
#include "afxdialogex.h"


// FlashUpdateInv 对话框

IMPLEMENT_DYNAMIC(FlashUpdateInv, CDialogEx)

FlashUpdateInv::FlashUpdateInv(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FLASH_UPDATE_INV, pParent)
{

}

FlashUpdateInv::~FlashUpdateInv()
{
}

void FlashUpdateInv::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FlashUpdateInv, CDialogEx)
END_MESSAGE_MAP()


// FlashUpdateInv 消息处理程序
