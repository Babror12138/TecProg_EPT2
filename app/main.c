#include <stdio.h>
#include "general.h"

int main(void) {
    Ponto p1 = create_point(0.0f, 0.0f);
    Ponto p2 = create_point(3.0f, 4.0f);
    Segmento s = create_segment(p1, p2);

    printf("Ponto p1 = (%.2f, %.2f)\n", p1.x, p1.y);
    printf("Ponto p2 = (%.2f, %.2f)\n", p2.x, p2.y);
    printf("Segmento s: (%.2f, %.2f) -> (%.2f, %.2f)\n", s.p1.x, s.p1.y, s.p2.x, s.p2.y);

    return 0;
}