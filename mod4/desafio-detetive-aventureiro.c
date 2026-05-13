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
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// - Use funções como criarSala(), conectarSalas() e explorarSalas().
// criarSala() – cria, de forma dinâmica, uma sala com nome
Sala *criarSala(const char *nome, const char *pista, Sala *esquerda, Sala *direita) {
    Sala *sala = (Sala *)malloc(sizeof(Sala));
    strncpy(sala->nome,  nome,  sizeof(sala->nome)  - 1);
    strncpy(sala->pista, pista, sizeof(sala->pista) - 1);
    sala->esquerda = esquerda;
    sala->direita  = direita;
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

// 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
//
// - Crie uma struct Pista com campo texto (string).
typedef struct PistaNode {
    char texto[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
// - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
// - Não precisa remover ou balancear a árvore.
// - Use funções para modularizar: inserirPista(), listarPistas().
// inserirPista() – insere uma nova pista na árvore de pistas
PistaNode *inserirPista(PistaNode *raiz, const char *texto) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode *)malloc(sizeof(PistaNode));
        strncpy(novo->texto, texto, sizeof(novo->texto) - 1);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    int cmp = strcmp(texto, raiz->texto);
    if (cmp < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else if (cmp > 0)
        raiz->direita  = inserirPista(raiz->direita,  texto);
    return raiz;
}

// - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
// - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.
// exibirPistas() – imprime a árvore de pistas em ordem alfabética
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("  - %s\n", raiz->texto);
    exibirPistas(raiz->direita);
}

// - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
// explorarSalasComPistas() – controla a navegação entre salas e coleta de pistas
PistaNode *explorarSalasComPistas(Sala *atual, PistaNode *pistas) {
    char opcao;

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            pistas = inserirPista(pistas, atual->pista);
        }

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Sem saidas. Fim da exploracao.\n");
            break;
        }

        printf("Caminhos disponiveis:");
        if (atual->esquerda) printf(" [e] Esquerda");
        if (atual->direita)  printf(" [d] Direita");
        printf(" [s] Sair\nEscolha: ");

        scanf(" %c", &opcao);

        if (opcao == 's') {
            printf("Exploracao encerrada.\n");
            break;
        } else if (opcao == 'e' && atual->esquerda) {
            atual = atual->esquerda;
        } else if (opcao == 'd' && atual->direita) {
            atual = atual->direita;
        } else {
            printf("Opcao invalida ou caminho inexistente. Tente novamente.\n");
        }
    }

    return pistas;
}

int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - Nenhuma inserção dinâmica é necessária neste nível.
    // main() – monta o mapa inicial e dá início à exploração
    Sala *mapa =
        criarSala("Hall de Entrada", "",
            criarSala("Sala de Estar", "Pegadas no tapete",
                criarSala("Biblioteca", "Livro com pagina marcada", NULL, NULL),
                criarSala("Sotao",      "Mala com roupas escondidas", NULL, NULL)),
            criarSala("Cozinha", "Faca fora do lugar",
                criarSala("Despensa",   "Veneno entre os alimentos", NULL, NULL),
                criarSala("Jardim",     "Buraco recentemente cavado", NULL, NULL)));

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.
    PistaNode *pistas = NULL;

    printf("=== Detective Quest ===\n");
    pistas = explorarSalasComPistas(mapa, pistas);

    printf("\n=== Pistas coletadas (ordem alfabetica) ===\n");
    if (pistas == NULL)
        printf("Nenhuma pista coletada.\n");
    else
        exibirPistas(pistas);

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
