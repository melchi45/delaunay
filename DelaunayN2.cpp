// DelaunayN2.cpp: implementation of the DelaunayN2 class.
//
//////////////////////////////////////////////////////////////////////

/*
This code is described in "Computational Geometry in C" (Second Edition),
Chapter 5.  It is not written to be comprehensible without the 
explanation in that book.

Input: 2n integer coordinates for the points.
Output: The Delaunay triangulation, in postscript with embedded comments.

Compile: gcc -o dt2 dt2.c (or simply: make)

Written by Joseph O'Rourke.
Last modified: July 1997
Questions to orourke@cs.smith.edu.
--------------------------------------------------------------------
This code is Copyright 1998 by Joseph O'Rourke.  It may be freely 
redistributed in its entirety provided that this copyright notice is 
not removed.
--------------------------------------------------------------------
*/

#include "stdafx.h"
#include "DelaunayN2.h"

#include "macros.h"
#include "VoronoiAlg.h"

#define ONHULL   	true
#define REMOVED  	true
#define VISIBLE  	true
#define PROCESSED	true
#define SAFE            1000000         /* Range of safe coord values. */

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DelaunayN2::DelaunayN2()
{
    vertices = NULL;
    edges    	 = NULL;
    faces    	 = NULL;
    debug = false;
    check = false;
    vnum = 0;

    b_dn2 = b_voronoi = true;
}

DelaunayN2::~DelaunayN2()
{
    CleanUp();
}


void DelaunayN2::computeAndDisplayVoronoi(CDC *pDC, bool dn2, bool vor)
{
    b_voronoi = vor;
    b_dn2 = dn2;

    DoubleTriangle();
   ConstructHull();
   LowerFaces(pDC);
   //Print();
}

void DelaunayN2::LowerFaces( CDC *pDC )
{
	tFace f = faces;
	//double   z;
	double   Flower = 0;   // Total number of lower faces. 
	VoronoiAlg vor;
	Node n1, n2, n3;
	
	do 
	{
		//z = N]ormz( f );
		//if ( z < 0 ) {
		if ( Normz( f ) < 0 ) 
		{
			Flower++;
			f->lower = true;
			//printf("z=%10d; lower face indices: %.0lf, %.0lf, %.0lf\n", z, 
			//        printf("lower face indices: %.0lf, %.0lf, %.0lf\n",
			//           f->vertex[0]->vnum,
			//           f->vertex[1]->vnum,
			//           f->vertex[2]->vnum );
			
			if(b_dn2) 
			{
				CPen myPen, *pOldPen;
				myPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));	
				pOldPen = (CPen*) pDC->SelectObject(&myPen);
				
				pDC->MoveTo(f->vertex[0]->v[0], f->vertex[0]->v[1]);
				pDC->LineTo(f->vertex[1]->v[0], f->vertex[1]->v[1]);
				pDC->LineTo(f->vertex[2]->v[0], f->vertex[2]->v[1]);
				pDC->LineTo(f->vertex[0]->v[0], f->vertex[0]->v[1]);
				
				pDC->SelectObject(pOldPen);

				myPen.DeleteObject();
			}
			
			n1.x = f->vertex[0]->v[0];
			n1.y = f->vertex[0]->v[1];
			n1.z = f->vertex[0]->v[2];
			
			n2.x = f->vertex[1]->v[0];
			n2.y = f->vertex[1]->v[1];
			n2.z = f->vertex[1]->v[2];
			
			n3.x = f->vertex[2]->v[0];
			n3.y = f->vertex[2]->v[1];
			n3.z = f->vertex[2]->v[2];
			
			if(b_voronoi)
				vor.addVoronoiData(
						f->vertex[0]->vnum, 
						f->vertex[1]->vnum, 
						f->vertex[2]->vnum,
						n1, n2, n3);        
		}
		else f->lower = false;
		f = f->next;
	} while ( f != faces );
	
	if(b_voronoi)
		vor.displayVoronoiDiagram (pDC);
	//printf("A total of %.0lf lower faces identified.\n", Flower);
}


void DelaunayN2::LowerFaces( void )
{
   tFace f = faces;
   /*int   z;*/
   int   Flower = 0;   /* Total number of lower faces. */

   do {
     /*z = Normz( f );
     if ( z < 0 ) {*/
     if ( Normz( f ) < 0 ) {
        Flower++;
        f->lower = true;
        //printf("z=%10d; lower face indices: %d, %d, %d\n", z, */
        printf("lower face indices: %d, %d, %d\n",
           f->vertex[0]->vnum,
           f->vertex[1]->vnum,
           f->vertex[2]->vnum );
     }
     else f->lower = false;
     f = f->next;
   } while ( f != faces );
   printf("A total of %d lower faces identified.\n", Flower);
}

