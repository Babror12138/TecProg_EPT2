#include "spatial.h"
#include "geometry2d.h"
#include <math.h>
#include <stdlib.h>

Ponto* closest_pair(Ponto* pontos, int n) {
  if (pontos == NULL || n < 2) {
    return NULL;
  }

  // guarda os melhores índices até então
  int melhorI = 0;
  int melhorJ = 1;
  // salvando a distancia ao quadrado
  double menorDist2 = pow(pontos[0].x - pontos[1].x, 2) + pow(pontos[0].y - pontos[1].y, 2);

  // verifica todos os pares de pontos para achar a menor distância
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      double dx = pontos[i].x - pontos[j].x;
      double dy = pontos[i].y - pontos[j].y;
      double dist2 = dx * dx + dy * dy;

      // achamos um par melhor de pontos
      if (dist2 < menorDist2) {
        menorDist2 = dist2;
        melhorI = i;
        melhorJ = j;
      }
    }
  }

  Ponto* parMaisProximo = (Ponto*) malloc(2 * sizeof(Ponto));
  if (parMaisProximo == NULL) return NULL;

  parMaisProximo[0] = pontos[melhorI];
  parMaisProximo[1] = pontos[melhorJ];

  return parMaisProximo;
}

// encontro o pontode interseção aos dois segmentos dados se existir
static int find_intersection_point(Segmento segA, Segmento segB, Ponto* intersection_point) {
  if (intersection_point == NULL) return 0;
  if (lines_position(&segA, &segB) == PARALELOS) return 0;

  Ponto point = line_intersection(&segA, &segB);

  // verifica se o ponto está nas arestas
  if (!point_on_segment(&segA, &point) || !point_on_segment(&segB, &point)) return 0;

  *intersection_point = point;
  return 1;
}

// verifica se o ponto não está no vetor de pontos
static int contains_point(Ponto point, const Ponto *points, int size) {
  for (int i = 0; i < size; i++) {
    if (
      fabs(points[i].x - point.x) <= EPS &&
      fabs(points[i].y - point.y) <= EPS
    ) {
      return 0;
    }
  }

  return 1;
}

// aloca mais memória e trata erros de alocação para o ponteiro dado
static void* resize_array(void* p, int size, int byte) {
  void* temp = realloc(p, size * byte);
  if (temp == NULL) {
    free(p);
    return NULL;
  }

  return temp;
}

// aloca mais memória e trata erros de alocação para o ponteiro dado
static void* resize_increment_array(void* p, int* size, int byte, int increment) {
  int newSize = *size + increment;

  void* temp = resize_array(p, newSize, byte);
  if (temp != NULL) *size = newSize;

  return temp;
}

Ponto* intersection_polygon(Poligono A, Poligono B, int *length) {
  int n = 10;
  int k = 0; // indice para o vetor dos pontos de interseção
  *length = 0;
  
  Ponto* intersects = (Ponto*) malloc(n * sizeof(Ponto));
  if (intersects == NULL) return NULL;

  // percorre todos os pares de arestas para contabilizar os pontos de interseção
  for (int i = 0; i < A.n; i++) {
    Segmento segA = create_segment(A.vertices[i], A.vertices[(i + 1) % A.n]);

    for (int j = 0; j < B.n; j++) {
      Segmento segB = create_segment(B.vertices[j], B.vertices[(j + 1) % B.n]);

      Ponto intersectionPoint;
      int is_intersection = find_intersection_point(segA, segB, &intersectionPoint);
      if (!is_intersection) continue;

      if (!contains_point(intersectionPoint, intersects, k)) continue;
    
      intersects[k++] = intersectionPoint;

      // alocação de mais memória
      if (k == n) {
        intersects = resize_increment_array(intersects, &n, sizeof(Ponto), 10);
        if (intersects == NULL) return NULL;
      }
    }
  }

  if (k == 0) {
    free(intersects);
    return NULL;
  }

  // garante que o vetor final tenha o tamanho correto
  intersects = resize_array(intersects, n, sizeof(Ponto));
  if (intersects == NULL) return NULL;

  *length = k;
  return intersects;
}

