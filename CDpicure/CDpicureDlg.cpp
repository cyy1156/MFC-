#include "pch.h"
#include "framework.h"
#include "CDpicure.h"
#include "CDpicureDlg.h"
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

// CCDpicureDlg 对话框
CCDpicureDlg::CCDpicureDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CDPICURE_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_iPicIndex = 0;  // 初始化循环变量
}

void CCDpicureDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCDpicureDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CCDpicureDlg::OnBnClickedButton1)
    ON_WM_TIMER()
    ON_WM_ERASEBKGND()  // 新增：背景擦除消息映射
    ON_WM_DRAWITEM()
END_MESSAGE_MAP()

BOOL CCDpicureDlg::OnInitDialog()
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

    return TRUE;
}

void CCDpicureDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCDpicureDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CCDpicureDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// 修复：点击按钮创建定时器（不立即销毁）
void CCDpicureDlg::OnBnClickedButton1()
{
    // 先销毁旧定时器（避免重复创建），再创建新定时器
    KillTimer(1);
    m_iPicIndex = 0;  // 重置循环变量
    SetTimer(1, 500, NULL);  // 500ms触发一次（放慢速度，便于观察）
}

// 修复：定时器绘制逻辑
void CCDpicureDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        CBitmap bitmap;
        BITMAP bmInfo;
        CDC memDC;
        CRect rect;
        CClientDC dc(this);

        // 1. 加载位图（确保IDB_BITMAP1已导入资源）
        if (!bitmap.LoadBitmapW(IDB_BITMAP1))
        {
            AfxMessageBox(_T("加载位图失败！"));
            KillTimer(1);
            return;
        }

        // 2. 获取位图信息
        bitmap.GetBitmap(&bmInfo);

        // 3. 创建兼容DC并选入位图
        memDC.CreateCompatibleDC(&dc);
        CBitmap* pOldBmp = memDC.SelectObject(&bitmap);

        // 4. 获取窗口客户区大小
        GetClientRect(&rect);

        // 5. 清空背景（避免图片重叠）
        dc.FillSolidRect(rect, RGB(255, 255, 255));

        // 6. 绘制位图（这里演示简单循环，若要多图可扩展case）
        // 如果你是分帧位图（一张图包含多帧），可修改BitBlt的源坐标
        dc.StretchBlt(
            0, 0, rect.Width(), rect.Height(),  // 目标区域（全屏）
            &memDC, 0, 0,                       // 源坐标（位图左上角）
            bmInfo.bmWidth, bmInfo.bmHeight,    // 源位图尺寸
            SRCCOPY
        );

        // 7. 循环变量自增并重置
        m_iPicIndex++;
        if (m_iPicIndex >= 5)  // 修复：>=5时重置为0
        {
            m_iPicIndex = 0;
            // 可选：循环5次后停止定时器
            // KillTimer(1);
        }

        // 8. 释放资源（恢复原有位图，避免泄漏）
        memDC.SelectObject(pOldBmp);
        memDC.DeleteDC();
        bitmap.DeleteObject();
    }

    CDialogEx::OnTimer(nIDEvent);
}

// 新增：阻止默认背景擦除，避免闪烁
BOOL CCDpicureDlg::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;  // 返回TRUE表示自行处理背景
}
/*void CCDpicureDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CBitmap bitmap;
	BITMAP bmInfo;
	CDC memDC;
	CRect rect;
	CClientDC dc(this);

	

		bitmap.LoadBitmapW(IDB_BITMAP1);
		bitmap.GetBitmap(&bmInfo);
		memDC.CreateCompatibleDC(&dc);
		memDC.SelectObject(bitmap);
		GetClientRect(&rect);

		//dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, i* bmInfo.bmWidth/5, i* bmInfo.bmHeight/5, bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY);
		i++;
		if (i >= 5)
			i = 0;
		memDC.DeleteDC();
		bitmap.DeleteObject();
		
}
*/
/*
void CCDpicureDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CBitmap bitmap;
    BITMAP bmInfo;
    CDC memDC;
    CRect rect=lpDrawItemStruct->rcItem;
    CDC* dc = CDC::FromHandle(lpDrawItemStruct->hDC);
        switch (nIDCtl)
        {
        case IDC_BUTTON2:
            bitmap.LoadBitmapW(IDB_BITMAP2);
            bitmap.GetBitmap(&bmInfo);
            memDC.CreateCompatibleDC(dc);
            memDC.SelectObject(bitmap);

            dc->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC,  0,  0 , 
                bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY);

            break;
        default:
            break;

        }
    CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}*/
void CCDpicureDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // 入参校验
    if (lpDrawItemStruct == NULL || nIDCtl != IDC_BUTTON2)
    {
        CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
        return;
    }

    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect btnRect = lpDrawItemStruct->rcItem; // 按钮的实际区域
    CBitmap bitmap;
    BITMAP bmInfo;

    // 1. 加载按钮位图
    if (!bitmap.LoadBitmapW(IDB_BITMAP2))
    {
        CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
        return;
    }
    bitmap.GetBitmap(&bmInfo);

    // 2. 创建兼容DC并选入位图
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBmp = memDC.SelectObject(&bitmap);

    // 3. 清空按钮背景（避免重叠）
    pDC->FillSolidRect(btnRect, RGB(255, 255, 255));

    // 4. 绘制位图到按钮区域（适配按钮大小）
    pDC->StretchBlt(
        btnRect.left, btnRect.top, btnRect.Width(), btnRect.Height(),
        &memDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY
    );

    // 5. 释放资源（关键！避免GDI泄漏）
    memDC.SelectObject(pOldBmp);
    memDC.DeleteDC();
    bitmap.DeleteObject();

    // 6. 处理按钮状态（按下/弹起）
    if (lpDrawItemStruct->itemState & ODS_SELECTED)
    {
        // 按钮按下时，绘制边框（可选）
        pDC->DrawEdge(btnRect, EDGE_SUNKEN, BF_RECT);
    }
    else
    {
        // 按钮弹起时，绘制边框
        pDC->DrawEdge(btnRect, EDGE_RAISED, BF_RECT);
    }

    CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
