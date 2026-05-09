#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Item;

// Estrutura para a lista encadeada contendo os dados do item e o ponteiro para o próximo nó.
struct No {
    Item dados;
    struct No* proximo;
};

// Protótipos das funções
void limparTela();
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void menuDeOrdenacao();
void bubbleSortNome(Item vetor[], int n);
void insertionSortTipo(Item vetor[], int n);
void selectionSortPrioridade(Item vetor[], int n);
void buscaBinariaVetor();
void buscarItemVetorSequencial();
void buscarItemListaSequencial();
void medirTempo(void (*algoritmo)(Item[], int), Item vetor[], int tamanho);
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
        if (scanf("%d", &opcao) != 1) {
            setbuf(stdin, NULL);
            continue;
        }
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


// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).


// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).
Item mochila[20]; // Aumentado para 20 conforme requisito final
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
    printf("=== TORRE DE RESGATE - FASE FINAL (AVANCADO) ===\n");
    printf("Componentes: %d/20 | Ordenado Nome: %s\n", numItens, ordenadaPorNome ? "SIM" : "NAO");
    printf("-------------------------------------------\n");
    printf("1. Cadastrar Componente (Vetor e Lista)\n");
    printf("2. Remover Componente\n");
    printf("3. Listar Inventario (Montagem Final)\n");
    printf("4. Estrategia de Ordenacao (Bubble/Insert/Select)\n");
    printf("5. Busca Sequencial [VETOR]\n");
    printf("6. Busca Sequencial [LISTA]\n");
    printf("7. Busca Binaria [ITEM-CHAVE]\n");
    printf("0. Sair\n");
    printf("-------------------------------------------\n");
}

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void inserirItem() {
    if (numItens < 20) {
        Item novoItem;
        printf("\n-- Cadastrar Componente da Torre --\n");
        printf("Nome (ex: chip central): ");
        fgets(novoItem.nome, 30, stdin);
        novoItem.nome[strcspn(novoItem.nome, "\n")] = 0;
        
        printf("Tipo (ex: controle, suporte): ");
        fgets(novoItem.tipo, 20, stdin);
        novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0;
        
        printf("Prioridade (1 a 10): ");
        scanf("%d", &novoItem.prioridade);
        setbuf(stdin, NULL);

        mochila[numItens] = novoItem;

        struct No* novoNo = (struct No*) malloc(sizeof(struct No));
        if (novoNo != NULL) {
            novoNo->dados = novoItem;
            novoNo->proximo = mochilaEncadeada;
            mochilaEncadeada = novoNo;
        }

        numItens++;
        ordenadaPorNome = false;
        printf("\nComponente integrado ao sistema!\n");
    } else {
        printf("\nLimite de componentes atingido!\n");
    }
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removerItem() {
    char busca[30];
    printf("\nNome do componente para remover: ");
    fgets(busca, 30, stdin);
    busca[strcspn(busca, "\n")] = 0;

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
        printf("\nComponente removido!\n");
        ordenadaPorNome = false;
    } else {
        printf("\nComponente nao localizado.\n");
    }
}

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
// Agora exibe os dados do Vetor e percorre a Lista Encadeada para validação.
void listarItens() {
    printf("\n%-20s | %-15s | %-10s\n", "NOME", "TIPO", "PRIORIDADE");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < numItens; i++) {
        printf("%-20s | %-15s | %-10d\n", mochila[i].nome, mochila[i].tipo, mochila[i].prioridade);
    }
}

// medirTempo():
// Funcao para medir o desempenho conforme requisito 5.
void medirTempo(void (*algoritmo)(Item[], int), Item vetor[], int tamanho) {
    clock_t inicio, fim;
    inicio = clock();
    algoritmo(vetor, tamanho);
    fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo de execucao: %f segundos\n", tempo);
    printf("Comparacoes realizadas: %d\n", comparacoesGlobal);
}

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
void menuDeOrdenacao() {
    if (numItens < 2) {
        printf("\nPoucos componentes para ordenar.\n");
        return;
    }
    printf("\nEstrategias de Ordenacao:\n");
    printf("1. Bubble Sort (por Nome)\n");
    printf("2. Insertion Sort (por Tipo)\n");
    printf("3. Selection Sort (por Prioridade)\n");
    printf("Escolha: ");
    int op;
    scanf("%d", &op);
    setbuf(stdin, NULL);

    ordenadaPorNome = false;
    switch(op) {
        case 1: 
            medirTempo(bubbleSortNome, mochila, numItens); 
            ordenadaPorNome = true;
            break;
        case 2: 
            medirTempo(insertionSortTipo, mochila, numItens); 
            break;
        case 3: 
            medirTempo(selectionSortPrioridade, mochila, numItens); 
            break;
        default: printf("Opcao invalida.\n"); return;
    }
    listarItens();
}

// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
void bubbleSortNome(Item vetor[], int n) {
    comparacoesGlobal = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparacoesGlobal++;
            if (strcmp(vetor[j].nome, vetor[j+1].nome) > 0) {
                Item temp = vetor[j];
                vetor[j] = vetor[j+1];
                vetor[j+1] = temp;
            }
        }
    }
}

// - Por tipo (ordem alfabética)
void insertionSortTipo(Item vetor[], int n) {
    comparacoesGlobal = 0;
    for (int i = 1; i < n; i++) {
        Item chave = vetor[i];
        int j = i - 1;
        while (j >= 0 && strcmp(vetor[j].tipo, chave.tipo) > 0) {
            comparacoesGlobal++;
            vetor[j + 1] = vetor[j];
            j--;
        }
        if (j >= 0) comparacoesGlobal++;
        vetor[j + 1] = chave;
    }
}

// - Por prioridade (da mais alta para a mais baixa)
void selectionSortPrioridade(Item vetor[], int n) {
    comparacoesGlobal = 0;
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            comparacoesGlobal++;
            if (vetor[j].prioridade < vetor[min_idx].prioridade)
                min_idx = j;
        }
        Item temp = vetor[min_idx];
        vetor[min_idx] = vetor[i];
        vetor[i] = temp;
    }
}

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
void buscarItemVetorSequencial() {
    char busca[30];
    int comps = 0;
    printf("\nBusca Sequencial [VETOR]: ");
    fgets(busca, 30, stdin);
    busca[strcspn(busca, "\n")] = 0;

    for (int i = 0; i < numItens; i++) {
        comps++;
        if (strcmp(mochila[i].nome, busca) == 0) {
            printf("Encontrado! Comparacoes: %d\n", comps);
            return;
        }
    }
    printf("Nao encontrado. Comparacoes: %d\n", comps);
}

void buscarItemListaSequencial() {
    char busca[30];
    int comps = 0;
    printf("\nBusca Sequencial [LISTA]: ");
    fgets(busca, 30, stdin);
    busca[strcspn(busca, "\n")] = 0;

    struct No* atual = mochilaEncadeada;
    while (atual != NULL) {
        comps++;
        if (strcmp(atual->dados.nome, busca) == 0) {
            printf("Encontrado na Lista! Comparacoes: %d\n", comps);
            return;
        }
        atual = atual->proximo;
    }
    printf("Nao encontrado na lista. Comparacoes: %d\n", comps);
}

void buscaBinariaVetor() {
    if (!ordenadaPorNome) {
        printf("\nERRO: Ordene por NOME (Bubble Sort) antes da busca binaria!\n");
        return;
    }
    char busca[30];
    printf("\nDigite o Nome do Componente-Chave: ");
    fgets(busca, 30, stdin);
    busca[strcspn(busca, "\n")] = 0;

    int comps = 0, inicio = 0, fim = numItens - 1;
    bool achou = false;

    while (inicio <= fim) {
        comps++;
        int meio = (inicio + fim) / 2;
        int res = strcmp(mochila[meio].nome, busca);
        if (res == 0) {
            printf("\n[CONFIRMADO] Componente-chave '%s' localizado!\n", mochila[meio].nome);
            printf("Tipo: %s | Prioridade: %d | Comparacoes: %d\n", mochila[meio].tipo, mochila[meio].prioridade, comps);
            achou = true;
            break;
        } else if (res < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    if (!achou) printf("\nComponente '%s' nao encontrado. Comparacoes: %d\n", busca, comps);
}