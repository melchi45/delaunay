// DelaunayView.cpp : implementation of the CDelaunayView class
//

#include "stdafx.h"
#include "Delaunay.h"

#include "DelaunayDoc.h"
#include "DelaunayView.h"

#include "DelaunayN2.h"
#include "VoronoiAlg.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDelaunayView

IMPLEMENT_DYNCREATE(CDelaunayView, CView)

BEGIN_MESSAGE_MAP(CDelaunayView, CView)
	//{{AFX_MSG_MAP(CDelaunayView)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDelaunayView construction/destruction

CDelaunayView::CDelaunayView()
{
	// TODO: add construction code here

}

CDelaunayView::~CDelaunayView()
{
}

BOOL CDelaunayView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDelaunayView drawing

void CDelaunayView::OnDraw(CDC* pDC)
{
	CDelaunayDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	Node node;
	double rad=2;

    for( int i = 0; i < pDoc->data.GetCount(); i++) 
	{
		POSITION pos = pDoc->data.FindIndex(i);
		node = (Node) pDoc->data.GetAt(pos);

		CBrush myBrush(RGB(255,0,0)), *pOldBrush;
		pOldBrush = pDC->SelectObject(&myBrush);
		pDC->Ellipse(node.x - rad, node.y - rad, node.x + rad, node.y + rad);
		pDC->SelectObject(pOldBrush);
	}

	if(pDoc->data.GetCount() > 2)
    {
        if(pDoc->b_dn4)
            applyDToN4(pDC);
        else 
            applyDToN2(pDC);
    }
    else
        return;

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CDelaunayView printing

BOOL CDelaunayView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDelaunayView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDelaunayView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDelaunayView diagnostics

#ifdef _DEBUG
void CDelaunayView::AssertValid() const
{
	CView::AssertValid();
}

void CDelaunayView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDelaunayDoc* CDelaunayView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDelaunayDoc)));
	return (CDelaunayDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDelaunayView message handlers

void CDelaunayView::OnFilePrintPreview() 
{
	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.

	// must not create this on the frame.  Must outlive this function
	CPrintPreviewState* pState = new CPrintPreviewState;

	// DoPrintPreview's return value does not necessarily indicate that
	// Print preview succeeded or failed, but rather what actions are necessary
	// at this point.  If DoPrintPreview returns TRUE, it means that
	// OnEndPrintPreview will be (or has already been) called and the
	// pState structure will be/has been deleted.
	// If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
	// WILL NOT be called and that cleanup, including deleting pState
	// must be done here.

	if (!DoPrintPreview(XT_IDD_PREVIEW_TOOLBAR, this,
		RUNTIME_CLASS(CXTPreviewView), pState))
	{
		// In derived classes, reverse special window handling here for
		// Preview failure case
		TRACE0("Error: DoPrintPreview failed.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}
}

void CDelaunayView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(nFlags)
	{
		CDelaunayDoc* pDoc = GetDocument();
		
		Node *d;
		
		d = new Node;
		
		/* Input points and compute z = x^2 + y^2. */
		d->x = point.x;
		d->y = point.y;
		d->z = d->x * d->x + d->y * d->y;
		
		/* add if atleast VERYNEAR pixels distance away */
		if(!pDoc->nearnessCheck(*d)) 
		{ 
			
			pDoc->data.AddTail(*d);
			
			//check the extents
			if(pDoc->data.GetCount() == 1) 
			{
				pDoc->xmin = pDoc->ymin = d->x;
				pDoc->ymin = pDoc->ymax = d->y;
			}
			else
			{
				if(pDoc->xmin > d->x ) pDoc->xmin = d->x;
				if(pDoc->xmax < d->x ) pDoc->xmax = d->x;
				
				if(pDoc->ymin > d->y ) pDoc->ymin = d->y;
				if(pDoc->ymax < d->y ) pDoc->ymax = d->y;
			}
			
			CString str;
			str.Format("Num Points : %d", pDoc->data.GetCount());

			//sbar->message( str, 2000 );
		}
		else
			;//sbar->message( "Point very near! discarded", 2000 );
	}

	Invalidate();
	
	CView::OnLButtonDown(nFlags, point);
}

void CDelaunayView::applyDToN4(CDC *pDC)
{
	CDelaunayDoc* pDoc = GetDocument();

	Node node;
	double	*my_x,*my_y,*my_z;/* input points xy,z=x^2+y^2 */
	int	n;			/* number of input points */
	int	i, j, k, m;		/* indices of four points */
	double	xn, yn, zn;		/* outward vector normal to (i,j,k) */
	int	F = 0;	
    VoronoiAlg  vor;/* # of lower faces */

	/* Input points into tmp array*/
	n = pDoc->data.GetCount();

	if(n < 3) return;

	my_x = (double *)malloc (sizeof(double)*n);
	my_y = (double *)malloc (sizeof(double)*n);
	my_z = (double *)malloc (sizeof(double)*n);

	i = 0;

	POSITION pos = pDoc->data.GetHeadPosition();

    for( int iter = 0; iter < pDoc->data.GetCount(); iter++)
	{
		node = (Node) pDoc->data.GetNext(pos);

		my_x[i] = node.x;
		my_y[i] = node.y;
		my_z[i] = node.z;		
		i++;
	}

	/* For each triple (i,j,k) */
	for ( i = 0; i < n; i++ ) 
	{
		for ( j = i + 1; j < n; j++ ) 
		{
			for ( k = i + 1; k < n; k++ ) 
			{ 
				zn = (my_x[j]-my_x[i])*(my_y[k]-my_y[i]) - 
					(my_x[k]-my_x[i])*(my_y[j]-my_y[i]);
				
				if(zn > 0 || j == k)
					continue;
				
				// Compute normal to triangle (i,j,k).
				
				xn = (my_y[j]-my_y[i])*(my_z[k]-my_z[i]) - 
					(my_y[k]-my_y[i])*(my_z[j]-my_z[i]);
				yn = (my_x[k]-my_x[i])*(my_z[j]-my_z[i]) - 
					(my_x[j]-my_x[i])*(my_z[k]-my_z[i]);
				
				
				/* For each other point m */
				for (m = 0; m < n; m++)
				{
					if(m !=i && m != j && m != k &&
						((my_x[m]-my_x[i])*xn +
						(my_y[m]-my_y[i])*yn +
						(my_z[m]-my_z[i])*zn) > 0)
						break;
				}
				
                
                if (m == n) 
				{
                    //printf("z=%10d; lower face indices: %d, %d, %d\n", 
                    //zn, i, j, k);
					
                    if(pDoc->b_dn4) 
					{
						CPen myPen, *pOldPen;
						myPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));	
						pOldPen = (CPen*) pDC->SelectObject(&myPen);

                        pDC->MoveTo(my_x[i], my_y[i]);
                        pDC->LineTo(my_x[j], my_y[j]);
                        pDC->LineTo(my_x[k], my_y[k]);
                        pDC->LineTo(my_x[i], my_y[i]);

						pDC->SelectObject(pOldPen);
                    }
					
					POSITION pos1 = pDoc->data.FindIndex(i);
					POSITION pos2 = pDoc->data.FindIndex(j);
					POSITION pos3 = pDoc->data.FindIndex(k);

					if(pDoc->b_voronoi)
						vor.addVoronoiData(i,j,k,
						(Node) (pDoc->data.GetAt(pos1)), 
                        (Node) (pDoc->data.GetAt(pos2)), 
						(Node) (pDoc->data.GetAt(pos3)));
                    F++;
                }
            }
        }
    }

    if(pDoc->b_voronoi)
        vor.displayVoronoiDiagram(pDC);

	free(my_x);
	free(my_y);
	free(my_z);
//	QString str = "A total of "+ QString::number(F) + "Faces and " +QString::number(data.count()) +"vertices found!";

}

void CDelaunayView::applyDToN2(CDC *pDC)
{
	CDelaunayDoc* pDoc = GetDocument();

    DelaunayN2 dn2;
    Node node;

    if(pDoc->data.GetCount() < 4)
        return;

	POSITION pos = pDoc->data.GetHeadPosition();

    for( int i = 0; i < pDoc->data.GetCount(); i++)
	{
		node = (Node) pDoc->data.GetNext(pos);
		dn2.addVertex (node.x, node.y, node.z);
	}
   
    dn2.computeAndDisplayVoronoi(pDC, pDoc->b_dn2, pDoc->b_voronoi);
}
