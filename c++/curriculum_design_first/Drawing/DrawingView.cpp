
// DrawingView.cpp : CDrawingView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Drawing.h"
#endif

#include "DrawingDoc.h"
#include "DrawingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Shape.h"
#include "Type.h"
// CDrawingView
#include "MainFrm.h"

CShape * cp = NULL, *crp = NULL;


IMPLEMENT_DYNCREATE(CDrawingView, CScrollView)

BEGIN_MESSAGE_MAP(CDrawingView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDrawingView::OnFilePrintPreview)
//	ON_WM_CONTEXTMENU()
//	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CDrawingView ����/����

CDrawingView::CDrawingView()
{
	// TODO: �ڴ˴���ӹ������

}

CDrawingView::~CDrawingView()
{
}

BOOL CDrawingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CDrawingView ����

void CDrawingView::OnDraw(CDC* pDC)
{
	CDrawingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 0;
	int mw,mh,ma; CString mt;

	for (int i = 0; i < pDoc->m_Elements.GetCount(); i++)
	{
		CShape* p = (CShape*)pDoc->m_Elements[i];
	
		if (p != nullptr)
		{
			p->Draw(pDC);
			p->GetDate(mw, mh, mt, ma);
			if (mw+p->OrgX > sizeTotal.cx) 
			{
				sizeTotal.cx = mw+ p->OrgX;
			}
			if (mh+p->OrgY > sizeTotal.cy)
			{
				sizeTotal.cy = mh+ p->OrgY;
			}
		}

	}
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDrawingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CDrawingView ��ӡ


void CDrawingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDrawingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDrawingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDrawingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CDrawingView ���

#ifdef _DEBUG
void CDrawingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDrawingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDrawingDoc* CDrawingView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawingDoc)));
	return (CDrawingDoc*)m_pDocument;
}
#endif //_DEBUG


// CDrawingView ��Ϣ�������


void CDrawingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDrawingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)	return;

	CClientDC dc(this);
	CPoint pntLogical = point;
	OnPrepareDC(&dc);
	dc.DPtoLP(&pntLogical);//DP->LP����ת��

	CType dlg;
	dlg.m_OrgX = pntLogical.x;
	dlg.m_OrgY = pntLogical.y;
	CShape* p=NULL;

	if ((nFlags&MK_CONTROL) == MK_CONTROL)//Ctrl������
	{
		if(dlg.DoModal()==IDOK)
		{
			//int index = /*((CDrawingApp *)AfxGetApp())->m_index*/ dlg.m_TYType.GetCurSel();
			int index = dlg.indexx;
			switch (index)
			{
			case 0:
				p = new CSquare(dlg.m_OrgX, dlg.m_OrgY, dlg.m_Width);
				p->SetBase(SQUARE, dlg.color_Line, dlg.m_intListType, dlg.m_LineWidt, dlg.color_Fill, dlg.m_intFillType);
				pDoc->m_Elements.Add(p);
				pDoc->SetModifiedFlag();
				pDoc->UpdateAllViews(NULL);
				break;
			case 1:
				p = new CRectangle(dlg.m_OrgX, dlg.m_OrgY, dlg.m_Width, dlg.m_Height);
				p->SetBase(RECTANGLE, dlg.color_Line, dlg.m_intListType, dlg.m_LineWidt, dlg.color_Fill, dlg.m_intFillType);
				pDoc->m_Elements.Add(p);
				pDoc->SetModifiedFlag();
				pDoc->UpdateAllViews(NULL);
				break;
			case 2:
				p = new CTriangle(dlg.m_OrgX, dlg.m_OrgY, dlg.m_Width, dlg.m_Height);
				p->SetBase(TRIANGLE, dlg.color_Line, dlg.m_intListType, dlg.m_LineWidt, dlg.color_Fill, dlg.m_intFillType);
				pDoc->m_Elements.Add(p);
				pDoc->SetModifiedFlag();
				pDoc->UpdateAllViews(NULL);
				break;
			case 3:
				p = new CCircle(dlg.m_OrgX, dlg.m_OrgY, dlg.m_Width);
				p->SetBase(CIRCLE, dlg.color_Line, dlg.m_intListType, dlg.m_LineWidt, dlg.color_Fill, dlg.m_intFillType);
				pDoc->m_Elements.Add(p);
				pDoc->SetModifiedFlag();
				pDoc->UpdateAllViews(NULL);
				break;
			case 4:
				p = new CEllipse(dlg.m_OrgX, dlg.m_OrgY, dlg.m_Width, dlg.m_Height);
				p->SetBase(ELLIPSE, dlg.color_Line, dlg.m_intListType, dlg.m_LineWidt, dlg.color_Fill, dlg.m_intFillType);
				pDoc->m_Elements.Add(p);
				pDoc->SetModifiedFlag();
				pDoc->UpdateAllViews(NULL);
				break;
			case 5:
				p = new CText(dlg.m_OrgX, dlg.m_OrgY, dlg.m_Width,dlg.m_Height, dlg.m_Angle, dlg.m_Text);
				p->SetBase(TEXT, dlg.color_Line, dlg.m_intListType, dlg.m_LineWidt, dlg.color_Fill, dlg.m_intFillType);
				pDoc->m_Elements.Add(p);
				pDoc->SetModifiedFlag();
				pDoc->UpdateAllViews(NULL);
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < (pDoc->m_Elements.GetSize()); i++)
		{
			CShape* pp = (CShape*)pDoc->m_Elements[i];

			if (pDoc->m_Elements[i] != NULL && pp->IsMatched(pntLogical))
			{
				p1.x = pntLogical.x, p1.y = pntLogical.y;
				cp = pp;
				break;
			}
		}

	isDown = 1;
	}

	CScrollView::OnLButtonDown(nFlags, point);
}


