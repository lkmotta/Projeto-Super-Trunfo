// Comando de compilacao
// gcc *.c -o main.exe
// makefile atual somente em windows
// usando chocolatey para instalar o make comandos: make para rodar o makefile e make clean para limpar os arquivos .o


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> // para Windows
#include "funcaux.h"
#include "filechange.h"


int main() {
    if (sistema()) {
        setlocale(LC_ALL, "Portuguese_Brazil.1252");
        system("cls");
    } else {
        system("clear");
    }

    // Abrindo arquivos
    FILE *arqbin = fopen("assets/data/arqbin.bin", "rb+");
    FILE *arq_cartas = abrir_arquivo("assets/data/cartas.csv", "r");
    Cartas *cartas = NULL;
    int size=32;
    int criadoBin = 0;
    if (arqbin == NULL) {
        printf("Bem-vindo! Parece que é a primeira vez que você está rodando o programa.\n");
        for (int i = 0; i < 32; i++) {
            cartas = (Cartas *)realloc(cartas, (i + 1) * sizeof(Cartas));
            if (cartas == NULL) {
                perror("\nErro ao alocar memoria para cartas");
                exit(1);
            } 
            fscanf(arq_cartas, "%[^,],%c,%i,%i,%i,%i,%i,%i,%i\n", 
                cartas[i].nome, 
                &cartas[i].letra, 
                &cartas[i].num, 
                &cartas[i].super_trunfo, 
                &cartas[i].atributo_1, 
                &cartas[i].atributo_2, 
                &cartas[i].atributo_3, 
                &cartas[i].atributo_4, 
                &cartas[i].atributo_5);
            }
    } else {
        printf("Bem-vindo de volta!\n");
        fseek(arqbin, 0, SEEK_END);
        size = ftell(arqbin)/sizeof(Cartas);
        fseek(arqbin, 0, SEEK_SET);

        cartas = (Cartas *)malloc( size* sizeof(Cartas));
        if (cartas == NULL) {
            perror("\nErro ao alocar memoria para cartas");
            exit(1);
        } else {
            fread(cartas, sizeof(Cartas), size, arqbin);
        }
        criadoBin=1;
    }
    if (criadoBin)
    {
        fclose(arqbin);
    }
    
    fclose(arq_cartas);
    
    printf("\nQuantidade de cartas: %i\n", size);

    // Inicializar posicoes ocupadas para cada letra
    int *ptr_posicoesA = NULL;
    int *ptr_posicoesB = NULL;
    int *ptr_posicoesC = NULL;
    int *ptr_posicoesD = NULL;
    int sizeA = 0, sizeB = 0, sizeC = 0, sizeD = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < size; j++) {
            if (cartas[j].letra == 'A' && cartas[j].num == i + 1) {
                sizeA++;
                ptr_posicoesA = realloc(ptr_posicoesA, sizeA * sizeof(int));
                ptr_posicoesA[sizeA - 1] = i + 1;
            } else if (cartas[j].letra == 'B' && cartas[j].num == i + 1) {
                sizeB++;
                ptr_posicoesB = realloc(ptr_posicoesB, sizeB * sizeof(int));
                ptr_posicoesB[sizeB - 1] = i + 1;
            } else if (cartas[j].letra == 'C' && cartas[j].num == i + 1) {
                sizeC++;
                ptr_posicoesC = realloc(ptr_posicoesC, sizeC * sizeof(int));
                ptr_posicoesC[sizeC - 1] = i + 1;
            } else if (cartas[j].letra == 'D' && cartas[j].num == i + 1) {
                sizeD++;
                ptr_posicoesD = realloc(ptr_posicoesD, sizeD * sizeof(int));
                ptr_posicoesD[sizeD - 1] = i + 1;
            }
        }
    }

    int sair = 0, escolha;

    // Loop do Menu Principal
    do {
         printf("\nEscolha uma opcao:\n1 - Inserir cartas\n2 - Listar as cartas\n3 - Pesquisar uma carta\n4 - Alterar uma carta\n5 - Excluir carta\n6 - Exportar CSV\n7 - Sair\n: ");
        fflush(stdout);

        do {
            if (!(scanf("%i", &escolha)) || (escolha > 7) || (escolha < 1)) {
                setbuf(stdin, NULL);
                printf("\n\033[1;91mEscolha invalida! Insira um numero de 1 a 7:\033[m ");
            } else break;
        } while (1);

        switch (escolha) {
        case 1:
            inserir_cartas(&cartas, &ptr_posicoesA, &ptr_posicoesB, &ptr_posicoesC, &ptr_posicoesD,&size);
            break;
        case 2:
            listar_cartas(cartas, size);
            break;
        case 3:
            buscar_carta(cartas,size);
            break;
        case 4:
            alterar_carta(&cartas, size);
            break;
        case 5:
            //remover_carta(arq_cartasCOPY, &cartas, size);
            //n_cartas = quant_cartas(arq_cartasCOPY);
            break;
        case 6:
            exportar_csv(cartas, size);
            break;
        case 7:
            printf("\n\033[1;32mSaindo...\033[m\n");
            sair = 1;
            break;
        }
    } while (!sair);

    // Atualizar arquivo binario
    
    arqbin = fopen("assets/data/arqbin.bin", "wb+");
    fseek(arqbin, 0, SEEK_SET);
    fwrite(cartas, sizeof(Cartas), size, arqbin);

    fclose(arqbin);

    // Liberar memoria alocada e fechar arquivos
    free(ptr_posicoesA);
    free(ptr_posicoesB);
    free(ptr_posicoesC);
    free(ptr_posicoesD);
    free(cartas);

    return 0;
}