/*---------------------------------------------------------------------
MakeNullVertex: Makes a vertex, nulls out fields.
---------------------------------------------------------------------*/
tVertex	DelaunayN2::MakeNullVertex( void )
{
   tVertex  v;
   
   DT_NEW( v, tsVertex );
   v->duplicate = NULL;
   v->onhull = !ONHULL;
   v->mark = !PROCESSED;
   DT_ADD( vertices, v );

   return v;
}

/*---------------------------------------------------------------------
addVertex: Reads in the vertices, and links them into a circular
list with MakeNullVertex. 
---------------------------------------------------------------------*/
void  DelaunayN2::addVertex( double x, double y, double z )
{
   tVertex  v;
   v = MakeNullVertex();
   v->v[X] = x;
   v->v[Y] = y;
   v->v[Z] = z;
   v->vnum = vnum++;
   if ( ( fabs(x) > SAFE ) || ( fabs(y) > SAFE ) || ( fabs(z) > SAFE ) ) {
       printf("Coordinate of vertex below might be too large: run with -c flag\n");
       PrintPoint(v);
   }
   
}

/*---------------------------------------------------------------------
ReadVertices: Reads in the vertices, and links them into a circular
list with MakeNullVertex.  There is no need for the # of vertices to be
the first line: the function looks for EOF instead.  Sets the global
variable vertices via the DT_ADD macro.
---------------------------------------------------------------------*/
void  DelaunayN2::ReadVertices( void )
{
   tVertex  v;
   float      x, y, z;
   double	    vnum = 0;

   while ( scanf ("%f %f", &x, &y ) != EOF )  {
      v = MakeNullVertex();
      v->v[X] = x;
      v->v[Y] = y;
      z = x*x + y*y;
      v->v[Z] = z;
      v->vnum = vnum++;
      if ( ( fabs(x) > SAFE ) || ( fabs(y) > SAFE ) || ( fabs(z) > SAFE ) ) {
         printf("Coordinate of vertex below might be too large: run with -c flag\n");
         PrintPoint(v);
      }
   }
}

/*---------------------------------------------------------------------
Print: Prints out the vertices and the faces.  Uses the vnum indices 
corresponding to the order in which the vertices were input.
Output is in PostScript format.
---------------------------------------------------------------------*/
void DelaunayN2::Print( void )
{
   /* Pointers to vertices, edges, faces. */
   tVertex  v;
   tEdge    e;
   tFace    f;
   double xmin, ymin, xmax, ymax;
   /* Counters for Euler's formula. */
   double 	V = 0, E = 0 , F = 0;
   /* Note: lowercase==pointer, uppercase==counter. */

   /*-- find X min & max --*/
   v = vertices;
   xmin = xmax = v->v[X];
   do {
      if( v->v[X] > xmax ) xmax = v->v[X];
      else
	 if( v->v[X] < xmin ) xmin = v->v[X];
      v = v->next;
   } while ( v != vertices );
	
   /*-- find Y min & max --*/
   v = vertices;
   ymin = ymax = v->v[Y];
   do {
      if( v->v[Y] > ymax ) ymax = v->v[Y];
      else
	 if( v->v[Y] < ymin ) ymin = v->v[Y];
      v = v->next;
   } while ( v != vertices );
	
   /* PostScript header */
   printf("%%!PS\n");
   printf("%%%%BoundingBox: %.0lf %.0lf %.0lf %.0lf\n", 
	  xmin, ymin, xmax, ymax);
   printf(".00 .00 setlinewidth\n");
   printf("%.0lf %.0lf translate\n", -xmin+100, -ymin+100 );
   /* The +72 shifts the figure one inch from the lower left corner */

   /* Vertices. */
   v = vertices;
   do {                                 
      if( v->mark ) V++;           
      v = v->next;
   } while ( v != vertices );
   printf("\n%%%% Vertices:\tV = %.0lf\n", V);
   printf("%%%% index:\tx\ty\tz\n");
   do {                                 
      printf( "%%%% %5.0f:\t%.0lf\t%.0lf\t%.0lf\n", 
	     v->vnum, v->v[X], v->v[Y], v->v[Z] );
      printf("newpath\n");
      printf("%.0lf\t%.0lf 2 0 360 arc\n", v->v[X], v->v[Y]);
      printf("closepath stroke\n\n");
      v = v->next;
   } while ( v != vertices );
	
   /* Faces. */
   /* visible faces are printed as PS output */
   f = faces;
   do {
      ++F;                              
      f  = f ->next;
   } while ( f  != faces );
   printf("\n%%%% Faces:\tF = %.0lf\n", F );
   printf("%%%% Visible faces only \n");
   do {           
      /* Print face only if it is lower */
      if ( f-> lower )
      {
	 printf("%%%% vnums:  %.0lf  %.0lf  %.0lf\n", 
		f->vertex[0]->vnum, 
		f->vertex[1]->vnum, 
		f->vertex[2]->vnum);
	 printf("newpath\n");
	 printf("%.0lf\t%.0lf\tmoveto\n", 
		f->vertex[0]->v[X], f->vertex[0]->v[Y] );
	 printf("%.0lf\t%.0lf\tlineto\n", 
		f->vertex[1]->v[X], f->vertex[1]->v[Y] );
	 printf("%.0lf\t%.0lf\tlineto\n", 
		f->vertex[2]->v[X], f->vertex[2]->v[Y] );
	 printf("closepath stroke\n\n");
      }
      f = f->next;
   } while ( f != faces );

   /* prints a list of all faces */
   printf("%%%% List of all faces: \n");
   printf("%%%%\tv0\tv1\tv2\t(vertex indices)\n");
   do {
      printf("%%%%\t%.0lf\t%.0lf\t%.0lf\n",
	     f->vertex[0]->vnum,
	     f->vertex[1]->vnum,
	     f->vertex[2]->vnum );
      f = f->next;
   } while ( f != faces );
	
   /* Edges. */	
   e = edges;
   do {
      E++;
      e = e->next;
   } while ( e != edges );
   printf("\n%%%% Edges:\tE = %.0lf\n", E );

   check = true;
   CheckEuler( V, E, F );

}

