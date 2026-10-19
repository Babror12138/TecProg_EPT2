# libgeom

Biblioteca de geometria computacional 2D escrita em C, que reúne operações sobre pontos, segmentos, polígonos, fecho convexo, proximidade e interseção.

O projeto inclui uma aplicação cliente de demonstração, que usa a biblioteca apenas pelos headers públicos, imprime os resultados no terminal e gera uma figura SVG.

Trabalho Prático 2 de MAC0216 – Técnicas de Programação I (IME-USP).

## Funcionalidades

- **Tipos geométricos básicos**: ponto (`Ponto`) e segmento (`Segmento`), com funções para criá-los.
- **Geometria 2D**: distância entre pontos, distância de ponto a reta, teste de ponto em segmento, interseção e posição relativa de retas.
- **Polígonos**: criação, área, perímetro, centroide, teste de convexidade e teste de inclusão de ponto.
- **Fecho convexo**: algoritmos Graham Scan e Jarvis, além de área, perímetro e diâmetro do fecho.
- **Operações espaciais**: par mais próximo de um conjunto de pontos, interseções de polígonos e teste de inclusão de um polígono em outro.

## Pré-requisitos

- Linux
- GCC com suporte a C11
- GNU Make
- Um navegador, para abrir as figuras SVG geradas pela aplicação (opcional)

## Estrutura do projeto

```text
.
├── app/                 aplicação cliente
│   ├── main.c           demonstração dos quatro módulos
│   ├── svg.c            funções de desenho em SVG
│   └── svg.h
├── include/             interface pública da biblioteca (um header por módulo)
│   ├── general.h
│   ├── geometry2d.h
│   ├── polygon.h
│   ├── convex_hull.h
│   └── spatial.h
├── src/                 implementação dos módulos
│   ├── general.c
│   ├── geometry2d.c
│   ├── polygon.c
│   ├── convex_hull.c
│   └── spatial.c
├── tests/               testes automatizados
│   ├── test_utils.h     macros de verificação e resumo dos testes
│   └── test_*.c         um arquivo de testes por módulo
├── Makefile
├── README.md
├── Relatório TecProg EPT2.pdf      relatório técnico
├── Apresentação - TecProg.pdf      slides da apresentação
└── Apresentação - TecProg.pptx
```

As pastas abaixo são criadas pela compilação e removidas por `make clean`. Elas estão no `.gitignore`.

| Pasta | Conteúdo |
| --- | --- |
| `build/` | arquivos objeto (`.o`) e de dependência (`.d`) |
| `lib/` | `libgeom.a` e `libgeom.so` |
| `bin/` | aplicações, executáveis dos testes e figuras SVG |

## Compilação

```sh
make
```

| Arquivo gerado | Descrição |
| --- | --- |
| `lib/libgeom.a` | biblioteca estática, criada com `ar rcs` |
| `lib/libgeom.so` | biblioteca dinâmica, criada com `gcc -shared` a partir de objetos compilados com `-fPIC` |
| `bin/app_static` | aplicação ligada à biblioteca estática |
| `bin/app_shared` | aplicação ligada à biblioteca dinâmica |

A aplicação dinâmica é ligada com `-Wl,-rpath,'$ORIGIN/../lib'`, então encontra `lib/libgeom.so` sem precisar de `LD_LIBRARY_PATH`. Para conferir qual biblioteca ela carrega:

```sh
ldd bin/app_shared
```

### Compilação incremental

Cada arquivo `.c` gera o seu próprio objeto em `build/`, e a opção `-MMD -MP` registra de quais headers ele depende. Assim, o `make` recompila apenas o que foi afetado por uma alteração:

- alterar `src/spatial.c` recompila só `spatial.c`, e depois refaz as bibliotecas e as aplicações;
- alterar `include/convex_hull.h` recompila só quem inclui esse header: `convex_hull.c`, `test_convex_hull.c` e `app/main.c`;
- alterar `include/general.h` recompila tudo, porque todos os módulos dependem dele.

## Alvos do Makefile

| Alvo | O que faz |
| --- | --- |
| `make` ou `make all` | compila as duas bibliotecas e as duas versões da aplicação |
| `make run` | compila, se necessário, e executa as duas versões da aplicação |
| `make run-static` | executa só a aplicação ligada à biblioteca estática |
| `make run-shared` | executa só a aplicação ligada à biblioteca dinâmica |
| `make test` | compila e executa todos os testes automatizados |
| `make install` | instala as bibliotecas e os headers em `PREFIX` (padrão: `/usr/local`) |
| `make uninstall` | remove os arquivos instalados por `make install` |
| `make clean` | remove `build/`, `lib/` e `bin/` |
| `make help` | lista os alvos disponíveis |

## Execução

```sh
make run
```

A aplicação demonstra cada módulo da biblioteca: retas e distâncias (`geometry2d`), medidas de um polígono côncavo (`polygon`), o fecho convexo de uma nuvem de pontos (`convex_hull`), e o par mais próximo e a interseção de dois triângulos (`spatial`). Os resultados aparecem no terminal, e cada versão grava uma figura:

- `bin/demo_static.svg`, gerada por `make run-static`;
- `bin/demo_shared.svg`, gerada por `make run-shared`.

As figuras podem ser abertas em qualquer navegador.

Também é possível executar a aplicação diretamente, informando onde salvar a figura:

