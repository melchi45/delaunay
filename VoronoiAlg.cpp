// VoronoiAlg.cpp: implementation of the VoronoiAlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VoronoiAlg.h"
#include <math.h>
#include <stdio.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VoronoiAlg::VoronoiAlg()
{

}

VoronoiAlg::~VoronoiAlg()
{

}

void VoronoiAlg::addVoronoiData(int v1,int v2,int v3, 
                                const Node &p1, const Node &p2, const Node &p3)
{
	int x, y;
    bool found = false;
    
    double dx2 = p2.x - p1.x;
    double dy2 = p2.y - p1.y;
    double dr2 = dx2 * dx2 + dy2 * dy2;
    double dx3 = p3.x - p1.x;
    double dy3 = p3.y - p1.y;
    double dr3 = dx3 * dx3 + dy3 * dy3;
    double A = 2 * (dx2 * dy3 - dx3 * dy2);
    double dx = (dr2 * dy3 - dr3 * dy2) / A;
    double dy = (dx2 * dr3 - dx3 * dr2) / A;
    
    int R = (int)sqrt(dx * dx + dy * dy);


	x = (int)(p1.x + dx);
	y = (int)(p1.y + dy);

    //check if x,y is in the interior of DT

    bool intri = false;
    if(InTriangle(p1,p2,p3,x,y) != '0')
        intri = true;

    //search the voronoi list and check if edges v1,v2  v2,v3 and v3,v1 are already available
	//if avaialable add the voronoi center, if not make one

    updateVoronoiData(v1,v2, p1, p2, x,y, R, intri);
    updateVoronoiData(v2,v3, p2, p3, x,y, R, intri);
    updateVoronoiData(v3,v1, p3, p1, x,y, R, intri);

}

void VoronoiAlg::updateVoronoiData (int v1, int v2, const Node &d1, const Node &d2,
                                    int x, int y, int R, bool intri)
{
    Voronoi vor;
    int mx, my;

    unsigned int i;
    bool found = false;
  
    //checking for v1 v2
    for(i = 0; i < (unsigned int)vlist.GetCount(); i++)
    {
		POSITION pos = vlist.FindIndex(i);

        vor = (Voronoi) (vlist.GetAt(pos));
        if( (vor.v1 == v1 && vor.v2 == v2) ||
            (vor.v1 == v2 && vor.v2 == v1)) {
            vor.x2 = x;
            vor.y2 = y;
            vor.r2 = R;
            vor.num = 2;
            found = true;
            vlist.RemoveAt(pos);
            vlist.AddTail(vor); //this list is value list so delete item and add modified item
            break;
        }
    }

    if(!found) { //add the new edge
        vor.num = 1;
        vor.v1 = v1;
        vor.v2 = v2;
        vor.x1 = x;
        vor.y1 = y;
        vor.r1 = R;

        //take the mid-point of the v1-v2 edge and center; extend it along to create a voronoi edge

        if(intri) {
            
            mx = (int)((d1.x + d2.x)/2);
            my = (int)((d1.y + d2.y)/2);
            
            vor.x2 =  2*mx - x;
            vor.y2 =  2*my - y;

            vor.num = 1;
        }

//        vlist.append(vor);
		vlist.AddTail(vor);
    }
}

void VoronoiAlg::displayVoronoiDiagram(CDC *pDC)
{
    Voronoi vor;
    CPen p1, p2, *pOldPen;
    int rad = 2;

	p1.CreatePen(PS_SOLID, 1, RGB(192,192,192));
	p2.CreatePen(PS_SOLID, 1, RGB(0,0,255));	
    //checking for v1 v2
    for( int i = 0; i < vlist.GetCount(); i++)
    {
		POSITION pos = vlist.FindIndex(i);

		vor = (Voronoi) vlist.GetAt(pos);

		pOldPen = pDC->SelectObject(&p1);
      
		if(vor.num == 2) 
		{
			rad = vor.r2;

			pDC->SelectStockObject(NULL_BRUSH);

			pDC->Ellipse(vor.x2 - rad, vor.y2 - rad, vor.x2 + rad, vor.y2 + rad);
		}
		
		rad = vor.r1;
				
		pDC->Ellipse(vor.x1 - rad, vor.y1 - rad, vor.x1 + rad, vor.y1 + rad);
		
		//reset
		rad = 2;

		pDC->SelectObject(&p2);
		pDC->SelectStockObject(NULL_BRUSH);

		//mark the vertices
		pDC->Ellipse (vor.x1 - rad, vor.y1 - rad, vor.x1 + rad, vor.y1 + rad);
		pDC->Ellipse (vor.x2 - rad, vor.y2 - rad, vor.x2 + rad, vor.y2 + rad);

		//draw the edges
		
		pDC->MoveTo(vor.x1,vor.y1);
		pDC->LineTo(vor.x2,vor.y2);
			
		pDC->SelectObject(pOldPen);
    }

	p1.DeleteObject();
	p2.DeleteObject();
}


char VoronoiAlg::InTriangle(const Node &a, const Node &b,
                            const Node &c, int x, int y)
{
    int area0, area1, area2;
    Node p;

    p.x = x;
    p.y = y;

   /* compute three AreaSign() values for pp w.r.t. each edge of the face in 2D */
   area0 = AreaSign( p, a, b );
   area1 = AreaSign( p, b, c );
   area2 = AreaSign( p, c, a );
   

   if ( ( area0 == 0 ) && ( area1 > 0 ) && ( area2 > 0 ) ||
        ( area1 == 0 ) && ( area0 > 0 ) && ( area2 > 0 ) ||
        ( area2 == 0 ) && ( area0 > 0 ) && ( area1 > 0 ) ) 
     return 'E';

   if ( ( area0 == 0 ) && ( area1 < 0 ) && ( area2 < 0 ) ||
        ( area1 == 0 ) && ( area0 < 0 ) && ( area2 < 0 ) ||
        ( area2 == 0 ) && ( area0 < 0 ) && ( area1 < 0 ) )
     return 'E';                 
   
   if ( ( area0 >  0 ) && ( area1 > 0 ) && ( area2 > 0 ) ||
        ( area0 <  0 ) && ( area1 < 0 ) && ( area2 < 0 ) )
     return 'F';

   if ( ( area0 == 0 ) && ( area1 == 0 ) && ( area2 == 0 ) )
     return '0';

   if ( ( area0 == 0 ) && ( area1 == 0 ) ||
        ( area0 == 0 ) && ( area2 == 0 ) ||
        ( area1 == 0 ) && ( area2 == 0 ) )
     return 'V';

   else  
     return '0';  
}

int VoronoiAlg::AreaSign(const Node &a, const Node &b, const Node &c )
{
    double area2;

    area2 = ( b.x - a.x ) * (double)( c.y - a.y ) -
            ( c.x - a.x ) * (double)( b.y - a.y );

    /* The area should be an integer. */
    if      ( area2 >  0.5 ) return  1;
    else if ( area2 < -0.5 ) return -1;
    else                     return  0;
}