#include <stdio.h>
#include "general.h"


Ponto CreatPoint(double Px, double Py){
	Ponto p;
	p.x = Px;
	p.y = Py;

	return p;
}

Segmento CreatSegment(Ponto point1, Ponto point2){
	Segmento SR;
	SR.p1 = point1;
	SR.p2 = point2;
	
	return SR;
}
