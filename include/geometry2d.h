#ifndef GEOMETRY2D_H
#define GEOMETRY2D_H

#include "general.h"

#define INVALIDO -1
#define PARALELOS 0
#define PERPENDICULARES 1
#define CONCORRENTES 2

/**
 * @brief Identificar se um ponto p está sobre o segmento de reta SR
 * @param SR Ponteiro do segmento de reta que se deseja analisar, do tipo Segmento, criado no módulo general
 * @param p Ponteiro do ponto que se deseja analisar, do tipo Ponto, criado no módulo general
 * @return Inteiro 1 caso o ponto está sobre o segmento de reta, e 0 caso contrário ou se algum ponteiro for NULL
 */
int point_on_segment(const Segmento *SR, const Ponto *p);

/**
 * @brief Calcular a distância entre dois pontos
 * @param p1 Ponteiro do primeiro ponto, do tipo Ponto, criado no módulo general
 * @param p2 Ponteiro do segundo ponto, do tipo Ponto, criado no módulo general
 * @return Valor, em double, da distância entre os pontos p1 e p2
 * @return -1, se algum ponteiro for NULL
 */
double point_distance(const Ponto *p1, const Ponto *p2);

/**
 * @brief Calcular a distância entre uma reta, definida a partir de um segmento de reta, e um ponto
 * @param SR Ponteiro do segmento de reta que serve de base para a construção da reta que passa sobre ele, do tipo Segmento, criado no módulo geral
 * @param p Ponteiro do ponto que se deseja analisar, do tipo Ponto, criado no módulo general
 * @return Valor, em double, da distância entre a reta definida pelo SR e ponto p1
 * @return -1, se algum ponteiro for NULL ou se os dois pontos do segmento forem iguais
 */
double point_line_distance(const Segmento *SR, const Ponto *p);

/**
 * @brief Encontrar o ponto de intersecção entre duas retas definidas por dois segmentos de reta
 * @param SR1 Ponteiro do segmento de reta que define a primeira reta, do tipo Segmento, criado no módulo geral
 * @param SR2 Ponteiro do segmento de reta que define a segunda reta, do tipo Segmento, criado no módulo geral
 * @return p o ponto, do tipo Ponto, criado no módulo geral, que representa o ponto de intersecção entre as duas retas
 * @return Ponto(NAN, NAN) se as retas forem paralelas ou se a entrada for inválida
 */
Ponto line_intersection(const Segmento *SR1, const Segmento *SR2);

/**
 * @brief Identificar a posição relativa das duas retas, definidas a partir de segmentos de reta SR1 e SR2
 * @param SR1 Ponteiro do segmento de reta que serve de base para construção da reta que passa por ele, do tipo Segmento, criado no módulo geral
 * @param SR2 Ponteiro do segmento de reta que serve de base para construção da reta que passa por ele, do tipo Segmento, criado no módulo geral
 * @return Inteiro 0 caso são paralelos, 1 caso são perpendiculares e 2 caso são concorrentes
 * @return INVALIDO se algum ponteiro for NULL ou algum segmento tiver os dois pontos iguais
 */
int lines_position(const Segmento *SR1, const Segmento *SR2);

#endif
