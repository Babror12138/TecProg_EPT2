#include "general.h"

#ifndef __GEOMETRY2D_H__
#define __GEOMETRY2D_H__

/**
*@brief Identificar se um ponto p está sobre o segmento de reta SR
*@param SR ponteiro do segmento de reta que se deseja analisar, do tipo Segmento, criado no módulo general
*@param p ponteiro do ponto que se deseja analisar, do tipo Ponto, criado no módulo general
*@pre Os dois pontos que forma o segmento de reta SR não podem ser iguais
*@return Inteiro 1 caso o ponto está sobre o segmento de reta, e 0 caso contrário
*/
int PointOnSegment(Segmento *SR, Ponto *p);

/**
*@brief Calcular a distância entre dois pontos
*@param p1 ponteiro do primeiro ponto, do tipo Ponto, criado no módulo general
*@param p2 ponteiro do segundo ponto, do tipo Ponto, criado no módulo general
*@return Valor, em double, da distância entre os pontos p1 e p2
*/
double PointDistance(Ponto *p1, Ponto *p2);

/**
*@brief Calcular a distância entre uma reta, definida a partir de um segmento de reta, e um ponto
*@param SR ponteiro do segmento de reta que serve de base para a construção da reta que passa sobre ele, do tipo Segmento, criado no módulo geral
*@param p ponteiro do ponto que se deseja analisar, do tipo Ponto, criado no módulo general
*@pre Os dois pontos que definem o segmento de reta não podem ser iguais
*@return Valor, em double, da distância entre a reta definida pelo SR e ponto p1
*/
double PointLineDistance(Segmento *SR, Ponto *p);

/**
*@brief Encontrar o ponto de intersecção entre duas retas definidas por dois segmentos de reta
*@param SR1 ponteiro do segmento de reta que define a primeira reta, do tipo Segmento, criado no módulo geral
*@param SR2 ponteiro do segmento de reta que define a segunda reta, do tipo Segmento, criado no módulo geral
*@pre Os respectivos dois pontos que definem cada segmento de reta não devem ser iguais
*@pre Os dois segmentos de reta não podem ser paralelas, a saída da função LinesPosition com SR1 e SR2 não pode ser 0
*@return p o ponto, do tipo Ponto, criado no módulo geral, que representa o ponto de intersecção entre as duas retas
*/
Ponto LineIntersection(Segmento *SR1, Segmento *SR2);

/**
*@brief Identificar a posição relativa das duas retas, definidas a partir de segmentos de reta SR1 e SR2
*@param SR1 ponteiro do segmento de reta que serve de base para construção da reta que passa por ele, do tipo Segmento, criado no módulo geral
*@param SR2 ponteiro do segmento de reta que serve de base para construção da reta que passa por ele, do tipo Segmento, criado no módulo geral
*@pre Os dois pontos que forma os segmentos de reta SR1 e SR2 não podem ser iguais
*@return Inteiro 0 caso são paralelos, 1 caso são perpendiculares e 2 caso são concorrentes
*/
int LinesPosition(Segmento *SR1, Segmento *SR2);

#endif
