#include <stdio.h>
#include <math.h>
#include "general.h"
#include "geometry2d.h"

static int is_degenerate(const Segmento *SR) {
	return fabs(SR->p1.x - SR->p2.x) < EPS && fabs(SR->p1.y - SR->p2.y) < EPS;
}

int point_on_segment(const Segmento *SR, const Ponto *p) {
	if (SR == NULL || p == NULL) return 0;

	// Identificar se o ponto p está dentro do escopo definido pelos dois pontos do segmento
	double minx = fmin(SR->p1.x, SR->p2.x), maxx = fmax(SR->p1.x, SR->p2.x);
	double miny = fmin(SR->p1.y, SR->p2.y), maxy = fmax(SR->p1.y, SR->p2.y);
	
	if(p->x < minx - EPS || p->x > maxx + EPS) return 0;
	if(p->y < miny - EPS || p->y > maxy + EPS) return 0;
	
	// Os dois elementos do vetor formado pelos dois pontos do segmento
	double x = SR->p1.x - SR->p2.x;
	double y = SR->p1.y - SR->p2.y;
	
	// Identificar se o ponto está sobre a reta definida pelo vetor anterior e um ponto do segmento da reta
	// Simples identificação já basta, pois já sabemos que o ponto está dentro do escopo dos pontos do segmento
	double u = p->x - SR->p1.x;
	double v = p->y - SR->p1.y;
	
	if(fabs(x*v - y*u) > EPS) return 0;
	
	return 1;
}

double point_distance(const Ponto *p1, const Ponto *p2) {
	if (p1 == NULL || p2 == NULL) return -1.0;

	// Aplicação da fórmula da distância entre pontos
	double dist = (p1->x - p2->x)*(p1->x - p2->x) + (p1->y - p2->y)*(p1->y - p2->y);
	return sqrt(dist);
}

double point_line_distance(const Segmento *SR, const Ponto *p) {
	if (SR == NULL || p == NULL || is_degenerate(SR)) return -1.0;
	// Definição dos coeficientes da equação da reta definda pelo segmento de reta SR
	double a = SR->p2.y - SR->p1.y;
	double b = -1*(SR->p2.x - SR->p1.x);
	double c = SR->p1.y * (SR->p2.x - SR->p1.x) - SR->p1.x * (SR->p2.y - SR->p1.y);
	
	// Aplicação da fórmula da distância e ponto
	double sup = a * p->x + b * p->y + c;
	if(sup < 0) sup = -sup;
	
	return sup / sqrt((a*a) + (b*b));
}

Ponto line_intersection(const Segmento *SR1, const Segmento *SR2) {
	int posicao = lines_position(SR1, SR2);
	if (posicao == INVALIDO || posicao == PARALELOS) return create_point(NAN, NAN);

	double a1 = SR1->p2.y - SR1->p1.y;
	double b1 = -(SR1->p2.x - SR1->p1.x);
	double c1 = -SR1->p1.y * (SR1->p2.x - SR1->p1.x) + SR1->p1.x * (SR1->p2.y - SR1->p1.y);

	double a2 = SR2->p2.y - SR2->p1.y;
	double b2 = -(SR2->p2.x - SR2->p1.x);
	double c2 = -SR2->p1.y * (SR2->p2.x - SR2->p1.x) + SR2->p1.x * (SR2->p2.y - SR2->p1.y);
	
	double det = a1*b2 - a2*b1;
	
	Ponto p;
	
	p.x = (c1*b2 - c2*b1) / det;
	p.y = (a1*c2 - a2*c1) / det;
	
	return p;
}


int lines_position(const Segmento *SR1, const Segmento *SR2) {
	if (SR1 == NULL || SR2 == NULL || is_degenerate(SR1) || is_degenerate(SR2)) return INVALIDO;

	// Define o vetor que define a reta formada pelo segmento de reta SR1
	double x1 = SR1->p1.x - SR1->p2.x;
	double y1 = SR1->p1.y - SR1->p2.y;
	
	// Define o vetor que define a reta formada pelo segmento da reta SR2
	double x2 = SR2->p1.x - SR2->p2.x;
	double y2 = SR2->p1.y - SR2->p2.y;

	// Comparação entre os vetores
	if(fabs(x1*y2 - x2*y1) < EPS) return PARALELOS;
	if(fabs(x1*x2 + y1*y2) < EPS) return PERPENDICULARES;
	else return CONCORRENTES;
}
