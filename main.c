#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> //para windows

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

void buracracia(char nome[], int tamanho){
    fgets(nome,tamanho,stdin);
    nome[strcspn(nome, "\n")] = 0;
    setbuf(stdin,NULL);
}
void inserir_cartas(FILE* arq_carta, Cartas carta[]){
    int cartas_add;
    do
    {
        printf("quantas cartas desejar inserir: ");
        scanf("%i",&cartas_add);
        setbuf(stdin, NULL);
        if (cartas_add<0)
        {
            printf("Número inválido. Digite um número positivo.\n");
        }
        
    } while (cartas_add<0);
    for (int i = 0; i < cartas_add; i++)
    {
        printf("Nome da carta: ");
        buracracia(carta[i].nome,21);
        printf("Letra da carta: ");
        scanf("%c",&carta[i].letra);
        setbuf(stdin, NULL);
        printf("Numero da carta: ");
        scanf("%i",&carta[i].num);
        printf("Super Trunfo:\n1 - sim\n2 - não\n");

        // VERIFICAR MAIS PRA FRENTE SE JÁ TEM UM SUPER TRUNFO, O MESMO COM A LETRA E NÚMERO

        scanf("%i",&carta[i].super_trunfo);
        printf("Atributo 1: ");
        scanf("%i",&carta[i].atributo_1);
        printf("Atributo 2: ");
        scanf("%i",&carta[i].atributo_2);
        printf("Atributo 3: ");
        scanf("%i",&carta[i].atributo_3);
        printf("Atributo 4: ");
        scanf("%i",&carta[i].atributo_4);
        setbuf(stdin, NULL);
        fprintf(arq_carta,"%s,%c,%i,%i,%i,%i,%i,%i\n",carta[i].nome,carta[i].letra,carta[i].num,carta[i].super_trunfo,carta[i].atributo_1,carta[i].atributo_2,carta[i].atributo_3,carta[i].atributo_4);
    }
    

};

int main(){
    setlocale(LC_ALL, "portuguese"); //para windows
    FILE* arq_cartas = fopen("cartas.csv","w");
    if(arq_cartas == NULL)
    {
        printf("Não foi possivel abrir o arquivo!\n");
        return 1;
    }
    Cartas carta[32]; //provalvelmente isso vai ser mudado

    int sair=0;
    int escolha;
    do
    {
        do
        {
            printf("qual ação deseja \afazer:\n1 - inserir cartas\n2 - listar as cartas\n3 - pesquisar uma carta\n4 - alterar uma carta\n5 - excluir carta\n6 - sair\n");
            scanf("%i",&escolha);
            if (escolha>5 || escolha<1)
            {
                printf("Escolha inválida!\n");
            }
            setbuf(stdin, NULL);
            
        } while (escolha>5 || escolha<1);

        switch (escolha)
        {
        case 1:
            inserir_cartas(arq_cartas,carta);
            break;
        case 2:
            
            break;
        case 3:
            
            break;
        case 4:
            
            break;
        case 5:
            
            break;
        case 6:
            sair=1;
        break;
        }
    } while (sair!=1);
    
    
    
    
    return 0;
}
