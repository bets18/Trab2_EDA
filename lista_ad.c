#include <stdio.h>
#include <stdlib.h>

// define infinito como um valor inalcancavel
#define INFINITO 999999

// estrutura para os vizinhos (lista encadeada)
typedef struct viz {
    int noj;        // id do vizinho
    int peso;       // peso da aresta
    struct viz* prox;
} Viz;

// estrutura principal do grafo
typedef struct grafo {
    int nv;         // numero de vertices
    int na;         // numero de arestas
    Viz** viz;      // vetor de listas de adjacencia
} Grafo;

// estrutura de fila para auxiliar a bfs
typedef struct fila {
    int itens[20]; 
    int inicio;
    int fim;
} Fila;


// funcao para criar e inicializar o grafo
Grafo* criar_grafo(int nv) {
    Grafo* g = (Grafo*) malloc(sizeof(Grafo));
    g->nv = nv;
    g->na = 0;
    // aloca vetor de listas com tamanho nv+1 para usar indices de 1 a 10
    g->viz = (Viz**) malloc(sizeof(Viz*) * (nv + 1));
    for (int i = 0; i <= nv; i++) {
        g->viz[i] = NULL;
    }
    return g;
}

// funcao para inserir aresta no grafo
void inserir_aresta(Grafo* g, int u, int v, int peso) {
    // insere aresta u -> v no inicio da lista de u
    Viz* novo1 = (Viz*) malloc(sizeof(Viz));
    novo1->noj = v;
    novo1->peso = peso;
    novo1->prox = g->viz[u];
    g->viz[u] = novo1;

    // insere aresta v -> u no inicio da lista de v
    Viz* novo2 = (Viz*) malloc(sizeof(Viz));
    novo2->noj = u;
    novo2->peso = peso;
    novo2->prox = g->viz[v];
    g->viz[v] = novo2;

    // incrementa numero de arestas
    g->na++;
}

// funcao para liberar memoria usada pelo grafo
void liberar_grafo(Grafo* g) {
    for (int i = 0; i <= g->nv; i++) {
        Viz* p = g->viz[i];
        while (p != NULL) {
            Viz* temp = p;
            p = p->prox;
            free(temp);
        }
    }
    free(g->viz);
    free(g);
}

// funcoes auxiliares de fila (usadas na bfs)

void iniciar_fila(Fila* f) {
    f->inicio = 0;
    f->fim = 0;
}

int fila_vazia(Fila* f) {
    return f->inicio == f->fim;
}

void enfileirar(Fila* f, int valor) {
    f->itens[f->fim] = valor;
    f->fim++;
}

int desenfileirar(Fila* f) {
    int v = f->itens[f->inicio];
    f->inicio++;
    return v;
}

// imprime o conteudo atual da fila
void imprimir_fila(Fila* f) {
    printf("[ ");
    for (int i = f->inicio; i < f->fim; i++) {
        printf("%d ", f->itens[i]);
    }
    printf("]\n");
}


// item a: imprime a lista de adjacencias do grafo
void imprimir_grafo(Grafo* g) {
    printf("\na) LISTA DE ADJACENCIAS\n");
    for (int i = 1; i <= 10; i++) {
        printf("No %d:", i);
        Viz* p = g->viz[i];
        while (p != NULL) {
            printf(" %d(%d)", p->noj, p->peso);
            p = p->prox;
        }
        printf("\n");
    }
}

// item b: algoritmo de busca em amplitude (bfs)
void bfs(Grafo* g, int inicio) {
    printf("\nb) BFS (inicio: %d)\n", inicio);
    
    int visitado[11];
    // zera vetor de visitados
    for(int i=0; i<=10; i++) visitado[i] = 0;

    Fila f;
    iniciar_fila(&f);

    // passo inicial: marca origem e enfileira
    visitado[inicio] = 1;
    enfileirar(&f, inicio);
    
    printf("Inicio | Fila: ");
    imprimir_fila(&f);

    // loop enquanto houver elementos na fila
    while (!fila_vazia(&f)) {
        int u = desenfileirar(&f);
        
        // percorre todos os vizinhos do no atual
        Viz* p = g->viz[u];
        while (p != NULL) {
            int v = p->noj;
            // se vizinho nao foi visitado, marca e enfileira
            if (!visitado[v]) {
                visitado[v] = 1;
                enfileirar(&f, v);
            }
            p = p->prox;
        }
        // mostra estado da fila a cada passo
        printf("Visitou: %d | Fila: ", u);
        imprimir_fila(&f);
    }
}

