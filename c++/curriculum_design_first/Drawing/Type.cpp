// Type.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Drawing.h"
#include "Type.h"
#include "afxdialogex.h"
#include "Shape.h"


// CType �Ի���

IMPLEMENT_DYNAMIC(CType, CDialogEx)

CType::CType(CWnd* pParent /*=NULL*/)
	: CDialogEx(CType::IDD, pParent)
{
	m_Angle = 30;
	m_Height = 100;
	m_Width = 200;
	m_LineWidt = 1;
	m_OrgX = 200;
	m_OrgY = 200;
	m_strTYType = _T("");
	m_intListType = 0;
	m_intFillType = 0;
	color_Fill = RGB(205, 20, 0);  //color
	color_Line = RGB(255, 0, 0);  //
}

CType::~CType()
{
}

void CType::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Angele, m_Angle);
	DDV_MinMaxInt(pDX, m_Angle, 0, 360);
	DDX_Text(pDX, IDC_Height, m_Height);
	DDV_MinMaxInt(pDX, m_Height, 0, 999999);
	DDX_Text(pDX, IDC_LineWidth, m_LineWidt);
	DDV_MinMaxInt(pDX, m_LineWidt, 0, 999999);
	DDX_Text(pDX, IDC_OrgX, m_OrgX);
	DDV_MinMaxInt(pDX, m_OrgX, 0, 999999);
	DDX_Text(pDX, IDC_OrgY, m_OrgY);
	DDV_MinMaxInt(pDX, m_OrgY, 0, 999999);
	DDX_Text(pDX, IDC_Width, m_Width);
	DDV_MinMaxInt(pDX, m_Width, 0, 9999999);
	DDX_Control(pDX, IDC_TYType, m_TYType);
	DDX_Control(pDX, IDC_FillType, m_FillType);
	DDX_Control(pDX, IDC_LineType, m_LineType);
	DDX_CBString(pDX, IDC_TYType, m_strTYType);
	DDX_LBIndex(pDX, IDC_LineType, m_intListType);
	DDX_LBIndex(pDX, IDC_FillType, m_intFillType);
}


BEGIN_MESSAGE_MAP(CType, CDialogEx)
	ON_BN_CLICKED(IDOK, &CType::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_TYType, &CType::OnCbnSelchangeTytype)
	ON_BN_CLICKED(IDC_LineColorBUTTON, &CType::OnBnClickedLinecolorbutton)
	ON_BN_CLICKED(IDC_FillColorBUTTON, &CType::OnBnClickedFillcolorbutton)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CType ��Ϣ�������
char *coure_Type[]={
	"������","����","������", "Բ", "��Բ","����"
};
char *coure_Line[]={
	  "PS_SOLID", "PS_DASH", "PS_DOT"
};
char *coure_Fill[]={
	"HS_DIAGCROSS", "HS_CROSS", "HS_BDIAGONAL"
};

BOOL CType::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for (int i=0;i<6;i++)
	{
		m_TYType.AddString(coure_Type[i]);
	}
	int ind = ((CDrawingApp *)AfxGetApp())->m_index;
	m_TYType.SetCurSel(ind);
	if(ind==5)
		GetDlgItem(IDC_Angele)->ShowWindow(TRUE),GetDlgItem(IDC_STATIC00)->ShowWindow(TRUE);
	for (int i=0;i<3;i++)
	{
		m_LineType.AddString(coure_Line[i]);
		m_FillType.AddString(coure_Fill[i]);
	}
	m_LineType.SetCurSel(0);
	m_FillType.SetCurSel(0);
	((CDrawingApp *)AfxGetApp())->m_index = m_TYType.GetCurSel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CType::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	CDialogEx::OnOK();
}


void CType::OnCbnSelchangeTytype()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int isSel=((CDrawingApp *)AfxGetApp())->m_index = m_TYType.GetCurSel();
	switch (isSel)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		GetDlgItem(IDC_Angele)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC00)->ShowWindow(FALSE);
		break;
	case 5:
		GetDlgItem(IDC_Angele)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC00)->ShowWindow(TRUE);
		break;
	}
}


void CType::OnBnClickedLinecolorbutton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	color_Line = RGB(255, 0, 0);//��ʼ��
	CColorDialog colorDlg(color_Line);
	if (IDOK == colorDlg.DoModal())
	{
		color_Line = colorDlg.GetColor();
		Invalidate(TRUE);
	}
}


void CType::OnBnClickedFillcolorbutton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CColorDialog colorDlg(color_Fill);
	if (IDOK == colorDlg.DoModal())
	{
		color_Fill = colorDlg.GetColor();
		Invalidate(TRUE);
	}
}


HBRUSH CType::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_LineColor)//����Ǳ༭��
	{
		pDC->SetBkColor(color_Line);//�������屳����ɫ
	}
	if (pWnd->GetDlgCtrlID() == IDC_FillColor)//����Ǳ༭��
	{
		pDC->SetBkColor(color_Fill);//�������屳����ɫ
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

