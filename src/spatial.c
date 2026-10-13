#include "spatial.h"
#include "geometry2d.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define EPS 1e-5f

Ponto* closest_pair(Ponto* pontos, int n) {
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

Ponto* intersection_polygon(Poligono A, Poligono B, int *length) {
  int n = 10;
  int k = 0;
  *length = 0;
  
  Ponto* intersects = (Ponto*) malloc(n * sizeof(Ponto));
  if (intersects == NULL) return NULL;

  for (int i = 0; i < A.n; i++) {
    Segmento segA = create_segment(A.vertices[i], A.vertices[(i + 1) % A.n]);

    for (int j = 0; j < B.n; j++) {
      Segmento segB = create_segment(B.vertices[j], B.vertices[(j + 1) % B.n]);

      int pos = lines_position(&segA, &segB);

      // verifica se são retas paralelas
      if (pos == 0) continue;

      Ponto intersectionPoint = line_intersection(&segA, &segB);

      // verifica se o ponto está nas arestas
      if (
        !point_on_segment(&segA, &intersectionPoint) ||
        !point_on_segment(&segB, &intersectionPoint)
      ) continue;

      // remove pontos de interseção duplicados
      bool duplicated = false;

      for (int l = 0; l < k; l++) {
        if (
          fabs(intersects[l].x - intersectionPoint.x) <= EPS &&
          fabs(intersects[l].y - intersectionPoint.y) <= EPS
        ) {
          duplicated = true;
          break;
        }
      }

      if (!duplicated) {
        intersects[k++] = intersectionPoint;
      }

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
  *length = k;

  return intersects;
}

Poligono addIntersectionPoints(Poligono A, Poligono B) {
  int n = 10;
  int k = 0;
  
  Ponto* xPolygonA = (Ponto*) malloc(n * sizeof(Ponto));
  // if (xPolygonA == NULL) return NULL;

  for (int i = 0; i < A.n; i++) {
    Segmento segA = create_segment(A.vertices[i], A.vertices[(i + 1) % A.n]);

    int m = 10;
    Ponto* intersects = (Ponto*) malloc(m * sizeof(Ponto));
    // if (intersects == NULL) return NULL;

    int l = 0;

    xPolygonA[k++] = A.vertices[i];
    if (k == n) {
      n += 10;

      Ponto* temp = realloc(xPolygonA, n * sizeof(Ponto));
      xPolygonA = temp;
    }

    for (int j = 0; j < B.n; j++) {
      Segmento segB = create_segment(B.vertices[j], B.vertices[(j + 1) % B.n]);

      int pos = lines_position(&segA, &segB);

      // verifica se são retas paralelas
      if (pos == 0) continue;

      Ponto intersectionPoint = line_intersection(&segA, &segB);

      // verifica se o ponto está nas arestas
      if (
        !point_on_segment(&segA, &intersectionPoint) ||
        !point_on_segment(&segB, &intersectionPoint)
      ) continue;

      // remove pontos de interseção duplicados
      bool duplicated = false;

      for (int x = 0; x < l; x++) {
        if (
          fabs(intersects[x].x - intersectionPoint.x) <= EPS &&
          fabs(intersects[x].y - intersectionPoint.y) <= EPS 
        ) {
          duplicated = true;
          break;
        }
      }

      if (!duplicated) {
        intersects[l++] = intersectionPoint;
      }

      // alocação de mais memória
      if (l == m) {
        m += 10;

        Ponto* temp = realloc(intersects, m * sizeof(Ponto));
        intersects = temp;
      }
    }

    for (int x = 1; x <= l; x++) {
      for (int y = 1; y <= l - x; y++) {
        double currDist = pow(intersects[y].x - segA.p1.x, 2) + pow(intersects[y].y - segA.p1.y, 2);
        double prevDist = pow(intersects[y - 1].x - segA.p1.x, 2) + pow(intersects[y - 1].y - segA.p1.y, 2);

        if (prevDist > currDist) {
          Ponto temp = intersects[y];
          intersects[y] = intersects[y - 1];
          intersects[y - 1] = temp;
        }
      }
    }

    for (int x = 0; x < l; x++) {
      xPolygonA[k++] = intersects[x];
      if (k == n) {
        n += 10;

        Ponto* temp = realloc(xPolygonA, n * sizeof(Ponto));
        xPolygonA = temp;
      }
    }
  }

  return create_polygon(k, xPolygonA);
}

bool is_polygon_inside(Poligono A, Poligono B) {
  for (int i = 0; i < A.n; i++) {
    if (!polygon_contains_point(&B, &A.vertices[i])) {
      return false;
    }
  }

  return true;
}

Poligono* intersection_region(Poligono A, Poligono B, int* length) {
  *length = 0;

  // algum poligono está contido no outro, então o poligono de interseção é o contido
  if (is_polygon_inside(A, B)) {
    Poligono* region = (Poligono*) malloc(1 * sizeof(Poligono));
    if (region == NULL) return NULL;

    region[0] = create_polygon(A.n, A.vertices);
    *length = 1;
    return region;
  }

  if (is_polygon_inside(B, A)) {
    Poligono* region = (Poligono*) malloc(1 * sizeof(Poligono));
    if (region == NULL) return NULL;

    region[0] = create_polygon(B.n, B.vertices);
    *length = 1;
    return region;
  }

  int intersectsCount = 0;
  Ponto* intersects = intersection_polygon(A, B, &intersectsCount);

  // poligonos não tem região de interseção em comum
  if (intersectsCount == 0) return NULL;

  Poligono xPolygonA = addIntersectionPoints(A, B);
  Poligono xPolygonB = addIntersectionPoints(B, A);

  bool* used = (bool*) calloc(intersectsCount, sizeof(bool));
  int regionsCount = 5;
  int regionsIdx = 0;
  Poligono* regions = (Poligono*) malloc(regionsCount * sizeof(Poligono));

  for (int i = 0; i < intersectsCount; i++) {
    if (used[i]) continue;

    Ponto X = intersects[i];
    Ponto Y = X;

    int regCount = 20;
    int regIdx = 0;
    Ponto* regPoints = (Ponto*) malloc(regCount * sizeof(Ponto));

    // procura o index do ponto X
    int Xidx = 0;
    for (int j = 0; j < xPolygonA.n; j++) {
      if (xPolygonA.vertices[j].x == X.x && xPolygonA.vertices[j].y == X.y) {
        Xidx = j;
        break;
      }
    }

    // define a direção de busca
    int dir = 0;
    if (polygon_contains_point(&B, &xPolygonA.vertices[(Xidx + 1) % xPolygonA.n])) {
      dir = 1;
    }
    if (polygon_contains_point(&B, &xPolygonA.vertices[(Xidx - 1 + xPolygonA.n) % xPolygonA.n])) {
      dir = -1;
    }

    Ponto curr = X;
    
    if (dir != 0) {
      int k = Xidx;

      while (true) {
        regPoints[regIdx++] = curr;
        if (regIdx == regCount) {
          regCount += 10;
          Ponto* temp = realloc(regPoints, regCount * sizeof(Ponto));
          if (temp == NULL) return NULL;
          regPoints = temp;
        }

        curr = xPolygonA.vertices[(k + dir + xPolygonA.n) % xPolygonA.n];
        k = (k + dir + xPolygonA.n) % xPolygonA.n;

        // Y é o ponto que fecha um lado da região
        bool found = false;
        for (int j = 0; j < intersectsCount; j++) {
          if (intersects[j].x == curr.x && intersects[j].y == curr.y) {
            found = true;
            used[j] = true;
            break;
          }
        }

        if (found) {
          Y = curr;
          break;
        }
      }
    }

    // procura o index do ponto Y
    int Yidx = 0;
    for (int j = 0; j < xPolygonB.n; j++) {
      if (xPolygonB.vertices[j].x == Y.x && xPolygonB.vertices[j].y == Y.y) {
        Yidx = j;
        break;
      }
    }

    // define a direção de busca
    dir = 0;
    if (polygon_contains_point(&A, &xPolygonB.vertices[(Yidx + 1) % xPolygonB.n])) {
      dir = 1;
    }
    if (polygon_contains_point(&A, &xPolygonB.vertices[(Yidx - 1 + xPolygonB.n) % xPolygonB.n])) {
      dir = -1;
    }

    if (dir != 0) {
      int k = Yidx;

      while (true) {
        regPoints[regIdx++] = curr;
        if (regIdx == regCount) {
          regCount += 10;
          Ponto* temp = realloc(regPoints, regCount * sizeof(Ponto));
          if (temp == NULL) return NULL;
          regPoints = temp;
        }

        curr = xPolygonB.vertices[(k + dir + xPolygonB.n) % xPolygonB.n];
        k = (k + dir + xPolygonB.n) % xPolygonB.n;

        // X é o ponto que fecha um lado da região
        bool found = false;
        for (int j = 0; j < intersectsCount; j++) {
          if (intersects[j].x == curr.x && intersects[j].y == curr.y) {
            found = true;
            used[j] = true;
            break;
          }
        }

        if (found) break;
      }
    }

    Poligono region = create_polygon(regIdx, regPoints);
    regions[regionsIdx++] = region;
    if (regionsIdx == regionsCount) {
      regionsCount += 10;
      Poligono* temp = realloc(regions, regionsCount * sizeof(Poligono));
      if (temp == NULL) return NULL;
      regions = temp;
    }
  }

  Poligono* temp = realloc(regions, regionsIdx * sizeof(Poligono));
  if (temp == NULL) return NULL;
  regions = temp;

  *length = regionsIdx;
  return regions;
}
