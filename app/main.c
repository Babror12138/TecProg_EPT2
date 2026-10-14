#include <stdio.h>
#include <stdlib.h>
#include "geometry2d.h"
#include "polygon.h"
#include "convex_hull.h"
#include "spatial.h"
#include "svg.h"

#define TAM(v) ((int)(sizeof(v) / sizeof((v)[0])))

// nuvem de pontos usada nas demonstrações de fecho convexo e de par mais próximo
static Ponto NUVEM[] = {
    {20, 1}, {27, 0}, {28, 5}, {24, 8}, {20, 6},
    {23, 3}, {25, 5}, {22, 5}, {25.5, 4.5}, {24, 1}
};

// demonstração do módulo geometry2d: posição relativa, interseção de retas e distâncias
static void demo_geometry2d(FILE *svg) {
    Segmento r = create_segment(create_point(0, 1), create_point(8, 5));
    Segmento s = create_segment(create_point(1, 7), create_point(6, 0));
    Ponto p = create_point(7, 1);

    printf("\n== geometry2d ==\n");

    int posicao = lines_position(&r, &s);
    printf("Posição relativa de r e s: %s\n",
           posicao == PARALELOS ? "paralelas" :
           posicao == PERPENDICULARES ? "perpendiculares" :
           posicao == CONCORRENTES ? "concorrentes" : "inválida");

    svg_segment(svg, r.p1, r.p2, "steelblue");
    svg_segment(svg, s.p1, s.p2, "seagreen");
    svg_point(svg, p, "black");

    if (posicao == PERPENDICULARES || posicao == CONCORRENTES) {
        Ponto x = line_intersection(&r, &s);
        printf("Interseção das retas: (%.2f, %.2f)\n", x.x, x.y);
        printf("A interseção está sobre o segmento r? %s\n", point_on_segment(&r, &x) ? "sim" : "não");
        svg_point(svg, x, "crimson");
    }

    printf("Comprimento de r: %.2f\n", point_distance(&r.p1, &r.p2));
    printf("Distância de p = (%.0f, %.0f) até a reta r: %.2f\n", p.x, p.y, point_line_distance(&r, &p));

    svg_text(svg, create_point(0, 9), "geometry2d: retas e pontos");
}

// demonstração do módulo polygon: medidas de um polígono não convexo em forma de L
static void demo_polygon(FILE *svg) {
    Ponto vertices[] = { {10, 0}, {18, 0}, {18, 3}, {13, 3}, {13, 8}, {10, 8} };
    Ponto dentro = create_point(11, 6), fora = create_point(16, 6);

    printf("\n== polygon ==\n");

    Poligono p = create_polygon(TAM(vertices), vertices);
    if (p.n == 0) {
        fprintf(stderr, "Erro: não foi possível criar o polígono\n");
        return;
    }

    Ponto c = polygon_centroid(&p);
    printf("Área: %.2f (com sinal: %.2f)\n", polygon_area(&p), signed_area(&p));
    printf("Perímetro: %.2f\n", polygon_perimeter(&p));
    printf("Centroide: (%.2f, %.2f)\n", c.x, c.y);
    printf("É convexo? %s\n", is_convex(&p) ? "sim" : "não");
    printf("Contém (%.0f, %.0f)? %s\n", dentro.x, dentro.y, polygon_contains_point(&p, &dentro) ? "sim" : "não");
    printf("Contém (%.0f, %.0f)? %s\n", fora.x, fora.y, polygon_contains_point(&p, &fora) ? "sim" : "não");

    svg_polygon(svg, &p, "darkorange");
    svg_point(svg, c, "crimson");
    svg_point(svg, dentro, "seagreen");
    svg_point(svg, fora, "black");
    svg_text(svg, create_point(10, 9), "polygon: centroide e pontos");

    free_polygon(&p);
}

