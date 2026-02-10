#pragma once



// CSellDlg 窗体视图

class CSellDlg : public CFormView
{
	DECLARE_DYNCREATE(CSellDlg)

protected:
	CSellDlg();           // 动态创建所使用的受保护的构造函数
	virtual ~CSellDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELL };
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
	afx_msg void OnCbnSelchangeCombo2();
private:
	CComboBox m_ccbomo;
public:
//	afx_msg void OnEnChangeEdit1();
private:
	int m_price;
	int m_left;
	int m_num;
	CString m_sellinfor;
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};


