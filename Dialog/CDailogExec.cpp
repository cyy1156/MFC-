// CDailogExec.cpp: 实现文件
//

#include "pch.h"
#include "Dialog.h"
#include "afxdialogex.h"
#include "CDailogExec.h"


// CDailogExec 对话框

IMPLEMENT_DYNAMIC(CDailogExec, CDialogEx)

CDailogExec::CDailogExec(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXEC, pParent)
{

}

CDailogExec::~CDailogExec()
{
}

void CDailogExec::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDailogExec, CDialogEx)
END_MESSAGE_MAP()


// CDailogExec 消息处理程序
