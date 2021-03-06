
// PaintingView.h : interface of the CPaintingView class
//

#pragma once

#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <stack>
using namespace std;

//VALUES
#define PI					3.1415
#define MINS				10

//mode
#define MODE_SELECT			0
#define MODE_DRAWING		1

//pattern
#define PATTERN_DRAW		0
#define PATTERN_LINE		1
#define PATTERN_ROUND		2
#define PATTERN_OVAL		3
#define PATTERN_SQUARE1		4
#define PATTERN_SQUARE2		5
#define PATTERN_POLYGONAL	6
#define PATTERN_FILL		7

//2D
#define M_2D_TRANS			0
#define M_2D_ROTATE			1
#define M_2D_SCALE			2
#define M_2D_BRUSH			3

class Pattern
{
public:
	vector<CPoint> v;
	int pattern;
	bool visiable = true;

	COLORREF outColor;
	COLORREF inColor;

	Pattern() { v.clear(); pattern = 0; visiable = true; }
	Pattern(vector<CPoint> iv, int ipattern, COLORREF ioutColor, COLORREF iinColor)
	{
		v = iv;
		pattern = ipattern;
		outColor = ioutColor;
		inColor = iinColor;
		visiable = true;
	}
};

class CPaintingView : public CView
{
protected: // create from serialization only
	CPaintingView();
	DECLARE_DYNCREATE(CPaintingView)

// Attributes
public:
	CPaintingDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CPaintingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()


public:
	int m_pattern		= PATTERN_LINE;
	int m_mode			= MODE_DRAWING;
	int m_2d			= M_2D_TRANS;
	int m_selected		= 0;

	bool m_drawing		= false;
	bool m_pressed		= false;

	COLORREF m_inColor	= 0x000000;
	COLORREF m_outColor	= 0x000000;

	CPoint pre_point;
	CPoint m_c;

	vector<Pattern> V;
	vector<Pattern> pre_V;
	vector<Pattern> next_V;

	void scale2D(int, CPoint, double, double);
	void rotate2D(int, CPoint, double);

	void mFlush();
	void mMark(int);

//	afx_msg void OnPatternLine();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPatternLine();
	afx_msg void OnPatternDraw();
	afx_msg void OnPatternCircle();
	afx_msg void OnPatternOval();
	afx_msg void OnPatternSquare1();
	afx_msg void OnPatternSquare2();
	afx_msg void OnContralBack();
	afx_msg void OnContralFront();
	afx_msg void OnContralIncolor();
	afx_msg void OnContralOutcolor();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void On2dRotate();
	afx_msg void On2dScale();
	afx_msg void On2dTrans();
	afx_msg void OnContralBrush();
};

#ifndef _DEBUG  // debug version in PaintingView.cpp
inline CPaintingDoc* CPaintingView::GetDocument() const
   { return reinterpret_cast<CPaintingDoc*>(m_pDocument); }
#endif

