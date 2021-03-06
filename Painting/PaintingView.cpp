
// PaintingView.cpp : implementation of the CPaintingView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Painting.h"
#endif

#include "PaintingDoc.h"
#include "PaintingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPaintingView

IMPLEMENT_DYNCREATE(CPaintingView, CView)

BEGIN_MESSAGE_MAP(CPaintingView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_COMMAND(PATTERN_LINE, &CPaintingView::OnPatternLine)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_PATTERN_LINE, &CPaintingView::OnPatternLine)
	ON_COMMAND(ID_PATTERN_DRAW, &CPaintingView::OnPatternDraw)
	ON_COMMAND(ID_PATTERN_CIRCLE, &CPaintingView::OnPatternCircle)
	ON_COMMAND(ID_PATTERN_OVAL, &CPaintingView::OnPatternOval)
	ON_COMMAND(ID_PATTERN_SQUARE1, &CPaintingView::OnPatternSquare1)
	ON_COMMAND(ID_PATTERN_SQUARE2, &CPaintingView::OnPatternSquare2)
	ON_COMMAND(ID_CONTRAL_BACK, &CPaintingView::OnContralBack)
	ON_COMMAND(ID_CONTRAL_FRONT, &CPaintingView::OnContralFront)
	ON_COMMAND(ID_CONTRAL_INCOLOR, &CPaintingView::OnContralIncolor)
	ON_COMMAND(ID_CONTRAL_OUTCOLOR, &CPaintingView::OnContralOutcolor)
//	ON_WM_ERASEBKGND()
ON_COMMAND(ID_2D_ROTATE, &CPaintingView::On2dRotate)
ON_COMMAND(ID_2D_SCALE, &CPaintingView::On2dScale)
ON_COMMAND(ID_2D_TRANS, &CPaintingView::On2dTrans)
ON_COMMAND(ID_CONTRAL_BRUSH, &CPaintingView::OnContralBrush)
END_MESSAGE_MAP()

// CPaintingView construction/destruction

CPaintingView::CPaintingView()
{
	// TODO: add construction code here

}

CPaintingView::~CPaintingView()
{
}

BOOL CPaintingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPaintingView drawing

void CPaintingView::OnDraw(CDC* /*pDC*/)
{
	CPaintingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	V.clear();
	pre_V.clear();
	next_V.clear(); 
}


// CPaintingView printing

BOOL CPaintingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPaintingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPaintingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

// CPaintingView diagnostics

#ifdef _DEBUG
void CPaintingView::AssertValid() const
{
	CView::AssertValid();
}

void CPaintingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPaintingDoc* CPaintingView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPaintingDoc)));
	return (CPaintingDoc*)m_pDocument;
}
#endif //_DEBUG

//My Functions

