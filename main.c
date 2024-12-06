#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> // para Windows
// teste
#define MAX_INSERIR_CARTAS 10

typedef struct{
    char nome[21];
    char letra;
    int num;
    int super_trunfo;
    int atributo_1;
    int atributo_2;
    int atributo_3;
    int atributo_4;
    int atributo_5;
}Cartas;

/**
 * @brief Tratamento de dados pra receber string
 * 
 * @param nome 
 * @param tamanho 
 */
void burocracia(char nome[], int tamanho){
    fgets(nome, tamanho, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    setbuf(stdin, NULL);
}

/**
 * @brief Pega os dados das cartas dados pelo usuÃ¡rio e insere no arquivo .csv
 * 
 * @param arq_carta 
 * @param carta 
 */
void inserir_cartas(FILE* arq_carta, Cartas carta[]){
    int cartas_add;
    
    printf("\nInsira quantas cartas desejar inserir: ");
    do{
        scanf("%i", &cartas_add);
        if (cartas_add < 0){
            printf("\n\033[1mNúmero inválido. Digite um número positivo:\033[m ");
        }else if (cartas_add > MAX_INSERIR_CARTAS){
            printf("\n\033[1mNúmero máximo de cartas excedido. Digite um número menor ou igual a %i:\033[m ", MAX_INSERIR_CARTAS);
        }else break;
    } while(1);
    
    for (int i = 0; i < cartas_add; i++){
        printf("\nNome da carta: ");
        burocracia(carta[i].nome, sizeof(carta[i].nome));
        
        printf("\nLetra da carta: ");
        scanf("%c",&carta[i].letra);
        setbuf(stdin, NULL);
        
        printf("\nNÂº da carta: ");
        scanf("%i",&carta[i].num);
        printf("Super Trunfo?\n1 - sim\n2 - nÃ£o\n");
        

        // VERIFICAR MAIS PRA FRENTE SE JÃ TEM UM SUPER TRUNFO, O MESMO COM A LETRA E NÃšMERO

        scanf("%i",&carta[i].super_trunfo);
        printf("Atributo 1: ");
        scanf("%i",&carta[i].atributo_1);
        printf("Atributo 2: ");
        scanf("%i",&carta[i].atributo_2);
        printf("Atributo 3: ");
        scanf("%i",&carta[i].atributo_3);
        printf("Atributo 4: ");
        scanf("%i",&carta[i].atributo_4);
        printf("Atributo 5: ");
        scanf("%i",&carta[i].atributo_5);
        setbuf(stdin, NULL);
        fprintf(arq_carta,"%s,%c,%i,%i,%i,%i,%i,%i\n",carta[i].nome,carta[i].letra,carta[i].num,carta[i].super_trunfo,carta[i].atributo_1,carta[i].atributo_2,carta[i].atributo_3,carta[i].atributo_4,carta[i].atributo_5);
    }
};

int main(){
    setlocale(LC_ALL, "portuguese");

    // abrir, passar pro vetor, fechar
    // inserir: abrir em "w" adicionar 
    
    FILE* arq_cartas = fopen("cartas.csv","w+");
    if(arq_cartas == NULL){
        printf("\n\033[1;91mNão foi possivel abrir o arquivo!\033[m\n");
        return 1;
    }

    Cartas carta[32]; // provalvelmente isso vai ser mudado

    int sair = 0;
    int escolha;

    do{ // loop Menu
        
        printf("\n\nEscolha uma opção:\n1 - Inserir cartas\n2 - Listar as cartas\n3 - Pesquisar uma carta\n4 - Alterar uma carta\n5 - Excluir carta\n6 - Sair\n: ");
        do{
            //system("CLS"); // se for Windows
            //system("clear") // se for Linux
            scanf("%i", &escolha);
            if ((escolha > 5) || (escolha < 1)){
                setbuf(stdin, NULL);
                printf("\n\033[1;91mEscolha inválida! Insira um nº de 1 a 6:\033[m ");
            }else break;
            
        } while (1);

        switch (escolha){
        case 1:
            inserir_cartas(arq_cartas, carta);
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
        default:
            break;
        }
    } while (sair != 1);
    
    fclose(arq_cartas);
    return 0;
}