// demonstração do módulo convex_hull: fecho convexo da nuvem pelos dois algoritmos
static void demo_convex_hull(FILE *svg) {
    printf("\n== convex_hull ==\n");

    Poligono graham = convex_hull_graham(TAM(NUVEM), NUVEM);
    Poligono jarvis = convex_hull_jarvis(TAM(NUVEM), NUVEM);
    if (graham.n == 0 || jarvis.n == 0) {
        fprintf(stderr, "Erro: não foi possível calcular o fecho convexo\n");
        free_polygon(&graham);
        free_polygon(&jarvis);
        return;
    }

    printf("Pontos na nuvem: %d\n", TAM(NUVEM));
    printf("Vértices do fecho: %d (Graham) e %d (Jarvis)\n", graham.n, jarvis.n);
    printf("Área do fecho: %.2f\n", convex_hull_area(TAM(NUVEM), NUVEM));
    printf("Perímetro do fecho: %.2f\n", convex_hull_perimeter(TAM(NUVEM), NUVEM));
    printf("Diâmetro do fecho: %.2f\n", convex_hull_diameter(TAM(NUVEM), NUVEM));

    svg_polygon(svg, &graham, "steelblue");
    for (int i = 0; i < TAM(NUVEM); i++) svg_point(svg, NUVEM[i], "black");
    svg_text(svg, create_point(20, 9), "convex_hull e par mais próximo");

    free_polygon(&graham);
    free_polygon(&jarvis);
}

// demonstração do módulo spatial: par mais próximo da nuvem e interseção de dois triângulos
static void demo_spatial(FILE *svg) {
    Ponto va[] = { {30, 2}, {38, 2}, {34, 8} }, vb[] = { {30, 6}, {38, 6}, {34, 0} };

    printf("\n== spatial ==\n");

    Ponto *par = closest_pair(TAM(NUVEM), NUVEM);
    if (par != NULL) {
        printf("Par mais próximo da nuvem: (%.1f, %.1f) e (%.1f, %.1f), distância %.2f\n",
               par[0].x, par[0].y, par[1].x, par[1].y, point_distance(&par[0], &par[1]));
        svg_segment(svg, par[0], par[1], "crimson");
        svg_point(svg, par[0], "crimson");
        svg_point(svg, par[1], "crimson");
        free(par);
    }

    Poligono a = create_polygon(TAM(va), va), b = create_polygon(TAM(vb), vb);
    svg_polygon(svg, &a, "steelblue");
    svg_polygon(svg, &b, "seagreen");

    int n_pontos = 0, n_regioes = 0;
    Ponto *pontos = intersection_polygon(a, b, &n_pontos);
    Poligono *regioes = intersection_region(a, b, &n_regioes);

    printf("Triângulo A está dentro de B? %s\n", is_polygon_inside(a, b) ? "sim" : "não");
    printf("Pontos em que as bordas se cruzam: %d\n", n_pontos);
    printf("Regiões de interseção: %d\n", n_regioes);

    for (int i = 0; i < n_regioes; i++) {
        printf("Região %d: %d vértices, área %.2f, dentro de A? %s\n", i + 1, regioes[i].n,
               polygon_area(&regioes[i]), is_polygon_inside(regioes[i], a) ? "sim" : "não");
        svg_polygon(svg, &regioes[i], "crimson");
        free_polygon(&regioes[i]);
    }
    for (int i = 0; i < n_pontos; i++) svg_point(svg, pontos[i], "black");
    svg_text(svg, create_point(30, 9), "spatial: interseção de polígonos");

    free(regioes);
    free(pontos);
    free_polygon(&a);
    free_polygon(&b);
}

int main(int argc, char **argv) {
    // se houver argumento usar ele, senão usar o default demo.svg
    const char *caminho = argc > 1 ? argv[1] : "demo.svg";

    FILE *svg = svg_begin(caminho);
    if (svg == NULL) {
        fprintf(stderr, "Erro: não foi possível criar o arquivo %s\n", caminho);
        return 1;
    }

    printf("Demonstração da libgeom\n");

    demo_geometry2d(svg);
    demo_polygon(svg);
    demo_convex_hull(svg);
    demo_spatial(svg);

    svg_end(svg);
    printf("\nFigura gerada em %s\n", caminho);

    return 0;
}
