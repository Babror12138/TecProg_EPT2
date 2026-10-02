#include <stdio.h>
#include "general.h"

/**
*@brief Criação de um ponto, a partir das suas coordenadas x e y
*@param Px a coordenada em x do ponto
*@param Py a coordenada em y do ponto
*@post Em sucesso, retorna uma estrutura Ponto com as coordeadas x e y
*@return Estrutura Ponto p
*/
Ponto CreatPoint(float Px, float Py){
	Ponto p;
	p.x = Px;
	p.y = Py;

	return p;
}

/**
*@brief Criação de um segmento de reta, a partir das coordenadas dos dois pontos geradores
*@param Px a coordenada em x do primeiro ponto
*@param Py a coordenada em y do primeiro ponto
*@param Pnx a coordenada em x do segundo ponto
*@param Pny a coordenada em y do segundo ponto
*@post Em sucesso, retorna uma estrutura Segmento, que representa um segmento de reta formado por dois pontos lidos na entrada
*@return Estrutura Segmento SR
*/
Segmento CreatSegment(float Px, float Py, float Pnx, float Pny){
	Segmento SR;
	SR.x = Px;
	SR.y = Py;
	SR.nx = Pnx;
	SR.ny = Pny;
	
	return SR;
}

/**
*@brief Criação de um segmento de reta, a partir das coordenadas dos dois pontos geradores
*@param point1 o primeiro ponto gerador, a partir do struct Ponto criado anteriormente
*@param point2 o segundo ponto gerador, a partir do struct Ponto criado anteriormente
*@post Em sucesso, retorna uma estrutura Segmento, que representa um segmento de reta formado por dois pontos lidos na entrada
*@return Estrutura Segmento SR
*/
/*
Segmento CreatSegment(Ponto point1, Ponto point2){
	Segmento SR;
	SR.p1 = point1;
	SR.p2 = point2;
	
	return SR;
}
*/+
