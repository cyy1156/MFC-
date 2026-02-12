#pragma once



// CDialogShow 窗体视图

class CDialogShow : public CFormView
{
	DECLARE_DYNCREATE(CDialogShow)

protected:
	CDialogShow();           // 动态创建所使用的受保护的构造函数
	virtual ~CDialogShow();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Show };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
private:
	CListCtrl m_list;
};


