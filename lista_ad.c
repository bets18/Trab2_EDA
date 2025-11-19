#include <stdio.h>
#include <stdlib.h>

#define INFINITO 999999

// --- ESTRUTURAS ---
typedef struct viz {
    int noj;
    int peso;
    struct viz* prox;
} Viz;

typedef struct grafo {
    int nv;
    int na;
    Viz** viz;
} Grafo;

typedef struct fila {
    int itens[20]; 
    int inicio;
    int fim;
} Fila;

// --- FUNCOES BASICAS ---

Grafo* criar_grafo(int nv) {
    Grafo* g = (Grafo*) malloc(sizeof(Grafo));
    g->nv = nv;
    g->na = 0;
    g->viz = (Viz**) malloc(sizeof(Viz*) * (nv + 1));
    for (int i = 0; i <= nv; i++) g->viz[i] = NULL;
    return g;
}

void inserir_aresta(Grafo* g, int u, int v, int peso) {
    Viz* novo1 = (Viz*) malloc(sizeof(Viz));
    novo1->noj = v; novo1->peso = peso; novo1->prox = g->viz[u];
    g->viz[u] = novo1;

    Viz* novo2 = (Viz*) malloc(sizeof(Viz));
    novo2->noj = u; novo2->peso = peso; novo2->prox = g->viz[v];
    g->viz[v] = novo2;
    g->na++;
}

void liberar_grafo(Grafo* g) {
    for (int i = 0; i <= g->nv; i++) {
        Viz* p = g->viz[i];
        while (p != NULL) {
            Viz* temp = p; p = p->prox; free(temp);
        }
    }
    free(g->viz); free(g);
}

// --- FILA ---
void iniciar_fila(Fila* f) { f->inicio = 0; f->fim = 0; }
int fila_vazia(Fila* f) { return f->inicio == f->fim; }
void enfileirar(Fila* f, int valor) { f->itens[f->fim++] = valor; }
int desenfileirar(Fila* f) { return f->itens[f->inicio++]; }

void imprimir_fila(Fila* f) {
    printf("[ ");
    for (int i = f->inicio; i < f->fim; i++) printf("%d ", f->itens[i]);
    printf("]\n");
}

// --- ALGORITMOS ---

// ITEM A
void imprimir_grafo(Grafo* g) {
    printf("\n--- ITEM A: LISTA ---\n");
    for (int i = 1; i <= 10; i++) {
        printf("%d: ", i);
        Viz* p = g->viz[i];
        while (p != NULL) {
            printf("%d(%d) ", p->noj, p->peso);
            p = p->prox;
        }
        printf("\n");
    }
}

// ITEM B
void bfs(Grafo* g, int inicio) {
    printf("\n--- ITEM B: BFS (%d) ---\n", inicio);
    int visitado[11] = {0};
    Fila f; iniciar_fila(&f);

    visitado[inicio] = 1;
    enfileirar(&f, inicio);
    
    printf("Inicio | Fila: ");
    imprimir_fila(&f);

    while (!fila_vazia(&f)) {
        int u = desenfileirar(&f);
        
        // Processa vizinhos
        Viz* p = g->viz[u];
        while (p != NULL) {
            int v = p->noj;
            if (!visitado[v]) {
                visitado[v] = 1;
                enfileirar(&f, v);
            }
            p = p->prox;
        }
        // Print limpo: No visitado e como ficou a fila
        printf("Visitou: %d | Fila: ", u);
        imprimir_fila(&f);
    }
}

// ITEM C
void imprimir_distancias(int dist[], int nv) {
    printf("[ ");
    for (int i = 1; i <= nv; i++) {
        if (dist[i] == INFINITO) printf("INF ");
        else printf("%d ", dist[i]);
    }
    printf("]\n");
}

void dijkstra(Grafo* g, int inicio) {
    printf("\n--- ITEM C: DIJKSTRA (%d) ---\n", inicio);
    int dist[11], ant[11], visitado[11];

    for (int i = 1; i <= 10; i++) {
        dist[i] = INFINITO; ant[i] = -1; visitado[i] = 0;
    }
    dist[inicio] = 0;

    printf("Inicial | Dist: ");
    imprimir_distancias(dist, 10);

    for (int count = 0; count < 10; count++) {
        int min = INFINITO, u = -1;
        for (int i = 1; i <= 10; i++) {
            if (!visitado[i] && dist[i] < min) { min = dist[i]; u = i; }
        }

        if (u == -1 || dist[u] == INFINITO) break;

        visitado[u] = 1;

        // Relaxamento
        Viz* p = g->viz[u];
        while (p != NULL) {
            int v = p->noj;
            int peso = p->peso;
            if (!visitado[v] && dist[u] != INFINITO && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                ant[v] = u;
            }
            p = p->prox;
        }
        
        // Print limpo: Quem foi selecionado e o vetor resultante
        printf("Selecionou: %d | Dist: ", u);
        imprimir_distancias(dist, 10);
    }

    printf("\n--- RESULTADO FINAL ---\n");
    for (int i = 1; i <= 10; i++) {
        if (i == inicio) continue;
        
        if (dist[i] == INFINITO) { 
            printf("Destino %d: Inalcancavel\n", i); 
            continue; 
        }

        printf("Destino %d: ", i);
        
        // Imprime caminho
        int caminho[11], k = 0, atual = i;
        while (atual != -1) { caminho[k++] = atual; atual = ant[atual]; }
        for (int j = k - 1; j >= 0; j--) {
            printf("%d", caminho[j]);
            if (j > 0) printf("->");
        }
        
        printf(" (Custo %d)\n", dist[i]);
    }
}

int main() {
    Grafo* g = criar_grafo(10);

    inserir_aresta(g, 1, 3, 3); inserir_aresta(g, 1, 5, 4);
    inserir_aresta(g, 1, 8, 3); inserir_aresta(g, 1, 9, 3);
    inserir_aresta(g, 1, 10, 1); inserir_aresta(g, 2, 3, 4);
    inserir_aresta(g, 2, 10, 1); inserir_aresta(g, 3, 4, 1);
    inserir_aresta(g, 3, 6, 1); inserir_aresta(g, 3, 8, 2);
    inserir_aresta(g, 3, 9, 4); inserir_aresta(g, 3, 10, 4);
    inserir_aresta(g, 4, 5, 2); inserir_aresta(g, 4, 7, 2);
    inserir_aresta(g, 5, 7, 2); inserir_aresta(g, 5, 8, 4);
    inserir_aresta(g, 6, 7, 3); inserir_aresta(g, 8, 9, 1);
    inserir_aresta(g, 8, 10, 2); inserir_aresta(g, 9, 10, 4);

    imprimir_grafo(g); 
    bfs(g, 3);
    dijkstra(g, 3);

    liberar_grafo(g);
    return 0;
}