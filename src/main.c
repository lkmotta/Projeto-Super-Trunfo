/**
 * @file main.c
 * @brief Arquivo principal do jogo
 * @authors @lkmotta, @Dirceu06, @boonfim
 * 
 * @note make -> compilar
 *       make run -> executar
 *       make clean -> limpar arquivos .o
 *       make clean_all -> limpar arquivos .o / executavel
 *       make reset -> limpar arquivos .o / executavel / arquivos gerados
 * 
 * LICENSE: MIT
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
#include "interface.h"
//alo
int main() {
    if (sistema()) {
        setlocale(LC_ALL, "portuguese");
        system("cls");   // limpando o prompt de comando -> Windows
    } else {
        system("clear"); // limpando o terminal -> Linux
    }

    // Abrindo arquivos
    FILE *arqbin = fopen("assets/data/arqbin.dat", "rb+");
    FILE *arq_cartas = abrir_arquivo("assets/data/cartas.csv", "r");
    Cartas *cartas = NULL;
    int size = 0;
    int criadoBin = 0;
    if (arqbin == NULL) {
        printf("\033[1;93mBem-vindo!\033[1;30m Parece que e a primeira vez que voce esta rodando o jogo.\033[m\n");

        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), arq_cartas) != NULL) {
            size++;
        }
        fseek(arq_cartas, 0, SEEK_SET);
        
        for (int i = 0; i < size; i++) {
            cartas = (Cartas*) realloc(cartas, (i + 1) * sizeof(Cartas));
            if (cartas == NULL) {
                perror("\n\033[1;91mErro ao alocar memoria para cartas\033[m");
                exit(1);
            }
            fscanf(arq_cartas, "%[^,],%c,%i,%i,%i,%i,%i,%i,%i,%s\n", 
                cartas[i].nome,
                &cartas[i].letra, 
                &cartas[i].num, 
                &cartas[i].super_trunfo, 
                &cartas[i].forca, 
                &cartas[i].habilidade, 
                &cartas[i].velocidade, 
                &cartas[i].poderes, 
                &cartas[i].poder_cura,
                cartas[i].textura);
            
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

    // inicializando posicoes ocupadas para cada letra
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
        printf("\nEscolha uma opcao:\n1 - \033[7mJOGAR (raylib)\033[m\n2 - \033[7mJOGAR (terminal)\033[m\n3 - Inserir cartas\n4 - Listar as cartas\n5 - Pesquisar uma carta\n6 - Alterar uma carta\n7 - Excluir carta\n8 - Exportar CSV\n9 - Historico de partidas\n10 - ranking\n0 - Sair\n: ");
        fflush(stdout);

        escolha = get_int(0, 11, "\033[1;91mOpção invalida!\033[1mInsira uma opção válida:\033[m ");

        switch (escolha) {
        case 0:
            sair = 1;
            printf("\n\033[1;36mEncerrando...\033[m\n");
            break;
        case 1: {
            int quantd_cartas_baralho = (int)((float)(size)/2);

            interface(cartas, size, quantd_cartas_baralho);
            
            desembaralhar(&cartas, size);
            break;
        }
        case 2: {
            int quantd_cartas_baralho = (int)((float)(size)/2);

            int *cartas_usadas = NULL;
            int size_cartas_usadas = 0;
            
            Cartas* baralho_player = gerar_baralho(cartas, size, quantd_cartas_baralho, &size_cartas_usadas, &cartas_usadas);
            Cartas* baralho_cpu = gerar_baralho(cartas, size, quantd_cartas_baralho, &size_cartas_usadas, &cartas_usadas);
            free(cartas_usadas);

            partida(baralho_player, baralho_cpu, quantd_cartas_baralho);
            printf("\n\n\033[1;33mFim de jogo!\033[m\n\n\n");
            desembaralhar(&cartas, size);
            break;
        }
        case 3:
            inserir_cartas(&cartas, &ptr_posicoesA, &ptr_posicoesB, &ptr_posicoesC, &ptr_posicoesD,&size);
            desembaralhar(&cartas, size);
            break;
        case 4:
            printf("\nNumero de Cartas: %d\n", size);
            listar_cartas(cartas, size);
            break;
        case 5:
            buscar_carta(cartas,size);
            break;
        case 6:
            alterar_carta(&cartas, size);
            break;
        case 7: {
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
        }
        case 8:
            exportar_csv(cartas, size);
            break;
        case 9:
            historico();
            break;
        
        case 10:
            ranking();
            break;
        default:
            break;
        }
    } while (!sair);

    // Atualizando arquivo binario
    
    arqbin = fopen("assets/data/arqbin.dat", "wb+");
    fseek(arqbin, 0, SEEK_SET);
    fwrite(cartas, sizeof(Cartas), size, arqbin);

    fclose(arqbin);

    // Liberando memoria alocada
    free(ptr_posicoesA);
    free(ptr_posicoesB);
    free(ptr_posicoesC);
    free(ptr_posicoesD);
    free(cartas);

    return 0;
}
