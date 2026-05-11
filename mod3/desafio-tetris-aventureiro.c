#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 10
#define MAX_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

// Fila circular de peças futuras
typedef struct {
    Peca itens[MAX_FILA];
    int frente, tras, tamanho;
} Fila;

// Pilha linear de peças reservadas
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->tamanho = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    return (Peca){tipos[rand() % 4], id};
}

int enqueue(Fila *f, Peca p) {
    if (f->tamanho == MAX_FILA) return 0;
    f->itens[f->tras] = p;
    f->tras = (f->tras + 1) % MAX_FILA;
    f->tamanho++;
    return 1;
}

int dequeue(Fila *f, Peca *p) {
    if (f->tamanho == 0) return 0;
    *p = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->tamanho--;
    return 1;
}

int push(Pilha *p, Peca peca) {
    if (p->topo == MAX_PILHA - 1) return 0;
    p->itens[++(p->topo)] = peca;
    return 1;
}

int pop(Pilha *p, Peca *peca) {
    if (p->topo == -1) return 0;
    *peca = p->itens[(p->topo)--];
    return 1;
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\nEstado atual:\n");

    printf("Fila de pecas\t\t");
    if (f->tamanho == 0)
        printf("(vazia)");
    else
        for (int i = 0; i < f->tamanho; i++)
            printf("[%c %d] ", f->itens[(f->frente + i) % MAX_FILA].nome,
                               f->itens[(f->frente + i) % MAX_FILA].id);

    printf("\nPilha de reserva\t(Topo -> Base): ");
    if (p->topo == -1)
        printf("(vazia)");
    else
        for (int i = p->topo; i >= 0; i--)
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);

    printf("\n");
}

int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int proximoId = 0;

    // Inicializa fila com 5 peças
    for (int i = 0; i < 5; i++)
        enqueue(&fila, gerarPeca(proximoId++));

    int opcao;
    do {
        exibirEstado(&fila, &pilha);
        printf("\n1 - Jogar peca\n2 - Reservar peca\n3 - Usar peca reservada\n0 - Sair\nOpcao: ");
        scanf("%d", &opcao);

        Peca p;
        if (opcao == 1) {
            // Remove da frente da fila e repõe automaticamente
            if (dequeue(&fila, &p)) {
                printf("Peca jogada: [%c %d]\n", p.nome, p.id);
                enqueue(&fila, gerarPeca(proximoId++));
            } else {
                printf("Fila vazia!\n");
            }
        } else if (opcao == 2) {
            // Move da frente da fila para o topo da pilha e repõe automaticamente
            if (fila.tamanho == 0) {
                printf("Fila vazia!\n");
            } else if (!push(&pilha, fila.itens[fila.frente])) {
                printf("Pilha de reserva cheia!\n");
            } else {
                dequeue(&fila, &p);
                printf("Peca reservada: [%c %d]\n", p.nome, p.id);
                enqueue(&fila, gerarPeca(proximoId++));
            }
        } else if (opcao == 3) {
            // Remove do topo da pilha
            if (pop(&pilha, &p))
                printf("Peca usada da reserva: [%c %d]\n", p.nome, p.id);
            else
                printf("Pilha de reserva vazia!\n");
        }
    } while (opcao != 0);

    return 0;
}
