#include "pch.h"
#include "framework.h"
#include "CDialogbg.h"
#include "CDialogbgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

// CCDialogbgDlg 对话框
CCDialogbgDlg::CCDialogbgDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CDIALOGBG_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCDialogbgDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCDialogbgDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_CTLCOLOR()
    ON_WM_NCHITTEST()
    ON_WM_ERASEBKGND()  // 新增：背景擦除消息映射
END_MESSAGE_MAP()

// 修复：将函数改为类成员函数（解决作用域问题）
BOOL CCDialogbgDlg::LoadImageFromResource(CImage *pImage, UINT nResID, LPCTSTR lpType)
{
    /*// 1. 获取当前模块的资源句柄
    HMODULE hModule = AfxGetResourceHandle();
    if (hModule == NULL)
    {
        AfxMessageBox(_T("获取资源句柄失败！"));
        return FALSE;
    }

    // 2. 查找指定ID和类型的资源
    HRSRC hRsrc = ::FindResource(hModule, MAKEINTRESOURCE(nResID), lpType);
    if (hRsrc == NULL)
    {
        AfxMessageBox(_T("查找资源失败！"));
        return FALSE;
    }

    // 3. 加载资源并锁定内存
    HGLOBAL hGlobal = ::LoadResource(hModule, hRsrc);
    if (hGlobal == NULL)
    {
        AfxMessageBox(_T("加载资源失败！"));
        return FALSE;
    }

    LPVOID pData = ::LockResource(hGlobal);
    DWORD dwSize = ::SizeofResource(hModule, hRsrc);
    if (pData == NULL || dwSize == 0)
    {
        AfxMessageBox(_T("锁定资源失败！"));
        ::FreeResource(hGlobal); // 释放资源
        return FALSE;
    }

    // 4. 创建内存流并加载图片到CImage
    IStream* pStream = NULL;
    HRESULT hr = ::CreateStreamOnHGlobal(hGlobal, FALSE, &pStream);
    if (FAILED(hr))
    {
        // 使用 _com_error 打印详细的 HRESULT 描述
        _com_error err(hr);
        TRACE(_T("CreateStreamOnHGlobal failed: %s (0x%08X)\n"), err.ErrorMessage(), hr);
        AfxMessageBox(_T("创建内存流失败！"));
        ::FreeResource(hGlobal);
        return FALSE;
    }

    // 5. 从流中加载图片
    hr = img.Load(pStream);
    pStream->Release(); // 释放流
    ::FreeResource(hGlobal); // 释放资源

    if (FAILED(hr))
    {
        AfxMessageBox(_T("加载图片到CImage失败！"));
        return FALSE;
    }

    return TRUE;*/
    if (pImage == NULL)
        return false;
	pImage->Destroy(); // 销毁之前的图像（如果有）
	//查找资源
	HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nResID), lpType);
    if (hRsrc==NULL)
    {
        return false;
    }
	//加载资源
    HGLOBAL hGlobal = ::LoadResource(AfxGetResourceHandle(), hRsrc);
    if (hGlobal==NULL)
    {
		::FreeResource(hGlobal);
        return false;
	}
	//锁定资源
	LPVOID IpVoid = ::LockResource(hGlobal);
	LPSTREAM pStream = NULL;
	DWORD dwSize = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
	LPBYTE IpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(IpByte, IpVoid,  dwSize);
	//解除锁定资源
	::GlobalUnlock(hNew);
	//创建内存流
	HRESULT hr = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
    if(hr != S_OK)
    {
        ::GlobalFree(hNew);
        
	}
    else
    {
		//从流中加载图片
		pImage->Load(pStream);
		GlobalFree(hNew);

    }
	//释放资源
    ::FreeResource(hGlobal);
	return true;
}

BOOL CCDialogbgDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。
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

    // 设置图标
    SetIcon(m_hIcon, TRUE);			// 大图标
    SetIcon(m_hIcon, FALSE);		// 小图标

    // 修复：初始化时加载背景图片到成员变量m_bgImage
    LoadImageFromResource(&m_bgImage, IDB_BG2, _T("PNG"));

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCDialogbgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCDialogbgDlg::OnPaint()
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

HCURSOR CCDialogbgDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// 修复：OnEraseBkgnd函数（核心背景绘制逻辑）
BOOL CCDialogbgDlg::OnEraseBkgnd(CDC* pDC)
{
    // 如果背景图片加载成功，则绘制
    if (m_bgImage.IsNull())
    {
        return CDialogEx::OnEraseBkgnd(pDC);
    }

    // 获取窗口客户区大小
    CRect rect;
    GetClientRect(&rect);

    // 绘制图片（拉伸填充整个客户区，支持PNG/JPG）
    m_bgImage.Draw(pDC->m_hDC, rect);

    return TRUE; // 阻止系统默认背景擦除，避免闪烁
}

// 修复：OnCtlColor函数（仅处理静态文本背景透明，避免控件污染）
HBRUSH CCDialogbgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // 仅让静态文本（Static Text）背景透明
    if (nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkMode(TRANSPARENT); // 设置文字背景透明
        return (HBRUSH)GetStockObject(NULL_BRUSH); // 返回空画刷
    }

    // 其他控件使用默认画刷
    return hbr;
}

// 修复：OnNcHitTest函数（保留窗口正常操作）
LRESULT CCDialogbgDlg::OnNcHitTest(CPoint point)
{
    // 先调用默认处理，获取原始命中测试结果
    LRESULT lResult = CDialogEx::OnNcHitTest(point);

    // 仅当命中客户区时，返回HTCAPTION（实现拖动窗口）
    if (lResult == HTCLIENT)
    {
        return HTCAPTION;
    }

    // 其他区域（如边框、标题栏）使用默认处理
    return lResult;
}