// DelaunayN2.h: interface for the DelaunayN2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELAUNAYN2_H__14C1216D_C36E_44A7_BCFB_7E8FCACFDE13__INCLUDED_)
#define AFX_DELAUNAYN2_H__14C1216D_C36E_44A7_BCFB_7E8FCACFDE13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <math.h>

/* Define vertex indices. */
#define X   0
#define Y   1
#define Z   2

/* Define structures for vertices, edges and faces */
typedef struct tVertexStructure tsVertex;
typedef tsVertex *tVertex;

typedef struct tEdgeStructure tsEdge;
typedef tsEdge *tEdge;

typedef struct tFaceStructure tsFace;
typedef tsFace *tFace;

struct tVertexStructure {
   double      v[3];
   double	    vnum;
   tEdge    duplicate;	        /* pointer to incident cone edge (or NULL) */
   bool     onhull;		/* T iff point on hull. */
   bool	    mark;		/* T iff point already processed. */
   tVertex  next, prev;
};

struct tEdgeStructure {
   tFace    adjface[2];
   tVertex  endpts[2];
   tFace    newface;            /* pointer to incident cone face. */
   bool     del;		/* T iff edge should be delete. */
   tEdge    next, prev;
};

struct tFaceStructure {
   tEdge    edge[3];
   tVertex  vertex[3];
   bool	    visible;	        /* T iff face visible from new point. */
   bool     lower;              /* T iff on the lower hull */
   tFace    next, prev;
};

class DelaunayN2  
{
public:
	DelaunayN2();
	virtual ~DelaunayN2();
    //add vertex to the data structure
    void addVertex( double x, double y , double z);
    void ReadVertices();

    //this function should be called after adding all the vertices
    void computeAndDisplayVoronoi (CDC* pDC,bool dn2, bool voronoi);

    /*---------------------------------------------------------------------
    DoubleTriangle builds the initial double triangle.  It first finds 3 
    noncollinear points and makes two faces out of them, in opposite order.
    It then finds a fourth point that is not coplanar with that face.  The  
    vertices are stored in the face structure in counterclockwise order so 
    that the volume between the face and the point is negative. Lastly, the
    3 newfaces to the fourth point are constructed and the data structures
    are cleaned up. 
    ---------------------------------------------------------------------*/
    void DoubleTriangle( void );

    /*---------------------------------------------------------------------
    ConstructHull adds the vertices to the hull one at a time.  The hull
    vertices are those in the list marked as onhull.
    ---------------------------------------------------------------------*/
    void ConstructHull( void );

    void LowerFaces(CDC *pDC);
	void LowerFaces( void );
    void Print( void );
private:
    bool b_dn2, b_voronoi;
    double vnum;

    tVertex	MakeNullVertex( void );

    //Computes a - b and puts it into c.
    void   SubVec( double a[3], double b[3], double c[3]);

    /*---------------------------------------------------------------------
    AddOne is passed a vertex.  It first determines all faces visible from 
    that point.  If none are visible then the point is marked as not 
    onhull.  Next is a loop over edges.  If both faces adjacent to an edge
    are visible, then the edge is marked for deletion.  If just one of the
    adjacent faces is visible then a new face is constructed.
    ---------------------------------------------------------------------*/
    bool AddOne( tVertex p );
    
    /*---------------------------------------------------------------------
    VolumeSign returns the sign of the volume of the tetrahedron determined by f
    and p.  VolumeSign is +1 iff p is on the negative side of f,
    where the positive side is determined by the rh-rule.  So the volume 
    is positive if the ccw normal to f points outside the tetrahedron.
    The final fewer-multiplications form is due to Robert Fraczkiewicz.
    ---------------------------------------------------------------------*/
    double VolumeSign( tFace f, tVertex p );

    double  Volumei( tFace f, tVertex p );

    /*---------------------------------------------------------------------
    Volumed is the same as VolumeSign but computed with doubles.  For 
    protection against overflow.
    ---------------------------------------------------------------------*/
    double Volumed( tFace f, tVertex p );

