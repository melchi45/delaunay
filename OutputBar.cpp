// OutputBar.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Delaunay.h"
#include "OutputBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputBar::COutputBar()
{

}

COutputBar::~COutputBar()
{
	// TODO: add destruction code here.
}

IMPLEMENT_DYNAMIC(COutputBar, CXTDockWindow)

BEGIN_MESSAGE_MAP(COutputBar, CXTDockWindow)
	//{{AFX_MSG_MAP(COutputBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputBar message handlers

int COutputBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTDockWindow::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create  the flat tab control.
	if (!m_flatTabCtrl.Create(WS_CHILD|WS_VISIBLE|FTS_XT_DEFAULT|FTS_XT_HSCROLL,
		CRect(0,0,0,0), this, IDC_FLATTABCTRL))
	{
		TRACE0( "Failed to create flattab control\n" );
		return -1;
	}

	// Define the default style for the output list boxes.
	DWORD dwStyle = WS_CHILD | WS_VSCROLL | WS_TABSTOP | LBS_NOINTEGRALHEIGHT;

	// Create the sheet1 list box.
	if (!m_sheet1.Create( dwStyle, CRect(0,0,0,0), this, IDC_SHEET1 ))
	{
		TRACE0( "Failed to create sheet1.\n" );
		return -1;
	}

	// Create the sheet2 list box.
	if (!m_sheet2.Create( dwStyle, CRect(0,0,0,0), this, IDC_SHEET2 ))
	{
		TRACE0( "Failed to create sheet2.\n" );
		return -1;
	}

	// Insert tabs into the flat tab control.
	m_flatTabCtrl.InsertItem(0, _T("Sheet 1"), &m_sheet1);
	m_flatTabCtrl.InsertItem(1, _T("Sheet 2"), &m_sheet2);

	// Insert text into the list box.
	m_sheet1.AddString(_T("Sheet 1 Output..."));
	m_sheet2.AddString(_T("Sheet 2 Output..."));

	// Set the current tab.
	m_flatTabCtrl.SetCurSel(0);

	// Draw an edge around the control.
	SetXTBarStyle(CBRS_XT_DEFAULT|CBRS_XT_CLIENT_STATIC);

	// Associate the flat tab control with the docking window.
	SetChild(&m_flatTabCtrl);

	return 0;
}
