// CDeleteDlg.cpp: 实现文件
//

#include "pch.h"
#include "SaleSystem.h"
#include "CDeleteDlg.h"
#include"CInfoFile.h"


// CDeleteDlg

IMPLEMENT_DYNCREATE(CDeleteDlg, CFormView)

CDeleteDlg::CDeleteDlg()
	: CFormView(IDD_DIALOG_Delete)
	, m_price(0)
	, m_num(0)
	, m_left(0)
	, m_price1(0)
	, m_left1(0)
{

}

CDeleteDlg::~CDeleteDlg()
{
}

void CDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO1, m_ccbomo);
	DDX_Text(pDX, IDC_EDIT3, m_price);
	DDX_Text(pDX, IDC_EDIT4, m_num);
	DDX_Text(pDX, IDC_EDIT7, m_left);
	DDX_Control(pDX, IDC_COMBO1, m_ccbomo);
	DDX_Control(pDX, IDC_COMBO2, m_cobomo1);
	DDX_Text(pDX, IDC_EDIT8, m_price1);
	DDX_Text(pDX, IDC_EDIT9, m_left1);
}

BEGIN_MESSAGE_MAP(CDeleteDlg, CFormView)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDeleteDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDeleteDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON7, &CDeleteDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CDeleteDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &CDeleteDlg::OnBnClickedButton4)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDeleteDlg::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CDeleteDlg 诊断

#ifdef _DEBUG
void CDeleteDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDeleteDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDeleteDlg 消息处理程序

void CDeleteDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CInfoFile file;
	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	{
		m_ccbomo.AddString(CString(it->name.c_str()));
		m_cobomo1.AddString(CString(it->name.c_str()));
	}
	m_ccbomo.SetCurSel(0);
	m_cobomo1.SetCurSel(0);
	OnCbnSelchangeCombo1();
	OnCbnSelchangeCombo2();

}

void CDeleteDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	// 获取当前选中的索引
	//int index = m_ccbomo.GetCurSel();
	//int index = m_ccombo.GetCurSel();
	int index = m_ccbomo.GetCurSel();
	if (index == CB_ERR) return;

	// 获取当前商品名称
	CString strName;
	//m_ccbomo.GetLBText(index, strName);
	m_ccbomo.GetLBText(index, strName);
	//m_ccombo.GetLBText(index, strName);

	// 根据名称查找商品信息
	CInfoFile file;
	// file.ReadDocLine(); // 构造函数已经读取了

	// 使用传统迭代器替代 auto
	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	{
		if (strName == CString(it->name.c_str()))
		{
			m_price = it->price;
			m_left = it->num;
			UpdateData(FALSE); // 更新界面
			return;
		}
	}

}

void CDeleteDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 获取界面数据
	if (m_num <= 0)
	{
			MessageBox(_T("请输入正确的数量！"), _T("错误"), MB_ICONERROR);
			return;
	}
	if(m_num > m_left)
	{
		MessageBox(_T("库存不足！"), _T("错误"), MB_ICONERROR);
		return;
	}
	//删除商品个数
	CInfoFile file;
	CString name;
	//int index = m_ccombo.GetCurSel();
	int index = m_ccbomo.GetCurSel();
	m_ccbomo.GetLBText(index, name);
	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	{
		if (name == CString(it->name.c_str()))
		{
			
			m_left -= m_num; // 更新界面显示的库存
			it->num = m_left; // 更新内存中的库存
			UpdateData(FALSE);
			MessageBox(_T("删除成功！"));
			//file.WriteDocLine();

		}
	}
	file.WriteDocLine();
	UpdateData(TRUE);
	m_num = 0;
	UpdateData(FALSE);


}

void CDeleteDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDeleteDlg::OnBnClickedButton6()
{
	/* TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 获取界面数据
	int index = m_cobomo1.GetCurSel();
	if (index == CB_ERR) return;
	// 获取当前商品名称
	CString strName;
	//m_ccbomo.GetLBText(index, strName);
	m_cobomo1.GetLBText(index, strName);
	CInfoFile file;
	file.DeleteDocLine(strName);
	MessageBox(_T("已经在库存中删除了") + strName);
	OnInitialUpdate();*/
	
		// 1. 获取选中的商品名称
		int index = m_cobomo1.GetCurSel();
		if (index == CB_ERR)
		{
			MessageBox(_T("请先选择要删除的商品！"), _T("提示"), MB_ICONINFORMATION);
			return;
		}

		CString strName;
		m_cobomo1.GetLBText(index, strName);

		// 2. 二次确认（防止误删）
		if (MessageBox(_T("确定要删除【") + strName + _T("】吗？"), _T("确认删除"), MB_YESNO | MB_ICONQUESTION) != IDYES)
		{
			return;
		}

		// 3. 调用删除函数
		CInfoFile file;
		// 先备份当前列表长度，用于判断是否删除成功
		int oldCount = (int)file.ls.size();
		file.DeleteDocLine(strName);
		int newCount = (int)file.ls.size();

		// 4. 判断删除结果并更新界面
		if (newCount < oldCount)
		{
			// 删除成功：移除下拉框中的对应项
			m_ccbomo.DeleteString(index);
			m_cobomo1.DeleteString(index);

			// 清空对应编辑框数据
			m_price1 = 0;
			m_left1 = 0;
			UpdateData(FALSE);

			// 重新选中第一个商品（如有）
			if (m_cobomo1.GetCount() > 0)
			{
				m_cobomo1.SetCurSel(0);
				OnCbnSelchangeCombo2();
				// 同步更新第一个下拉框
				m_ccbomo.SetCurSel(0);
				OnCbnSelchangeCombo1();
			}

			MessageBox(_T("已成功删除商品【") + strName + _T("】！"), _T("提示"), MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(_T("删除失败：未找到商品【") + strName + _T("】！"), _T("错误"), MB_ICONERROR);
		}
	}
	


void CDeleteDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 获取界面数据
	m_num = 0;
	UpdateData(FALSE);
}
void CDeleteDlg::OnCbnSelchangeCombo2()
{
	int index = m_cobomo1.GetCurSel();
	if (index == CB_ERR) return;

	// 获取当前商品名称
	CString strName;
	//m_ccbomo.GetLBText(index, strName);
	m_cobomo1.GetLBText(index, strName);
	//m_ccombo.GetLBText(index, strName);

	// 根据名称查找商品信息
	CInfoFile file;
	// file.ReadDocLine(); // 构造函数已经读取了

	// 使用传统迭代器替代 auto
	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	{
		if (strName == CString(it->name.c_str()))
		{
			m_price1 = it->price;
			m_left1 = it->num;
			UpdateData(FALSE); // 更新界面
			return;
		}
	}

}