/*---------------------------------------------------------------------
SubVec:  Computes a - b and puts it into c.
---------------------------------------------------------------------*/
void   DelaunayN2::SubVec( double a[3], double b[3], double c[3])
{
   int i;

   for( i=0; i < 2; i++ )
      c[i] = a[i] - b[i];

}

void DelaunayN2::DoubleTriangle( void )
{
   tVertex  v0, v1, v2, v3;
   tFace    f0, f1 = NULL;
   double      vol;
	
	
   /* Find 3 non-Collinear points. */
   v0 = vertices;
   while ( Collinear( v0, v0->next, v0->next->next ) )
      if ( ( v0 = v0->next ) == vertices )
         printf("DoubleTriangle:  All points are Collinear!\n"), exit(0);
   v1 = v0->next;
   v2 = v1->next;
	
   /* Mark the vertices as processed. */
   v0->mark = PROCESSED;
   v1->mark = PROCESSED;
   v2->mark = PROCESSED;
   
   /* Create the two "twin" faces. */
   f0 = MakeFace( v0, v1, v2, f1 );
   f1 = MakeFace( v2, v1, v0, f0 );

   /* Link adjacent face fields. */
   f0->edge[0]->adjface[1] = f1;
   f0->edge[1]->adjface[1] = f1;
   f0->edge[2]->adjface[1] = f1;
   f1->edge[0]->adjface[1] = f0;
   f1->edge[1]->adjface[1] = f0;
   f1->edge[2]->adjface[1] = f0;
	
   /* Find a fourth, non-coplanar point to form tetrahedron. */
   v3 = v2->next;
   vol = VolumeSign( f0, v3 );
   while ( !vol )   {
      if ( ( v3 = v3->next ) == v0 ) 
         printf("DoubleTriangle:  All points are coplanar!\n"), exit(0);
      vol = VolumeSign( f0, v3 );
   }
	
   /* Insure that v3 will be the first added. */
   vertices = v3;
   if ( debug ) {
      fprintf(stderr, "DoubleTriangle: finished. Head repositioned at v3.\n");
      PrintOut( vertices );
   }

	
}

  
/*---------------------------------------------------------------------
ConstructHull adds the vertices to the hull one at a time.  The hull
vertices are those in the list marked as onhull.
---------------------------------------------------------------------*/
void  DelaunayN2::ConstructHull( void )
{
   tVertex  v, vnext;
   bool	    changed;	/* T if addition changes hull; not used. */

   v = vertices;
   do {
      vnext = v->next;
      if ( !v->mark ) {
         v->mark = PROCESSED;
	 changed = AddOne( v );
	 CleanUp();

	 if ( check ) {
	    fprintf(stderr,"ConstructHull: After Add of %.0lf & Cleanup:\n", 
               v->vnum);
	    Checks();
	 }
	 if ( debug )
            PrintOut( v );
      }
      v = vnext;
   } while ( v != vertices );
}

