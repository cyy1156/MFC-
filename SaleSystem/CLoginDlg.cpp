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
	m_staticColor = RGB(255, 255, 255); // 白色示例
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
	ON_WM_ERASEBKGND() // 新增：背景擦除消息映射
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
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
	
	/*/CBitmap bitmap;
	BITMAP bmInfo;
	CDC memDC;
	CRect rect;
	CClientDC dc(this);
	bitmap.LoadBitmapW(IDB_BLOGIN_BG1);
	bitmap.GetBitmap(&bmInfo);
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(bitmap);
	GetClientRect(&rect);
	//dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC,0,
		0, bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY);
	memDC.DeleteDC();
	bitmap.DeleteObject();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
*/return TRUE;  // return TRUE unless you set the focus to a control
// 异常: OCX 属性页应返回 FALSE
}
BOOL CLoginDlg::OnEraseBkgnd(CDC* pDC)
{
	// 1. 加载位图（确保 IDB_BLOGIN_BG1 是导入的 BMP 资源）
	CBitmap bitmap;
	if (!bitmap.LoadBitmapW(IDB_BLOGIN_BG1))
	{
		// 加载失败则执行默认处理
		return CDialogEx::OnEraseBkgnd(pDC);
	}

	// 2. 获取位图信息和对话框客户区大小
	BITMAP bmInfo;
	bitmap.GetBitmap(&bmInfo);
	CRect rect;
	GetClientRect(&rect);

	// 3. 创建兼容 DC 并选入位图
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap); // 保存原有位图

	// 4. 拉伸绘制位图到整个对话框
	pDC->StretchBlt(
		0, 0, rect.Width(), rect.Height(),  // 目标区域（全屏）
		&memDC, 0, 0,                       // 源坐标（位图左上角）
		bmInfo.bmWidth, bmInfo.bmHeight,    // 源位图尺寸
		SRCCOPY
	);

	// 5. 释放资源（恢复原有位图，避免泄漏）
	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	// 6. 返回 TRUE，阻止系统默认的背景擦除（关键！）
	return TRUE;
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

HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	
		if (nCtlColor == CTLCOLOR_STATIC)
		{
			pDC->SetBkMode(TRANSPARENT); // 文字背景透明
			
			// 根据控件 ID 精确控制某个静态文本的颜色
			INT id = pWnd->GetDlgCtrlID();
			if (id == IDC_STATIC_USER || id == IDC_STATIC_PWD) // 用你的控件 ID 替换
			{
				pDC->SetTextColor(m_staticColor);
				pDC->SetBkMode(TRANSPARENT);
			}
				return (HBRUSH)GetStockObject(NULL_BRUSH);
			

		}
			// 或者统一设置所有静态文本颜色
			// pDC->SetTextColor(m_staticColor);
			// pDC->SetBkMode(TRANSPARENT);
			// return (HBRUSH)GetStockObject(NULL_BRUSH);
	// 其他控件使用默认画刷
	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
