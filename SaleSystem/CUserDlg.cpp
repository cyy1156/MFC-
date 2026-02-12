// CUserDlg.cpp: 实现文件
//

#include "pch.h"
#include "SaleSystem.h"
#include "CUserDlg.h"
#include"CInfoFile.h"

// CUserDlg

IMPLEMENT_DYNCREATE(CUserDlg, CFormView)

CUserDlg::CUserDlg()
	: CFormView(IDD_DIALOG_USER)
	, m_user(_T(""))
	, m_name(_T(""))
	, m_newpwd(_T(""))
	, m_surepwd(_T(""))
{

}

CUserDlg::~CUserDlg()
{
}

void CUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_user);
	DDX_Text(pDX, IDC_EDIT2, m_name);
	DDX_Text(pDX, IDC_EDIT3, m_newpwd);
	DDX_Text(pDX, IDC_EDIT4, m_surepwd);
}

BEGIN_MESSAGE_MAP(CUserDlg, CFormView)
	ON_BN_CLICKED(IDC_BUTTON3, &CUserDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CUserDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CUserDlg 诊断

#ifdef _DEBUG
void CUserDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CUserDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CUserDlg 消息处理程序

void CUserDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//初始化
	m_user = TEXT("客户");
	CInfoFile file;
	CString name, pwd;
	file.ReadLoginInfo(name, pwd);
	m_name = name;
	UpdateData(FALSE);


}

void CUserDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//修改密码功能
	UpdateData(TRUE);//获取数据
	
	if (m_surepwd.IsEmpty() || m_newpwd.IsEmpty())
	{
		AfxMessageBox(TEXT("请输入完整信息"));
		return;
	}
	CInfoFile file;
	CString name, pwd;
	file.ReadLoginInfo(name, pwd);
	if(m_newpwd != m_surepwd)
		{
			AfxMessageBox(TEXT("两次输入密码不一致"));
			return;
	    }
	if(m_newpwd==pwd)
	{
		AfxMessageBox(TEXT("新密码不能与旧密码相同"));
		return;
	}
	//CString转为char*
	CStringA tmp;
	CStringA tmp2;
		tmp = name;
		tmp2 = m_newpwd;

	file.WriteLoginInfo(tmp.GetBuffer(), tmp2.GetBuffer());
	MessageBox(_T("修改成功"));
	//清空内容
	m_newpwd.Empty();
	m_surepwd.Empty();
	UpdateData(FALSE);

}

void CUserDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//取消按钮
	m_newpwd.Empty();
	m_surepwd.Empty();
	UpdateData(FALSE);
}
