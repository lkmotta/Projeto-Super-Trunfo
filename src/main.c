/**
 * @file main.c
 * @brief Arquivo principal do jogo
 * 
 * @note make -> compilar
 *       make run -> executar
 *       make clean -> limpar arquivos .o
 *       make clean_all -> limpar arquivos .o / executável
 *       make reset -> limpar arquivos .o / executável / arquivos gerados
 * 
 * @license MIT
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> // para Windows
#include "funcaux.h"
#include "filechange.h"
#include "game.h"

int main() {
    if (sistema()) {
        setlocale(LC_ALL, "Portuguese_Brazil.1252");
        system("cls"); // limpando o prompt de comando -> Windows
    } else {
        system("clear"); // limpando o terminal -> Linux
    }

    // Abrindo arquivos
    FILE *arqbin = fopen("assets/data/arqbin.dat", "rb+");
    FILE *arq_cartas = abrir_arquivo("assets/data/cartas.csv", "r");
    Cartas *cartas = NULL;
    int size = 32;
    int criadoBin = 0;
    if (arqbin == NULL) {
        printf("\033[1;93mBem-vindo!\033[1m Parece que é a primeira vez que você está rodando o programa.\033[m\n");
        for (int i = 0; i < 32; i++) {
            cartas = (Cartas*) realloc(cartas, (i + 1) * sizeof(Cartas));
            if (cartas == NULL) {
                perror("\n\033[1;91mErro ao alocar memoria para cartas\033[m");
                exit(1);
            }
            fscanf(arq_cartas, "%[^,],%c,%i,%i,%i,%i,%i,%i,%i\n", 
                cartas[i].nome, 
                &cartas[i].letra, 
                &cartas[i].num, 
                &cartas[i].super_trunfo, 
                &cartas[i].forca, 
                &cartas[i].habilidade, 
                &cartas[i].velocidade, 
                &cartas[i].poderes, 
                &cartas[i].poder_cura);
            }
    } else {
        printf("\033[1;93mBem-vindo de volta!\033[m\n");
        fseek(arqbin, 0, SEEK_END);
        size = ftell(arqbin) / sizeof(Cartas);
        fseek(arqbin, 0, SEEK_SET);

        cartas = (Cartas*) malloc(size * sizeof(Cartas));
        if (cartas == NULL) {
            perror("\nErro ao alocar memoria para cartas");
            exit(1);
        } else {
            fread(cartas, sizeof(Cartas), size, arqbin);
        }
        criadoBin = 1;
    }
    if (criadoBin) fclose(arqbin);

    fclose(arq_cartas);
    
    printf("\nQuantidade de cartas: %i\n", size);

    // Inicializando posicoes ocupadas para cada letra
    int *ptr_posicoesA = NULL, *ptr_posicoesB = NULL, *ptr_posicoesC = NULL, *ptr_posicoesD = NULL;
    int sizeA = 0, sizeB = 0, sizeC = 0, sizeD = 0;

    for (int i = 0; i < size; i++) {
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

    // Menu Principal
    do {
        printf("\nEscolha uma opcao:\n1 - \033[7mJOGAR\033[m\n2 - Inserir cartas\n3 - Listar as cartas\n4 - Pesquisar uma carta\n5 - Alterar uma carta\n6 - Excluir carta\n7 - Exportar CSV\n8 - ver historico de partidas\n9 - sair\n: ");
        fflush(stdout);

        do {
            if (!(scanf("%i", &escolha)) || (escolha > 9) || (escolha < 1)) {
                setbuf(stdin, NULL);
                printf("\n\033[1;91mEscolha invalida! Insira um numero de 1 a 9:\033[m ");
            } else break;
        } while (1);

        switch (escolha) {
        case 1: {
            //modo();
            int quantd_cartas_baralho = (int)((float)(size)/2);

            int *cartas_usadas = NULL;
            int size_cartas_usadas = 0;
            
            Cartas* baralho_player = get_baralho(cartas, size, quantd_cartas_baralho, &size_cartas_usadas, &cartas_usadas);
            Cartas* baralho_cpu = get_baralho(cartas, size, quantd_cartas_baralho, &size_cartas_usadas, &cartas_usadas);
            free(cartas_usadas);

            partida(baralho_player, baralho_cpu, quantd_cartas_baralho);
            printf("\n\n\033[1;33mFim de jogo!\033[m\n\n\n");
            desembaralhar(&cartas, size);
            break;
        }

        case 2:
            inserir_cartas(&cartas, &ptr_posicoesA, &ptr_posicoesB, &ptr_posicoesC, &ptr_posicoesD,&size);
            desembaralhar(&cartas, size);
            break;
        case 3:
            printf("\nNúmero de Cartas: %d\n", size);
            listar_cartas(cartas, size);
            break;
        case 4:
            buscar_carta(cartas,size);
            break;
        case 5:
            alterar_carta(&cartas, size);
            break;
        case 6:
            remover_carta(&cartas, &size);
            int sizeA = 0, sizeB = 0, sizeC = 0, sizeD = 0;

            for (int i = 0; i < size; i++) {
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
            
            break;
        case 7:
            exportar_csv(cartas, size);
            break;
        case 8:
            historico();
            break;
        case 9:
            printf("\n\033[1;32mSaindo...\033[m\n");
            sair = 1;
            break;
        }
    } while (!sair);

    // Atualizando arquivo binario
    arqbin = fopen("assets/data/arqbin.dat", "wb+");
    fseek(arqbin, 0, SEEK_SET);
    fwrite(cartas, sizeof(Cartas), size, arqbin);

    fclose(arqbin);

    // Liberando memoria alocada e fechando arquivos
    free(ptr_posicoesA);
    free(ptr_posicoesB);
    free(ptr_posicoesC);
    free(ptr_posicoesD);
    free(cartas);

    return 0;
}
