# libgeom

Biblioteca de geometria computacional 2D escrita em C, que reúne operações sobre pontos, segmentos, polígonos.

Além disso, inclui uma aplicação de demonstração que imprime resultados no terminal e gera uma figura SVG.

## Funcionalidades

- **Tipos geométricos básicos**: ponto (`Ponto`) e segmento (`Segmento`), com funções para criá-los.
- **Geometria 2D**: distância entre pontos, distância de ponto a reta, teste de ponto em segmento, interseção e posição relativa de retas.
- **Polígonos**: criação, área, perímetro, centroide, teste de convexidade e teste de inclusão de ponto.
- **Fecho convexo**: algoritmos Graham Scan e Jarvis para encontrar o fecho convexo como também funções para calcular área, perímetro e diâmetro.
- **Operações espaciais**: par mais próximo de um conjunto de pontos, interseções de polígonos e teste de inclusão de um polígono em outro.

## Requisitos

- A biblioteca foi construída para ser utilizada em um ambiente GNU/Linux.
- O sistema deve ter GCC e Make instalados.

## Como instalar

Instala a biblioteca para o local designado por `PREFIX` (por padrão, o prefixo é `/usr/local`):

```sh
make install PREFIX="$HOME/.local"
```

Para remover a instalação, use o mesmo prefixo escolhido anteriormente:

```sh
make uninstall PREFIX="$HOME/.local"
```

## Como executar

Para executar as duas aplicações - uma ligada à biblioteca estática e outra à dinâmica, execute:

```sh
make run
```

- Execute `make run-static` para executar a aplicação vinculado estaticamente
- Execute `make run-shared` para executar a aplicação vinculado dinamicamente

## Como testar

Para verificar a biblioteca, compile e execute os testes automatizados com:

```sh
make test
```
