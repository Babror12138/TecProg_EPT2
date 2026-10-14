#include <stdlib.h>
#include "test_utils.h"
#include "spatial.h"

#define TAM(v) ((int)(sizeof(v) / sizeof((v)[0])))

// quadrado de lado 4 usado como base dos testes
static const Ponto QUADRADO_A[] = { {0, 0}, {4, 0}, {4, 4}, {0, 4} };

// quadrado de lado 4 que pega o canto superior direito do quadrado A
static const Ponto QUADRADO_B[] = { {2, 2}, {6, 2}, {6, 6}, {2, 6} };

// quadrado dentro do quadrado A
static const Ponto QUADRADO_DENTRO[] = { {1, 1}, {2, 1}, {2, 2}, {1, 2} };

// quadrado completamente fora de A
static const Ponto QUADRADO_LONGE[] = { {10, 10}, {12, 10}, {12, 12}, {10, 12} };

// função utilidade que retorna 1 se o ponto (x, y) está no vetor
static int contemPonto(const Ponto *pontos, int n, double x, double y) {
    for (int i = 0; i < n; i++) {
        if (fabs(pontos[i].x - x) <= EPS && fabs(pontos[i].y - y) <= EPS) return 1;
    }
    return 0;
}

static void test_closest_pair(void) {
    // testa conjunto com um único par mais próximo
    Ponto pontos[] = { {0, 0}, {5, 5}, {1, 0}, {9, 9} };
    Ponto *par = closest_pair(pontos, TAM(pontos));
    ASSERT_TRUE(par != NULL);
    ASSERT_TRUE(contemPonto(par, 2, 0, 0));
    ASSERT_TRUE(contemPonto(par, 2, 1, 0));
    free(par);

    // testa conjunto com só dois pontos
    par = closest_pair(pontos, 2);
    ASSERT_TRUE(par != NULL);
    ASSERT_TRUE(contemPonto(par, 2, 0, 0));
    ASSERT_TRUE(contemPonto(par, 2, 5, 5));
    free(par);

    // testa pontos repetidos (distância zero)
    Ponto repetidos[] = { {3, 3}, {7, 1}, {3, 3} };
    par = closest_pair(repetidos, TAM(repetidos));
    ASSERT_TRUE(par != NULL);
    ASSERT_NEAR(par[0].x, par[1].x, EPS);
    ASSERT_NEAR(par[0].y, par[1].y, EPS);
    free(par);

    // testa entradas inválidas
    ASSERT_TRUE(closest_pair(pontos, 1) == NULL);
    ASSERT_TRUE(closest_pair(NULL, 4) == NULL);
}

static void test_intersection_polygon(void) {
    Poligono a = create_polygon(TAM(QUADRADO_A), QUADRADO_A);
    Poligono b = create_polygon(TAM(QUADRADO_B), QUADRADO_B);
    Poligono dentro = create_polygon(TAM(QUADRADO_DENTRO), QUADRADO_DENTRO);
    Poligono longe = create_polygon(TAM(QUADRADO_LONGE), QUADRADO_LONGE);
    int n = -1;

    // testa quadrados sobrepostos, as bordas cruzam em dois pontos
    Ponto *pontos = intersection_polygon(a, b, &n);
    ASSERT_TRUE(n == 2);
    ASSERT_TRUE(pontos != NULL);
    ASSERT_TRUE(contemPonto(pontos, n, 4, 2));
    ASSERT_TRUE(contemPonto(pontos, n, 2, 4));
    free(pontos);

    // testa polígonos sem ponto em comum
    pontos = intersection_polygon(a, longe, &n);
    ASSERT_TRUE(pontos == NULL);
    ASSERT_TRUE(n == 0);

    // testa um dentro do outro (ou seja, sem interseção nas bordas)
    pontos = intersection_polygon(a, dentro, &n);
    ASSERT_TRUE(pontos == NULL);
    ASSERT_TRUE(n == 0);

    free_polygon(&a);
    free_polygon(&b);
    free_polygon(&dentro);
    free_polygon(&longe);
}

