// Comando de compilação
// gcc *.c -o main.exe

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> // para Windows
#include "funcaux.h"
#include "filechange.h"


int main() {
    if (sistema()) {
        setlocale(LC_ALL, "pt-BR.UTF-8");
        system("cls");
    } else {
        system("clear");
    }

    // Abrindo arquivos
    FILE *arq_cartas = abrir_arquivo("assets/data/cartas.csv", "r");
    FILE *arq_cartasCOPY = abrir_arquivo("assets/data/cartas_copia.csv", "w+");

    // Copiando as primeiras 32 linhas para a copia
    char linha_csv[256]; // buffer
    for (int i = 0; i < 32; i++) {
        fscanf(arq_cartas, "%[^\n]\n", linha_csv);
        fprintf(arq_cartasCOPY, "%s\n", linha_csv);
    }
    rewind(arq_cartas);
    rewind(arq_cartasCOPY);

    // Alocar cartas
    Cartas *cartas = (Cartas *)malloc(32 * sizeof(Cartas));
    if (cartas == NULL) {
        perror("\nErro ao alocar memoria para cartas");
        exit(1);
    }

    // Preenchendo cartas a partir do arquivo base
    for (int i = 0; i < 32; i++) {
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
    fclose(arq_cartas);

    int n_cartas = quant_cartas(arq_cartasCOPY); // quantidade de cartas no arquivo

    // Inicializar posicoes ocupadas para cada letra
    int *ptr_posicoesA = (int*) malloc(8 * sizeof(int));
    int *ptr_posicoesB = (int*) malloc(8 * sizeof(int));
    int *ptr_posicoesC = (int*) malloc(8 * sizeof(int));
    int *ptr_posicoesD = (int*) malloc(8 * sizeof(int));

    if (!ptr_posicoesA || !ptr_posicoesB || !ptr_posicoesC || !ptr_posicoesD) {
        perror("\nErro ao alocar memoria para posicoes");
        exit(1);
    }

    for (int i = 0; i < 8; i++) {
        ptr_posicoesA[i] = i + 1;
        ptr_posicoesB[i] = i + 1;
        ptr_posicoesC[i] = i + 1;
        ptr_posicoesD[i] = i + 1;
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
            // Implementar funcao de alteracao
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

    // Liberar memoria alocada e fechar arquivos
    free(ptr_posicoesA);
    free(ptr_posicoesB);
    free(ptr_posicoesC);
    free(ptr_posicoesD);
    free(cartas);
    fclose(arq_cartasCOPY);

    return 0;
}
