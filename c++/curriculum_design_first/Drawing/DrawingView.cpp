
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

IMPLEMENT_DYNCREATE(CDrawingView, CScrollView)

BEGIN_MESSAGE_MAP(CDrawingView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDrawingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
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
	for (int i = 0; i < pDoc->m_Elements.GetCount(); i++)
	{
		CShape* p = (CShape*)pDoc->m_Elements[i];
	
		if (p != nullptr)
			p->Draw(pDC);
	}

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

void CDrawingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CDrawingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
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
			int index = ((CDrawingApp *)AfxGetApp())->m_index;
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
				p = new CText(dlg.m_OrgX, dlg.m_OrgY, dlg.m_Width,dlg.m_Height, dlg.m_Angle);
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
		for ( int i = 0; i < pDoc->m_Elements.GetCount(); i++)
		{
			p = (CShape*)pDoc->m_Elements[i];
			if (p != nullptr && p->IsMatched(pntLogical))
			{
				((CDrawingApp *)AfxGetApp())->m_index = p->GetDate();
				if (dlg.DoModal() == IDOK) {
					p->SetBase(p->GetDate(), dlg.color_Line, dlg.m_intListType, dlg.m_LineWidt, dlg.color_Fill, dlg.m_intFillType);
					p->SetDate(dlg.m_OrgX, dlg.m_OrgY, dlg.m_Width, dlg.m_Height, dlg.m_Angle);
					pDoc->SetModifiedFlag();
					pDoc->UpdateAllViews(NULL);
				}
			}
		}
		pDoc->SetModifiedFlag();
		pDoc->UpdateAllViews(NULL);
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
