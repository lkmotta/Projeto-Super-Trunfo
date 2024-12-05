#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char nome[21];
    char letra;
    int num;
    int super_trunfo;
    int atributo_1;
    int atributo_2;
    int atributo_3;
    int atributo_4;
}Cartas;

void inserir_cartas();
void listar_cartas();
void pesquisar_carta();
void alterar_carta();
void excluir_carta();

int main(){
    FILE* arq_cartas = fopen("cartas.csv","w+");
    int sair=0;
    int escolha;
    do
    {
        do
        {
            printf("qual ação deseja fazer:\n1 - inserir cartas\n2 - listar as cartas\n3 - pesquisar uma carta\n4 - alterar uma carta\n5 - excluir carta\n\n");
            scanf("%i",&escolha);
        } while (escolha>5 || escolha<1);

        switch (escolha)
        {
        case 1:
            inserir_cartas();
            break;
        case 2:
            listar_cartas();
            break;
        case 3:
            pesquisar_carta();
            break;
        case 4:
            alterar_carta();
            break;
        case 5:
            excluir_carta();
            break;
        }
    } while (sair!=1);
    
    
    
    
    return 0;
}