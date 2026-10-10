#include "polygon.h"
//#include "geometry2d.h"
#include <math.h>
#include <stdlib.h>

//placeholder
double PointDistance(Ponto p1, Ponto p2){
	double dist = (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
	return sqrt(dist);
}

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
	if (poligono == null) return;
	free(poligono->vertices);
	poligono->vertices = NULL;
	poligono->n = 0;
}

double signed_area(const Poligono *poligono) {
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
	int n = poligono->n;
	Ponto *vertices = poligono->vertices;

	double perimeter = 0;

	for (int i = 0; i < n - 1; i++) {
		perimeter += PointDistance(vertices[i], vertices[i+1]);
	}
	perimeter += PointDistance(vertices[n-1], vertices[0]);

	return perimeter;
}

Ponto polygon_centroid(const Poligono *poligono) {
	int n = poligono->n;
	Ponto *vertices = poligono->vertices;

	Ponto centroid;
	double Cx = 0;
	double Cy = 0;
	double A = signed_area(poligono);

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

}

int polygon_contains_point(const Poligono *poligono, const Ponto *ponto) {

}
