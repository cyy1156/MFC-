// CSellDlg.cpp: 实现文件
//

#include "pch.h"
#include "SaleSystem.h"
#include "CSellDlg.h"
#include"CInfoFile.h"



// CSellDlg

IMPLEMENT_DYNCREATE(CSellDlg, CFormView)

CSellDlg::CSellDlg()
	: CFormView(IDD_DIALOG_SELL)
	, m_left(0)
	, m_num(0)
	, m_sellinfor(_T(""))
{

}

CSellDlg::~CSellDlg()
{
}

void CSellDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_ccbomo);
	DDX_Text(pDX, IDC_EDIT1, m_price);
	DDX_Text(pDX, IDC_EDIT3, m_left);
	DDX_Text(pDX, IDC_EDIT2, m_num);
	DDX_Text(pDX, IDC_EDIT4, m_sellinfor);
}

BEGIN_MESSAGE_MAP(CSellDlg, CFormView)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CSellDlg::OnCbnSelchangeCombo2)
//	ON_EN_CHANGE(IDC_EDIT1, &CSellDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CSellDlg 诊断

#ifdef _DEBUG
void CSellDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSellDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG






void CSellDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//初始化销售订单窗口
	CInfoFile file;

	//读取商品信息
	file.ReadDocLine();
	//将商品名称添加到下拉列表框中
    for(list<msg>::iterator it=file.ls.begin();it!=file.ls.end();it++)
	{
		m_ccbomo.AddString(CString(it->name.c_str()));
	}
	//设置默认选中项
	m_ccbomo.SetCurSel(0);
    // 更新第一个商品的信息
    OnCbnSelchangeCombo2();

}

void CSellDlg::OnCbnSelchangeCombo2()
{
    // 获取当前选中的索引
    int index = m_ccbomo.GetCurSel();
    if (index == CB_ERR) return;

    // 获取当前商品名称
    CString strName;
    m_ccbomo.GetLBText(index, strName);

    // 根据名称查找商品信息
    CInfoFile file;
    // file.ReadDocLine(); // 构造函数已经读取了
    
    // 使用传统迭代器替代 auto
    for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
    {
        if (strName == CString(it->name.c_str()))
        {
            m_price = it->price;
            m_left = it->age; // 库存
            UpdateData(FALSE); // 更新界面
            return;
        }
    }
}
