#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include "general.h"
#include "polygon.h"

/**
 * @brief Calcula o fecho convexo de um conjunto de pontos pelo algoritmo Graham Scan, em O(n log n).
 * @param n Número de pontos no conjunto.
 * @param pontos Vetor com os n pontos do conjunto. Não é modificado.
 * @return Poligono com os vértices do fecho em ordem anti-horária, começando pelo
 *         ponto de menor y (em empate, menor x), sem pontos repetidos nem colineares.
 *         Deve ser liberado com free_polygon().
 * @return Poligono vazio (n == 0 e vertices == NULL) se pontos == NULL, se n < 3
 *         ou se o conjunto tiver menos de 3 pontos não colineares.
 */
Poligono convex_hull_graham(int n, const Ponto *pontos);

/**
 * @brief Calcula o fecho convexo de um conjunto de pontos pelo algoritmo de Jarvis, em O(nm), onde m é o número de pontos no fecho.
 * @param n Número de pontos no conjunto.
 * @param pontos Vetor com os n pontos do conjunto. Não é modificado.
 * @return Poligono com os vértices do fecho em ordem anti-horária, começando pelo
 *         ponto de menor y (em empate, menor x), sem pontos repetidos nem colineares.
 *         Deve ser liberado com free_polygon().
 * @return Poligono vazio (n == 0 e vertices == NULL) se pontos == NULL, se n < 3
 *         ou se o conjunto tiver menos de 3 pontos não colineares.
 */
Poligono convex_hull_jarvis(int n, const Ponto *pontos);

/**
 * @brief Retorna a área do fecho convexo de um conjunto de pontos.
 * @param n Número de pontos no conjunto.
 * @param pontos Vetor com os n pontos do conjunto. Não é modificado.
 * @return O valor da área do fecho convexo.
 * @return 0 se pontos == NULL, se n < 3 ou se o conjunto tiver menos de 3 pontos não colineares.
 */
double convex_hull_area(int n, const Ponto *pontos);

/**
 * @brief Retorna o perímetro do fecho convexo de um conjunto de pontos.
 * @param n Número de pontos no conjunto.
 * @param pontos Vetor com os n pontos do conjunto. Não é modificado.
 * @return O valor do perímetro do fecho convexo.
 * @return 0 se pontos == NULL, se n < 3 ou se o conjunto tiver menos de 3 pontos não colineares.
 */
double convex_hull_perimeter(int n, const Ponto *pontos);

/**
 * @brief Retorna o diâmetro do fecho convexo de um conjunto de pontos.
 * @param n Número de pontos no conjunto.
 * @param pontos Vetor com os n pontos do conjunto. Não é modificado.
 * @return O valor do diâmetro do fecho convexo.
 * @return 0 se pontos == NULL, se n < 3 ou se o conjunto tiver menos de 3 pontos não colineares.
 */
double convex_hull_diameter(int n, const Ponto *pontos);

#endif
