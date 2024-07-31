#define NIL -1
#include <stdio.h>
#include <stdlib.h>
#include "blocos.h"

typedef struct {
    int u, v;
} Aresta;

typedef struct {
    int* adj;
    int tam;
} AdjList;

typedef struct {
    AdjList* lista;
    int V, E;
} Grafo;

Grafo* cria_grafo(int V, int E);
void add_aresta(Grafo* grafo, int u, int v);
void encontra_blocos(Grafo* grafo, int u, int pre[], int low[], Aresta* arestas, int* qtd_arestas, int pais[], int* tempo, int* count, info_t* blocos, int* bloco);

Grafo* cria_grafo(int V, int E) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->V = V;
    grafo->E = E;
    grafo->lista = (AdjList*)malloc((size_t)V * sizeof(AdjList));
    for (int i = 0; i < V; i++) {
        grafo->lista[i].adj = (int*)malloc((size_t)V * sizeof(int));
        grafo->lista[i].tam = 0;
    }
    return grafo;
}

void add_aresta(Grafo* grafo, int u, int v) {
    grafo->lista[u - 1].adj[grafo->lista[u - 1].tam++] = v - 1;
    grafo->lista[v - 1].adj[grafo->lista[v - 1].tam++] = u - 1;
}

void encontra_blocos(Grafo* grafo, int u, int pre[], int low[], Aresta* arestas, int* qtd_arestas, int pais[], int* tempo, int* count, info_t* blocos, int* bloco) {
    pre[u] = low[u] = ++(*tempo);
    int filho = 0;

    // Para cada vértice adjacente
    for (int i = 0; i < grafo->lista[u].tam; ++i) {
        int v = grafo->lista[u].adj[i];

        // Se o vértice não foi visitado
        if (pre[v] == NIL) {
            filho++;
            pais[v] = u;
            arestas[*qtd_arestas].u = u;
            arestas[*qtd_arestas].v = v;
            (*qtd_arestas)++;

            encontra_blocos(grafo, v, pre, low, arestas, qtd_arestas, pais, tempo, count, blocos, bloco);

            low[u] = (low[u] < low[v]) ? low[u] : low[v];

            // Se u é um ponto de articulação
            if ((pre[u] == 1 && filho > 1) || (pre[u] > 1 && low[v] >= pre[u])) {
                int vertices_count = 0, arestas_count = 0;
                while (arestas[*qtd_arestas - 1].u != u || arestas[*qtd_arestas - 1].v != v) {
                    arestas_count++;
                    vertices_count++;
                    (*qtd_arestas)--;
                }
                arestas_count++;
                vertices_count++;
                (*qtd_arestas)--;
                blocos[*bloco].vertices = (unsigned int)vertices_count;
                blocos[*bloco].arestas = (unsigned int)arestas_count;
                (*bloco)++;
                (*count)++;
            }
        } else if (v != pais[u]) { // Se v não é o pai de u
            low[u] = (low[u] < pre[v]) ? low[u] : pre[v];
            if (pre[v] < pre[u]) { 
                arestas[*qtd_arestas].u = u;
                arestas[*qtd_arestas].v = v;
                (*qtd_arestas)++;
            }
        }
    }
}

info_t* info_blocos(unsigned int* num_blocos) {
    int V;
    scanf("%d", &V);

    Grafo* grafo = cria_grafo(V, 0);

    int u, v;
    while (scanf("%d %d", &u, &v) != EOF) {
        add_aresta(grafo, u, v);
        grafo->E++;
    }

    int* pre = (int*)malloc((size_t)V * sizeof(int)); // Pre para cada vértice
    int* low = (int*)malloc((size_t)V * sizeof(int)); // Low point para cada vértice
    int* pais = (int*)malloc((size_t)V * sizeof(int)); // Pai de cada vértice
    Aresta* arestas = (Aresta*)malloc((size_t)grafo->E * sizeof(Aresta));  // Para armazenar as arestas
    int qtd_arestas = 0;

    // Seta todos os valores iniciais como -1
    for (int i = 0; i < V; i++) {
        pre[i] = NIL;
        low[i] = NIL;
        pais[i] = NIL;
    }

    *num_blocos = 0;
    info_t* blocos = (info_t*)malloc((size_t)grafo->E * sizeof(info_t));
    int tempo = 0;
    int count = 0;
    int bloco = 0;

    // Encontra os blocos
    for (int i = 0; i < V; i++) {
        if (pre[i] == NIL) {
            encontra_blocos(grafo, i, pre, low, arestas, &qtd_arestas, pais, &tempo, &count, blocos, &bloco);

            if (qtd_arestas > 0) {
                int vertices_count = 0, arestas_count = 0;
            
                // Conta os vértices e arestas do último bloco
                while (qtd_arestas > 0) {
                    arestas_count++;
                    vertices_count++;
                    qtd_arestas--;
                }

                blocos[bloco].vertices = (unsigned int)vertices_count;
                blocos[bloco].arestas = (unsigned int)arestas_count;
                bloco++;
                count++;
            }
        }
    }

    *num_blocos = (unsigned int)count;
    return blocos;
}
