#ifndef __GENERAL_H__
#define __GENERAL_H__

//Cria estrutura ponto, com as coordenadas em (x, y) em um plano cartesiano
typedef struct{
	float x;
	float y;
} Ponto;

//Estutura Segmento, que recebe dois pontos geradores de um segmento de reta
typedef struct{
	Ponto *p1;
	Ponto *p2;
} Segmento;

/**
*@brief Criação de um ponto, a partir das suas coordenadas x e y
*@param Px a coordenada em x do ponto
*@param Py a coordenada em y do ponto
*@post Em sucesso, retorna uma estrutura Ponto com as coordeadas x e y
*@return Estrutura Ponto p
*/
Ponto CreatPoint(float x, float y);

/**
*@brief Criação de um segmento de reta, a partir das coordenadas dos dois pontos geradores
*@param point1 o primeiro ponto gerador, a partir do struct Ponto criado anteriormente
*@param point2 o segundo ponto gerador, a partir do struct Ponto criado anteriormente
*@post Em sucesso, retorna uma estrutura Segmento, que representa um segmento de reta formado por dois pontos lidos na entrada
*@return Estrutura Segmento SR
*/
Segmento CreatSegment(Ponto *point1, Ponto *point2);

#endif
