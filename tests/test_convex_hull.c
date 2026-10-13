#include "test_utils.h"
#include "convex_hull.h"

#define TAM(v) ((int)(sizeof(v) / sizeof((v)[0])))

// quadrado de lado 4 com mais dois pontos dentro
static const Ponto QUADRADO[] = { {4, 4}, {2, 2}, {0, 0}, {1, 3}, {4, 0}, {0, 4} };
static const Ponto FECHO_QUADRADO[] = { {0, 0}, {4, 0}, {4, 4}, {0, 4} };

// triângulo retângulo 3-4-5
static const Ponto TRIANGULO[] = { {0, 0}, {4, 0}, {0, 3} };

// mesmo quadrado, mas com pontos no meio das arestas e um vértice repetido
static const Ponto QUADRADO_SUJO[] = { {0, 0}, {2, 0}, {4, 0}, {4, 2}, {4, 4}, {0, 4}, {0, 0} };

// todos os pontos na mesma reta, ou seja, não tem fecho
static const Ponto COLINEARES[] = { {0, 0}, {1, 1}, {2, 2}, {3, 3} };

typedef Poligono (*FuncaoFecho)(int, const Ponto *);

// retorna 1 se o polígono tem os mesmos pontos esperados na mesma ordem
static int fecho_igual(const Poligono *fecho, const Ponto *esperado, int n) {
    if (fecho->n != n) return 0;

    for (int i = 0; i < n; i++) {
        if (fabs(fecho->vertices[i].x - esperado[i].x) > EPS) return 0;
        if (fabs(fecho->vertices[i].y - esperado[i].y) > EPS) return 0;
    }

    return 1;
}

// os testes a seguir são válidos tanto para Graham quanto Jarvis

static void test_fecho_quadrado(FuncaoFecho fecho) {
    Poligono h = fecho(TAM(QUADRADO), QUADRADO);
    ASSERT_TRUE(fecho_igual(&h, FECHO_QUADRADO, TAM(FECHO_QUADRADO)));
    free_polygon(&h);
}

static void test_fecho_triangulo(FuncaoFecho fecho) {
    Poligono h = fecho(TAM(TRIANGULO), TRIANGULO);
    ASSERT_TRUE(fecho_igual(&h, TRIANGULO, TAM(TRIANGULO)));
    free_polygon(&h);
}

static void test_fecho_ignora_colineares_e_repetidos(FuncaoFecho fecho) {
    Poligono h = fecho(TAM(QUADRADO_SUJO), QUADRADO_SUJO);
    ASSERT_TRUE(fecho_igual(&h, FECHO_QUADRADO, TAM(FECHO_QUADRADO)));
    free_polygon(&h);
}

static void test_fecho_entrada_invalida(FuncaoFecho fecho) {
    // pontos é nulo
    Poligono h = fecho(5, NULL);
    ASSERT_TRUE(h.n == 0 && h.vertices == NULL);
    free_polygon(&h);

    // menos de 3 pontos
    h = fecho(2, QUADRADO);
    ASSERT_TRUE(h.n == 0 && h.vertices == NULL);
    free_polygon(&h);

    // menos de 3 pontos não colineares
    h = fecho(TAM(COLINEARES), COLINEARES);
    ASSERT_TRUE(h.n == 0 && h.vertices == NULL);
    free_polygon(&h);
}

static void test_graham_igual_jarvis(void) {
    Poligono graham = convex_hull_graham(TAM(QUADRADO_SUJO), QUADRADO_SUJO);
    Poligono jarvis = convex_hull_jarvis(TAM(QUADRADO_SUJO), QUADRADO_SUJO);
    ASSERT_TRUE(graham.n >= 3);
    ASSERT_TRUE(fecho_igual(&graham, jarvis.vertices, jarvis.n));
    free_polygon(&graham);
    free_polygon(&jarvis);
}

static void test_area(void) {
    ASSERT_NEAR(convex_hull_area(TAM(QUADRADO), QUADRADO), 16.0, EPS);
    ASSERT_NEAR(convex_hull_area(TAM(TRIANGULO), TRIANGULO), 6.0, EPS);

    // entradas inválidas
    ASSERT_NEAR(convex_hull_area(5, NULL), 0.0, EPS);
    ASSERT_NEAR(convex_hull_area(TAM(COLINEARES), COLINEARES), 0.0, EPS);
}

static void test_perimetro(void) {
    ASSERT_NEAR(convex_hull_perimeter(TAM(QUADRADO), QUADRADO), 16.0, EPS);
    ASSERT_NEAR(convex_hull_perimeter(TAM(TRIANGULO), TRIANGULO), 12.0, EPS);

    // entradas inválidas
    ASSERT_NEAR(convex_hull_perimeter(5, NULL), 0.0, EPS);
    ASSERT_NEAR(convex_hull_perimeter(TAM(COLINEARES), COLINEARES), 0.0, EPS);
}

static void test_diametro(void) {
    // testa se o diâmetro é o que deveria ser
    ASSERT_NEAR(convex_hull_diameter(TAM(QUADRADO), QUADRADO), 4.0 * sqrt(2.0), EPS);
    ASSERT_NEAR(convex_hull_diameter(TAM(TRIANGULO), TRIANGULO), 5.0, EPS);

    // entradas inválidas: diâmetro deve retornar 0
    Ponto dois_pontos[] = { {0, 0}, {3, 4} };
    ASSERT_NEAR(convex_hull_diameter(5, NULL), 0.0, EPS);
    ASSERT_NEAR(convex_hull_diameter(2, dois_pontos), 0.0, EPS);
    ASSERT_NEAR(convex_hull_diameter(TAM(COLINEARES), COLINEARES), 0.0, EPS);
}

int main(void) {
    test_fecho_quadrado(convex_hull_graham);
    test_fecho_quadrado(convex_hull_jarvis);
    test_fecho_triangulo(convex_hull_graham);
    test_fecho_triangulo(convex_hull_jarvis);
    test_fecho_ignora_colineares_e_repetidos(convex_hull_graham);
    test_fecho_ignora_colineares_e_repetidos(convex_hull_jarvis);
    test_fecho_entrada_invalida(convex_hull_graham);
    test_fecho_entrada_invalida(convex_hull_jarvis);
    test_graham_igual_jarvis();

    test_area();
    test_perimetro();
    test_diametro();

    return testReport("convex_hull");
}
