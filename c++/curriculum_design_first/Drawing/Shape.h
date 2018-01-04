#pragma once
#define M_PI 3.14159265358979323846
// CShape ����Ŀ��
enum ElementType { SQUARE, RECTANGLE, TRIANGLE, CIRCLE, ELLIPSE, TEXT };
class CShape : public CObject
{
public:
	CShape();
	virtual ~CShape();
	virtual void Draw(CDC*pDC) = 0;//����ͼԪ
	virtual bool IsMatched(CPoint pnt) = 0;//���Ƿ�����ͼ���ڲ�
	virtual void Serialize(CArchive& ar) = 0;//���л�
	void SetBase(ElementType T, COLORREF BorderC, int BorderT, int BorderW, COLORREF FillC, int FillT) {
		Type = T;
		BorderColor = BorderC;
		BorderType = BorderT;
		BorderWidth = BorderW;
		FillColor = FillC;
		FillType = FillT;
	};
	virtual void SetDate(int x,int y, int w, int h, int a) = 0;
	ElementType GetDate()
	{
		return Type;
	};
protected:
	ElementType Type;//ͼԪ����
	int OrgX;//ԭ������
	int OrgY;
	COLORREF BorderColor;//�߽���ɫ
	int BorderType;//�߽�����ʵ�ߡ����ߡ������ PS_SOLID PS_DASH   PS_DOT
	int BorderWidth;//�߽���
	COLORREF FillColor;//�����ɫ
	int FillType;//�����----ʵ�ġ�˫�Խǡ�ʮ�ֽ���  HS_DIAGCROSS  HS_CROSS HS_BDIAGONAL
};

class CSquare : public CShape
{
public:
	CSquare();
	CSquare(int x, int y, int w);
	void Draw(CDC*pDC);//����
	bool IsMatched(CPoint pnt);//���ص�pnt�Ƿ�����ͼԪ��
	virtual void Serialize(CArchive& ar);//���л�������ͼԪ
	void SetDate(int x, int y, int w, int h, int a);
private:
	int width;
	DECLARE_SERIAL(CSquare)//������CSquare֧�����л�
};

//����
class CRectangle : public CShape
{
public:
	CRectangle();
	CRectangle(int x, int y, int w ,int h);
	void Draw(CDC*pDC);//����
	bool IsMatched(CPoint pnt);//���ص�pnt�Ƿ�����ͼԪ��
	virtual void Serialize(CArchive& ar);//���л�������ͼԪ
	void SetDate(int x, int y, int w, int h, int a);
private:
	int width,height;
	DECLARE_SERIAL(CRectangle)//������CRectangle֧�����л�
};

//����
class CTriangle : public CShape
{
public:
	CTriangle();
	CTriangle(int x, int y, int w, int h);
	void Draw(CDC*pDC);//����
	bool IsMatched(CPoint pnt);//���ص�pnt�Ƿ�����ͼԪ��
	virtual void Serialize(CArchive& ar);//���л�������ͼԪ
	void SetDate(int x, int y, int w, int h, int a);
private:
	int width, height;
	DECLARE_SERIAL(CTriangle)//������CTriangle֧�����л�
};

//Բ
class CCircle : public CShape
{
public:
	CCircle();
	CCircle(int x, int y, int w);
	void Draw(CDC*pDC);//����
	bool IsMatched(CPoint pnt);//���ص�pnt�Ƿ�����ͼԪ��
	virtual void Serialize(CArchive& ar);//���л�������ͼԪ
	void SetDate(int x, int y, int w, int h, int a);
private:
	int width, height;
	DECLARE_SERIAL(CCircle)//������CTriangle֧�����л�
};

//��Բ
class CEllipse : public CShape
{
public:
	CEllipse();
	CEllipse(int x, int y, int w ,int h);
	void Draw(CDC*pDC);//����
	bool IsMatched(CPoint pnt);//���ص�pnt�Ƿ�����ͼԪ��
	virtual void Serialize(CArchive& ar);//���л�������ͼԪ
	void SetDate(int x, int y, int w, int h, int a);
private:
	int width, height;
	DECLARE_SERIAL(CEllipse)//������CTriangle֧�����л�
};

//����
class CText : public CShape
{
public:
	CText();
	CText(int x, int y, int w, int h, int a);
	void Draw(CDC*pDC);//����
	bool IsMatched(CPoint pnt);//���ص�pnt�Ƿ�����ͼԪ��
	virtual void Serialize(CArchive& ar);//���л�������ͼԪ
	void SetDate(int x, int y, int w, int h, int a);
private:
	int width, height, angle;
	DECLARE_SERIAL(CText)
};