/*---------------------------------------------------------------------
AddOne is passed a vertex.  It first determines all faces visible from 
that point.  If none are visible then the point is marked as not 
onhull.  Next is a loop over edges.  If both faces adjacent to an edge
are visible, then the edge is marked for deletion.  If just one of the
adjacent faces is visible then a new face is constructed.
---------------------------------------------------------------------*/
bool DelaunayN2::AddOne( tVertex p )
{
   tFace  f; 
   tEdge  e;
   double 	  vol;
   bool	  vis = false;

   if ( debug ) {
      fprintf(stderr, "AddOne: starting to add v%.0lf.\n", p->vnum);
      PrintOut( vertices );
   }

   /* Mark faces visible from p. */
   f = faces;
   do {
      vol = VolumeSign( f, p );
      if (debug) fprintf(stderr, 
         "faddr: %6x   paddr: %6x   Vol = %.0lf\n", f,p,vol);
      if ( vol < 0 ) {
	 f->visible = VISIBLE;  
	 vis = true;                      
      }
      f = f->next;
   } while ( f != faces );

   /* If no faces are visible from p, then p is inside the hull. */
   if ( !vis ) {
      p->onhull = !ONHULL;  
      return false; 
   }

   /* Mark edges in interior of visible region for deletion.
      Erect a newface based on each border edge. */
   e = edges;
   do {
      tEdge temp;
      temp = e->next;
      if ( e->adjface[0]->visible && e->adjface[1]->visible )
	 /* e interior: mark for deletion. */
	 e->del = REMOVED;
      else if ( e->adjface[0]->visible || e->adjface[1]->visible ) 
	 /* e border: make a new face. */
	 e->newface = MakeConeFace( e, p );
      e = temp;
   } while ( e != edges );
   return true;
}

/*---------------------------------------------------------------------
VolumeSign returns the sign of the volume of the tetrahedron determined by f
and p.  VolumeSign is +1 iff p is on the negative side of f,
where the positive side is determined by the rh-rule.  So the volume 
is positive if the ccw normal to f points outside the tetrahedron.
The final fewer-multiplications form is due to Robert Fraczkiewicz.
---------------------------------------------------------------------*/
double  DelaunayN2::VolumeSign( tFace f, tVertex p )
{
   double  vol;
   double     voli=0;
   double  ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;
   double  bxdx, bydy, bzdz, cxdx, cydy, czdz;

   ax = f->vertex[0]->v[X];
   ay = f->vertex[0]->v[Y];
   az = f->vertex[0]->v[Z];
   bx = f->vertex[1]->v[X];
   by = f->vertex[1]->v[Y];
   bz = f->vertex[1]->v[Z];
   cx = f->vertex[2]->v[X];
   cy = f->vertex[2]->v[Y];
   cz = f->vertex[2]->v[Z];
   dx = p->v[X];
   dy = p->v[Y];
   dz = p->v[Z];
   
   bxdx=bx-dx;
   bydy=by-dy;
   bzdz=bz-dz;
   cxdx=cx-dx;
   cydy=cy-dy;
   czdz=cz-dz;
   vol =   (az-dz) * (bxdx*cydy - bydy*cxdx)
         + (ay-dy) * (bzdz*cxdx - bxdx*czdz)
	 + (ax-dx) * (bydy*czdz - bzdz*cydy);

   /*if ( debug )
      fprintf(stderr,"Face=%6x; Vertex=%.0lf: vol(double) = %.0lf, vol(double) = %.0lf\n",
	      f,p->vnum,voli,vol);*/

   /* The volume should be an integer. */
   if      ( vol > 0.5 )   return  1;
   else if ( vol < -0.5 )  return -1;
   else                    return  0;
}
/*---------------------------------------------------------------------*/
double  DelaunayN2::Volumei( tFace f, tVertex p )
{
   double 	   vol;
   double 	   ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;
   double	   bxdx, bydy, bzdz, cxdx, cydy, czdz;

   ax = f->vertex[0]->v[X];
   ay = f->vertex[0]->v[Y];
   az = f->vertex[0]->v[Z];
   bx = f->vertex[1]->v[X];
   by = f->vertex[1]->v[Y];
   bz = f->vertex[1]->v[Z];
   cx = f->vertex[2]->v[X];
   cy = f->vertex[2]->v[Y];
   cz = f->vertex[2]->v[Z];
   dx = p->v[X];
   dy = p->v[Y];
   dz = p->v[Z];
   
   bxdx=bx-dx;
   bydy=by-dy;
   bzdz=bz-dz;
   cxdx=cx-dx;
   cydy=cy-dy;
   czdz=cz-dz;
   vol =   (az-dz)*(bxdx*cydy-bydy*cxdx)
         + (ay-dy)*(bzdz*cxdx-bxdx*czdz)
	 + (ax-dx)*(bydy*czdz-bzdz*cydy);

   return vol;
}		

