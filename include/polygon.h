#ifndef POLYGON_H
#define POLYGON_H

#include "general.h"

/**
 * @brief Estrutura de polígono no plano cartesiano
 */
typedef struct {
	int n;
	Ponto *vertices;
} Poligono;

/**
 * @brief Criação de um polígono a partir de seu tamanho e lista de vértices
 * @param n Número de vértices do polígono
 * @param vertices Lista de vértices do polígono
 * @return Estrutura Poligono com os vértices dados
 */
Poligono create_polygon(int n, Ponto *vertices);

#endif
