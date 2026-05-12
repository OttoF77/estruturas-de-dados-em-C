#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* Cria dinamicamente uma sala com nome e filhos esquerdo/direito */
Sala *criarSala(const char *nome, Sala *esquerda, Sala *direita) {
    Sala *sala = (Sala *)malloc(sizeof(Sala));
    strncpy(sala->nome, nome, sizeof(sala->nome) - 1);
    sala->esquerda = esquerda;
    sala->direita = direita;
    return sala;
}

/* Permite a navegação interativa do jogador pela árvore até um nó-folha */
void explorarSalas(Sala *atual) {
    char opcao;

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Sem saidas. Fim da exploracao.\n");
            return;
        }

        printf("Caminhos disponiveis:");
        if (atual->esquerda) printf(" [e] Esquerda");
        if (atual->direita)  printf(" [d] Direita");
        printf(" [s] Sair\nEscolha: ");

        scanf(" %c", &opcao);

        if (opcao == 's') {
            printf("Exploracao encerrada.\n");
            return;
        } else if (opcao == 'e' && atual->esquerda) {
            atual = atual->esquerda;
        } else if (opcao == 'd' && atual->direita) {
            atual = atual->direita;
        } else {
            printf("Opcao invalida ou caminho inexistente. Tente novamente.\n");
        }
    }
}

/* Monta o mapa da mansão e inicia a exploração */
int main() {
    Sala *mapa =
        criarSala("Hall de Entrada",
            criarSala("Sala de Estar",
                criarSala("Biblioteca", NULL, NULL),
                criarSala("Escritorio", NULL, NULL)),
            criarSala("Cozinha",
                criarSala("Despensa", NULL, NULL),
                criarSala("Jardim", NULL, NULL)));

    printf("=== Detective Quest ===\n");
    explorarSalas(mapa);

    return 0;
}
