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
// - Foco em: Alocação dinâmica, lógica de vitória e missões estratégicas.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
#define TOTAL_MISSOES 5
#define MAX_DESC_MISSAO 100

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
void exibirMapa(const Territorio* territorios, int total);
void exibirMissao(const char* missao);

// Funções de lógica principal do jogo:
Territorio* inicializarMapa(int* total);
Territorio* cadastrarTerritorios(Territorio* territorios, int* total);
void faseDeAtaque(Territorio* territorios, int total);
void atacar(Territorio* atacante, Territorio* defensor);

// Funções de Missão:
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);

// Função utilitária:
void liberarMemoria(Territorio* territorios, char* missao);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    setlocale(LC_ALL, "Portuguese");

    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));

    int total = 0;
    // - Inicializa o mapa com 5 territórios automáticos.
    Territorio* territorios = inicializarMapa(&total);
    char* missaoJogador = (char*)malloc(MAX_DESC_MISSAO * sizeof(char));
    
    if (territorios == NULL || missaoJogador == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        return 1;
    }

    int opcao;

    // Vetor de descrições de missões
    char* listaMissoes[TOTAL_MISSOES] = {
        "Conquistar 3 territorios",
        "Eliminar todas as tropas inimigas",
        "Acumular um total de 15 tropas",
        "Dominar 4 territorios estrategicos",
        "Ter 10 tropas no seu exercito"
    };

    printf("Bem-vindo ao WAR Estruturado!\n");

    // Sorteia e atribui a missão secreta
    atribuirMissao(missaoJogador, listaMissoes, TOTAL_MISSOES);
    exibirMissao(missaoJogador);

    // 2. Laço Principal do Jogo (Game Loop):
    do {
        // Verificação silenciosa de vitória
        if (total > 0 && verificarMissao(missaoJogador, territorios, total)) {
            printf("\n******************************************\n");
            printf("PARABENS! Voce cumpriu sua missao:\n\"%s\"\n", missaoJogador);
            printf("VOCE EH O VENCEDOR!\n");
            printf("******************************************\n");
            break;
        }

        exibirMenuPrincipal();
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                territorios = cadastrarTerritorios(territorios, &total);
                break;
            case 2:
                exibirMapa(territorios, total);
                break;
            case 3:
                faseDeAtaque(territorios, total);
                break;
            case 4:
                exibirMissao(missaoJogador);
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
    liberarMemoria(territorios, missaoJogador);

    return 0;
}

// --- Implementação das Funções ---

// inicializarMapa():
// Cria automaticamente os 5 territórios iniciais para agilizar o início do jogo.
Territorio* inicializarMapa(int* total) {
    int n = 5;
    Territorio* t = (Territorio*)malloc(n * sizeof(Territorio));
    if (t == NULL) return NULL;

    // Dados pré-definidos para os territórios iniciais
    char nomes[5][30] = {"America", "Asia", "Africa", "Europa", "Oceania"};
    char cores[5][10] = {"Verde", "Preto", "Branco", "Amarelo", "Vermelho"};
    int tropas[5] = {10, 8, 5, 7, 6};

    for (int i = 0; i < n; i++) {
        strcpy(t[i].nome, nomes[i]);
        strcpy(t[i].cor, cores[i]);
        t[i].tropas = tropas[i];
    }

    *total = n;
    printf("Mapa inicializado com %d territorios automáticos.\n", n);
    return t;
}

// cadastrarTerritorios():
// Gerencia a expansão dinâmica do mapa e entrada de dados dos territórios.
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
// Libera a memória do mapa e da string de missão.
void liberarMemoria(Territorio* territorios, char* missao) {
    free(territorios);
    free(missao);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal() {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1. Cadastrar território\n");
    printf("2. Exibir territórios\n");
    printf("3. Realizar ataque\n");
    printf("4. Exibir missão\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
void exibirMapa(const Territorio* territorios, int total) {
    printf("\n=== LISTA DE TERRITÓRIOS ===\n");
    for(int i=0; i<total; i++){
        printf("%d. Território: %-15s | Cor: %-10s | Tropas: %d\n", i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
}

// atribuirMissao():
// Sorteia uma missão do vetor de strings e copia para o destino via ponteiro.
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// exibirMissao():
// Mostra a missão atual (passagem por valor/ponteiro constante).
void exibirMissao(const char* missao) {
    printf("\nSUA MISSAO SECRETA: %s\n", missao);
}

// faseDeAtaque():
// Interface para selecionar atacante e defensor com validações básicas.
void faseDeAtaque(Territorio* territorios, int total) {
    if (total < 2) {
        printf("Sao necessarios pelo menos 2 territorios para atacar.\n");
        return;
    }

    // Exibe a lista para o usuário se orientar
    exibirMapa(territorios, total);

    int escolhaAtacante, escolhaDefensor;
    printf("\nDigite o NUMERO do territorio ATACANTE (1 a %d): ", total);
    scanf("%d", &escolhaAtacante);
    limparBuffer();

    printf("Digite o NUMERO do territorio DEFENSOR (1 a %d): ", total);
    scanf("%d", &escolhaDefensor);
    limparBuffer();

    // Converte a escolha do usuário (1-base) para o índice do vetor (0-base)
    int origem = escolhaAtacante - 1;
    int destino = escolhaDefensor - 1;

    if (origem < 0 || origem >= total || destino < 0 || destino >= total) {
        printf("Opção inválida! Operação cancelada.\n");
        return;
    }

    if (territorios[origem].tropas <= 1) {
        printf("Erro: O territorio atacante deve ter mais de 1 tropa para realizar um ataque!\n");
        return;
    }

    if (strcmp(territorios[origem].cor, territorios[destino].cor) == 0) {
        printf("Não é possível atacar um território da mesma cor!\n");
        return;
    }

    atacar(&territorios[origem], &territorios[destino]);
}

// atacar():
// Simula a batalha entre dois ponteiros de Territorio e aplica as regras de conquista.
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n--- BATALHA: %s vs %s ---\n", atacante->nome, defensor->nome);
    printf("Ataque: %d | Defesa: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // Requisito: transfere a cor e metade das tropas se o atacante vencer
        printf("Vitoria no ataque! %s foi conquistado.\n", defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas -= defensor->tropas;
    } else {
        // Requisito: Caso contrário, o atacante perde uma tropa.
        atacante->tropas--;
        printf("A defesa resistiu! O atacante perdeu 1 tropa.\n");
    }
}

// verificarMissao():
// Analisa o estado do mapa para ver se a condição da string 'missao' foi atendida.
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    const char* corJogador = "Verde"; 
    int contagemTerritorios = 0;
    int totalTropas = 0;
    int inimigosRestantes = 0;

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            contagemTerritorios++;
            totalTropas += mapa[i].tropas;
        } else {
            inimigosRestantes += mapa[i].tropas;
        }
    }

    if (strstr(missao, "3 territorios") && contagemTerritorios >= 3) return 1;
    if (strstr(missao, "Eliminar") && inimigosRestantes <= 0) return 1;
    if (strstr(missao, "15 tropas") && totalTropas >= 15) return 1;
    if (strstr(missao, "4 territorios") && contagemTerritorios >= 4) return 1;
    if (strstr(missao, "10 tropas") && totalTropas >= 10) return 1;
    
    return 0; // Missão ainda não cumprida
}

// limparBuffer():
// Evita problemas de leitura com scanf e fgets.
void limparBuffer(){
    int c;
    while ((c=getchar()) != '\n' && c != EOF);
}