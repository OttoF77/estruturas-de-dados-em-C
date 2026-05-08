#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

// Protótipos das funções
void limparTela();
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void menuDeOrdenacao();
void insertionSortPorNome();
void insertionSortPorTipo();
void buscarItemVetorSequencial(); // Busca no Vetor
void buscarItemListaSequencial();  // Busca na Lista
void buscaBinariaVetor();         // Busca Binária no Vetor
int main() {

    int opcao;

    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair
    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        setbuf(stdin, NULL);

        // A estrutura switch trata cada opção chamando a função correspondente.
        switch (opcao) {
            case 1: inserirItem(); break;
            case 2: removerItem(); break;
            case 3: listarItens(); break;
            case 4: menuDeOrdenacao(); break;
            case 5: buscarItemVetorSequencial(); break;
            case 6: buscarItemListaSequencial(); break;
            case 7: buscaBinariaVetor(); break;
            case 0: printf("\nSaindo da Ilha...\n"); break;
            default: printf("\nOpcao invalida!\n");
        }
        
        if(opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
    } while (opcao != 0);

    // A ordenação e busca binária exigem que os dados estejam bem organizados.

    return 0;
}

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Estrutura para a lista encadeada contendo os dados do item e o ponteiro para o próximo nó.
struct No {
    Item dados;
    struct No* proximo;
};

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).


// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).
Item mochila[10];
int numItens = 0;
int comparacoesGlobal = 0;
bool ordenadaPorNome = false;
struct No* mochilaEncadeada = NULL;

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.
void limparTela() {
    for (int i = 0; i < 20; i++) printf("\n");
}

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.
void exibirMenu() {
    limparTela();
    printf("=== INVENTARIO AVENTUREIRO - FASE INTERMEDIARIA ===\n");
    printf("Itens: %d/10 | Ordenado Nome: %s\n", numItens, ordenadaPorNome ? "SIM" : "NAO");
    printf("-------------------------------------------\n");
    printf("1. Adicionar Item (Vetor e Lista)\n");
    printf("2. Remover Item (Vetor e Lista)\n");
    printf("3. Listar Itens (Ambas Estruturas)\n");
    printf("4. Ordenar Vetor (Habilitar Busca Binaria)\n");
    printf("5. Busca Sequencial [VETOR]\n");
    printf("6. Busca Sequencial [LISTA]\n");
    printf("7. Busca Binaria [VETOR]\n");
    printf("0. Sair\n");
    printf("-------------------------------------------\n");
}

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void inserirItem() {
    if (numItens < 10) {
        Item novoItem;
        printf("\n-- Adicionando Recurso --\n");
        printf("Nome do item: ");
        scanf("%30s", novoItem.nome);
        printf("Tipo: ");
        scanf("%20s", novoItem.tipo);
        printf("Quantidade: ");
        scanf("%d", &novoItem.quantidade);
        setbuf(stdin, NULL);

        // Atualiza Vetor
        mochila[numItens] = novoItem;

        // Atualiza Lista Encadeada (Inserção no início)
        struct No* novoNo = (struct No*) malloc(sizeof(struct No));
        if (novoNo != NULL) {
            novoNo->dados = novoItem;
            novoNo->proximo = mochilaEncadeada;
            mochilaEncadeada = novoNo;
        }

        numItens++;
        ordenadaPorNome = false;
        printf("\nItem adicionado com sucesso!\n");
    } else {
        printf("\nMochila cheia!\n");
    }
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removerItem() {
    char busca[30];
    printf("\nNome do item para remover: ");
    scanf("%s", busca);
    setbuf(stdin, NULL);

    // Remoção no Vetor
    int acheiVetor = -1;
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, busca) == 0) {
            acheiVetor = i;
            break;
        }
    }

    if (acheiVetor != -1) {
        for (int i = acheiVetor; i < numItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        numItens--;

        // Remoção na Lista Encadeada
        struct No* atual = mochilaEncadeada;
        struct No* anterior = NULL;
        while (atual != NULL) {
            if (strcmp(atual->dados.nome, busca) == 0) {
                if (anterior == NULL) mochilaEncadeada = atual->proximo;
                else anterior->proximo = atual->proximo;
                free(atual);
                break;
            }
            anterior = atual;
            atual = atual->proximo;
        }
        printf("\nItem removido de ambas as estruturas!\n");
        ordenadaPorNome = false;
    } else {
        printf("\nItem nao encontrado.\n");
    }
}

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
// Agora exibe os dados do Vetor e percorre a Lista Encadeada para validação.
void listarItens() {
    printf("\n--- [VETOR] ---\n");
    for (int i = 0; i < numItens; i++) {
        printf("%s | %s | Qtd: %d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }

    printf("\n--- [LISTA ENCADEADA] ---\n");
    struct No* atual = mochilaEncadeada;
    while (atual != NULL) {
        printf("%s | %s | Qtd: %d\n", atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }
}

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
void menuDeOrdenacao() {
    if (numItens < 2) {
        printf("\nItens insuficientes para ordenar.\n");
        return;
    }
    printf("\n1. Ordenar por Nome (A-Z)\n2. Ordenar por Tipo (A-Z)\nEscolha: ");
    int op;
    scanf("%d", &op);
    setbuf(stdin, NULL);

    if (op == 1) {
        insertionSortPorNome();
        ordenadaPorNome = true;
        printf("\nVetor ordenado por Nome. Comparacoes: %d\n", comparacoesGlobal);
        listarItens();
    } else if (op == 2) {
        insertionSortPorTipo();
        ordenadaPorNome = false;
        printf("\nVetor ordenado por Tipo. Comparacoes: %d\n", comparacoesGlobal);
        listarItens();
    }
}

// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
void insertionSortPorNome() {
    comparacoesGlobal = 0;
    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;
        while (j >= 0) {
            int res = strcmp(mochila[j].nome, chave.nome);
            comparacoesGlobal++;
            if (res > 0) {
                mochila[j + 1] = mochila[j];
                j--;
            } else {
                break;
            }
        }
        mochila[j + 1] = chave;
    }
}

// - Por tipo (ordem alfabética)
void insertionSortPorTipo() {
    comparacoesGlobal = 0;
    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;
        while (j >= 0) {
            int res = strcmp(mochila[j].tipo, chave.tipo);
            comparacoesGlobal++;
            if (res > 0) {
                mochila[j + 1] = mochila[j];
                j--;
            } else {
                break;
            }
        }
        mochila[j + 1] = chave;
    }
}

// - Por prioridade (da mais alta para a mais baixa)


// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
void buscarItemVetorSequencial() {
    char busca[30];
    int comps = 0;
    printf("\nBusca Sequencial [VETOR] - Nome: ");
    scanf("%s", busca);
    setbuf(stdin, NULL);

    for (int i = 0; i < numItens; i++) {
        comps++;
        if (strcmp(mochila[i].nome, busca) == 0) {
            printf("Encontrado! Nome: %s | Comparações: %d\n", mochila[i].nome, comps);
            return;
        }
    }
    printf("Item nao encontrado. Comparacoes: %d\n", comps);
}

void buscarItemListaSequencial() {
    char busca[30];
    int comps = 0;
    printf("\nBusca Sequencial [LISTA] - Nome: ");
    scanf("%s", busca);
    setbuf(stdin, NULL);

    struct No* atual = mochilaEncadeada;
    while (atual != NULL) {
        comps++;
        if (strcmp(atual->dados.nome, busca) == 0) {
            printf("Encontrado! Nome: %s | Comparações: %d\n", atual->dados.nome, comps);
            return;
        }
        atual = atual->proximo;
    }
    printf("Item nao encontrado. Comparacoes: %d\n", comps);
}

void buscaBinariaVetor() {
    if (!ordenadaPorNome) {
        printf("\nERRO: O vetor precisa estar ordenado por NOME primeiro!\n");
        return;
    }
    char busca[30];
    int comps = 0;
    printf("\nBusca Binaria [VETOR] - Nome: ");
    scanf("%s", busca);
    setbuf(stdin, NULL);

    int inicio = 0, fim = numItens - 1;
    while (inicio <= fim) {
        comps++;
        int meio = (inicio + fim) / 2;
        int res = strcmp(mochila[meio].nome, busca);
        if (res == 0) {
            printf("Encontrado! Nome: %s | Comparações: %d\n", mochila[meio].nome, comps);
            return;
        } else if (res < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    printf("Item nao encontrado. Comparacoes: %d\n", comps);
}