#include <stdio.h>
#include <strings.h>

//cria constante para armazenar o numero de territórios
#define QTDE_TERRITORIOS 5

//cria a struct Territorio com os campos: char nome[30],char cor[10], int tropas
typedef struct{
	char nome[30];
	char cor[10];
	int tropas;
}Territorio;

//cria vetor para armazenar 5 territórios
Territorio territorios[QTDE_TERRITORIOS];

//função para limpar o buffer de entrada
void limparBuffer(){
    int c;
    while ((c=getchar()) != '\n' && c != EOF);
}

int main(){

    //limpa o buffer de entrada
    setbuf(stdin, NULL);

    //laço para ler os dados dos territórios
    for(int i=0; i<QTDE_TERRITORIOS; i++){
        printf("Digite o nome do território %d: ", i+1);
        fgets(territorios[i].nome, 30, stdin);
        //remove o caractere de nova linha do final da string
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Digite a cor do território %s: ", territorios[i].nome);
        fgets(territorios[i].cor, 10, stdin);
        //remove o caractere de nova linha do final da string
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Digite o número de tropas no território %s: ", territorios[i].nome);
        scanf("%d", &territorios[i].tropas);
        limparBuffer(); //limpa o buffer de entrada após ler o número de tropas

    }

    //laço para imprimir os dados dos territórios
    printf("\nDados dos territórios:\n");
    for(int i=0; i<QTDE_TERRITORIOS; i++){
        printf("Território: %s, Cor: %s, Tropas: %d\n", territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }

    return 0;

}