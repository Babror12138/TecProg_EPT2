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