// intercala o poligono A com as interseções com o poligono B
static int add_intersection_points(Poligono A, Poligono B, Poligono* xA) {
  int n = 10;
  int k = 0; // indice para xPolygonA, que são os pontos de A com interseção B

  Ponto* xPolygonA = (Ponto*) malloc(n * sizeof(Ponto));
  if (xPolygonA == NULL) return 0;

  for (int i = 0; i < A.n; i++) {
    int m = 10;
    Ponto* intersects = (Ponto*) malloc(m * sizeof(Ponto));
    if (intersects == NULL) return 0;

    xPolygonA[k++] = A.vertices[i];

    // alocação de mais memória
    if (k == n) {
      xPolygonA = resize_increment_array(xPolygonA, &n, sizeof(Ponto), 10);
      if (xPolygonA == NULL) return 0;
    }

    Segmento segA = create_segment(A.vertices[i], A.vertices[(i + 1) % A.n]);
    int l = 0; // indice de intersects

    for (int j = 0; j < B.n; j++) {
      Segmento segB = create_segment(B.vertices[j], B.vertices[(j + 1) % B.n]);

      Ponto intersectionPoint;
      int is_intersection = find_intersection_point(segA, segB, &intersectionPoint);
      if (!is_intersection) continue;

      // os extremos da aresta já entram em xPolygonA como vértices de A
      if (
        (fabs(intersectionPoint.x - segA.p1.x) < EPS && fabs(intersectionPoint.y - segA.p1.y) < EPS) ||
        (fabs(intersectionPoint.x - segA.p2.x) < EPS && fabs(intersectionPoint.y - segA.p2.y) < EPS)
      ) continue;

      if (!contains_point(intersectionPoint, intersects, l)) continue;

      intersects[l++] = intersectionPoint;

      // alocação de mais memória
      if (l == m) {
        intersects = resize_increment_array(intersects, &m, sizeof(Ponto), 10);
        if (intersects == NULL) return 0;
      }
    }

    // bubble sort
    // ordena os pontos de interseção em ordem em relação ao ponto do segmento atual
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

    // adiciona os pontos de interseção na ordem
    for (int x = 0; x < l; x++) {
      xPolygonA[k++] = intersects[x];
      if (k == n) {
        xPolygonA = resize_increment_array(xPolygonA, &n, sizeof(Ponto), 10);
        if (xPolygonA == NULL) return 0;
      }
    }

    free(intersects);
  }

  *xA = create_polygon(k, xPolygonA);
  free(xPolygonA);
  return 1;
}

// verifica se um poligono A está totalmente contido no poligono B
int is_polygon_inside(Poligono A, Poligono B) {
  // verifica se todos os pontos de A estão dentro de B
  for (int i = 0; i < A.n; i++) {
    if (!polygon_contains_point(&B, &A.vertices[i])) {
      return 0;
    }
  }

  return 1;
}

// retona, em ordem, os pontos, começando em start, que estão contido em B até o próximo ponto de interseção
static Ponto* find_points_path(Ponto start, Poligono xA, Poligono B, Ponto* intersects, int intersectsCount, int* end, int* pathSize) {
  int pathCount = 20;
  int pathIdx = 0;

  // procura o indice de start
  int startIndex = 0;
  for (int j = 0; j < xA.n; j++) {
    if (fabs(xA.vertices[j].x - start.x) < EPS && fabs(xA.vertices[j].y - start.y) < EPS) {
      startIndex = j;
      break;
    }
  }

  Ponto next = xA.vertices[(startIndex + 1) % xA.n];
  Ponto prev = xA.vertices[(startIndex - 1 + xA.n) % xA.n];
  Ponto midNext = create_point((start.x + next.x) / 2, (start.y + next.y) / 2);
  Ponto midPrev = create_point((start.x + prev.x) / 2, (start.y + prev.y) / 2);

  // define a direção de busca
  int dir = 0;
  if (polygon_contains_point(&B, &midNext)) {
    dir = 1;
  }
  if (polygon_contains_point(&B, &midPrev)) {
    dir = -1;
  }

  if (dir == 0) return NULL;

  Ponto* pathPoints = (Ponto*) malloc(pathCount * sizeof(Ponto));
  if (pathPoints == NULL) return NULL;

  Ponto curr = start;
  int k = startIndex; // indice nos pontos de xA

  for (int i = 0; i < xA.n; i++) {
    // adiciona o ponto atual no caminho
    pathPoints[pathIdx++] = curr;

    // alocação de mais memória
    if (pathIdx == pathCount) {
      pathPoints = resize_increment_array(pathPoints, &pathCount, sizeof(Ponto), 20);
      if (pathPoints == NULL) return NULL;
    }

    // próximo ponto
    k = (k + dir + xA.n) % xA.n;
    curr = xA.vertices[k];

    // verifica se curr é ponto de interseção
    int found = -1;
    for (int j = 0; j < intersectsCount; j++) {
      if (fabs(intersects[j].x - curr.x) < EPS && fabs(intersects[j].y - curr.y) < EPS) {
        found = j;
        break;
      }
    }

    // se curr é ponto de interseção, então o caminho de pontos de
    // start até curr (end) é totalmente contido em B
    if (found != -1) {
      *end = found;
      break;
    }
  }

  *pathSize = pathIdx;
  return pathPoints;
}