/*---------------------------------------------------------------------
Volumed is the same as VolumeSign but computed with doubles.  For 
protection against overflow.
---------------------------------------------------------------------*/
double 	DelaunayN2::Volumed( tFace f, tVertex p )
{
   double  vol;
   double  ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;
   double  bxdx, bydy, bzdz, cxdx, cydy, czdz;

   ax = f->vertex[0]->v[X];
   ay = f->vertex[0]->v[Y];
   az = f->vertex[0]->v[Z];
   bx = f->vertex[1]->v[X];
   by = f->vertex[1]->v[Y];
   bz = f->vertex[1]->v[Z];
   cx = f->vertex[2]->v[X];
   cy = f->vertex[2]->v[Y];
   cz = f->vertex[2]->v[Z];
   dx = p->v[X];
   dy = p->v[Y];
   dz = p->v[Z];
   
   bxdx=bx-dx;
   bydy=by-dy;
   bzdz=bz-dz;
   cxdx=cx-dx;
   cydy=cy-dy;
   czdz=cz-dz;
   vol = (az-dz)*(bxdx*cydy-bydy*cxdx)
         + (ay-dy)*(bzdz*cxdx-bxdx*czdz)
	 + (ax-dx)*(bydy*czdz-bzdz*cydy);

   return vol;
}

/*-------------------------------------------------------------------*/
void	DelaunayN2::PrintPoint( tVertex p )
{
   int i;

   for ( i = 0; i < 3; i++ )
      printf("\t%.0lf", p->v[i]);
   putchar('\n');
}

/*---------------------------------------------------------------------
MakeConeFace makes a new face and two new edges between the 
edge and the point that are passed to it. It returns a pointer to
the new face.
---------------------------------------------------------------------*/
tFace	DelaunayN2::MakeConeFace( tEdge e, tVertex p )
{
   tEdge  new_edge[2];
   tFace  new_face;
   int i, j;

   /* Make two new edges (if don't already exist). */
   for ( i=0; i < 2; ++i ) 
      /* If the edge exists, copy it into new_edge. */
      if ( !( new_edge[i] = e->endpts[i]->duplicate) ) {
	 /* Otherwise (duplicate is NULL), MakeNullEdge. */
	 new_edge[i] = MakeNullEdge();
	 new_edge[i]->endpts[0] = e->endpts[i];
	 new_edge[i]->endpts[1] = p;
	 e->endpts[i]->duplicate = new_edge[i];
      }

   /* Make the new face. */
   new_face = MakeNullFace();   
   new_face->edge[0] = e;
   new_face->edge[1] = new_edge[0];
   new_face->edge[2] = new_edge[1];
   MakeCcw( new_face, e, p ); 
        
   /* Set the adjacent face pointers. */
   for ( i=0; i < 2; ++i )
      for ( j=0; j < 2; ++j )  
	 /* Only one NULL link should be set to new_face. */
	 if ( !new_edge[i]->adjface[j] ) {
	    new_edge[i]->adjface[j] = new_face;
	    break;
	 }
        
   return new_face;
}

/*---------------------------------------------------------------------
MakeCcw puts the vertices in the face structure in counterclock wise 
order.  We want to store the vertices in the same 
order as in the visible face.  The third vertex is always p.
---------------------------------------------------------------------*/
void	DelaunayN2::MakeCcw( tFace f, tEdge e, tVertex p )
{
   tFace  fv;   /* The visible face adjacent to e */
   int i;    /* Index of e->endpoint[0] in fv. */
   tEdge  s;	/* Temporary, for swapping */
      
   if  ( e->adjface[0]->visible )      
        fv = e->adjface[0];
   else fv = e->adjface[1];
       
   /* Set vertex[0] & [1] of f to have the same orientation
      as do the corresponding vertices of fv. */ 
   for ( i=0; fv->vertex[i] != e->endpts[0]; ++i )
      ;
   /* Orient f the same as fv. */
   if ( fv->vertex[ (i+1) % 3 ] != e->endpts[1] ) {
      f->vertex[0] = e->endpts[1];  
      f->vertex[1] = e->endpts[0];    
   }
   else {                               
      f->vertex[0] = e->endpts[0];   
      f->vertex[1] = e->endpts[1];      
      SWAP( s, f->edge[1], f->edge[2] );
   }
   /* This swap is tricky. e is edge[0]. edge[1] is based on endpt[0],
      edge[2] on endpt[1].  So if e is oriented "forwards," we
      need to move edge[1] to follow [0], because it precedes. */
   
   f->vertex[2] = p;
}
 
/*---------------------------------------------------------------------
MakeNullEdge creates a new cell and initializes all pointers to NULL
and sets all flags to off.  It returns a pointer to the empty cell.
---------------------------------------------------------------------*/
tEdge 	DelaunayN2::MakeNullEdge( void )
{
   tEdge  e;

   DT_NEW( e, tsEdge );
   e->adjface[0] = e->adjface[1] = e->newface = NULL;
   e->endpts[0] = e->endpts[1] = NULL;
   e->del = !REMOVED;
   DT_ADD( edges, e );
   return e;
}

