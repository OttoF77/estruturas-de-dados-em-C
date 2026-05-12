#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA  10
#define MAX_PILHA  3

typedef struct {
    char nome;
    int  id;
} Peca;

// Fila circular de peças futuras
typedef struct {
    Peca itens[MAX_FILA];
    int  frente, tras, tamanho;
} Fila;

// Pilha linear de peças reservadas (LIFO)
typedef struct {
    Peca itens[MAX_PILHA];
    int  topo;
} Pilha;

// ---------- fila ----------

void inicializarFila(Fila *f) {
    f->frente = f->tras = f->tamanho = 0;
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

// ---------- pilha ----------

void inicializarPilha(Pilha *p) { p->topo = -1; }

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

// ---------- geração ----------

Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    return (Peca){tipos[rand() % 4], id};
}

// ---------- exibição ----------

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

// ---------- troca múltipla ----------

// Troca as 3 primeiras peças da fila com as 3 peças da pilha.
// Após a troca: as 3 primeiras posições da fila recebem as peças da pilha
// (do topo para a base) e a pilha recebe as 3 primeiras da fila (invertidas,
// mantendo a semântica LIFO — a que estava na frente vai para o topo).
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->tamanho < MAX_PILHA || p->topo < MAX_PILHA - 1) {
        printf("Troca multipla requer pelo menos 3 pecas na fila e 3 na pilha!\n");
        return;
    }

    // Salva as 3 primeiras da fila
    Peca daFila[MAX_PILHA];
    for (int i = 0; i < MAX_PILHA; i++)
        daFila[i] = f->itens[(f->frente + i) % MAX_FILA];

    // Salva as 3 da pilha (índice 0 = base, topo = MAX_PILHA-1)
    Peca daPilha[MAX_PILHA];
    for (int i = 0; i < MAX_PILHA; i++)
        daPilha[i] = p->itens[i];

    // Coloca peças da pilha nas 3 primeiras posições da fila
    // topo da pilha → frente da fila
    for (int i = 0; i < MAX_PILHA; i++)
        f->itens[(f->frente + i) % MAX_FILA] = daPilha[MAX_PILHA - 1 - i];

    // Coloca peças da fila na pilha
    // frente da fila → topo da pilha
    for (int i = 0; i < MAX_PILHA; i++)
        p->itens[i] = daFila[MAX_PILHA - 1 - i];

    printf("Acao: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// ---------- main ----------

int main() {
    srand(time(NULL));

    Fila  fila;
    Pilha pilha;
    int   proximoId = 0;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa fila com 5 peças
    for (int i = 0; i < 5; i++)
        enqueue(&fila, gerarPeca(proximoId++));

    int opcao;
    do {
        exibirEstado(&fila, &pilha);
        printf("\n1 - Jogar peca da frente da fila\n"
               "2 - Enviar peca da fila para a pilha de reserva\n"
               "3 - Usar peca da pilha de reserva\n"
               "4 - Trocar peca da frente da fila com o topo da pilha\n"
               "5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n"
               "0 - Sair\n"
               "Opcao: ");
        scanf("%d", &opcao);

        Peca p, tmp;
        switch (opcao) {
            case 1:
                // Remove da frente da fila e repõe automaticamente
                if (dequeue(&fila, &p)) {
                    printf("Peca jogada: [%c %d]\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca(proximoId++));
                } else {
                    printf("Fila vazia!\n");
                }
                break;

            case 2:
                // Move frente da fila para o topo da pilha e repõe automaticamente
                if (fila.tamanho == 0) {
                    printf("Fila vazia!\n");
                } else if (!push(&pilha, fila.itens[fila.frente])) {
                    printf("Pilha de reserva cheia!\n");
                } else {
                    dequeue(&fila, &p);
                    printf("Peca reservada: [%c %d]\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca(proximoId++));
                }
                break;

            case 3:
                // Remove do topo da pilha
                if (pop(&pilha, &p))
                    printf("Peca usada da reserva: [%c %d]\n", p.nome, p.id);
                else
                    printf("Pilha de reserva vazia!\n");
                break;

            case 4:
                // Troca simples: frente da fila <-> topo da pilha
                if (fila.tamanho == 0) {
                    printf("Fila vazia!\n");
                } else if (pilha.topo == -1) {
                    printf("Pilha de reserva vazia!\n");
                } else {
                    tmp = fila.itens[fila.frente];
                    fila.itens[fila.frente] = pilha.itens[pilha.topo];
                    pilha.itens[pilha.topo] = tmp;
                    printf("Troca realizada: fila[frente]=[%c %d] <-> pilha[topo]=[%c %d]\n",
                           fila.itens[fila.frente].nome, fila.itens[fila.frente].id,
                           pilha.itens[pilha.topo].nome, pilha.itens[pilha.topo].id);
                }
                break;

            case 5:
                // Troca múltipla: 3 primeiros da fila <-> 3 da pilha
                trocaMultipla(&fila, &pilha);
                break;
        }
    } while (opcao != 0);

    return 0;
}
