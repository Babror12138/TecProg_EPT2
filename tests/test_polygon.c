#include "test_utils.h"
#include "polygon.h"

#define TAM(v) ((int)(sizeof(v) / sizeof((v)[0])))

// quadrado de lado 4 em ordem horária e anti-horária
static const Ponto QUADRADO[] = { {0, 0}, {4, 0}, {4, 4}, {0, 4} };
static const Ponto QUADRADO_HORARIO[] = { {0, 0}, {0, 4}, {4, 4}, {4, 0} };

// triângulo retângulo 3-4-5
static const Ponto TRIANGULO[] = { {0, 0}, {4, 0}, {0, 3} };

// polígono côncavo
static const Ponto CONCAVO[] = { {0, 0}, {4, 0}, {2, 1}, {4, 4}, {0, 4} };

// mesmo quadrado, mas com um vértice dentro de uma aresta
static const Ponto QUADRADO_COLINEAR[] = { {0, 0}, {2, 0}, {4, 0}, {4, 4}, {0, 4} };

static void test_create_polygon(void) {
    // testa criação de polígono válido
    Poligono p = create_polygon(TAM(QUADRADO), QUADRADO);
    ASSERT_TRUE(p.n == 4);
    ASSERT_TRUE(p.vertices != NULL);
    ASSERT_NEAR(p.vertices[2].x, 4.0, GEOM_EPS);
    ASSERT_NEAR(p.vertices[2].y, 4.0, GEOM_EPS);
    free_polygon(&p);

    // testa se o polígono guarda uma cópia dos vértices
    Ponto vertices[] = { {0, 0}, {4, 0}, {0, 3} };
    p = create_polygon(3, vertices);
    vertices[0].x = 99;
    ASSERT_NEAR(p.vertices[0].x, 0.0, GEOM_EPS);
    free_polygon(&p);

    // testa menos de 3 vértices
    p = create_polygon(2, QUADRADO);
    ASSERT_TRUE(p.n == 0 && p.vertices == NULL);

    // testa vetor nulo
    p = create_polygon(4, NULL);
    ASSERT_TRUE(p.n == 0 && p.vertices == NULL);
}

static void test_free_polygon(void) {
    // testa se o polígono fica vazio depois de liberado
    Poligono p = create_polygon(TAM(QUADRADO), QUADRADO);
    free_polygon(&p);
    ASSERT_TRUE(p.n == 0 && p.vertices == NULL);

    // testa casos degenerados (liberar duas vezes ou liberar NULL)
    // para ter certeza que não gera erro
    free_polygon(&p);
    free_polygon(NULL);
    ASSERT_TRUE(p.n == 0 && p.vertices == NULL);
}

static void test_signed_area(void) {
    // sentido anti-horário dá área positiva
    Poligono p = create_polygon(TAM(QUADRADO), QUADRADO);
    ASSERT_NEAR(signed_area(&p), 16.0, GEOM_EPS);
    free_polygon(&p);

    // sentido horário dá área negativa
    p = create_polygon(TAM(QUADRADO_HORARIO), QUADRADO_HORARIO);
    ASSERT_NEAR(signed_area(&p), -16.0, GEOM_EPS);
    free_polygon(&p);

    // testa entrada inválida
    ASSERT_NEAR(signed_area(NULL), 0.0, GEOM_EPS);
}

static void test_polygon_area(void) {
    Poligono p = create_polygon(TAM(QUADRADO), QUADRADO);
    ASSERT_NEAR(polygon_area(&p), 16.0, GEOM_EPS);
    free_polygon(&p);

    // testa que a área não depende da ordem dos vértices
    p = create_polygon(TAM(QUADRADO_HORARIO), QUADRADO_HORARIO);
    ASSERT_NEAR(polygon_area(&p), 16.0, GEOM_EPS);
    free_polygon(&p);

    p = create_polygon(TAM(TRIANGULO), TRIANGULO);
    ASSERT_NEAR(polygon_area(&p), 6.0, GEOM_EPS);
    free_polygon(&p);

    // testa entradas inválidas (polígono nulo e polígono vazio)
    ASSERT_NEAR(polygon_area(NULL), 0.0, GEOM_EPS);
    p = create_polygon(2, QUADRADO);
    ASSERT_NEAR(polygon_area(&p), 0.0, GEOM_EPS);
}

