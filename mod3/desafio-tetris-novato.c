#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 10

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca itens[MAX];
    int frente, tras, tamanho;
} Fila;

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->tamanho = 0;
}

// Gera uma peça automaticamente com tipo aleatório e id sequencial
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    return (Peca){tipos[rand() % 4], id};
}

// Insere peça no final da fila (enqueue)
int enqueue(Fila *f, Peca p) {
    if (f->tamanho == MAX) return 0;
    f->itens[f->tras] = p;
    f->tras = (f->tras + 1) % MAX;
    f->tamanho++;
    return 1;
}

// Remove peça da frente da fila (dequeue)
int dequeue(Fila *f, Peca *p) {
    if (f->tamanho == 0) return 0;
    *p = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX;
    f->tamanho--;
    return 1;
}

void exibirFila(Fila *f) {
    printf("\nFila de pecas\n");
    if (f->tamanho == 0) {
        printf("(vazia)\n");
        return;
    }
    for (int i = 0; i < f->tamanho; i++)
        printf("[%c %d] ", f->itens[(f->frente + i) % MAX].nome, f->itens[(f->frente + i) % MAX].id);
    printf("\n");
}

int main() {
    srand(time(NULL));

    Fila fila;
    inicializarFila(&fila);

    int proximoId = 0;

    // Inicializa fila com 5 peças
    for (int i = 0; i < 5; i++)
        enqueue(&fila, gerarPeca(proximoId++));

    int opcao;
    do {
        exibirFila(&fila);
        printf("\n1 - Jogar peca (dequeue)\n2 - Inserir nova peca (enqueue)\n0 - Sair\nOpcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca p;
            if (dequeue(&fila, &p))
                printf("Peca jogada: [%c %d]\n", p.nome, p.id);
            else
                printf("Fila vazia!\n");
        } else if (opcao == 2) {
            Peca p = gerarPeca(proximoId++);
            if (enqueue(&fila, p))
                printf("Peca inserida: [%c %d]\n", p.nome, p.id);
            else
                printf("Fila cheia!\n");
        }
    } while (opcao != 0);

    return 0;
}
