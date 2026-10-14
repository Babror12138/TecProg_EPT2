#include "svg.h"

#define LARGURA 39.0
#define ALTURA 10.0
#define ESCALA 25.0
#define MARGEM 20.0

// transforma coordenadas do plano em pixels (no caso o eixo Y cresce pra baixo, por isso o ALTURA - y)

static double px(double x) {
    return MARGEM + x * ESCALA;
}

static double py(double y) {
    return MARGEM + (ALTURA - y) * ESCALA;
}

FILE *svg_begin(const char *caminho) {
    if (caminho == NULL) return NULL;

    FILE *f = fopen(caminho, "w");
    if (f == NULL) return NULL;

    double w = LARGURA * ESCALA + 2 * MARGEM, h = ALTURA * ESCALA + 2 * MARGEM;
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%.0f\" height=\"%.0f\">\n", w, h);
    fprintf(f, "<rect width=\"100%%\" height=\"100%%\" fill=\"white\"/>\n");
    return f;
}

void svg_end(FILE *f) {
    if (f == NULL) return;
    fprintf(f, "</svg>\n");
    fclose(f);
}

void svg_polygon(FILE *f, const Poligono *p, const char *cor) {
    if (f == NULL || p == NULL || p->vertices == NULL) return;

    fprintf(f, "<polygon points=\"");
    for (int i = 0; i < p->n; i++) {
        fprintf(f, "%.1f,%.1f ", px(p->vertices[i].x), py(p->vertices[i].y));
    }
    fprintf(f, "\" fill=\"%s\" fill-opacity=\"0.25\" stroke=\"%s\" stroke-width=\"2\"/>\n", cor, cor);
}

void svg_segment(FILE *f, Ponto a, Ponto b, const char *cor) {
    if (f == NULL) return;
    fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"%s\" stroke-width=\"2\"/>\n",
            px(a.x), py(a.y), px(b.x), py(b.y), cor);
}

void svg_point(FILE *f, Ponto p, const char *cor) {
    if (f == NULL) return;
    fprintf(f, "<circle cx=\"%.1f\" cy=\"%.1f\" r=\"4\" fill=\"%s\"/>\n", px(p.x), py(p.y), cor);
}

void svg_text(FILE *f, Ponto p, const char *texto) {
    if (f == NULL) return;
    fprintf(f, "<text x=\"%.1f\" y=\"%.1f\" font-family=\"sans-serif\" font-size=\"14\">%s</text>\n",
            px(p.x), py(p.y), texto);
}