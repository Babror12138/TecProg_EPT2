#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include "polygon.h"

// funções auxiliares da aplicação cliente para desenhar figuras geométricas em um arquivo SVG

// cria o arquivo e escreve o cabeçalho do SVG, retornando NULL se não conseguir criar
FILE *svg_begin(const char *caminho);

// escreve o fim do SVG e fecha o arquivo
void svg_end(FILE *f);

void svg_polygon(FILE *f, const Poligono *p, const char *cor);
void svg_segment(FILE *f, Ponto a, Ponto b, const char *cor);
void svg_point(FILE *f, Ponto p, const char *cor);
void svg_text(FILE *f, Ponto p, const char *texto);

#endif