/*--------------------------------------------------------------------
MakeNullFace creates a new face structure and initializes all of its
flags to NULL and sets all the flags to off.  It returns a pointer
to the empty cell.
---------------------------------------------------------------------*/
tFace 	DelaunayN2::MakeNullFace( void )
{
   tFace  f;
   int   i;

   DT_NEW( f, tsFace);
   for ( i=0; i < 3; ++i ) {
      f->edge[i] = NULL;
      f->vertex[i] = NULL;
   }
   f->visible = !VISIBLE;
   DT_ADD( faces, f );
   return f;
}

/*---------------------------------------------------------------------
MakeFace creates a new face structure from three vertices (in ccw
order).  It returns a pointer to the face.
---------------------------------------------------------------------*/
tFace   DelaunayN2::MakeFace( tVertex v0, tVertex v1, tVertex v2, tFace fold )
{
   tFace  f;
   tEdge  e0, e1, e2;

   /* Create edges of the initial triangle. */
   if( !fold ) {
     e0 = MakeNullEdge();
     e1 = MakeNullEdge();
     e2 = MakeNullEdge();
   }
   else { /* Copy from fold, in reverse order. */
     e0 = fold->edge[2];
     e1 = fold->edge[1];
     e2 = fold->edge[0];
   }
   e0->endpts[0] = v0;              e0->endpts[1] = v1;
   e1->endpts[0] = v1;              e1->endpts[1] = v2;
   e2->endpts[0] = v2;              e2->endpts[1] = v0;
	
   /* Create face for triangle. */
   f = MakeNullFace();
   f->edge[0]   = e0;  f->edge[1]   = e1; f->edge[2]   = e2;
   f->vertex[0] = v0;  f->vertex[1] = v1; f->vertex[2] = v2;
	
   /* Link edges to face. */
   e0->adjface[0] = e1->adjface[0] = e2->adjface[0] = f;
	
   return f;
}

/*---------------------------------------------------------------------
CleanUp goes through each data structure list and clears all
flags and NULLs out some pointers.  The order of processing
(edges, faces, vertices) is important.
---------------------------------------------------------------------*/
void	DelaunayN2::CleanUp( void )
{
   CleanEdges();
   CleanFaces();
   CleanVertices();
}

/*---------------------------------------------------------------------
CleanEdges runs through the edge list and cleans up the structure.
If there is a newface then it will put that face in place of the 
visible face and NULL out newface. It also dels so marked edges.
---------------------------------------------------------------------*/
void	DelaunayN2::CleanEdges( void )
{
   tEdge  e;	/* Primary index into edge list. */
   tEdge  t;	/* Temporary edge pointer. */
		
   /* Integrate the newface's into the data structure. */
   /* Check every edge. */
   e = edges;

   if(!e)
       return;

   do {
      if ( e->newface ) { 
	 if ( e->adjface[0]->visible )
	    e->adjface[0] = e->newface; 
	 else	e->adjface[1] = e->newface;
	 e->newface = NULL;
      }
      e = e->next;
   } while ( e != edges );

   /* Delete any edges marked for deletion. */
   while ( edges && edges->del ) { 
      e = edges;
      DT_DELETE( edges, e );
   }
   e = edges->next;
   do {
      if ( e->del ) {
	 t = e;
	 e = e->next;
	 DT_DELETE( edges, t );
      }
      else e = e->next;
   } while ( e != edges );
}

/*---------------------------------------------------------------------
CleanFaces runs through the face list and dels any face marked visible.
---------------------------------------------------------------------*/
void	DelaunayN2::CleanFaces( void )
{
   tFace  f;	/* Primary pointer into face list. */
   tFace  t;	/* Temporary pointer, for deleting. */
	

   while ( faces && faces->visible ) { 
      f = faces;
      DT_DELETE( faces, f );
   }

   if(!faces)
       return;

   f = faces->next;
   do {
      if ( f->visible ) {
	 t = f;
	 f = f->next;
	 DT_DELETE( faces, t );
      }
      else f = f->next;
   } while ( f != faces );
}

