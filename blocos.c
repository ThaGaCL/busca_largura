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
void encontra_blocos(Grafo* grafo, int u, int pre[], int low[], Aresta* arestas, int* qtd_arestas, int pais[], int* tempo, int* count, info_t* blocos, int* bloco, int* visitado);
void liberar_grafo(Grafo* grafo);

Grafo* cria_grafo(int V, int E) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    if(!grafo) 
        return NULL;

    grafo->V = V;
    grafo->E = E;
    grafo->lista = (AdjList*)malloc((size_t)V * sizeof(AdjList));
    if (!grafo->lista) {
        free(grafo);
        return NULL;
    }
    for (int i = 0; i < V; i++) {
        grafo->lista[i].adj = (int*)malloc((size_t)V * sizeof(int));
        if (!grafo->lista[i].adj) {
            for (int j = 0; j < i; j++) {
                free(grafo->lista[j].adj);
            }
            free(grafo->lista);
            free(grafo);
            return NULL;
        }
        grafo->lista[i].tam = 0;
    }
    return grafo;
}

void add_aresta(Grafo* grafo, int u, int v) {
    grafo->lista[u - 1].adj[grafo->lista[u - 1].tam++] = v - 1;
    grafo->lista[v - 1].adj[grafo->lista[v - 1].tam++] = u - 1;

    return;
}

void liberar_grafo(Grafo* grafo){
    for (int i = 0; i < grafo->V; i++) {
        free(grafo->lista[i].adj);
    }
    free(grafo->lista);
    free(grafo);

    return;
}

void encontra_blocos(Grafo* grafo, int u, int pre[], int low[], Aresta* arestas, int* qtd_arestas, int pais[], int* tempo, int* count, info_t* blocos, int* bloco, int* visitado) {
    pre[u] = low[u] = ++(*tempo);
    int filho = 0;
    visitado[u] = 1;

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

            encontra_blocos(grafo, v, pre, low, arestas, qtd_arestas, pais, tempo, count, blocos, bloco, visitado);

            low[u] = (low[u] < low[v]) ? low[u] : low[v];

            // Se u é um ponto de articulação
            if ((pre[u] == 1 && filho > 1) || (pre[u] > 1 && low[v] >= pre[u])) {
                int vertices_count = 0, arestas_count = 0;
                while ((*qtd_arestas) > 0 && (arestas[*qtd_arestas - 1].u != u || arestas[*qtd_arestas - 1].v != v)) {
                    arestas_count++;
                    vertices_count++;
                    (*qtd_arestas)--;
                }
                if ((*qtd_arestas) > 0) {
                    arestas_count++;
                    vertices_count++;
                    (*qtd_arestas)--;
                }
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

    return;
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
    
    if(grafo->E == 0) {
        *num_blocos = (unsigned int)V;
        info_t* blocos = malloc((size_t)V * sizeof(info_t));
        for (int i = 0; i < V; i++) {
            blocos[i].vertices = 1;
            blocos[i].arestas = 0;
        }
        liberar_grafo(grafo); // Libera memória do grafo
        return blocos;
    }

    int* visitado = (int*)malloc((size_t)V * sizeof(int));
    int* pre = (int*)malloc((size_t)V * sizeof(int)); // Pre para cada vértice
    int* low = (int*)malloc((size_t)V * sizeof(int)); // Low point para cada vértice
    int* pais = (int*)malloc((size_t)V * sizeof(int)); // Pai de cada vértice
    Aresta* arestas = (Aresta*)malloc((size_t)grafo->E * sizeof(Aresta));  // Para armazenar as arestas

    int qtd_arestas = 0;

    // Seta todos os valores iniciais como -1 e não visitado
    for (int i = 0; i < V; i++) {
        pre[i] = NIL;
        low[i] = NIL;
        pais[i] = NIL;
        visitado[i] = 0;
    }

    *num_blocos = 0;

    info_t* blocos = malloc((size_t)grafo->E * sizeof(info_t));
    int tempo = 0;
    int count = 0;
    int bloco = 0;

    // Encontra os blocos
    for (int i = 0; i < V; i++) {
        if (!visitado[i]) {
            encontra_blocos(grafo, i, pre, low, arestas, &qtd_arestas, pais, &tempo, &count, blocos, &bloco, visitado);

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

    // Adiciona vértices isolados como blocos separados
    for (int i = 0; i < V; i++) {
        if (grafo->lista[i].tam == 0) { // Vértice isolado
            blocos[bloco].vertices = 1;
            blocos[bloco].arestas = 0;
            bloco++;
            count++;
        }
    }

    *num_blocos = (unsigned int)count;
    liberar_grafo(grafo); // Libera memória do grafo
    free(visitado); // Libera memória do vetor visitado
    free(pre); // Libera memória do vetor pre
    free(low); // Libera memória do vetor low
    free(pais); // Libera memória do vetor pais
    free(arestas); // Libera memória do vetor arestas
    
    return blocos;
}
