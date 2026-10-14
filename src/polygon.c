#include "polygon.h"
#include "geometry2d.h"
#include <math.h>
#include <stdlib.h>

Poligono create_polygon(int n, const Ponto *vertices) {
	Poligono poligono;
	
	if (n < 3 || vertices == NULL) {
		poligono.n = 0;
		poligono.vertices = NULL;
		return poligono;
	}

	poligono.n = n;
	poligono.vertices = malloc(n * sizeof(Ponto));

	if (poligono.vertices == NULL) {
		poligono.n = 0;
	} else {
		for (int i = 0; i < n; i++) {
			poligono.vertices[i] = vertices[i];
		}
	}

	return poligono;
}

void free_polygon(Poligono *poligono) {
	if (poligono == NULL) return;
	free(poligono->vertices);
	poligono->vertices = NULL;
	poligono->n = 0;
}

double signed_area(const Poligono *poligono) {
	if (poligono == NULL || poligono->vertices == NULL || poligono->n < 3) {
		return 0.0;
	}

	int n = poligono->n;
	Ponto *vertices = poligono->vertices;

	double area = 0;

	for (int i = 0; i < n - 1; i++) {
		area += vertices[i].x * vertices[i+1].y - vertices[i+1].x * vertices[i].y;
	}
	area += vertices[n-1].x * vertices[0].y - vertices[0].x * vertices[n-1].y;

	area = area / 2;

	return area;
}

double polygon_area(const Poligono *poligono) {
	return fabs(signed_area(poligono));
}

double polygon_perimeter(const Poligono *poligono) {
	if (poligono == NULL || poligono->vertices == NULL || poligono->n < 3) {
		return 0.0;
	}

	int n = poligono->n;
	Ponto *vertices = poligono->vertices;

	double perimeter = 0;

	for (int i = 0; i < n - 1; i++) {
		perimeter += point_distance(&vertices[i], &vertices[i+1]);
	}
	perimeter += point_distance(&vertices[n-1], &vertices[0]);

	return perimeter;
}

Ponto polygon_centroid(const Poligono *poligono) {
	Ponto centroid;
	centroid.x = 0;
	centroid.y = 0;

	if (poligono == NULL || poligono->vertices == NULL || poligono->n < 3) {
		return centroid;
	}

	
	int n = poligono->n;
	Ponto *vertices = poligono->vertices;

	double Cx = 0;
	double Cy = 0;
	double A = signed_area(poligono);

	if (fabs(A) < GEOM_EPS) {
		return centroid;
	}

	for (int i = 0; i < n - 1; i++) {
		Cx += (vertices[i].x + vertices[i+1].x) * (vertices[i].x * vertices[i+1].y - vertices[i+1].x * vertices[i].y);
		Cy += (vertices[i].y + vertices[i+1].y) * (vertices[i].x * vertices[i+1].y - vertices[i+1].x * vertices[i].y);
	}
	Cx += (vertices[n-1].x + vertices[0].x) * (vertices[n-1].x * vertices[0].y - vertices[0].x * vertices[n-1].y);
	Cy += (vertices[n-1].y + vertices[0].y) * (vertices[n-1].x * vertices[0].y - vertices[0].x * vertices[n-1].y);

	centroid.x = Cx / (6 * A);
	centroid.y = Cy / (6 * A);

	return centroid;
}

int is_convex(const Poligono *poligono) {
	if (poligono == NULL || poligono->vertices == NULL || poligono->n < 3) {
		return 0;
	}

	int n = poligono->n;
	Ponto *vertices = poligono->vertices;

	int orientation = 0;

	for (int i = 0; i < n; i++) {
		Ponto p1 = vertices[i];
		Ponto p2 = vertices[(i+1) % n];
		Ponto p3 = vertices[(i+2) % n];

		double cross = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);

		// fix: se os pontos forem colineares, segue para o próximo, sem mudar a orientação
		if (fabs(cross) < GEOM_EPS) continue;

		if (orientation == 0) {
			if (cross > 0) {
				orientation = 1;
			} else {
				orientation = -1;
			}
		} else if (orientation == 1 && cross < 0) {
			return 0;
		} else if (orientation == -1 && cross > 0) {
			return 0;
		}
	}

	return 1;
}

int polygon_contains_point(const Poligono *poligono, const Ponto *ponto) {
	if (poligono == NULL || poligono->vertices == NULL || poligono->n < 3 || ponto == NULL) {
		return 0;
	}

	int n = poligono->n;
	Ponto *vertices = poligono->vertices;

	int inter = 0;
	
	for (int i = 0; i < n; i++) {
		Ponto p1 = vertices[i];
		Ponto p2 = vertices[(i+1) % n];

		Segmento s = create_segment(p1, p2);

		if (point_on_segment(&s, ponto)) {
			return 1;
		}

		if ((p1.y > ponto->y) != (p2.y > ponto->y)) {
			double x_inter = p1.x + (ponto->y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);

			if (ponto->x < x_inter) {
				inter++;
			}
		}
	}

	return (inter % 2 == 1);
}
