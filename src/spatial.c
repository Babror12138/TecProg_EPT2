#include "spatial.h"
#include "geometry2d.h"
#include <math.h>
#include <stdlib.h>

Ponto* closestPair(Ponto* pontos, int n) {
  if (pontos == NULL || n < 2) {
    return NULL;
  }

  int melhorI = 0;
  int melhorJ = 1;
  double menorDist2 = pow(pontos[0].x - pontos[1].x, 2) + pow(pontos[0].y - pontos[1].y, 2);

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      double dx = pontos[i].x - pontos[j].x;
      double dy = pontos[i].y - pontos[j].y;
      double dist2 = dx * dx + dy * dy;

      if (dist2 < menorDist2) {
        menorDist2 = dist2;
        melhorI = i;
        melhorJ = j;
      }
    }
  }

  Ponto* parMaisProximo = (Ponto*) malloc(2 * sizeof(Ponto));
  if (parMaisProximo == NULL) {
    return NULL;
  }

  parMaisProximo[0] = pontos[melhorI];
  parMaisProximo[1] = pontos[melhorJ];

  return parMaisProximo;
}

Ponto* intersectionPolygon(Poligono A, Poligono B, int *count) {
  int n = 10;
  int k = 0;
  *count = 0;
  
  Ponto* intersects = (Ponto*) malloc(n * sizeof(Ponto));
  if (intersects == NULL) return NULL;

  for (int i = 0; i < A.n; i++) {
    Segmento segA = CreatSegment(A.vertices[i], A.vertices[(i + 1) % A.n]);

    for (int j = 0; j < B.n; j++) {
      Segmento segB = CreatSegment(B.vertices[j], B.vertices[(j + 1) % B.n]);

      int pos = LinesPosition(&segA, &segB);

      // verifica se são retas paralelas
      if (pos == 0) continue;

      Ponto intersectionPoint = LineIntersection(&segA, &segB);

      // verifica se o ponto está nas retas
      if (
        !PointOnSegment(&segA, &intersectionPoint) ||
        !PointOnSegment(&segB, &intersectionPoint)
      ) continue;

      // alocação de mais memória
      if (k == n) {
        n += 10;

        Ponto* temp = realloc(intersects, n * sizeof(Ponto));
        if (temp == NULL) {
          free(intersects);
          return NULL;
        }

        intersects = temp;
      }
    }
  }

  if (k == 0) {
    free(intersects);
    return NULL;
  }

  Ponto* temp = realloc(intersects, k * sizeof(Ponto));

  if (temp == NULL) {
    free(intersects);
    return NULL;
  }

  intersects = temp;
  *count= k;

  return intersects;
}
