#ifndef __GENERAL_H__
#define __GENERAL_H__

//Cria estrutura ponto, com as coordenadas em (x, y)
typedef struct{
	float x;
	float y;
} Ponto;

//Cria estrutura segmento de reta, com as coordenadas dos dois pontos (x, y) e (nx, ny)
typedef struct{
	float x;
	float y;
	float nx;
	float ny;
} Segmento;

/*
typedef struct{
	Ponto p1;
	Ponto p2;
} Segmento;
*/

//Funções para criação de Ponto e Segmento de reta
Ponto CreatPoint(float x, float y);
Segmento CreatSegment(float x, float y, float nx, float xy);
//Segmento CreatSegment(Ponto p1, Ponto p2);

#endif
