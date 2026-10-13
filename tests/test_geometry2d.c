#include "test_utils.h"
#include "geometry2d.h"

static void testPointOnSegment(void) {
    Ponto a = create_point(0, 0), b = create_point(4, 0), meio = create_point(2, 0);
    Segmento s = create_segment(a, b);

    // testa se o meio está no segmento
    ASSERT_TRUE(point_on_segment(&s, &meio) == 1);

    // testa os extremos do segmento
    ASSERT_TRUE(point_on_segment(&s, &a) == 1);
    ASSERT_TRUE(point_on_segment(&s, &b) == 1);

    // testa pontos fora do segmento
    Ponto colinearFora = create_point(5, 0), fora = create_point(2, 3);
    ASSERT_TRUE(point_on_segment(&s, &colinearFora) == 0);
    ASSERT_TRUE(point_on_segment(&s, &fora) == 0);

    // testa segmento diferente
    a = create_point(1, 1), b = create_point(3, 3), meio = create_point(2, 2);
    s = create_segment(a, b);
    ASSERT_TRUE(point_on_segment(&s, &meio) == 1);
    ASSERT_TRUE(point_on_segment(&s, &fora) == 0);
}

static void testPointDistance(void) {
    Ponto origem = create_point(0, 0), p = create_point(3, 4);

    // triângulo retângulo 3-4-5
    ASSERT_NEAR(point_distance(&origem, &p), 5.0, EPS);

    // testa distância com outra ordem
    ASSERT_NEAR(point_distance(&p, &origem), 5.0, EPS);

    // a distância de p a p é 0
    ASSERT_NEAR(point_distance(&p, &p), 0.0, EPS);

    // testa coordenadas negativas
    Ponto negativo = create_point(-3, -4);
    ASSERT_NEAR(point_distance(&negativo, &p), 10.0, EPS);
}

static void testPointLineDistance(void) {
    Ponto a = create_point(0, 0), b = create_point(4, 0);
    Segmento s = create_segment(a, b);

    // testa ponto acima da reta
    Ponto acima = create_point(2, 3);
    ASSERT_NEAR(point_line_distance(&s, &acima), 3.0, EPS);

    // testa ponto abaixo da reta
    Ponto abaixo = create_point(2, -3);
    ASSERT_NEAR(point_line_distance(&s, &abaixo), 3.0, EPS);

    // testa ponto além do fim do segmento para garantir que a distância é feita
    // com a extensão do segmento
    Ponto alem = create_point(10, 3);
    ASSERT_NEAR(point_line_distance(&s, &alem), 3.0, EPS);

    // testa ponto sobre a reta
    Ponto sobre = create_point(2, 0);
    ASSERT_NEAR(point_line_distance(&s, &sobre), 0.0, EPS);

    // testa reta diagonal
    a = create_point(1, 1), b = create_point(3, 3);
    s = create_segment(a, b);
    Ponto p = create_point(0, 2);
    ASSERT_NEAR(point_line_distance(&s, &p), sqrt(2.0), EPS);
}

static void testLineIntersection(void) {
    // testa reta horizontal com reta vertical
    Segmento horizontal = create_segment(create_point(0, 0), create_point(4, 0));
    Segmento vertical = create_segment(create_point(2, -1), create_point(2, 5));
    Ponto p = line_intersection(&horizontal, &vertical);
    ASSERT_NEAR(p.x, 2.0, EPS);
    ASSERT_NEAR(p.y, 0.0, EPS);

    // testa a interseção com outra ordem dos parâmetros
    p = line_intersection(&vertical, &horizontal);
    ASSERT_NEAR(p.x, 2.0, EPS);
    ASSERT_NEAR(p.y, 0.0, EPS);

    // testa segmentos que não se tocam, mas os prolongamentos cruzam
    Segmento s1 = create_segment(create_point(0, 0), create_point(1, 1));
    Segmento s2 = create_segment(create_point(0, 4), create_point(1, 3));
    p = line_intersection(&s1, &s2);
    ASSERT_NEAR(p.x, 2.0, EPS);
    ASSERT_NEAR(p.y, 2.0, EPS);
}

static void testLinesPosition(void) {
    Segmento base = create_segment(create_point(0, 0), create_point(4, 0));

    // testa retas paralelas
    Segmento paralela = create_segment(create_point(0, 1), create_point(4, 1));
    ASSERT_TRUE(lines_position(&base, &paralela) == 0);

    // testa retas coincidentes, que são um caso específico das paralelas
    Segmento coincidente = create_segment(create_point(1, 0), create_point(3, 0));
    ASSERT_TRUE(lines_position(&base, &coincidente) == 0);

    // testa retas perpendiculares
    Segmento perpendicular = create_segment(create_point(2, -1), create_point(2, 5));
    ASSERT_TRUE(lines_position(&base, &perpendicular) == 1);

    // testa retas concorrentes que não são perpendiculares
    Segmento concorrente = create_segment(create_point(1, 1), create_point(3, 3));
    ASSERT_TRUE(lines_position(&base, &concorrente) == 2);
}

int main(void) {
    testPointOnSegment();
    testPointDistance();
    testPointLineDistance();
    testLineIntersection();
    testLinesPosition();
    return testReport("geometry2d");
}
