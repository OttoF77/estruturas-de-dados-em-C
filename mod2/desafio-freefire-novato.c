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
void buscarItem();

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
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                buscarItem();
                break;
            case 0:
                printf("\nSaindo da Ilha...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
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

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).
Item mochila[10];
int numItens = 0;
int comparacoes = 0;
bool ordenadaPorNome = false;

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.
void limparTela() {
    for (int i = 0; i < 20; i++) printf("\n");
}

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.
void exibirMenu() {
    limparTela();
    printf("=== INVENTARIO DE LOOT - FASE INICIAL ===\n");
    printf("Itens: %d/10\n", numItens);
    printf("-----------------------------------------\n");
    printf("1. Adicionar Item\n");
    printf("2. Remover Item (por nome)\n");
    printf("3. Listar Itens\n");
    printf("4. Buscar Item (Sequencial)\n");
    printf("0. Sair\n");
    printf("-----------------------------------------\n");
}

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void inserirItem() {
    if (numItens < 10) {
        printf("\n-- Adicionando Recurso --\n");
        printf("Nome do item: ");
        scanf("%s", mochila[numItens].nome);
        printf("Tipo (arma, municao, cura): ");
        scanf("%s", mochila[numItens].tipo);
        printf("Quantidade: ");
        scanf("%d", &mochila[numItens].quantidade);

        numItens++;
        ordenadaPorNome = false;
        printf("\nItem adicionado com sucesso!\n");
        listarItens();
    } else {
        printf("\nMochila cheia!\n");
    }
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removerItem() {
    char busca[30];
    int achei = -1;

    printf("\nNome do item para remover: ");
    scanf("%s", busca);
    setbuf(stdin, NULL);

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, busca) == 0) {
            achei = i;
            break;
        }
    }

    if (achei != -1) {
        for (int i = achei; i < numItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        numItens--;
        printf("\nItem removido!\n");
        listarItens();
    } else {
        printf("\nItem nao encontrado.\n");
    }
}

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
void listarItens() {
    printf("\n--- LISTA DE ITENS NA MOCHILA ---\n");
    printf("%-15s | %-10s | %-5s\n", "NOME", "TIPO", "QTD");
    for (int i = 0; i < numItens; i++) {
        printf("%-15s | %-10s | %-5d\n", 
            mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
void buscarItem() {
    char busca[30];
    bool encontrado = false;

    printf("\nDigite o nome do item para buscar: ");
    scanf("%s", busca);
    setbuf(stdin, NULL);

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, busca) == 0) {
            printf("\nItem Localizado!");
            printf("\nNome: %s | Tipo: %s | Qtd: %d\n", 
                mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        printf("\nO item '%s' nao esta na mochila.\n", busca);
    }
}