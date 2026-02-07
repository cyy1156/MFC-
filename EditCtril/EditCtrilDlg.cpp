
// EditCtrilDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "EditCtril.h"
#include "EditCtrilDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEditCtrilDlg 对话框



CEditCtrilDlg::CEditCtrilDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDITCTRIL_DIALOG, pParent)
	, m_text(_T(""))
	, v_edit1(_T(""))
	, v_edit2(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditCtrilDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_edit1);
	DDX_Control(pDX, IDC_EDIT1, m_edit2);
	DDX_Text(pDX, IDC_EDIT3, m_text);
	DDX_Text(pDX, IDC_EDIT2, v_edit1);
	DDX_Text(pDX, IDC_EDIT1, v_edit2);
}

BEGIN_MESSAGE_MAP(CEditCtrilDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_EN_CHANGE(IDC_EDIT2, &CEditCtrilDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON1, &CEditCtrilDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CEditCtrilDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT3, &CEditCtrilDlg::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON3, &CEditCtrilDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CEditCtrilDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CEditCtrilDlg 消息处理程序

BOOL CEditCtrilDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_edit1.SetWindowTextW(_T("C++学院"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEditCtrilDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEditCtrilDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEditCtrilDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEditCtrilDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CEditCtrilDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//获取编辑框内容
	
}

void CEditCtrilDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString str;
	//m_edit1.GetWindowTextW(str);
	UpdateData(TRUE);//TRUE:表示从控件到变量
	v_edit2 = v_edit1;
	UpdateData(FALSE);//FALSE:表示从变量到控件
	

	//m_edit2.SetWindowTextW(str);
}

void CEditCtrilDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	// exit(0);// 退出程序
	// 关闭对话框
	//CDialogEx::OnOK();按钮确定关闭对话框
	CDialogEx::OnCancel();//按钮取消关闭对话框


}

void CEditCtrilDlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CEditCtrilDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	// CDialogEx::OnOK();
}

void CEditCtrilDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//利用关联value的方式获取编辑框内容
	//设置内容
	m_text = TEXT("哈哈");
	UpdateData(FALSE);//FALSE:表示从变量到控件
}

void CEditCtrilDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//将控件里面的内容同步到变量
	UpdateData(TRUE);//TRUE:表示从控件到变量
	MessageBox(m_text);
}
