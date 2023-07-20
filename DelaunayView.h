// DelaunayView.h : interface of the CDelaunayView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELAUNAYVIEW_H__BF642379_A5EE_4A83_9A3E_4BE0593A4B67__INCLUDED_)
#define AFX_DELAUNAYVIEW_H__BF642379_A5EE_4A83_9A3E_4BE0593A4B67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDelaunayView : public CView
{
protected: // create from serialization only
	CDelaunayView();
	DECLARE_DYNCREATE(CDelaunayView)

// Attributes
public:
	CDelaunayDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDelaunayView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void applyDToN2(CDC *pDC);
	void applyDToN4(CDC *pDC);
	virtual ~CDelaunayView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDelaunayView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DelaunayView.cpp
inline CDelaunayDoc* CDelaunayView::GetDocument()
   { return (CDelaunayDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELAUNAYVIEW_H__BF642379_A5EE_4A83_9A3E_4BE0593A4B67__INCLUDED_)
