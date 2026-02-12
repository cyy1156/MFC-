// CAddDlg.cpp: 实现文件
//

#include "pch.h"
#include "SaleSystem.h"
#include "CAddDlg.h"
#include"CInfoFile.h"


// CAddDlg

IMPLEMENT_DYNCREATE(CAddDlg, CFormView)

CAddDlg::CAddDlg()
	: CFormView(IDD_DIALOG_Insert)
	, m_price1(0)
	, m_num1(0)
	, m_name2(_T(""))
	, m_price2(0)
	, m_num2(0)
	, m_left(0)
{

}

CAddDlg::~CAddDlg()
{
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ccombo);
	DDX_Text(pDX, IDC_EDIT1, m_price1);
	DDX_Text(pDX, IDC_EDIT5, m_num1);
	DDX_Text(pDX, IDC_EDIT3, m_name2);
	DDX_Text(pDX, IDC_EDIT4, m_price2);
	DDX_Text(pDX, IDC_EDIT2, m_num2);
	DDX_Text(pDX, IDC_EDIT6, m_left);
}

BEGIN_MESSAGE_MAP(CAddDlg, CFormView)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAddDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON5, &CAddDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CAddDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CAddDlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// CAddDlg 诊断

#ifdef _DEBUG
void CAddDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAddDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAddDlg 消息处理程序

void CAddDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//初始化销售订单窗口
	CInfoFile file;

	//读取商品信息
	//file.ReadDocLine();
	//将商品名称添加到下拉列表框中
	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	{
		//m_ccbomo.AddString(CString(it->name.c_str()));
		m_ccombo.AddString(CString(it->name.c_str()));
	}
	//设置默认选中项
	//m_ccbomo.SetCurSel(0);
	//m_ccbomo.SetCurSel(0);
	m_ccombo.SetCurSel(0);
	// 更新第一个商品的信息
	OnCbnSelchangeCombo1();

	// TODO: 在此添加专用代码和/或调用基类
}

void CAddDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	// 获取当前选中的索引
	//int index = m_ccbomo.GetCurSel();
	int index = m_ccombo.GetCurSel();
	if (index == CB_ERR) return;

	// 获取当前商品名称
	CString strName;
	//m_ccbomo.GetLBText(index, strName);
	m_ccombo.GetLBText(index, strName);

	// 根据名称查找商品信息
	CInfoFile file;
	// file.ReadDocLine(); // 构造函数已经读取了

	// 使用传统迭代器替代 auto
	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	{
		if (strName == CString(it->name.c_str()))
		{
			m_price1 = it->price;
			m_left = it->count; // 显示当前库存
			UpdateData(FALSE); // 更新界面
			return;
		}
	}
}

void CAddDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 获取界面数据
	if (m_num1 <= 0)
	{
		MessageBox(_T("请输入正确的数量！"), _T("错误"), MB_ICONERROR);
		return;
	}
	//添加个数
	//int index = m_ccbomo.GetCurSel();
	int index = m_ccombo.GetCurSel();
	CString name;
	//m_ccbomo.GetLBText(index, name);
	m_ccombo.GetLBText(index, name);
	CInfoFile file;
	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	{
		if (name == CString(it->name.c_str()))
		{
			it->num += m_num1; // 更新库存
			it->count= it->num; // 更新界面显示的库存
			m_left = it->num; // 更新界面显示的库存
			UpdateData(FALSE);
			MessageBox(_T("添加成功！"));

		}
	}
	file.WriteDocLine(); // 保存修改
	//清空数据
	
	m_num1 = 0;
	
	UpdateData(FALSE); // 更新界面

}

void CAddDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 获取界面数据
	if (m_num2 <= 0)
	{
		MessageBox(_T("请输入正确的数量！"), _T("错误"), MB_ICONERROR);
		return;
	}
	if (m_price2<= 0)
	{
		MessageBox(_T("请输入正确的价格！"), _T("错误"), MB_ICONERROR);
		return;
	}
	//添加新商品
	CInfoFile file;
	file.AddDocLine(m_name2, m_num2, m_price2);
	file.WriteDocLine(); // 保存修改
	MessageBox(_T("添加成功！"));

	
}

void CAddDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 获取界面数据
	m_name2.Empty();
	m_price2=0;
	m_num2=0;
	UpdateData(FALSE); // 更新界面
}
