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
Ponto* closestPair(Ponto* pontos, int n);

Ponto* intersectionRegion(Poligono A, Poligono B);

#endif
