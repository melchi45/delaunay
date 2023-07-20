#if !defined(AFX_TABTREECTRL_H__FD87CA66_AB6F_4CA4_9329_E1E0E1F53CAA__INCLUDED_)
#define AFX_TABTREECTRL_H__FD87CA66_AB6F_4CA4_9329_E1E0E1F53CAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabTreeCtrl window

class CTabTreeCtrl : public CXTTreeCtrl
{
// Construction
public:
	CTabTreeCtrl();

// Attributes
protected:
	CImageList	m_imageList;
	CBitmap     m_bitmap;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabTreeCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABTREECTRL_H__FD87CA66_AB6F_4CA4_9329_E1E0E1F53CAA__INCLUDED_)
