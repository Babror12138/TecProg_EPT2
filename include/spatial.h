#ifndef SPATIAL
#define SPATIAL

#include "general.h"
#include "polygon.h"

/**
 * @brief Encontra o par de pontos mais próximo entre si em um conjunto. Em caso de empate qualquer par é retornado.
 * @param pontos Lista de pontos
 * @param n Tamanho da lista de pontos
 * @return O por de pontos mais próximo no conjunto se existir, caso contrário retorna NULL
 */
Ponto* closest_pair(Ponto* pontos, int n);

/**
 * @brief Encontra os pontos de interseção entre dois polígonos
 * @param A Poligono A
 * @param B Poligono B
 * @param count ponteiro para se obter o tamanho da lista de interseções
 * @return A lista contendo os pontos de interseção
 */
Ponto* intersection_polygon(Poligono A, Poligono B, int *length);

/**
 * @brief Encontra as regiões de interseção entre dois polígonos
 * @param A Poligono A
 * @param B Poligono B
 * @return A lista contendo as regiões de interseção
 */
Poligono* intersection_region(Poligono A, Poligono B, int *length);

/**
 * @brief Verifica se o poligono A está totalmente contido em B
 * @param A Poligono A
 * @param B Poligono B
 * @return 1 se A está contido em B, caso contrário 0 é retornado
 */
int is_polygon_inside(Poligono A, Poligono B);

#endif
