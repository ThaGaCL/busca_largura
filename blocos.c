#include <stdio.h>
#include <stdlib.h>
#include "blocos.h"

// Protótipos
void busca_em_profundidade(int vertice, int **matriz_adj, int *visitado, int *visitado_arestas, int n, info_t *info_bloco);
void imprime_matriz(int **matriz_adj, int n);

void imprime_matriz(int **matriz_adj, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matriz_adj[i][j]);
        }
        printf("\n");
    }
}

void busca_em_profundidade(int vertice, int **matriz_adj, int *visitado, int *visitado_arestas, int n, info_t *info_bloco) {
    visitado[vertice] = 1; // Marca vértice como visitado
    info_bloco->vertices++; // Incrementa número de vértices do bloco
    for (int i = 0; i < n; i++) {
        if (matriz_adj[vertice][i]) {
            int aresta_id = vertice * n + i;
            if (!visitado_arestas[aresta_id]) {
                visitado_arestas[aresta_id] = 1;
                visitado_arestas[i * n + vertice] = 1;
                info_bloco->arestas++;
            }
            if (!visitado[i]) {
                busca_em_profundidade(i, matriz_adj, visitado, visitado_arestas, n, info_bloco);
            }
        }
    }
}

info_t *info_blocos(unsigned int *num_blocos) {
    int n;
    scanf("%d", &n);
    // Alocar matriz de adjacência
    int **matriz_adj = malloc((size_t)n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        matriz_adj[i] = calloc((size_t)n, sizeof(int));
    }

    // Lê arestas
    int u, v;
    while (scanf("%d %d", &u, &v) != EOF) {
        matriz_adj[u - 1][v - 1] = 1;
        matriz_adj[v - 1][u - 1] = 1;
    }

    int *visitado = calloc((size_t)n, sizeof(int)); 
    int *visitado_arestas = calloc((size_t)(n * n), sizeof(int)); // Marcar arestas visitadas (evita erros de contagem)
    info_t *blocos = malloc((size_t)n * sizeof(info_t));
    *num_blocos = 0;

    // Encontrar blocos
    for (int i = 0; i < n; i++) {
        // Se o vértice não foi visitado, então é um novo bloco
        if (!visitado[i]) {
            // Inicializa bloco 
            blocos[*num_blocos].vertices = 0;
            blocos[*num_blocos].arestas = 0;
            busca_em_profundidade(i, matriz_adj, visitado, visitado_arestas, n, &blocos[*num_blocos]);
            (*num_blocos)++;
        }
    }

    free(visitado);
    free(visitado_arestas);
    for (int i = 0; i < n; i++) {
        free(matriz_adj[i]);
    }
    free(matriz_adj);

    return blocos;
}