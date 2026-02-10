#pragma once



// CAddDlg 窗体视图

class CAddDlg : public CFormView
{
	DECLARE_DYNCREATE(CAddDlg)

protected:
	CAddDlg();           // 动态创建所使用的受保护的构造函数
	virtual ~CAddDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Insert };
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
private:
	CComboBox m_ccombo;
	int m_price1;
	int m_num1;
	CString m_name2;
	int m_price2;
	int m_num2;
public:
	virtual void OnInitialUpdate();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton5();
private:
	int m_left;
public:
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
};