double distance(CPoint &a, CPoint &b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void full(CDC *p, CPoint point, COLORREF o_clr, COLORREF clr)
{
	bool vis[1395][663];
	memset(vis, false, sizeof(vis));
	stack<CPoint> s;
	s.push(point);
	vis[point.x][point.y] = true;
	while (!s.empty())
	{
		CPoint _ = s.top();
		s.pop();

		p->SetPixel(_, clr);

		int dx[] = { -1, 0, 0, 1, -1, -1, 1, 1 };
		int dy[] = { 0, -1, 1, 0, -1, 1, -1, 1 };

		for (int i = 0; i < 8; i++)
		{
			CPoint __(CPoint(_.x + dx[i], _.y + dy[i]));
			if (p->GetPixel(__) == o_clr && !vis[__.x][__.y])
				s.push(__), vis[__.x][__.y] = true;
		}
	}
}

void CPaintingView::mMark(int x)
{	
	cout << "mark" << endl;
	CDC *pDC = GetDC();
	CPen pen(PS_DOT, 5, COLORREF(0xff0000));
	pDC->SelectObject(pen);
	pDC->SelectStockObject(NULL_BRUSH);
	
	for (vector<CPoint>::iterator i = V[x].v.begin(); i != V[x].v.end(); i++)
		pDC->Ellipse(i->x - 2, i->y - 2, i->x + 2, i->y + 2);
}


void CPaintingView::mFlush()
{
	cout << "Flush" << endl;
	CDC *pDC = GetDC();

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(0, 0, rect.Width(), rect.Height(), COLORREF(0xffffff));

	for (vector<Pattern>::iterator i = V.begin(); i != V.end(); i++)
	{
		if (i->visiable == false)
			continue;

		CPen pen(PS_DOT, 2, i->outColor);
		pDC->SelectObject(pen);
		pDC->SelectStockObject(NULL_BRUSH);

		if (i->pattern == PATTERN_DRAW || i->pattern == PATTERN_LINE)
		{
			for (vector<CPoint>::iterator r = i->v.begin() + 1; r != i->v.end(); r++)
			{
				pDC->MoveTo(*(r - 1));
				pDC->LineTo(*r);
			}
		}
		else if (i->pattern == PATTERN_CIRCLE)
		{
			int R = min(i->v[0].x - i->v[1].x, i->v[0].y - i->v[1].y);
			pDC->Ellipse(i->v[0].x, i->v[0].y, i->v[0].x - R, i->v[0].y - R);
			CPoint p((i->v[0].x + i->v[1].x) / 2, (i->v[0].y + i->v[1].y) / 2);
			//full(pDC, p, pDC->GetPixel(p), m_inColor);
		}

		else if (i->pattern == PATTERN_OVAL)
		{
			pDC->Ellipse(i->v[0].x, i->v[0].y, i->v[1].x, i->v[1].y);
			//full(pDC, CPoint((i->v[0].x + i->v[1].x) / 2, (i->v[0].y + i->v[1].y) / 2), m_outColor, m_inColor);
		}

		else if (i->pattern == PATTERN_SQUARE1)
		{
			int R = min(i->v[0].x - i->v[1].x, i->v[0].y - i->v[1].y);
			pDC->Rectangle(i->v[0].x, i->v[0].y, i->v[0].x - R, i->v[0].y - R);
			CPoint p((i->v[0].x + i->v[1].x) / 2, (i->v[0].y + i->v[1].y) / 2);
			//full(pDC, p, pDC->GetPixel(p), m_inColor);

		}
		else if (i->pattern == PATTERN_SQUARE2)
		{
			pDC->Rectangle(i->v[0].x, i->v[0].y, i->v[1].x, i->v[1].y);
			//full(pDC, CPoint((i->v[0].x + i->v[1].x) / 2, (i->v[0].y + i->v[1].y) / 2), m_outColor, m_inColor);
		}
	}
}

void CPaintingView::scale2D(int x, CPoint c, double px, double py)
{
	for (vector<CPoint>::iterator i = V[x].v.begin(); i != V[x].v.end(); i++)
	{
		int x = i->x, y = i->y;
		i->x = x * px + c.x * (1 - px);
		i->y = y * py + c.y * (1 - py);

		cout << "(" << x << ", " << y << ") -> (" << i->x << ", " << i->y << ")" << endl;
	}
}

void CPaintingView::rotate2D(int x, CPoint c, double alpha)
{
	double theta = alpha / 180 * PI;

	for (vector<CPoint>::iterator i = V[x].v.begin(); i != V[x].v.end(); i++)
	{
		int x = i->x, y = i->y;
		i->x = x * cos(theta) - y * sin(theta) + c.x - c.x * cos(theta) + c.y * sin(theta);
		i->y = x * sin(theta) + y * cos(theta) + c.y - c.y * cos(theta) - c.x * sin(theta);
	}
}

// CPaintingView message handlers


void CPaintingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pressed = true;

	if (m_mode == MODE_DRAWING) 
	{		
		m_drawing = true;
		pre_point = point;
	}
	else if (m_mode == MODE_SELECT)
	{
		for (int i = 0; i < V.size(); i++)
		{
			cout << "finding" << endl;
			bool selected = false;
			
			if (V[i].visiable = false)
				continue;

			if (V[i].pattern == PATTERN_LINE)
			{
				cout << distance(V[i].v[0], V[i].v[1]) << ", " << distance(V[i].v[0], point) + distance(V[i].v[1], point) << " ," << MINS * 100 << endl;
				
				if (abs(distance(V[i].v[0], V[i].v[1]) - distance(V[i].v[0], point) - distance(V[i].v[1], point)) <= MINS)
				{
					m_selected = i;
					mMark(m_selected);
					selected = true;
					cout << "selected->" << m_selected << endl;
					break;
				}
			}

			else if (V[i].pattern == PATTERN_DRAW)
			{
				for(vector<CPoint>::iterator r = V[i].v.begin(); r != V[i].v.end(); r++)
					if (distance(*r, point) <= MINS)
					{
						m_selected = i;
						mMark(m_selected);
						selected = true;
						cout << "selected->" << m_selected << endl;
						break;
					}
			}
			else if (V[i].pattern == PATTERN_CIRCLE || V[i].pattern == PATTERN_OVAL || V[i].pattern == PATTERN_SQUARE1 || V[i].pattern == PATTERN_SQUARE2)
			{
				if ((V[i].v[0].x - point.x) * (V[i].v[1].x - point.x) <= 0 && (V[i].v[0].y - point.y) * (V[i].v[1].y - point.y) <= 0)
				{
					m_selected = i;
					mMark(m_selected);
					selected = true;
					cout << "selected->" << m_selected << endl;
					break;
				}
			}


			if (selected) break;
		}
			
		if (m_2d == M_2D_TRANS)
		{
			pre_point = point;
		}
		else if (m_2d == M_2D_ROTATE)
		{
			m_c = point;
			pre_point = point;
		}
		else if (m_2d == M_2D_SCALE)
		{
			m_c = point;
			pre_point = point;
		}
		else if (m_2d == M_2D_BRUSH)
		{
			V[m_selected].visiable = false;
			cout << "deleted" << endl;
			mFlush();
		}
	}

	CView::OnLButtonDown(nFlags, point);
}


void CPaintingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pressed = false;
	if (m_mode == MODE_DRAWING)
	{
		pre_V = V;
		Pattern pattern;
		pattern.pattern = m_pattern;
		pattern.inColor = m_inColor;
		pattern.outColor = m_outColor;
		pattern.visiable = true;

		if (m_pattern == PATTERN_LINE)
		{
			pattern.v.push_back(pre_point);
			pattern.v.push_back(point);
			V.push_back(pattern);
		}
		else if (m_pattern == PATTERN_CIRCLE || m_pattern == PATTERN_OVAL
				|| m_pattern == PATTERN_SQUARE1 || m_pattern == PATTERN_SQUARE2)
		{
			pattern.v.push_back(pre_point);
			pattern.v.push_back(point);
			V.push_back(pattern);
		}
		
		mFlush();
	}
	CView::OnLButtonUp(nFlags, point);
}


void CPaintingView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pressed && m_mode == MODE_DRAWING)
	{
		pre_V = V;
		Pattern pattern;
		pattern.inColor = m_inColor;
		pattern.outColor = m_outColor;

		if (m_pattern == PATTERN_DRAW)
		{
			pattern.pattern = PATTERN_DRAW;
			if (V.empty() || V.back().pattern != PATTERN_DRAW || (!V.back().v.empty() && distance(V.back().v.back(), point) >= MINS))
				V.push_back(pattern);
			V.back().v.push_back(point);
			mFlush();
		}
		
	}
	else if (m_mode == MODE_SELECT)
	{
		if (m_2d == M_2D_TRANS && m_pressed)
		{
			cout << "trans" << m_selected << endl;
			for (vector<CPoint>::iterator i = V[m_selected].v.begin(); i != V[m_selected].v.end(); i++)
			{
				i->x += point.x - pre_point.x; 
				i->y += point.y - pre_point.y;
			}
			pre_point = point;
			mFlush();
		}
		else if (m_2d == M_2D_ROTATE && m_pressed)
		{
			double px = 1.0, py = 1.0;
			if (point.x <= pre_point.x && point.y >= pre_point.y)
				rotate2D(m_selected, m_c, 3);
			else 
				rotate2D(m_selected, m_c, -3);

			pre_point = point;
			mFlush();
		}
		else if (m_2d == M_2D_SCALE && m_pressed)
		{
			double px = 1.0, py = 1.0;
			if (point.x < pre_point.x)
				px = 1.01;
			else if (point.x > pre_point.x && px > 0)
				px = 0.99;

			if (point.y < pre_point.y)
				py = 1.01;
			else if (point.y > pre_point.y && py > 0)
				py = 0.99;

			scale2D(m_selected, m_c, px, py);

			pre_point = point;
			mFlush();
		}
	}

	CView::OnMouseMove(nFlags, point);
}

