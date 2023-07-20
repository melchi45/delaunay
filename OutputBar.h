// OutputBar.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTBAR_H__BE8D972F_9240_4D91_8058_EF1152815AC9__INCLUDED_)
#define AFX_OUTPUTBAR_H__BE8D972F_9240_4D91_8058_EF1152815AC9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// COutputBar dock window class

class COutputBar : public CXTDockWindow
{
	DECLARE_DYNAMIC(COutputBar)

// Construction / destruction
public:
	COutputBar();
	virtual ~COutputBar();

// Attributes
protected:

	CXTFlatTabCtrl	m_flatTabCtrl;
	CXTListBox		m_sheet1;
	CXTListBox		m_sheet2;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputBar)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTBAR_H__BE8D972F_9240_4D91_8058_EF1152815AC9__INCLUDED_)