/*---------------------------------------------------------------------
CleanVertices runs through the vertex list and dels the 
vertices that are marked as processed but are not incident to any 
undeld edges. 
---------------------------------------------------------------------*/
void	DelaunayN2::CleanVertices( void )
{
   tEdge    e;
   tVertex  v, t;
	
   /* Mark all vertices incident to some undeld edge as on the hull. */
   e = edges;

   if(!e)
       return;

   do {
      e->endpts[0]->onhull = e->endpts[1]->onhull = ONHULL;
      e = e->next;
   } while (e != edges);
	
   /* Delete all vertices that have been processed but
      are not on the hull. */
   while ( vertices && vertices->mark && !vertices->onhull ) { 
      v = vertices;
      DT_DELETE( vertices, v );
   }
   v = vertices->next;
   do {
      if ( v->mark && !v->onhull ) {    
	 t = v; 
	 v = v->next;
	 DT_DELETE( vertices, t )
      }
      else v = v->next;
   } while ( v != vertices );
	
   /* Reset flags. */
   v = vertices;
   do {
      v->duplicate = NULL; 
      v->onhull = !ONHULL; 
      v = v->next;
   } while ( v != vertices );
}

/*---------------------------------------------------------------------
Collinear checks to see if the three points given are collinear,
by checking to see if each element of the cross product is zero.
---------------------------------------------------------------------*/
bool	DelaunayN2::Collinear( tVertex a, tVertex b, tVertex c )
{
   return 
         ( c->v[Z] - a->v[Z] ) * ( b->v[Y] - a->v[Y] ) -
         ( b->v[Z] - a->v[Z] ) * ( c->v[Y] - a->v[Y] ) == 0
      && ( b->v[Z] - a->v[Z] ) * ( c->v[X] - a->v[X] ) -
         ( b->v[X] - a->v[X] ) * ( c->v[Z] - a->v[Z] ) == 0
      && ( b->v[X] - a->v[X] ) * ( c->v[Y] - a->v[Y] ) -
         ( b->v[Y] - a->v[Y] ) * ( c->v[X] - a->v[X] ) == 0  ;
}

/*---------------------------------------------------------------------
Computes the z-coordinate of the vector normal to face f.
---------------------------------------------------------------------*/
double	DelaunayN2::Normz( tFace f )
{
   tVertex a, b, c;
   /*double ba0, ca1, ba1, ca0,z;*/

   a = f->vertex[0];
   b = f->vertex[1];
   c = f->vertex[2];

/*
   ba0 = ( b->v[X] - a->v[X] );
   ca1 = ( c->v[Y] - a->v[Y] );
   ba1 = ( b->v[Y] - a->v[Y] );
   ca0 = ( c->v[X] - a->v[X] );

   z = ba0 * ca1 - ba1 * ca0; 
   printf("Normz = %.0lf=%g\n", z,z);
   if      ( z > 0.0 )  return  1;
   else if ( z < 0.0 )  return -1;
   else                 return  0;
*/
   return 
      ( b->v[X] - a->v[X] ) * ( c->v[Y] - a->v[Y] ) -
      ( b->v[Y] - a->v[Y] ) * ( c->v[X] - a->v[X] );
}

/*---------------------------------------------------------------------
Consistency runs through the edge list and checks that all
adjacent faces have their endpoints in opposite order.  This verifies
that the vertices are in counterclockwise order.
---------------------------------------------------------------------*/
void	DelaunayN2::Consistency( void )
{
   register tEdge  e;
   register int    i, j;

   e = edges;

   do {
      /* find index of endpoint[0] in adjacent face[0] */
      for ( i = 0; e->adjface[0]->vertex[i] != e->endpts[0]; ++i )
	 ;
   
      /* find index of endpoint[0] in adjacent face[1] */
      for ( j = 0; e->adjface[1]->vertex[j] != e->endpts[0]; ++j )
	 ;

      /* check if the endpoints occur in opposite order */
      if ( !( e->adjface[0]->vertex[ (i+1) % 3 ] ==
	      e->adjface[1]->vertex[ (j+2) % 3 ] ||
	      e->adjface[0]->vertex[ (i+2) % 3 ] ==
	      e->adjface[1]->vertex[ (j+1) % 3 ] )  )
	 break;
      e = e->next;

   } while ( e != edges );

   if ( e != edges )
      fprintf( stderr, "Checks: edges are NOT consistent.\n");
   else
      fprintf( stderr, "Checks: edges consistent.\n");

}

/*---------------------------------------------------------------------
Convexity checks that the volume between every face and every
point is negative.  This shows that each point is inside every face
and therefore the hull is convex.
---------------------------------------------------------------------*/
void	DelaunayN2::Convexity( void )
{
   register tFace    f;
   register tVertex  v;
   double               vol;

   f = faces;
   
   do {
      v = vertices;
      do {
	 if ( v->mark ) {
	    vol = VolumeSign( f, v );
	    if ( vol < 0 )
	       break;
	 }
	 v = v->next;
      } while ( v != vertices );

      f = f->next;

   } while ( f != faces );

   if ( f != faces )
      fprintf( stderr, "Checks: NOT convex.\n");
   else if ( check ) 
      fprintf( stderr, "Checks: convex.\n");
}

