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

}

double polygon_perimeter(Poligono *poligono) {

}

Ponto polygon_centroid(Poligono *poligono) {

}

bool is_convex(Poligono *poligono) {

}

bool polygon_contains_point(Poligono *poligono, Ponto *ponto) {

}

