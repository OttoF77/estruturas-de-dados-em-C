// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

typedef struct{
	char nome[30];
	char cor[10];
	int tropas;
}Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:

void limparBuffer();

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirTerritorios(const Territorio* territorios, int total);

// Funções de lógica principal do jogo:
Territorio* cadastrarTerritorios(Territorio* territorios, int* total);
void faseDeAtaque(Territorio* territorios, int total);
void simularAtaque(Territorio* territorios, int total, int origem, int destino);

// Função utilitária:
void liberarMemoria(Territorio* territorios);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    setlocale(LC_ALL, "Portuguese");

    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));

    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    Territorio* territorios = (Territorio*)malloc(sizeof(Territorio));
    if (territorios == NULL) {
        printf("Erro: Falha na alocação inicial de memória!\n");
        return 1;
    }
    int total = 0;
    int opcao;

    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    //limpa buffer de entrada para evitar problemas com scanf e getchar
    limparBuffer();

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
    //laço para imprimir os dados dos territórios
    do {
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                territorios = cadastrarTerritorios(territorios, &total);
                break;
            case 2:
                exibirTerritorios(territorios, total);
                break;
            case 3:
                faseDeAtaque(territorios, total);
                break;
            case 0:
                printf("Encerrando o jogo. Obrigado por jogar!\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

        if (opcao != 0) {
            printf("Pressione Enter para continuar...");
            getchar();
        }
    } while (opcao != 0);

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(territorios);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
Territorio* cadastrarTerritorios(Territorio* territorios, int* total) {
    int continuar = 1;
    int i = *total;

    while(continuar) {
        Territorio* tmp = (Territorio*)realloc(territorios, (i + 1) * sizeof(Territorio));
        if (tmp == NULL) {
            printf("Erro: Falha na alocação de memória!\n");
            break;
        }
        territorios = tmp;
        printf("Digite o nome do território %d: ", i+1);
        fgets(territorios[i].nome, 30, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Digite a cor do território %s: ", territorios[i].nome);
        fgets(territorios[i].cor, 10, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Digite o número de tropas no território %s: ", territorios[i].nome);
        scanf("%d", &territorios[i].tropas);
        limparBuffer();

        i++;

        printf("Deseja cadastrar outro território? (1 - Sim, 0 - Não): ");
        scanf("%d", &continuar);
        limparBuffer();
    }

    *total = i;
    return territorios;
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio* territorios) {
    free(territorios);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal() {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1. Cadastrar território\n");
    printf("2. Exibir territórios\n");
    printf("3. Realizar ataque\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirTerritorios(const Territorio* territorios, int total) {
    printf("\nDados dos territórios:\n");
    for(int i=0; i<total; i++){
        printf("Território: %s, Cor: %s, Tropas: %d\n", territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
void faseDeAtaque(Territorio* territorios, int total) {
    printf("Digite o índice do território de origem (0 a %d): ", total-1);
    int origem;
    scanf("%d", &origem);
    limparBuffer();

    printf("Digite o índice do território de destino (0 a %d): ", total-1);
    int destino;
    scanf("%d", &destino);
    limparBuffer();

    if (origem < 0 || origem >= total || destino < 0 || destino >= total) {
        printf("Territórios inválidos. Tente novamente.\n");
        return;
    }

    simularAtaque(territorios, total, origem, destino);
}

// Chama a função simularAtaque() para executar a lógica da batalha.
// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio* territorios, int total, int origem, int destino) {
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("Atacante rolou: %d | Defensor rolou: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        territorios[destino].tropas--;
        printf("Atacante venceu! %s perdeu uma tropa.\n", territorios[destino].nome);
    } else if (dadoAtacante < dadoDefensor) {
        territorios[origem].tropas--;
        printf("Defensor venceu! %s perdeu uma tropa.\n", territorios[origem].nome);
    } else {
        territorios[origem].tropas--;
        territorios[destino].tropas--;
        printf("Empate! Ambos perdem uma tropa.\n");
    }

    if (territorios[destino].tropas <= 0) {
        strcpy(territorios[destino].cor, territorios[origem].cor);
        territorios[destino].tropas = territorios[origem].tropas / 2;
        territorios[origem].tropas /= 2;
        printf("%s foi conquistado por %s!\n", territorios[destino].nome, territorios[origem].nome);
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBuffer(){
    int c;
    while ((c=getchar()) != '\n' && c != EOF);
}