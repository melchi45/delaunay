// DelaunayDoc.cpp : implementation of the CDelaunayDoc class
//

#include "stdafx.h"
#include "Delaunay.h"

#include "DelaunayDoc.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDelaunayDoc

IMPLEMENT_DYNCREATE(CDelaunayDoc, CDocument)

BEGIN_MESSAGE_MAP(CDelaunayDoc, CDocument)
	//{{AFX_MSG_MAP(CDelaunayDoc)
	ON_COMMAND(ID_DELAUNAY_N2, OnDelaunayN2)
	ON_COMMAND(ID_DELAUNAY_N4, OnDelaunayN4)
	ON_COMMAND(ID_VORONOI, OnVoronoi)
	ON_UPDATE_COMMAND_UI(ID_VORONOI, OnUpdateVoronoi)
	ON_UPDATE_COMMAND_UI(ID_DELAUNAY_N2, OnUpdateDelaunayN2)
	ON_UPDATE_COMMAND_UI(ID_DELAUNAY_N4, OnUpdateDelaunayN4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDelaunayDoc construction/destruction

CDelaunayDoc::CDelaunayDoc()
{
	// TODO: add one-time construction code here
	b_voronoi = TRUE;
	b_dn2 = TRUE;
	b_dn4 = FALSE;
	b_random = TRUE;
}

CDelaunayDoc::~CDelaunayDoc()
{
}

BOOL CDelaunayDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDelaunayDoc serialization

void CDelaunayDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDelaunayDoc diagnostics

#ifdef _DEBUG
void CDelaunayDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDelaunayDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDelaunayDoc commands

bool CDelaunayDoc::nearnessCheck(const Node &d)
{
	Node node;

	for( int i = 0; i < data.GetCount() ; i++) 
	{
		POSITION pos = data.FindIndex(i);

		node = (Node) data.GetAt(pos);

		if ( sqrt ( (d.x - node.x)*(d.x - node.x) + (d.y - node.y)*(d.y - node.y) ) <= VERYNEAR)
			return true;
	}

	return false;
}

void CDelaunayDoc::OnDelaunayN2() 
{
	// TODO: Add your command handler code here
	b_dn2 = !b_dn2;
	UpdateAllViews(NULL);
}

void CDelaunayDoc::OnDelaunayN4() 
{
	// TODO: Add your command handler code here
	b_dn4 = !b_dn4;	
	UpdateAllViews(NULL);
}

void CDelaunayDoc::OnVoronoi() 
{
	// TODO: Add your command handler code here
	b_voronoi	 = !b_voronoi;	
	UpdateAllViews(NULL);
}

void CDelaunayDoc::OnUpdateVoronoi(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(b_voronoi);
}

void CDelaunayDoc::OnUpdateDelaunayN2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(b_dn2);
}

void CDelaunayDoc::OnUpdateDelaunayN4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(b_dn4);
}