Poligono* intersection_region(Poligono A, Poligono B, int* length) {
  *length = 0;

  // se algum poligono está contido no outro, então o poligono de interseção é o contido
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

  // adiciona as interseções aos poligonos, preservando a ordem dos pontos
  Poligono xPolygonA;
  if (!add_intersection_points(A, B, &xPolygonA)) return NULL;

  Poligono xPolygonB;
  if (!add_intersection_points(B, A, &xPolygonB)) return NULL;

  // diz se o i-ésimo ponto de interseção já foi utlizado
  int* used = (int*) calloc(intersectsCount, sizeof(int));
  int regionsCount = 5;
  int regionsIdx = 0;
  Poligono* regions = (Poligono*) malloc(regionsCount * sizeof(Poligono));

  for (int i = 0; i < intersectsCount; i++) {
    if (used[i]) continue;
    used[i] = 1;

    int regCount = 0;
    int regCapacity = 20;
    Ponto* regPoints = (Ponto*) malloc(regCapacity * sizeof(Ponto));
    if (regPoints == NULL) break;

    // contorna a região alternando entre os dois polígonos a cada ponto de
    // interseção, até voltar ao ponto de partida
    int current = i;
    int inA = 1;
    int closed = 0;

    for (int leg = 0; leg < intersectsCount && regPoints != NULL; leg++) {
      int next = -1;
      int pathCount = 0;
      Ponto* path = inA
        ? find_points_path(intersects[current], xPolygonA, B, intersects, intersectsCount, &next, &pathCount)
        : find_points_path(intersects[current], xPolygonB, A, intersects, intersectsCount, &next, &pathCount);

      if (path == NULL) break;

      // acrescenta o trecho ao contorno da região
      for (int j = 0; j < pathCount && regPoints != NULL; j++) {
        regPoints[regCount++] = path[j];
        if (regCount == regCapacity) {
          regPoints = resize_increment_array(regPoints, &regCapacity, sizeof(Ponto), 20);
        }
      }

      free(path);
      if (regPoints == NULL) break;

      used[next] = 1;
      current = next;
      inA = !inA;

      if (current == i) {
        closed = 1;
        break;
      }
    }

    if (regPoints == NULL) break;

    // pontos em que os polígonos apenas se tocam não fecham uma região
    Poligono region = create_polygon(closed ? regCount : 0, regPoints);
    free(regPoints);
    if (region.n == 0) continue;

    regions[regionsIdx++] = region;
    if (regionsIdx == regionsCount) {
      regions = resize_increment_array(regions, &regionsCount, sizeof(Poligono), 10);
      if (regions == NULL) break;
    }
  }

  free(intersects);
  free(used);
  free_polygon(&xPolygonA);
  free_polygon(&xPolygonB);

  if (regions == NULL) return NULL;

  if (regionsIdx == 0) {
    free(regions);
    return NULL;
  }

  regions = resize_array(regions, regionsIdx, sizeof(Poligono));
  if (regions == NULL) return NULL;

  *length = regionsIdx;
  return regions;
}
