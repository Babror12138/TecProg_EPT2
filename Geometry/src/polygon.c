#include "polygon.h"
#include <stdlib.h>

Poligono create_polygon(int n, Ponto *vertices) {
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
	free(poligono->vertices);
	poligono->vertices = NULL;
	poligono->n = 0;
}

double polygon_area(Poligono *poligono) {
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

double polygon_perimeter(Poligono *poligono) {

}

Ponto polygon_centroid(Poligono *poligono) {
	int n = poligono->n;
	Ponto *vertices = poligono->vertices;

	Ponto centroid;
	centroid.x = 0;
	centroid.y = 0;

	for (int i = 0; i < n; i++) {
		centroid.x += vertices[i].x;
		centroid.y += vertices[i].x;
	}

	centroid.x = centroid.x / 2;
	centroid.y = centroid.y / 2;

	return centroid;
}

bool is_convex(Poligono *poligono) {

}

bool polygon_contains_point(Poligono *poligono, Ponto *ponto) {

}
