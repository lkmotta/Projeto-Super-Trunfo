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
        setlocale(LC_ALL, "portuquese");
        system("cls");
    } else {
        system("clear");
    }

    // Abrindo arquivos
    FILE *arqbin = fopen("assets/data/arqbin.bin", "rb+");
    FILE *arq_cartas = abrir_arquivo("assets/data/cartas.csv", "r");
    FILE *arq_cartasCOPY = abrir_arquivo("assets/data/cartas_temp.csv", "w+");

    if (arqbin == NULL) {
        printf("Bem-vindo! Parece que é a primeira vez que você está rodando o programa.\n");
        char linha_csv[1024]; // buffer
        for (int i = 0; i < 32; i++) {
            fscanf(arq_cartas, "%[^\n]\n", linha_csv);
            fprintf(arq_cartasCOPY, "%s\n", linha_csv);
        }
    } else {
        printf("Bem-vindo de volta!\n");
        char buffer[1024];
        size_t bytesRead;
        while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), arqbin)) > 0) {
            fwrite(buffer, sizeof(char), bytesRead, arq_cartasCOPY);
        }
    }
    fclose(arqbin);
    fclose(arq_cartas);
    rewind(arq_cartasCOPY);


    // Copiando as primeiras n_cartas linhas para a copia
    int n_cartas = quant_cartas(arq_cartasCOPY); // quantidade de cartas no arquivo
    printf("\nQuantidade de cartas: %i\n", n_cartas);
    // Alocar cartas
    Cartas *cartas = (Cartas *)malloc( n_cartas* sizeof(Cartas));
    if (cartas == NULL) {
        perror("\nErro ao alocar memoria para cartas");
        exit(1);
    }

    // Preenchendo cartas a partir do arquivo base
    for (int i = 0; i < n_cartas; i++) {
        fscanf(arq_cartasCOPY, "%[^,],%c,%i,%i,%i,%i,%i,%i,%i\n", 
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


    // Inicializar posicoes ocupadas para cada letra
    int *ptr_posicoesA = NULL;
    int *ptr_posicoesB = NULL;
    int *ptr_posicoesC = NULL;
    int *ptr_posicoesD = NULL;
    int sizeA = 0, sizeB = 0, sizeC = 0, sizeD = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < n_cartas; j++) {
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
        printf("\nEscolha uma opcao:\n1 - Inserir cartas\n2 - Listar as cartas\n3 - Pesquisar uma carta\n4 - Alterar uma carta\n5 - Excluir carta\n6 - Sair\n: ");
        fflush(stdout);

        do {
            if (!(scanf("%i", &escolha)) || (escolha > 6) || (escolha < 1)) {
                setbuf(stdin, NULL);
                printf("\n\033[1;91mEscolha invalida! Insira um numero de 1 a 6:\033[m ");
            } else break;
        } while (1);

        switch (escolha) {
        case 1:
            inserir_cartas(arq_cartasCOPY, &cartas, &ptr_posicoesA, &ptr_posicoesB, &ptr_posicoesC, &ptr_posicoesD);
            n_cartas = quant_cartas(arq_cartasCOPY);
            break;
        case 2:
            listar_cartas(cartas, n_cartas);
            break;
        case 3:
            // Implementar funcao de pesquisa
            break;
        case 4:
            alterar_carta(arq_cartasCOPY, &cartas, n_cartas);
            break;
        case 5:
            remover_carta(arq_cartasCOPY, &cartas, n_cartas);
            n_cartas = quant_cartas(arq_cartasCOPY);
            break;
        case 6:
            printf("\n\033[1;32mSaindo...\033[m\n");
            sair = 1;
            break;
        default:
            break;
        }
    } while (!sair);

    // Atualizar arquivo binario
    fseek(arqbin, 0, SEEK_SET);
    arqbin = fopen("assets/data/arqbin.bin", "wb+");
    fwrite(cartas, sizeof(Cartas), n_cartas, arqbin);

    fclose(arqbin);
    fclose(arq_cartasCOPY);

    // Remover o arquivo
    if (remove("assets/data/cartas_temp.csv") == 0) {
        printf("Arquivo removido com sucesso.\n");
    } else {
        perror("Erro ao remover o arquivo");
    }
    // Liberar memoria alocada e fechar arquivos
    free(ptr_posicoesA);
    free(ptr_posicoesB);
    free(ptr_posicoesC);
    free(ptr_posicoesD);
    free(cartas);

    return 0;
}
