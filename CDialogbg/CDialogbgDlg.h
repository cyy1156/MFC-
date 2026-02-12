#pragma once
#include <atlimage.h>  // 必须包含CImage头文件

// CCDialogbgDlg 对话框
class CCDialogbgDlg : public CDialogEx
{
    // 构造
public:
    CCDialogbgDlg(CWnd* pParent = nullptr);	// 标准构造函数

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CDIALOGBG_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

    // 实现
protected:
    HICON m_hIcon;
    CImage m_bgImage;  // 存储背景图片（成员变量，避免提前释放）

    // 声明自定义函数（解决作用域问题）
    BOOL LoadImageFromResource(CImage* pImage, UINT nResID, LPCTSTR lpType);

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);  // 新增：背景绘制函数
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg LRESULT OnNcHitTest(CPoint point);
    DECLARE_MESSAGE_MAP()
};