static void test_polygon_perimeter(void) {
    // testa perímetro do quadrado
    Poligono p = create_polygon(TAM(QUADRADO), QUADRADO);
    ASSERT_NEAR(polygon_perimeter(&p), 16.0, GEOM_EPS);
    free_polygon(&p);

    // testa perímetro do triângulo 3-4-5
    p = create_polygon(TAM(TRIANGULO), TRIANGULO);
    ASSERT_NEAR(polygon_perimeter(&p), 12.0, GEOM_EPS);
    free_polygon(&p);

    // testa entrada inválida
    ASSERT_NEAR(polygon_perimeter(NULL), 0.0, GEOM_EPS);
}

static void test_polygon_centroid(void) {
    Poligono p = create_polygon(TAM(QUADRADO), QUADRADO);
    Ponto c = polygon_centroid(&p);
    ASSERT_NEAR(c.x, 2.0, GEOM_EPS);
    ASSERT_NEAR(c.y, 2.0, GEOM_EPS);
    free_polygon(&p);

    // testa que o centróide não depende da ordem dos vértices
    p = create_polygon(TAM(QUADRADO_HORARIO), QUADRADO_HORARIO);
    c = polygon_centroid(&p);
    ASSERT_NEAR(c.x, 2.0, GEOM_EPS);
    ASSERT_NEAR(c.y, 2.0, GEOM_EPS);
    free_polygon(&p);

    // o centroide do triângulo é a média dos vértices
    p = create_polygon(TAM(TRIANGULO), TRIANGULO);
    c = polygon_centroid(&p);
    ASSERT_NEAR(c.x, 4.0 / 3.0, GEOM_EPS);
    ASSERT_NEAR(c.y, 1.0, GEOM_EPS);
    free_polygon(&p);

    // testa entrada inválida
    c = polygon_centroid(NULL);
    ASSERT_NEAR(c.x, 0.0, GEOM_EPS);
    ASSERT_NEAR(c.y, 0.0, GEOM_EPS);
}

static void test_is_convex(void) {
    // testa polígonos convexos nos dois sentidos
    Poligono p = create_polygon(TAM(QUADRADO), QUADRADO);
    ASSERT_TRUE(is_convex(&p) == 1);
    free_polygon(&p);

    p = create_polygon(TAM(QUADRADO_HORARIO), QUADRADO_HORARIO);
    ASSERT_TRUE(is_convex(&p) == 1);
    free_polygon(&p);

    p = create_polygon(TAM(TRIANGULO), TRIANGULO);
    ASSERT_TRUE(is_convex(&p) == 1);
    free_polygon(&p);

    // testa polígono côncavo
    p = create_polygon(TAM(CONCAVO), CONCAVO);
    ASSERT_TRUE(is_convex(&p) == 0);
    free_polygon(&p);

    // um vértice no meio de uma aresta não deve alterar o resultado
    p = create_polygon(TAM(QUADRADO_COLINEAR), QUADRADO_COLINEAR);
    ASSERT_TRUE(is_convex(&p) == 1);
    free_polygon(&p);

    // testa entrada inválida
    ASSERT_TRUE(is_convex(NULL) == 0);
}

static void test_polygon_contains_point(void) {
    Poligono p = create_polygon(TAM(QUADRADO), QUADRADO);

    // testa ponto dentro do polígono
    Ponto dentro = create_point(1, 3);
    ASSERT_TRUE(polygon_contains_point(&p, &dentro) == 1);

    // testa pontos em cima da borda ou do vértice
    Ponto vertice = create_point(4, 4), borda = create_point(2, 0);
    ASSERT_TRUE(polygon_contains_point(&p, &vertice) == 1);
    ASSERT_TRUE(polygon_contains_point(&p, &borda) == 1);

    // testa pontos fora do polígono
    Ponto foraEsquerda = create_point(-1, 2), foraAcima = create_point(2, 5);
    ASSERT_TRUE(polygon_contains_point(&p, &foraEsquerda) == 0);
    ASSERT_TRUE(polygon_contains_point(&p, &foraAcima) == 0);
    free_polygon(&p);

    // testa ponto na reentrância de um polígono côncavo, isto é, fora dele
    p = create_polygon(TAM(CONCAVO), CONCAVO);
    Ponto reentrancia = create_point(3.5, 1);
    ASSERT_TRUE(polygon_contains_point(&p, &reentrancia) == 0);
    ASSERT_TRUE(polygon_contains_point(&p, &dentro) == 1);
    free_polygon(&p);

    // testa entrada inválida
    ASSERT_TRUE(polygon_contains_point(NULL, &dentro) == 0);
    ASSERT_TRUE(polygon_contains_point(&p, NULL) == 0);
}

int main(void) {
    test_create_polygon();
    test_free_polygon();
    test_signed_area();
    test_polygon_area();
    test_polygon_perimeter();
    test_polygon_centroid();
    test_is_convex();
    test_polygon_contains_point();
    return testReport("polygon");
}
