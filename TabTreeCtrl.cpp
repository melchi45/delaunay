// TabTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Delaunay.h"
#include "TabTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabTreeCtrl

CTabTreeCtrl::CTabTreeCtrl()
{
}

CTabTreeCtrl::~CTabTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CTabTreeCtrl, CXTTreeCtrl)
	//{{AFX_MSG_MAP(CTabTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabTreeCtrl message handlers

BOOL CTabTreeCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	if ( !CXTTreeCtrl::PreCreateWindow( cs ) )
		return FALSE;

	// Set the style for the tree control.
	cs.style |= TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_EDITLABELS;

	// If non-xp mode, add a client edge border to the tree.
	if (!xtAfxData.bXPMode)
	{
		cs.dwExStyle |= WS_EX_CLIENTEDGE;
	}

	// TODO: Add your specialized code here and/or call the base class
	
	return TRUE;
}

int CTabTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create the image list used by the tree control.
	if ( !m_imageList.Create( 16, 16, ILC_COLOR24 | ILC_MASK, 1, 1 ) )
	{
		TRACE0("Failed to create image list.\n");
		return -1;
	}

	// load the tree images bitmap and add it to the image list.
	m_bitmap.LoadBitmap( IDB_IMGLIST_VIEW );
	m_imageList.Add( &m_bitmap, RGB( 0x00,0xff,0x00 ) );

	// Set the image list for the tree control.
	SetImageList( &m_imageList, TVSIL_NORMAL );

	// Add the parent item
	HTREEITEM htItem = InsertItem( _T( "Tree item 1" ) );
	SetItemState( htItem, TVIS_BOLD, TVIS_BOLD );
	
	// Add children
	int i;
	for( i = 1; i < 8; i++ )
	{
		InsertItem( _T( "Tree item 2" ), 1, 1, htItem );
	}
	
	// Add children
	for ( i = 0; i < 5; ++i )
	{
		CString str;
		str.Format( _T( "Tree item %d" ), i+2 );
		InsertItem( str, 2, 3, htItem );
	}

	Expand( htItem, TVE_EXPAND );
	
	return 0;
}

void CTabTreeCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	// TODO: Add your control notification handler code here
	if ( pTVDispInfo->item.pszText )
	{
		SetItemText( pTVDispInfo->item.hItem,
			pTVDispInfo->item.pszText );
	}
	
	*pResult = 0;
}
