#ifndef SPATIAL_H
#define SPATIAL_H

#include "general.h"
#include "polygon.h"

/**
 * @brief Encontra o par de pontos mais próximo entre si em um conjunto. Em caso de empate qualquer par é retornado.
 * @param n Tamanho da lista de pontos
 * @param pontos Lista de pontos
 * @return Vetor alocado com os dois pontos mais próximos, o chamados deve liberar com free()
 * @return NULL se pontos == NULL, se n < 2 ou se faltar memória
 */
Ponto* closest_pair(int n, const Ponto* pontos);

/**
 * @brief Encontra os pontos de interseção entre dois polígonos
 * @param A Poligono A
 * @param B Poligono B
 * @param length ponteiro que recebe a quantidade de pontos da interseção
 * @return Vetor alocado com *length pontos, sem repetição, que deve ser liberado com free()
 * @return NULL, com *length == 0, se as bordas não se cruzarem, se algum polígono for inválido,
 *         ou se faltar memória. E NULL se length == NULL.
 */
Ponto* intersection_polygon(Poligono A, Poligono B, int *length);

/**
 * @brief Encontra as regiões de interseção entre dois polígonos
 * @param A Poligono A
 * @param B Poligono B
 * @param length Ponteiro que recebe a quantidade de regiões
 * @return Vetor alocado com *length polígonos. Cada um deve ser liberado depois com free_polygon()
 *         e o vetor deve ser liberado com free()
 * @return NULL, com *length == 0, se não houver região em comum, se algum polígono for inválido ou
 *         se faltar memória. E NULL se length == NULL
 * @note Limitação: se um vértice do polígono cair na borda do outro ou se houver arestas sobrepostas,
 *       a região nem sempre será considerada
 */
Poligono* intersection_region(Poligono A, Poligono B, int *length);

/**
 * @brief Verifica se o poligono A está totalmente contido em B
 * @param A Poligono A
 * @param B Poligono B
 * @return 1 se todos vértices de A estão dentro ou na borda de B e 0 caso contrário ou se A ou B for inválido
 * @note A verificação usa apenas os vértices de A, portanto só é exata quando B é convexo
 */
int is_polygon_inside(Poligono A, Poligono B);

#endif
