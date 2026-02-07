// CLoginDlg.cpp: 实现文件
//

#include "pch.h"
#include "SaleSystem.h"
#include "afxdialogex.h"
#include "CLoginDlg.h"
#include"CInfoFile.h"

// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
	, m_pmd(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_user);
	DDX_Text(pDX, IDC_EDIT2, m_pmd);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CLoginDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLoginDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLoginDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序

void CLoginDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
/*
	UpdateDate(TRUE);
   if(m_user.IsEmpty()||m_pmd.IsEmpty())
	{
		MessageBox(_T("登录信息不能为空"));
		return;
	}
	//获取正确的值
	CInfoFile info;

	CString name, pwd;
	info.ReadLoginInfo(name, pwd);
	if (name == m_user)
	{
		if (pwd == m_pmd) {
			MessageBox(_T("登录成功"));
			CDialog::OnCancel();
		}
		else
		{
			MessageBox(_T("密码错误"));
		}
	}
	else
	{
		MessageBox(_T("用户名"));
	}*/
}

void CLoginDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_user.IsEmpty() || m_pmd.IsEmpty())
	{
		MessageBox(_T("登录信息不能为空"));
		return;
	}
	//获取正确的值
	CInfoFile info;

	CString name, pwd;
	info.ReadLoginInfo(name, pwd);
	if (name == m_user)
	{
		if (pwd == m_pmd) {
			MessageBox(_T("登录成功"));
			CDialog::OnCancel();
		}
		else
		{
			MessageBox(_T("密码错误"));
		}
	}
	else
	{
		MessageBox(_T("用户名"));
	}
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CInfoFile file;
	CString pmd, name;
	file.ReadLoginInfo(name, pmd);
	m_user = name;
	m_pmd = pmd;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLoginDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}

void CLoginDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}

void CLoginDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CDialogEx::OnClose();
	exit(0);
}
