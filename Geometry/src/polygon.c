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
		return poligono;
	}
	
	for (int i = 0; i < n; i++) {
		poligono.vertices[i] = vertices[i];
	}

	return poligono;
}