    void PrintPoint( tVertex p );

    /*---------------------------------------------------------------------
    MakeConeFace makes a new face and two new edges between the 
    edge and the point that are passed to it. It returns a pointer to
    the new face.
    ---------------------------------------------------------------------*/
    tFace MakeConeFace( tEdge e, tVertex p );

    /*---------------------------------------------------------------------
    MakeCcw puts the vertices in the face structure in counterclock wise 
    order.  We want to store the vertices in the same 
    order as in the visible face.  The third vertex is always p.
    ---------------------------------------------------------------------*/
    void MakeCcw( tFace f, tEdge e, tVertex p );

    /*---------------------------------------------------------------------
    MakeNullEdge creates a new cell and initializes all pointers to NULL
    and sets all flags to off.  It returns a pointer to the empty cell.
    ---------------------------------------------------------------------*/
    tEdge MakeNullEdge( void );

    /*--------------------------------------------------------------------
    MakeNullFace creates a new face structure and initializes all of its
    flags to NULL and sets all the flags to off.  It returns a pointer
    to the empty cell.
    ---------------------------------------------------------------------*/
    tFace 	MakeNullFace( void );
    
    /*---------------------------------------------------------------------
    MakeFace creates a new face structure from three vertices (in ccw
    order).  It returns a pointer to the face.
    ---------------------------------------------------------------------*/
    tFace   MakeFace( tVertex v0, tVertex v1, tVertex v2, tFace fold );
    
    /*---------------------------------------------------------------------
    CleanUp goes through each data structure list and clears all
    flags and NULLs out some pointers.  The order of processing
    (edges, faces, vertices) is important.
    ---------------------------------------------------------------------*/
    void CleanUp( void );

    /*---------------------------------------------------------------------
    CleanEdges runs through the edge list and cleans up the structure.
    If there is a newface then it will put that face in place of the 
    visible face and NULL out newface. It also deletes so marked edges.
    ---------------------------------------------------------------------*/
    void CleanEdges( void );
    
    /*---------------------------------------------------------------------
    CleanFaces runs through the face list and deletes any face marked visible.
    ---------------------------------------------------------------------*/
    void CleanFaces( void );
    
    /*---------------------------------------------------------------------
    CleanVertices runs through the vertex list and deletes the 
    vertices that are marked as processed but are not incident to any 
    undeleted edges. 
    ---------------------------------------------------------------------*/
    void CleanVertices( void );

    /*---------------------------------------------------------------------
    Collinear checks to see if the three points given are collinear,
    by checking to see if each element of the cross product is zero.
    ---------------------------------------------------------------------*/
    bool	Collinear( tVertex a, tVertex b, tVertex c );
    
    /*---------------------------------------------------------------------
    Computes the z-coordinate of the vector normal to face f.
    ---------------------------------------------------------------------*/
    double Normz( tFace f );
    
    /*---------------------------------------------------------------------
    Consistency runs through the edge list and checks that all
    adjacent faces have their endpoints in opposite order.  This verifies
    that the vertices are in counterclockwise order.
    ---------------------------------------------------------------------*/
    void Consistency( void );
    
    /*---------------------------------------------------------------------
    Convexity checks that the volume between every face and every
    point is negative.  This shows that each point is inside every face
    and therefore the hull is convex.
    ---------------------------------------------------------------------*/
    void Convexity( void );
    
    /*---------------------------------------------------------------------
    CheckEuler checks Euler's relation, as well as its implications when
    all faces are known to be triangles.  Only prints positive information
    when debug is true, but always prints negative information.
    ---------------------------------------------------------------------*/
    void CheckEuler( double V, double E, double F );

    void Checks( void );

    void PrintOut( tVertex v );
    void PrintVertices( void );
    void PrintEdges( void );
    void PrintFaces( void );

    tVertex vertices;
    tEdge edges;
    tFace faces;
public:
    bool debug;
    bool check;
};

#endif // !defined(AFX_DELAUNAYN2_H__14C1216D_C36E_44A7_BCFB_7E8FCACFDE13__INCLUDED_)
