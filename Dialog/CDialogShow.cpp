// CDialogShow.cpp: 实现文件
//

#include "pch.h"
#include "Dialog.h"
#include "afxdialogex.h"
#include "CDialogShow.h"


// CDialogShow 对话框

IMPLEMENT_DYNAMIC(CDialogShow, CDialogEx)

CDialogShow::CDialogShow(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHOW, pParent)
{

}

CDialogShow::~CDialogShow()
{
}

void CDialogShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogShow, CDialogEx)
END_MESSAGE_MAP()


// CDialogShow 消息处理程序
