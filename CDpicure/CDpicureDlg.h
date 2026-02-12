#pragma once
#include <atlimage.h>  // 可选，若用CImage加载非BMP图片

// CCDpicureDlg 对话框
class CCDpicureDlg : public CDialogEx
{
    // 构造
public:
    CCDpicureDlg(CWnd* pParent = nullptr);	// 标准构造函数

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CDPICURE_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

    // 实现
protected:
    HICON m_hIcon;
    int m_iPicIndex;  // 用成员变量替代全局变量，更安全

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);  // 新增：背景擦除，避免闪烁
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};