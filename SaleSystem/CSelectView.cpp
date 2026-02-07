// CSelectView.cpp: 实现文件
//

#include "pch.h"
#include "SaleSystem.h"
#include "CSelectView.h"
#include "MainFrm.h"


// CSelectView

IMPLEMENT_DYNCREATE(CSelectView, CTreeView)

CSelectView::CSelectView()
{

}

CSelectView::~CSelectView()
{
}

BEGIN_MESSAGE_MAP(CSelectView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CSelectView::OnTvnSelchanged)
END_MESSAGE_MAP()


// CSelectView 诊断

#ifdef _DEBUG
void CSelectView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CSelectView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSelectView 消息处理程序

void CSelectView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	//初始化 树视图
	//获取树控件指针	
	m_treeCtrl = &GetTreeCtrl();
	HICON icons[5];
	icons[0] = AfxGetApp()->LoadIconW(IDI_ICON_Single1);
	icons[1] = AfxGetApp()->LoadIconW(IDI_ICON_Single2);
	icons[2] = AfxGetApp()->LoadIconW(IDI_ICON_Single3);
	icons[3] = AfxGetApp()->LoadIconW(IDI_ICON_Single4);
	icons[4] = AfxGetApp()->LoadIconW(IDI_ICON_Single5);
	//1.准备图片集合
	m_imageList.Create(32, 32, ILC_COLOR32 , 5, 1);
	for (int i = 0; i < 5; i++)
	{
		if (icons[i] != NULL)
		{
			m_imageList.Add(icons[i]);
		}
	}
	m_treeCtrl->SetImageList(&m_imageList, TVSIL_NORMAL);

	//2.添加节点
	m_treeCtrl->InsertItem(_T("个人信息"), 1,1 ,NULL);
	m_treeCtrl->InsertItem(_T("销售管理"), 2, 2, NULL);
	m_treeCtrl->InsertItem(_T("帅哥的信息"), 3, 3, NULL);
	m_treeCtrl->InsertItem(_T("帅哥添加"), 0, 0, NULL);
	m_treeCtrl->InsertItem(_T("帅哥删除"), 4, 4, NULL);



	// TODO: 在此添加专用代码和/或调用基类
}

void CSelectView::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//获取选中节点的项
	HTREEITEM item = m_treeCtrl->GetSelectedItem();
	//获取节点文本
	CString str = m_treeCtrl->GetItemText(item);
	//MessageBox(str);

	if (str == _T("个人信息"))
	{
		//需要包含头文件#include“MainFrm.h”
		//Cwnd::PostMessage将一个消息放入主窗口的消息队列中
		//AfxGetMainWnd()获取主窗口指针
		//AfxGetMainWnd()->GetSafeHwnd()获取主窗口句柄,CWnd::GetSafeHwnd()获取窗口句柄
		//NM_A,发送自定义消息
		//(WPARAM)NM_A,指定了附件信息
		//(LPARAM)0,指定了附加信息,此参数这里没有意义
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_A, (WPARAM)NM_A, (LPARAM)0);
	}
	else if (str == _T("销售管理"))
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_B, (WPARAM)NM_B, (LPARAM)0);
	}
	else if (str == _T("帅哥的信息"))
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_C, (WPARAM)NM_C, (LPARAM)0);
	}
	else if (str == _T("帅哥添加"))
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_D, (WPARAM)NM_D, (LPARAM)0);
	}
	else if (str == _T("帅哥删除"))
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_E, (WPARAM)NM_E, (LPARAM)0);
	}

}
