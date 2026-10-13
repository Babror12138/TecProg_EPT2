#include "convex_hull.h"
#include "geometry2d.h"
#include <stdlib.h>

// produto vetoria
static double cross(const Ponto *o, const Ponto *a, const Ponto *b) {
    return (a->x - o->x) * (b->y - o->y) - (a->y - o->y) * (b->x - o->x);
}

// distância entre pontos ao quadrado
static double dist2(const Ponto* a, const Ponto* b) {
    return (a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y);
}

// função para retornar o polígono vazio
static Poligono vazio(void) {
    Poligono p;
    p.n = 0;
    p.vertices = NULL;
    return p;
}

// função para comparar dois pontos (checa se um é "menor" que outro)
static int menorPonto(const Ponto *a, const Ponto *b) {
    if (a->y == b->y) return a->x < b->x;
    return a->y < b->y;
}

// implementação do algoritmo de Jarvis para achar o fecho convexo
Poligono convex_hull_jarvis(int n, const Ponto *pontos) {
    // testa entrada inválida
    if (pontos == NULL || n < 3) return vazio();
    Ponto *fecho = malloc(n * sizeof(Ponto));
    // testa falha na alocação
    if (!fecho) return vazio();

    // seleciona ponto inicial como o menor ponto
    int ini = 0;
    for (int i = 1; i < n; i++) if (menorPonto(&pontos[i], &pontos[ini])) ini = i;

    int atual = ini, k = 0;
    do {
        fecho[k++] = pontos[atual];
        int cand = -1;
        
        // acha o próximo ponto do fecho
        for (int i = 0; i < n; i++) {
            // se for o próprio ponto atual ignora
            if (dist2(&pontos[i], &pontos[atual]) == 0) continue;
            if (cand < 0) {
                cand = i;
                continue;
            }

            // faz o produto vetorial entre atual, cand e i, se achar um ponto melhor, troca cand por ele
            double c = cross(&pontos[atual], &pontos[cand], &pontos[i]);
            if (c < 0 || (c == 0 && dist2(&pontos[atual], &pontos[i]) > dist2(&pontos[atual], &pontos[cand]))) cand = i;
        }

        // condição de parada, cand < 0
        if (cand < 0) break;

        // avança com o ponto atual para cand
        atual = cand;
    } while (dist2(&pontos[atual], &pontos[ini]) != 0 && k < n);
    Poligono res = create_polygon(k, fecho);
    free(fecho);
    return res;
}

// pivô do algoritmo de Graham
static Ponto pivo;

// função de comparação para ordenar os pontos no algoritmo de Graham
// ordena por sentido anti-horário do ângulo com o pivô e desempata com a distância
static int cmp(const void *pa, const void *pb) {
    const Ponto *a = pa, *b = pb;
    double c = cross(&pivo, a, b);
    if (c < 0) return 1;
    if (c > 0) return -1;
    double da = dist2(&pivo, a), db = dist2(&pivo, b);
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

Poligono convex_hull_graham(int n, const Ponto *pontos) {
    // testa entrada inválida
    if (pontos == NULL || n < 3) return vazio();
    Ponto* v = malloc(n * sizeof(Ponto));
    Ponto* pilha = malloc(n * sizeof(Ponto));
    // testa falha na alocação
    if (!v || !pilha) {
        free(v);
        free(pilha);
        return vazio();
    }

    for (int i = 0; i < n; i++) v[i] = pontos[i];

    // acha menor ponto
    int menor = 0;
    for (int i = 1; i < n; i++) if (menorPonto(&v[i], &v[menor])) menor = i;

    // troca o menor ponto pelo primeiro ponto
    Ponto t = v[0];
    v[0] = v[menor];
    v[menor] = t;

    // ordena a array v em relação ao pivô
    pivo = v[0];
    qsort(v + 1, n - 1, sizeof(Ponto), cmp);

    int k = 0;
    for (int i = 0; i < n; i++) {
        // enquanto o v[i] ofuscar o topo da pilha, retire o topo da pilha
        while (k >= 2 && cross(&pilha[k - 2], &pilha[k - 1], &v[i]) <= 0) k--;

        // elimina pontos repetidos
        if (k == 1 && dist2(&pilha[0], &v[i]) == 0) continue;

        // acrescenta v[i] à pilha
        pilha[k++] = v[i];
    }
    Poligono res = create_polygon(k, pilha);
    free(v);
    free(pilha);
    return res;
}

// retorna área do convex hull
double convex_hull_area(int n, const Ponto *pontos) {
    Poligono conv = convex_hull_graham(n, pontos);
    double a = polygon_area(&conv);
    free_polygon(&conv);
    return a;
}

// retorna perímetro do convex hull
double convex_hull_perimeter(int n, const Ponto *pontos) {
    Poligono conv = convex_hull_graham(n, pontos);
    double p = polygon_perimeter(&conv);
    free_polygon(&conv);
    return p;
}

// retorna diâmetro do convex hull
double convex_hull_diameter(int n, const Ponto *pontos) {
    Poligono conv = convex_hull_graham(n, pontos);
    double d = 0;
    for (int i = 0; i < conv.n; i++) {
        for (int j = i + 1; j < conv.n; j++) {
            double x = PointDistance(&conv.vertices[i], &conv.vertices[j]);
            if (x > d) d = x;
        }
    }
    free_polygon(&conv);
    return d;
}