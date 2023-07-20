// VoronoiAlg.h: interface for the VoronoiAlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VORONOIALG_H__C0608692_2911_47D8_8D7A_44E6D4A62B7A__INCLUDED_)
#define AFX_VORONOIALG_H__C0608692_2911_47D8_8D7A_44E6D4A62B7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "macros.h"

typedef struct
{
	int v1, v2; // one of the bounding edges of the voronoi region
	double x1,y1; //voronoi center on one side
	double x2,y2; //voronoi center on the other side
    int r1, r2;
    int num;
} Voronoi;

class VoronoiAlg  
{
public:
	VoronoiAlg();
	virtual ~VoronoiAlg();
public:
    void addVoronoiData(int v1,int v2,int v3,
        const Node &p1, const Node &p2, const Node &p3);
	void displayVoronoiDiagram(CDC *pDC);
	
private:
	//    QValueList <Voronoi>  vlist;
	CList<Voronoi,Voronoi&>  vlist;
	
    void updateVoronoiData (int v1, int v2, const Node &d1, const Node &d2,
		int x, int y, int R, bool intri);
    
    char InTriangle(const Node &a, const Node &b,
		const Node &c, int x, int y);
    int AreaSign(const Node &a, const Node &b, const Node &c );
};

#endif // !defined(AFX_VORONOIALG_H__C0608692_2911_47D8_8D7A_44E6D4A62B7A__INCLUDED_)
