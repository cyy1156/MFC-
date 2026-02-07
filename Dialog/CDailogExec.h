#pragma once
#include "afxdialogex.h"


// CDailogExec 对话框

class CDailogExec : public CDialogEx
{
	DECLARE_DYNAMIC(CDailogExec)

public:
	CDailogExec(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDailogExec();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXEC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
