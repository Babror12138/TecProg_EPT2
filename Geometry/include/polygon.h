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
 * @pre n >= 3
 * @pre vertices != NULL
 * @pre vertices aponta para um vetor com n elementos do tipo Ponto
 */
Poligono create_polygon(int n, Ponto *vertices);

/**
 * @brief Calcula a área do polígono
 * @param poligono Ponteiro para o polígono 
 * @return Área do polígono
 * @pre poligono != NULL
 */
double polygon_area(Poligono* poligono);

/**
 * @brief Calcula o perímetro do polígono
 * @param poligono Ponteiro para o polígono 
 * @return Perímetro do polígono
 * @pre poligono != NULL
 */
double polygon_perimeter(Poligono* poligono);

/**
 * @brief Calcula o centroide do polígono
 * @param poligono Ponteiro para o polígono 
 * @return Ponto correspondente ao centroide do polígono
 * @pre poligono != NULL
 */
Ponto polygon_centroid(Poligono* poligono);

/**
 * @brief Verifica se um polígono é convexo
 * @param poligono Ponteiro para o polígono a ser verificado
 * @return true se o polígono for convexo;
 * 	   false caso contrário
 * @pre poligono != NULL
 */
bool is_convex(Poligono* poligono);

/**
 * @brief Verifica se um polígono contém um ponto
 * @param poligono Ponteiro para o polígono a ser verificado
 * @param ponto Ponteiro para o ponto a ser verificado
 * @return true se o ponto estiver dentro do polígono ou sobre os lados;
 * 	   false se o ponto estiver fora do polígono
 * @pre poligono != NULL
 * @pre ponto != NULL
 */
bool polygon_contains_point(Poligono* poligono, Ponto* ponto);

#endif