void CPaintingView::OnPatternLine()
{
	// TODO: Add your command handler code here
	m_mode = MODE_DRAWING;
	m_pattern = PATTERN_LINE;
}


void CPaintingView::OnPatternDraw()
{
	// TODO: Add your command handler code here
	m_mode = MODE_DRAWING;
	m_pattern = PATTERN_DRAW;
}


void CPaintingView::OnPatternCircle()
{
	// TODO: Add your command handler code here
	m_mode = MODE_DRAWING;
	m_pattern = PATTERN_CIRCLE;
}


void CPaintingView::OnPatternOval()
{
	// TODO: Add your command handler code here
	m_mode = MODE_DRAWING;
	m_pattern = PATTERN_OVAL;
}


void CPaintingView::OnPatternSquare1()
{
	// TODO: Add your command handler code here
	m_mode = MODE_DRAWING;
	m_pattern = PATTERN_SQUARE1;
}


void CPaintingView::OnPatternSquare2()
{
	// TODO: Add your command handler code here
	m_mode = MODE_DRAWING;
	m_pattern = PATTERN_SQUARE2;
}


void CPaintingView::OnContralBack()
{
	// TODO: Add your command handler code here

	if (!pre_V.empty())
		next_V = V, V = pre_V;
	mFlush();
}


void CPaintingView::OnContralFront()
{
	// TODO: Add your command handler code here
	if (!pre_V.empty())
		pre_V = V, V = next_V;
	mFlush();
}


void CPaintingView::OnContralIncolor()
{
	// TODO: Add your command handler code here
	CColorDialog m_setClrDlg;
	m_setClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	m_setClrDlg.m_cc.rgbResult = m_inColor;
	if (IDOK == m_setClrDlg.DoModal())
	{
		m_inColor = m_setClrDlg.m_cc.rgbResult;
	}
	
	V[m_selected].inColor = m_inColor;
}


void CPaintingView::OnContralOutcolor()
{
	// TODO: Add your command handler code here
	CColorDialog m_setClrDlg;
	m_setClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	m_setClrDlg.m_cc.rgbResult = m_outColor;
	if (IDOK == m_setClrDlg.DoModal())
	{
		m_outColor = m_setClrDlg.m_cc.rgbResult;
	}
	V[m_selected].outColor = m_outColor;
	mFlush();
}



void CPaintingView::On2dRotate()
{
	// TODO: Add your command handler code here
	m_mode = MODE_SELECT;
	m_2d = M_2D_ROTATE;
}


void CPaintingView::On2dScale()
{
	// TODO: Add your command handler code here
	m_mode = MODE_SELECT;
	m_2d = M_2D_SCALE;
}


void CPaintingView::On2dTrans()
{
	// TODO: Add your command handler code here
	m_mode = MODE_SELECT;
	m_2d = M_2D_TRANS;
}


void CPaintingView::OnContralBrush()
{
	// TODO: Add your command handler code here
	m_mode = MODE_SELECT;
	m_2d = M_2D_BRUSH;
}
