#include "test_utils.h"
#include "general.h"

#define EPS 1e-6

static void testCreatPoint(void) {
    // teste com ponto qualquer
    Ponto p = CreatPoint(1.5f, -2.0f);
    ASSERT_NEAR(p.x, 1.5, EPS);
    ASSERT_NEAR(p.y, -2.0, EPS);

    // teste com a origem
    Ponto origem = CreatPoint(0.0f, 0.0f);
    ASSERT_NEAR(origem.x, 0.0, EPS);
    ASSERT_NEAR(origem.y, 0.0, EPS);
}

static void testCreatSegment(void) {
    // teste com segmento qualquer
    Ponto a = CreatPoint(0.0f, 0.0f);
	Ponto b = CreatPoint(3.0f, 4.0f);
	Segmento s = CreatSegment(a, b);
	ASSERT_NEAR(s.p1.x, 0.0, EPS);
	ASSERT_NEAR(s.p1.y, 0.0, EPS);
	ASSERT_NEAR(s.p2.x, 3.0, EPS);
	ASSERT_NEAR(s.p2.y, 4.0, EPS);

    // teste com segmento degenerado (comprimento 0)
    Segmento degenerado = CreatSegment(a, a);
	ASSERT_NEAR(degenerado.p1.x, degenerado.p2.x, EPS);
	ASSERT_NEAR(degenerado.p1.y, degenerado.p2.y, EPS);
}

int main(void) {
    testCreatPoint();
    testCreatSegment();
    return test_report("general");
}