// funcao auxiliar para imprimir vetor de distancias (usada no dijkstra)
void imprimir_distancias(int dist[], int nv) {
    printf("[ ");
    for (int i = 1; i <= nv; i++) {
        if (dist[i] == INFINITO) printf("INF ");
        else printf("%d ", dist[i]);
    }
    printf("]\n");
}

// item c: algoritmo de dijkstra para menor caminho
void dijkstra(Grafo* g, int inicio) {
    printf("\nc) DIJKSTRA (inicio: %d)\n", inicio);

    int dist[11];     // guarda as menores distancias
    int ant[11];      // guarda o no anterior para reconstruir caminho
    int visitado[11]; // marca se o no ja foi processado

    // inicializa vetores com valores padrao
    for (int i = 1; i <= 10; i++) {
        dist[i] = INFINITO;
        ant[i] = -1;
        visitado[i] = 0;
    }
    dist[inicio] = 0; // distancia para a origem eh zero

    printf("Inicial | Dist: ");
    imprimir_distancias(dist, 10);

    // loop principal: executa uma vez para cada no
    for (int count = 0; count < 10; count++) {
        // busca o no nao visitado com menor distancia atual
        int min = INFINITO;
        int u = -1;

        for (int i = 1; i <= 10; i++) {
            if (!visitado[i] && dist[i] < min) {
                min = dist[i];
                u = i;
            }
        }

        // se nao achou ou distancia eh infinita, para
        if (u == -1 || dist[u] == INFINITO) break;

        visitado[u] = 1; // marca como processado

        // relaxamento das arestas dos vizinhos
        Viz* p = g->viz[u];
        while (p != NULL) {
            int v = p->noj;
            int peso = p->peso;

            // verifica se achou um caminho mais curto
            if (!visitado[v] && dist[u] != INFINITO && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                ant[v] = u; // atualiza anterior
            }
            p = p->prox;
        }
        
        // mostra quem foi processado e como ficou o vetor
        printf("Selecionou: %d | Dist: ", u);
        imprimir_distancias(dist, 10);
    }

    // imprime relatorio final dos caminhos encontrados
    printf("\nRESULTADO FINAL \n");
    for (int i = 1; i <= 10; i++) {
        if (i == inicio) continue;
        
        if (dist[i] == INFINITO) { 
            printf("Destino %d: Inalcancavel\n", i); 
            continue; 
        }

        printf("Destino %d: ", i);
        
        // reconstroi o caminho usando o vetor ant (de tras para frente)
        int caminho[11];
        int k = 0;
        int atual = i;
        while (atual != -1) {
            caminho[k++] = atual;
            atual = ant[atual];
        }
        
        // imprime o caminho na ordem correta
        for (int j = k - 1; j >= 0; j--) {
            printf("%d", caminho[j]);
            if (j > 0) printf("->");
        }
        
        printf(" (Custo %d)\n", dist[i]);
    }
}

// funcao principal 
int main() {
    // cria o grafo com 10 vertices
    Grafo* g = criar_grafo(10);

    // insere as arestas conforme definicao do trabalho
    // no 1
    inserir_aresta(g, 1, 3, 3);
    inserir_aresta(g, 1, 5, 4);
    inserir_aresta(g, 1, 8, 3);
    inserir_aresta(g, 1, 9, 3);
    inserir_aresta(g, 1, 10, 1);

    // no 2
    inserir_aresta(g, 2, 3, 4);
    inserir_aresta(g, 2, 10, 1);

    // no 3
    inserir_aresta(g, 3, 4, 1);
    inserir_aresta(g, 3, 6, 1);
    inserir_aresta(g, 3, 8, 2);
    inserir_aresta(g, 3, 9, 4);
    inserir_aresta(g, 3, 10, 4);

    // no 4
    inserir_aresta(g, 4, 5, 2);
    inserir_aresta(g, 4, 7, 2);

    // no 5
    inserir_aresta(g, 5, 7, 2);
    inserir_aresta(g, 5, 8, 4);

    // no 6
    inserir_aresta(g, 6, 7, 3);

    // no 8
    inserir_aresta(g, 8, 9, 1);
    inserir_aresta(g, 8, 10, 2);

    // no 9
    inserir_aresta(g, 9, 10, 4);

    // executa os itens solicitados
    imprimir_grafo(g); 
    bfs(g, 3);
    dijkstra(g, 3);

    // libera memoria antes de encerrar
    liberar_grafo(g);
    return 0;
}