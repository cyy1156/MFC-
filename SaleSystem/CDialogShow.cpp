// CDialogShow.cpp: 实现文件
//

#include "pch.h"
#include "SaleSystem.h"
#include "CDialogShow.h"
#include"CInfoFile.h"


// CDialogShow

IMPLEMENT_DYNCREATE(CDialogShow, CFormView)

CDialogShow::CDialogShow()
	: CFormView(IDD_DIALOG_Show)
{

}

CDialogShow::~CDialogShow()
{
}

void CDialogShow::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CDialogShow, CFormView)
END_MESSAGE_MAP()


// CDialogShow 诊断

#ifdef _DEBUG
void CDialogShow::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDialogShow::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDialogShow 消息处理程序

void CDialogShow::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	// TODO: 在此添加额外的初始化代码
	//ID|Name|Price|Stock|Sales
	CString str[] = { TEXT("ID"),TEXT("名字"), TEXT("价格"),
		TEXT("库存") ,TEXT("销量")};
	for (int a = 0; a < 5; a++)
	{
		m_list.InsertColumn(a, str[a], LVCFMT_LEFT, 80);
	}
	//m_list.InsertItem(0, TEXT("张三"));
	// 设置子项
	//m_list.SetItemText(0, 1, TEXT("男"));
	//m_list.SetItemText(0, 2, TEXT("20"));
	
	/*for (int i = 0; i < 10; i++)
	

		CString name;
		name.Format(TEXT("张三%d"), i + 1);
		m_list.InsertItem(i, name);
		if (i % 2 == 0)
			m_list.SetItemText(i, 1, TEXT("男"));
		else
			m_list.SetItemText(i, 1, TEXT("女"));
		CString age;
		age.Format(TEXT("%d"), 20 + i);
		m_list.SetItemText(i, 2, age);*/
	 CInfoFile file;
	 int id;
	 CString name;
	 int price;
	 int num;   // 库存
	 int count; // 销量
	 for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	 {
		 id = it->id;
		 name = it->name.c_str();
		 price = it->price;
		 num = it->num;
		 count = it->count;
		 int index = m_list.InsertItem(m_list.GetItemCount(), CString(std::to_string(id).c_str()));
		 m_list.SetItemText(index, 1, name);
		 m_list.SetItemText(index, 2, CString(std::to_string(price).c_str()));
		 m_list.SetItemText(index, 3, CString(std::to_string(num).c_str()));
		 m_list.SetItemText(index, 4, CString(std::to_string(count).c_str()));
	 }
	
	
	// 设置列表视图风格
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
}