void CDrawingView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDrawingDoc* pDoc = GetDocument();
	int i;
	CShape* p;
	CClientDC dc(this);
	CPoint pntLogical = point;
	OnPrepareDC(&dc);
	dc.DPtoLP(&pntLogical);//DP->LP����ת��
	for (i = 0; i < pDoc->m_Elements.GetCount(); i++)
	{
		p = (CShape*)pDoc->m_Elements[i];
		if (p!=nullptr && p->IsMatched(pntLogical))
		{
			pDoc->m_Elements[i] = nullptr;
		}
	}
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL);

	CScrollView::OnRButtonDblClk(nFlags, point);
}


void CDrawingView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDrawingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)	return;

	CClientDC dc(this);
	CPoint pntLogical = point;
	OnPrepareDC(&dc);
	dc.DPtoLP(&pntLogical);//DP->LP����ת��

	CType dlg;
	dlg.m_OrgX = pntLogical.x;
	dlg.m_OrgY = pntLogical.y;
	CShape* p = NULL;

	for (int i = 0; i < pDoc->m_Elements.GetCount(); i++)
	{
		p = (CShape*)pDoc->m_Elements[i];
		if (p != nullptr && p->IsMatched(pntLogical))
		{
			dlg.indexx/*((CDrawingApp *)AfxGetApp())->m_index*/ = p->GetDate();
			p->GetBase(dlg.color_Line, dlg.m_intListType, dlg.m_LineWidt, dlg.color_Fill, dlg.m_intFillType);
			p->GetDate(dlg.m_Width, dlg.m_Height, dlg.m_Text, dlg.m_Angle);
			dlg.isOR = 1;
			if (dlg.DoModal() == IDOK) {
				p->SetBase(p->GetDate(), dlg.color_Line, dlg.m_intListType, dlg.m_LineWidt, dlg.color_Fill, dlg.m_intFillType);
				p->SetDate(dlg.m_OrgX, dlg.m_OrgY, dlg.m_Width, dlg.m_Height, dlg.m_Angle);
				//if (p->GetDate() == TEXT) { p->SetText(dlg.m_Text); }
				pDoc->SetModifiedFlag();
				pDoc->UpdateAllViews(NULL);
			}
		}
	}
	dlg.isOR = 0;

	CScrollView::OnLButtonDblClk(nFlags, point);
}


void CDrawingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	isDown = 0;

	CScrollView::OnLButtonUp(nFlags, point);
}


void CDrawingView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	isRDown = 1;

	CDrawingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)	return;

	CClientDC dc(this);
	CPoint pntLogical = point;
	OnPrepareDC(&dc);
	dc.DPtoLP(&pntLogical);

	for (int i = 0; i < (pDoc->m_Elements.GetSize()); i++)
	{
		CShape* pp = (CShape*)pDoc->m_Elements[i];

		if (pDoc->m_Elements[i] != NULL && pp->IsMatched(pntLogical))
		{
			crp = pp;
			break;
		}
	}

	CScrollView::OnRButtonDown(nFlags, point);
}


void CDrawingView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	isRDown = 0;


	CScrollView::OnRButtonUp(nFlags, point);
}


void CDrawingView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CClientDC dc(this);
	CPoint pntLogical = point;
	OnPrepareDC(&dc);
	dc.DPtoLP(&pntLogical);
	CString x, y;
	x.Format(_T("%d"), pntLogical.x);//����ת�ַ���  
	y.Format(_T("%d"), pntLogical.y);
	CString text = "���ص�:" + x + "," + +y;

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetPaneText(0, text);//�޸�ԭ�е�״̬���Ͼ���һ������Ϣ    
	//pFrame->m_wndStatusBar.SetPaneText(1, "�򿪳ɹ���");//�޸�ԭ�е�״̬����CAPһ������Ϣ  
	//pFrame->m_wndStatusBar.SetPaneText(2, "�򿪳ɹ���");//�޸�ԭ�е�״̬����NUMһ������Ϣ  
	//pFrame->m_wndStatusBar.SetPaneText(3, "�򿪳ɹ���");//�޸�ԭ�е�״̬����SCRLһ������Ϣ  

	if (isDown&&cp != NULL) {
		pFrame->m_wndStatusBar.SetPaneText(3, "down!");
		int mw, mh, ma; CString mt;

		CDrawingDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)	return;

		CClientDC dc(this);
		CPoint pntLogical = point;
		OnPrepareDC(&dc);
		dc.DPtoLP(&pntLogical);


		cp->GetDate(mw, mh, mt ,ma);
		int ww = fabs(pntLogical.x - cp->OrgX), hh = fabs(pntLogical.y - cp->OrgY), aa = ma; CString tt = mt;

		cp->SetDate(cp->OrgX, cp->OrgY,ww, hh, aa);
		pDoc->SetModifiedFlag();
		pDoc->UpdateAllViews(NULL);


	}
	else {
		pFrame->m_wndStatusBar.SetPaneText(3, "up!");
		cp = NULL;
		p1.x = 0;
	};

	if (isRDown&&crp != NULL)
	{
		pFrame->m_wndStatusBar.SetPaneText(3, "down!");
		CDrawingDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)	return;

		crp->OrgX = pntLogical.x, crp->OrgY = pntLogical.y;
		pDoc->SetModifiedFlag();
		pDoc->UpdateAllViews(NULL);
	}
	else {
		pFrame->m_wndStatusBar.SetPaneText(3, "up!");
		crp = NULL;
	}

	CScrollView::OnMouseMove(nFlags, point);
}
