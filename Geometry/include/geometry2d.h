#ifndef __GEOMETRY2D_H__
#define __GEOMETRY2D_H__


int PointOnSegment(Segmento SR, Ponto p);

double PointDistance(Ponto p1, Ponto p2);

double PointLineDistance(Segmento SR, Ponto p);

Ponto LineIntersection(Segmento SR1, Segmento SR2);

int LinesPosition(Segmento SR1, Segmento SR2);

#endif
