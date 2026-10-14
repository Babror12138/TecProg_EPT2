#ifndef GENERAL_H
#define GENERAL_H

#include <math.h>
#include <stdlib.h>

// erro de tolerância para pontos flutuantes
#define GEOM_EPS 1e-6

/**
 * @brief Cria estrutura ponto, com as coordenadas em (x, y)
 */
typedef struct {
	double x;
	double y;
} Ponto;

/**
 * @brief Cria estrutura segmento de reta, a partir de dois pontos
 */
typedef struct {
	Ponto p1;
	Ponto p2;
} Segmento;

/**
 * @brief Criação de um ponto, a partir das suas coordenadas x e y
 * @param x A coordenada em x do ponto
 * @param y A coordenada em y do ponto
 * @post Em sucesso, retorna uma estrutura Ponto com as coordeadas x e y
 * @return Estrutura Ponto p
 */
Ponto create_point(double x, double y);

/**
 * @brief Criação de um segmento de reta, a partir das coordenadas dos dois pontos geradores
 * @param p1 O primeiro ponto gerador, a partir do struct Ponto criado anteriormente
 * @param p2 O segundo ponto gerador, a partir do struct Ponto criado anteriormente
 * @post Em sucesso, retorna uma estrutura Segmento, que representa um segmento de reta formado por dois pontos lidos na entrada
 * @return Estrutura Segmento SR
 */
Segmento create_segment(Ponto p1, Ponto p2);

#endif