/*---------------------------------------------------------------------
CheckEuler checks Euler's relation, as well as its implications when
all faces are known to be triangles.  Only prints positive information
when debug is true, but always prints negative information.
---------------------------------------------------------------------*/
void	DelaunayN2::CheckEuler( double V, double E, double F )
{
   if ( check )
      fprintf( stderr, "Checks: V, E, F = %.0lf %.0lf %.0lf:\t", V, E, F);

   if ( (V - E + F) != 2 )
      fprintf( stderr, "Checks: V-E+F != 2\n");
   else if ( check )
      fprintf( stderr, "V-E+F = 2\t");


   if ( F != (2 * V - 4) )
      fprintf( stderr, "Checks: F=%.0lf != 2V-4=%.0lf; V=%.0lf\n",
	      F, 2*V-4, V);
   else if ( check ) 
      fprintf( stderr, "F = 2V-4\t");
   
   if ( (2 * E) != (3 * F) )
      fprintf( stderr, "Checks: 2E=%.0lf != 3F=%.0lf; E=%.0lf, F=%.0lf\n",
	      2*E, 3*F, E, F );
   else if ( check ) 
      fprintf( stderr, "2E = 3F\n");
}

/*-------------------------------------------------------------------*/
void	DelaunayN2::Checks( void )
{
   tVertex  v;
   tEdge    e;
   tFace    f;
   double 	   V = 0, E = 0 , F = 0;

   Consistency();
   Convexity();
   if ( v = vertices )
      do {
         if (v->mark) V++;
	 v = v->next;
      } while ( v != vertices );
   if ( e = edges )
      do {
         E++;
	 e = e->next;
      } while ( e != edges );
   if ( f = faces )
      do {
         F++;
	 f  = f ->next;
      } while ( f  != faces );
   CheckEuler( V, E, F );
}


/*===================================================================
These functions are used whenever the debug flag is set.
They print out the entire contents of each data structure.  
Printing is to standard error.  To grab the output in a file in the csh, 
use this:
	chull < i.file >&! o.file
=====================================================================*/
/*-------------------------------------------------------------------*/
void	DelaunayN2::PrintOut( tVertex v )
{
   fprintf( stderr, "\nHead vertex %.0lf = %6x :\n", v->vnum, v );
   PrintVertices();
   PrintEdges();
   PrintFaces();
}

/*-------------------------------------------------------------------*/
void	DelaunayN2::PrintVertices( void )
{
   tVertex  temp;

   temp = vertices;
   fprintf (stderr, "Vertex List\n");
   if (vertices) do {
      fprintf(stderr,"  addr %6x\t", vertices );
      fprintf(stderr,"  vnum %4d", vertices->vnum );
      fprintf(stderr,"   (%6d,%6d,%6d)",vertices->v[X],
	      vertices->v[Y], vertices->v[Z] );
      fprintf(stderr,"   active:%3d", vertices->onhull );
      fprintf(stderr,"   dup:%5x", vertices->duplicate );
      fprintf(stderr,"   mark:%2d\n", vertices->mark );
      vertices = vertices->next;
   } while ( vertices != temp );

}

/*-------------------------------------------------------------------*/
void	DelaunayN2::PrintEdges( void )
{
   tEdge  temp;
   int 	  i;
	
   temp = edges;
   fprintf (stderr, "Edge List\n");
   if (edges) do {
      fprintf( stderr, "  addr: %6x\t", edges );
      fprintf( stderr, "adj: ");
      for (i=0; i<2; ++i) 
	 fprintf( stderr, "%6x", edges->adjface[i] );
      fprintf( stderr, "  endpts:");
      for (i=0; i<2; ++i) 
	 fprintf( stderr, "%4d", edges->endpts[i]->vnum);
      fprintf( stderr, "  del:%3d\n", edges->del );
      edges = edges->next; 
   } while (edges != temp );

}

/*-------------------------------------------------------------------*/
void	DelaunayN2::PrintFaces( void )
{
   int 	  i;
   tFace  temp;

   temp = faces;
   fprintf (stderr, "Face List\n");
   if (faces) do {
      fprintf(stderr, "  addr: %6x\t", faces );
      fprintf(stderr, "  edges:");
      for( i=0; i<3; ++i )
	 fprintf(stderr, "%6x", faces->edge[i] );
      fprintf(stderr, "  vert:");
      for ( i=0; i<3; ++i)
	 fprintf(stderr, "%4d", faces->vertex[i]->vnum );
      fprintf(stderr, "  vis: %.0lf\n", faces->visible );
      faces= faces->next;
   } while ( faces != temp );

}