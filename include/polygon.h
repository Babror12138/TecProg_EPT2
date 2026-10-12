#ifndef POLYGON_H
#define POLYGON_H

#include "general.h"

/**
 * @brief Estrutura de polígono no plano cartesiano.
 */
typedef struct {
	int n;
	Ponto *vertices;
} Poligono;

/**
 * @brief Criação de um polígono a partir de seu número e da lista de seus vértices.
 * @param n Número de vértices do polígono.
 * @param vertices Vetor de vértices do polígono.
 * @return Poligono contendo uma cópia dos vértices dados.
 * @pre n >= 3.
 * @pre vertices != NULL.
 * @pre vertices aponta para um vetor contendo n elementos do tipo Ponto.
 * @pre Os vértice consecutivos do vetor devem ser adjacentes no polígono.
 */
Poligono create_polygon(int n, const Ponto *vertices);

/**
 * @brief Liberação da memória usada pelos vértices de um polígono.
 * @param poligono Ponteiro para o polígono a ser liberado.
 * @pre poligono != NULL.
 * @post poligono->vertices == NULL.
 * @post poligono->n == 0.
 */
void free_polygon(Poligono *poligono);

/**
 * @brief Calcula a área do polígono com sinal.
 * @param poligono Ponteiro para o polígono.
 * @return Área do polígono com sinal positivo, se os vértices estiverem ordenados em sentido anti-horário, e negativo caso contrário.
 * @pre poligono != NULL.
 */
double signed_area(const Poligono *poligono);

/**
 * @brief Calcula a área do polígono, em módulo.
 * @param poligono Ponteiro para o polígono.
 * @return Área do polígono.
 * @pre poligono != NULL.
 */
double polygon_area(const Poligono *poligono);

/**
 * @brief Calcula o perímetro do polígono.
 * @param poligono Ponteiro para o polígono.
 * @return Perímetro do polígono.
 * @pre poligono != NULL.
 */
double polygon_perimeter(const Poligono *poligono);

/**
 * @brief Calcula o centroide do polígono.
 * @param poligono Ponteiro para o polígono.
 * @return Ponto correspondente ao centroide do polígono.
 * @pre poligono != NULL.
 */
Ponto polygon_centroid(const Poligono *poligono);

/**
 * @brief Verifica se um polígono é convexo.
 * @param poligono Ponteiro para o polígono a ser verificado.
 * @return 1 se o polígono for convexo; 0 caso contrário.
 * @pre poligono != NULL.
 */
int is_convex(const Poligono *poligono);

/**
 * @brief Verifica se um polígono contém um ponto.
 * @param poligono Ponteiro para o polígono a ser verificado.
 * @param ponto Ponteiro para o ponto a ser verificado.
 * @return 1 se o ponto estiver dentro do polígono ou sobre os lados; 0 se o ponto estiver fora do polígono.
 * @pre poligono != NULL.
 * @pre ponto != NULL.
 */
int polygon_contains_point(const Poligono *poligono, Ponto *ponto);

#endif
