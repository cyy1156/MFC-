#pragma once



// CDeleteDlg 窗体视图

class CDeleteDlg : public CFormView
{
	DECLARE_DYNCREATE(CDeleteDlg)

protected:
	CDeleteDlg();           // 动态创建所使用的受保护的构造函数
	virtual ~CDeleteDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Delete };
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
	CComboBox m_ccbomo;
	int m_price;
	int m_num;
public:
	virtual void OnInitialUpdate();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton6();
private:
	int m_left;
public:
	afx_msg void OnBnClickedButton4();
	afx_msg void OnCbnSelchangeCombo2();
private:
//	CComboBox m_ccbomo1;
	CComboBox m_cobomo1;
	int m_price1;
	int m_left1;
};


