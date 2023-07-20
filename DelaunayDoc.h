// DelaunayDoc.h : interface of the CDelaunayDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELAUNAYDOC_H__B55867FD_7528_4EC9_ACE0_2F7AA617375B__INCLUDED_)
#define AFX_DELAUNAYDOC_H__B55867FD_7528_4EC9_ACE0_2F7AA617375B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "macros.h"

#define VERYNEAR 5 //pixels

class CDelaunayDoc : public CDocument
{
protected: // create from serialization only
	CDelaunayDoc();
	DECLARE_DYNCREATE(CDelaunayDoc)

// Attributes
public:
	CList <Node, Node&> data;
	bool b_voronoi, b_dn2, b_dn4, b_random; //current display selection
	double xmin, xmax, ymin, ymax; //extenst of the input vertices

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDelaunayDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool nearnessCheck(const Node &d);
	virtual ~CDelaunayDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDelaunayDoc)
	afx_msg void OnDelaunayN2();
	afx_msg void OnDelaunayN4();
	afx_msg void OnVoronoi();
	afx_msg void OnUpdateVoronoi(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDelaunayN2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDelaunayN4(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELAUNAYDOC_H__B55867FD_7528_4EC9_ACE0_2F7AA617375B__INCLUDED_)