static void test_is_polygon_inside(void) {
    Poligono a = create_polygon(TAM(QUADRADO_A), QUADRADO_A);
    Poligono b = create_polygon(TAM(QUADRADO_B), QUADRADO_B);
    Poligono dentro = create_polygon(TAM(QUADRADO_DENTRO), QUADRADO_DENTRO);
    Poligono longe = create_polygon(TAM(QUADRADO_LONGE), QUADRADO_LONGE);

    // testa que dentro está dentro de A
    ASSERT_TRUE(is_polygon_inside(dentro, a) == 1);

    // testa que A não está dentro de dentro
    ASSERT_TRUE(is_polygon_inside(a, dentro) == 0);

    // testa que A está dentro de A (pois a borda conta)
    ASSERT_TRUE(is_polygon_inside(a, a) == 1);

    // testa polígonos que não estão totalmente contidos um no outro
    ASSERT_TRUE(is_polygon_inside(a, b) == 0);
    ASSERT_TRUE(is_polygon_inside(a, longe) == 0);

    free_polygon(&a);
    free_polygon(&b);
    free_polygon(&dentro);
    free_polygon(&longe);
}

static void test_intersection_region(void) {
    Poligono a = create_polygon(TAM(QUADRADO_A), QUADRADO_A);
    Poligono b = create_polygon(TAM(QUADRADO_B), QUADRADO_B);
    Poligono dentro = create_polygon(TAM(QUADRADO_DENTRO), QUADRADO_DENTRO);
    Poligono longe = create_polygon(TAM(QUADRADO_LONGE), QUADRADO_LONGE);
    int n = -1;

    // testa quadrados sobrepostos, a região é um quadrado menor
    Poligono *regioes = intersection_region(a, b, &n);
    ASSERT_TRUE(n == 1);
    ASSERT_TRUE(regioes != NULL);
    ASSERT_TRUE(regioes[0].n == 4);
    ASSERT_NEAR(polygon_area(&regioes[0]), 4.0, EPS);
    free_polygon(&regioes[0]);
    free(regioes);

    // testa um polígono dentro do outro, a região deve ser o próprio polígono menor
    regioes = intersection_region(dentro, a, &n);
    ASSERT_TRUE(n == 1);
    ASSERT_TRUE(regioes != NULL);
    ASSERT_NEAR(polygon_area(&regioes[0]), 1.0, EPS);
    free_polygon(&regioes[0]);
    free(regioes);

    // testa que a ordem dos polígonos não muda a região
    regioes = intersection_region(a, dentro, &n);
    ASSERT_TRUE(n == 1);
    ASSERT_TRUE(regioes != NULL);
    ASSERT_NEAR(polygon_area(&regioes[0]), 1.0, EPS);
    free_polygon(&regioes[0]);
    free(regioes);

    // testa polígonos sem região em comum
    regioes = intersection_region(a, longe, &n);
    ASSERT_TRUE(regioes == NULL);
    ASSERT_TRUE(n == 0);

    free_polygon(&a);
    free_polygon(&b);
    free_polygon(&dentro);
    free_polygon(&longe);

    Ponto ta[] = { {0, 0}, {6, 0}, {3, 6} }, tb[] = { {0, 4}, {6, 4}, {3, -2} };
    Poligono t1 = create_polygon(3, ta), t2 = create_polygon(3, tb);
    regioes = intersection_region(t1, t2, &n);
    ASSERT_TRUE(n == 1);
    ASSERT_TRUE(regioes != NULL);
    ASSERT_TRUE(regioes[0].n == 6);
    ASSERT_NEAR(polygon_area(&regioes[0]), 12.0, EPS);
    free_polygon(&regioes[0]);
    free(regioes);
    free_polygon(&t1);
    free_polygon(&t2);
}

int main(void) {
    test_closest_pair();
    test_intersection_polygon();
    test_is_polygon_inside();
    test_intersection_region();
    return testReport("spatial");
}
