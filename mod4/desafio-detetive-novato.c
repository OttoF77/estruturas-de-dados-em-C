#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
//
// - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// - Use funções como criarSala(), conectarSalas() e explorarSalas().
// criarSala() – cria, de forma dinâmica, uma sala com nome
Sala *criarSala(const char *nome, Sala *esquerda, Sala *direita) {
    Sala *sala = (Sala *)malloc(sizeof(Sala));
    strncpy(sala->nome, nome, sizeof(sala->nome) - 1);
    sala->esquerda = esquerda;
    sala->direita = direita;
    return sala;
}

// explorarSalas() – permite a navegação do jogador pela árvore
// - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
// - Finalize a exploração com uma opção de saída (s).
// - Exiba o nome da sala a cada movimento.
// - Use recursão ou laços para caminhar pela árvore.
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

int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - Nenhuma inserção dinâmica é necessária neste nível.
    // main() – monta o mapa inicial e dá início à exploração
    Sala *mapa =
        criarSala("Hall de Entrada",
            criarSala("Sala de Estar",
                criarSala("Biblioteca", NULL, NULL),
                criarSala("Sotao",      NULL, NULL)),
            criarSala("Cozinha",
                criarSala("Despensa",   NULL, NULL),
                criarSala("Jardim",     NULL, NULL)));

    printf("=== Detective Quest ===\n");
    explorarSalas(mapa);

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o "suspeito mais provável" baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}
