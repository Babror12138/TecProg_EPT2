#include "test_utils.h"
#include "geometry2d.h"

#define EPS 1e-6

static void testPointOnSegment(void) {
    Ponto a = CreatPoint(0, 0), b = CreatPoint(4, 0), meio = CreatPoint(2, 0);
    Segmento s = CreatSegment(a, b);

    // testa se o meio está no segmento
    ASSERT_TRUE(PointOnSegment(&s, &meio) == 1);

    // testa os extremos do segmento
    ASSERT_TRUE(PointOnSegment(&s, &a) == 1);
    ASSERT_TRUE(PointOnSegment(&s, &b) == 1);

    // testa pontos fora do segmento
    Ponto colinearFora = CreatPoint(5, 0), fora = CreatPoint(2, 3);
    ASSERT_TRUE(PointOnSegment(&s, &colinearFora) == 0);
    ASSERT_TRUE(PointOnSegment(&s, &fora) == 0);

    // testa segmento diferente
    a = CreatPoint(1, 1), b = CreatPoint(3, 3), meio = CreatPoint(2, 2);
    s = CreatSegment(a, b);
    ASSERT_TRUE(PointOnSegment(&s, &meio) == 1);
    ASSERT_TRUE(PointOnSegment(&s, &fora) == 0);
}

static void testPointDistance(void) {
    Ponto origem = CreatPoint(0, 0), p = CreatPoint(3, 4);

    // triângulo retângulo 3-4-5
    ASSERT_NEAR(PointDistance(&origem, &p), 5.0, EPS);

    // testa distância com outra ordem
    ASSERT_NEAR(PointDistance(&p, &origem), 5.0, EPS);

    // a distância de p a p é 0
    ASSERT_NEAR(PointDistance(&p, &p), 0.0, EPS);

    // testa coordenadas negativas
    Ponto negativo = CreatPoint(-3, -4);
    ASSERT_NEAR(PointDistance(&negativo, &p), 10.0, EPS);
}

static void testPointLineDistance(void) {
    Ponto a = CreatPoint(0, 0), b = CreatPoint(4, 0);
    Segmento s = CreatSegment(a, b);

    // testa ponto acima da reta
    Ponto acima = CreatPoint(2, 3);
    ASSERT_NEAR(PointLineDistance(&s, &acima), 3.0, EPS);

    // testa ponto abaixo da reta
    Ponto abaixo = CreatPoint(2, -3);
    ASSERT_NEAR(PointLineDistance(&s, &abaixo), 3.0, EPS);

    // testa ponto além do fim do segmento para garantir que a distância é feita
    // com a extensão do segmento
    Ponto alem = CreatPoint(10, 3);
    ASSERT_NEAR(PointLineDistance(&s, &alem), 3.0, EPS);

    // testa ponto sobre a reta
    Ponto sobre = CreatPoint(2, 0);
    ASSERT_NEAR(PointLineDistance(&s, &sobre), 0.0, EPS);

    // testa reta diagonal
    a = CreatPoint(1, 1), b = CreatPoint(3, 3);
    s = CreatSegment(a, b);
    Ponto p = CreatPoint(0, 2);
    ASSERT_NEAR(PointLineDistance(&s, &p), sqrt(2.0), EPS);
}

static void testLineIntersection(void) {
    // testa reta horizontal com reta vertical
    Segmento horizontal = CreatSegment(CreatPoint(0, 0), CreatPoint(4, 0));
    Segmento vertical = CreatSegment(CreatPoint(2, -1), CreatPoint(2, 5));
    Ponto p = LineIntersection(&horizontal, &vertical);
    ASSERT_NEAR(p.x, 2.0, EPS);
    ASSERT_NEAR(p.y, 0.0, EPS);

    // testa a interseção com outra ordem dos parâmetros
    p = LineIntersection(&vertical, &horizontal);
    ASSERT_NEAR(p.x, 2.0, EPS);
    ASSERT_NEAR(p.y, 0.0, EPS);

    // testa segmentos que não se tocam, mas os prolongamentos cruzam
    Segmento s1 = CreatSegment(CreatPoint(0, 0), CreatPoint(1, 1));
    Segmento s2 = CreatSegment(CreatPoint(0, 4), CreatPoint(1, 3));
    p = LineIntersection(&s1, &s2);
    ASSERT_NEAR(p.x, 2.0, EPS);
    ASSERT_NEAR(p.y, 2.0, EPS);
}

static void testLinesPosition(void) {
    Segmento base = CreatSegment(CreatPoint(0, 0), CreatPoint(4, 0));

    // testa retas paralelas
    Segmento paralela = CreatSegment(CreatPoint(0, 1), CreatPoint(4, 1));
    ASSERT_TRUE(LinesPosition(&base, &paralela) == 0);

    // testa retas coincidentes, que são um caso específico das paralelas
    Segmento coincidente = CreatSegment(CreatPoint(1, 0), CreatPoint(3, 0));
    ASSERT_TRUE(LinesPosition(&base, &coincidente) == 0);

    // testa retas perpendiculares
    Segmento perpendicular = CreatSegment(CreatPoint(2, -1), CreatPoint(2, 5));
    ASSERT_TRUE(LinesPosition(&base, &perpendicular) == 1);

    // testa retas concorrentes que não são perpendiculares
    Segmento concorrente = CreatSegment(CreatPoint(1, 1), CreatPoint(3, 3));
    ASSERT_TRUE(LinesPosition(&base, &concorrente) == 2);
}

int main(void) {
    testPointOnSegment();
    testPointDistance();
    testPointLineDistance();
    testLineIntersection();
    testLinesPosition();
    return test_report("geometry2d");
}