```sh
./bin/app_static figura.svg
```

Sem argumento, a figura é salva como `demo.svg` na pasta atual. Se o arquivo não puder ser criado, a aplicação mostra uma mensagem de erro e termina com código 1.

## Testes

```sh
make test
```

Há um arquivo de testes por módulo em `tests/`, com casos de uso normal, valores limite e entradas inválidas. Cada arquivo mostra quantas verificações passaram, e o alvo termina com um resumo:

```text
[convex_hull] 27/27 testes passaram -> OK
[general] 10/10 testes passaram -> OK
[geometry2d] 39/39 testes passaram -> OK
[polygon] 43/43 testes passaram -> OK
[spatial] 48/48 testes passaram -> OK
==============================================
RESULTADO: SUCESSO em todos os testes
```

Quando uma verificação falha, o teste mostra a expressão, o arquivo e a linha, e `make test` termina com erro.

## Instalação

```sh
make install PREFIX="$HOME/.local"
```

`PREFIX` é a pasta onde a biblioteca será instalada. Os exemplos usam `$HOME/.local`, uma pasta dentro do diretório do usuário, porque a instalação ali não precisa de permissão de administrador. Qualquer outra pasta funciona, desde que o mesmo prefixo seja usado na instalação, na desinstalação e na compilação dos programas que usam a biblioteca.

Os arquivos são instalados em:

| Destino | Arquivos |
| --- | --- |
| `PREFIX/lib/` | `libgeom.a` e `libgeom.so` |
| `PREFIX/include/geom/` | os cinco headers da biblioteca |

Sem `PREFIX`, a instalação vai para `/usr/local` e exige permissão de administrador (`sudo make install`).

Para remover, use o mesmo prefixo da instalação:

```sh
make uninstall PREFIX="$HOME/.local"
```

### Usando a biblioteca instalada

Um programa de exemplo, `exemplo.c`:

```c
#include <stdio.h>
#include <geom/convex_hull.h>

int main(void) {
    Ponto pontos[] = { {0, 0}, {4, 0}, {4, 3}, {0, 3}, {2, 1} };
    int n = sizeof(pontos) / sizeof(pontos[0]);

    Poligono fecho = convex_hull_graham(n, pontos);
    printf("Vértices do fecho: %d\n", fecho.n);
    printf("Área do fecho: %.2f\n", polygon_area(&fecho));

    free_polygon(&fecho);
    return 0;
}
```

Os comandos abaixo usam a variável `PREFIX`, que deve ter o mesmo valor usado em `make install`:

```sh
PREFIX="$HOME/.local"
```

Com ligação dinâmica, `LD_LIBRARY_PATH` indica onde o programa deve procurar `libgeom.so` ao ser executado:

```sh
gcc exemplo.c -I"$PREFIX/include" -L"$PREFIX/lib" -lgeom -lm -o exemplo
LD_LIBRARY_PATH="$PREFIX/lib" ./exemplo
```

Com ligação estática, o código da biblioteca é copiado para o executável, que roda sem precisar de `libgeom.so`:

```sh
gcc exemplo.c -I"$PREFIX/include" "$PREFIX/lib/libgeom.a" -lm -o exemplo
./exemplo
```

## Módulos

| Módulo | Header | Funções |
| --- | --- | --- |
| general | `general.h` | `create_point`, `create_segment` |
| geometry2d | `geometry2d.h` | `point_on_segment`, `point_distance`, `point_line_distance`, `line_intersection`, `lines_position` |
| polygon | `polygon.h` | `create_polygon`, `free_polygon`, `signed_area`, `polygon_area`, `polygon_perimeter`, `polygon_centroid`, `is_convex`, `polygon_contains_point` |
| convex_hull | `convex_hull.h` | `convex_hull_graham`, `convex_hull_jarvis`, `convex_hull_area`, `convex_hull_perimeter`, `convex_hull_diameter` |
| spatial | `spatial.h` | `closest_pair`, `intersection_polygon`, `intersection_region`, `is_polygon_inside` |

Cada função está documentada no seu header, com parâmetros, retorno e comportamento em caso de erro. Algumas regras valem para toda a biblioteca:

- **Memória**: um `Poligono` devolvido por uma função é liberado com `free_polygon`. Os vetores devolvidos por `closest_pair` e `intersection_polygon` são liberados com `free`. Em `intersection_region`, cada polígono do vetor é liberado com `free_polygon`, e depois o vetor com `free`.
- **Erros**: entradas inválidas, como ponteiro nulo ou polígono com menos de 3 vértices, não interrompem o programa. A função devolve um valor de erro documentado: `0`, `-1`, `NULL`, um polígono vazio, `INVALIDO`, o ponto `(0, 0)` ou um ponto com coordenadas `NAN`.
- **Tolerância**: as comparações entre valores `double` usam `GEOM_EPS` (`1e-6`), definido em `general.h`. A exceção é o fecho convexo, que compara de forma exata para reconhecer pontos repetidos.

## Autores

| Integrante | Responsabilidade |
| --- | --- |
| Marcelo Zhang | módulos `general` e `geometry2d`, relatório |
| Marcos Akira | módulo `polygon`, slides |
| Davi Mendonça | módulo `convex_hull`, Makefile |
| Leandro Takara | módulo `spatial` e extras |
