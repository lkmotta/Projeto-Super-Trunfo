/**
 * @file funcaux.c
 * @brief Fun��es auxiliares para o jogo
 * 
 * @license MIT
 * @copyright Copyright (c) 2024
 * 
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "funcaux.h"

/**
 * @brief Retorna 1(true) se o S.O. � Windows, retorna 0(false) caso contr�rio.
 * 
 * @return int 
 */
int sistema() {
    #ifdef _WIN32 // macro do sistema 
        return 1;
    #else
        return 0;
    #endif
}

/**
 * @brief Retorna o ponteiro do arquivo aberto solicitado. Caso erro, exit(1).
 * 
 * @param nome_arq // caminho para o arquivo a ser aberto
 * @param mode    // modo de acordo com o uso necessario. Exemplo: "a+"
 * @return FILE* 
 */
FILE* abrir_arquivo(const char nome_arq[], const char mode[]) {
    FILE* arquivo = fopen(nome_arq, mode);
    if (arquivo == NULL) {
        perror("\n\033[1;91mNao foi possovel abrir o arquivo!\033[m");
        exit(EXIT_FAILURE);
    }
    return arquivo;
}

/**
 * @brief Retorna uma string do stdin(teclado) tratada.
 * 
 * @param nome // ponteiro char onde sera armazenada a string do stdin
 * @param tamanho // tamanho de 'nome'
 */
void burocracia(char *nome, int tamanho) {
    fgets(nome, tamanho, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    setbuf(stdin, NULL);
}

/**
 * @brief Retorna quantidade de cartas no arquivo do 'caminho' de acordo com num de '\n'.
 * 
 * @param arquivo // caminho do arquivo a ler a quantidade
 * @return int 
 */
int quant_cartas(FILE* arquivo) {
    int count = 0;
    char buffer[1024];

    rewind(arquivo); // voltando o ponteiro para o in��cio
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        count++;
    }

    rewind(arquivo);
    return count;
}

/**
 * @brief Retorna um inteiro do stdin(teclado) tratado.
 * 
 * @param valor_min // valor m��nimo
 * @param valor_max // valor m�ximo
 * @param mensagem_erro // mensagem exibida se entrada inv�lida
 * 
 * @return int 
 */
int get_int(int valor_min, int valor_max, const char mensagem_erro[]) {
    int num;
    do{
        if(!(scanf("%d", &num)) || num < valor_min || num > valor_max){
            setbuf(stdin, NULL);
            printf("\n%s", mensagem_erro);
        }else break;
    } while (1);
    
    return num;
}

/**
 * @brief Retorna a posicao da carta no vetor de cartas com nome 'nome_carta'.
 * 
 * @param cartas 
 * @param quantid_cartas // quantidade de cartas no vetor
 * @param nome_carta // nome da carta a ser procurada
 * @return int 
 */
int get_pos_carta(Cartas** cartas, int quantid_cartas, const char nome_carta[]){
    for (int i = 0; i < quantid_cartas; i++){
        if(strcasecmp((*cartas)[i].nome, nome_carta) == 0){
            return i;
        }
    }
    return -1; // carta n�o encontrada
}

/**
 * @brief Verifica se valor esta no vetor. Retorna 1 se encontrado, 0 se nao encontrado.
 * 
 * @param vetor 
 * @param valor 
 * @return int 
 */
int verify_int_in_vetor(int* vetor, int size_vetor, int valor){
    for (int i = 0; i < size_vetor; i++){
        if(vetor[i] == valor) return 1;
    }
    return 0; // valor n�o encontrado
}

/**
 * @brief Fun��o para buscar cartas com base em um atributo espec�fico
 * 
 * @param carta Vetor de cartas
 * @param qnt_cartas Quantidade de cartas dispon�veis
 * @param atributo Atributo escolhido para pesquisa (1 a 5)
 * @param comparacao Tipo de compara��o (1 para maior que, 2 para menor que)
 * @param valor Valor base para compara��o
 */
void buscar_por_atributo(Cartas carta[], int qnt_cartas, int atributo, int comparacao, int valor) {
    const char *atributos[] = {"Forca", "Habilidade", "Velocidade", "Poderes", "Poder de Cura"};
    printf("--%s--\n", atributos[atributo - 1]);

    int contador = 0;
    for (int i = 0; i < qnt_cartas; i++) {
        int valor_atual = 0;
        switch (atributo) {
            case FORCA: valor_atual = carta[i].forca; break;
            case HABILIDADE: valor_atual = carta[i].habilidade; break;
            case VELOCIDADE: valor_atual = carta[i].velocidade; break;
            case PODERES: valor_atual = carta[i].poderes; break;
            case CURA: valor_atual = carta[i].poder_cura; break;
        }

        if ((comparacao == 1 && valor_atual > valor) || (comparacao == 2 && valor_atual < valor)) {
            contador++;
            printf("%02d - Carta [%d%c]: %s\n", contador, carta[i].num, carta[i].letra, carta[i].nome);
        }
    }

    if (contador == 0) {
        printf("\n---> Nenhuma carta encontrada com esses criterios!\n");
    }
}

/**
 * @brief Fun��o para buscar cartas por uma letra espec��fica
 * 
 * @param carta Vetor de cartas
 * @param qnt_cartas Quantidade de cartas dispon��veis
 * @param letra Letra utilizada como crit�rio de busca
 */
void buscar_por_letra(Cartas carta[], int qnt_cartas, char letra) {
    int contador = 0;
    for (int i = 0; i < qnt_cartas; i++) {
        if (toupper(carta[i].letra) == letra) {
            contador++;
            printf("%02d - Carta [%d%c]: %s\n", contador, carta[i].num, carta[i].letra, carta[i].nome);
        }
    }

    if (contador == 0) {
        printf("\n---> Nenhuma carta encontrada com a letra %c!\n", letra);
    }
}

/**
 * @brief Fun��o para buscar cartas por n�mero
 * 
 * @param carta Vetor de cartas
 * @param qnt_cartas Quantidade de cartas dispon�veis
 * @param numero N�mero utilizado como crit�rio de busca
 */
void buscar_por_numero(Cartas carta[], int qnt_cartas, int numero) {
    int contador = 0;
    for (int i = 0; i < qnt_cartas; i++) {
        if (carta[i].num == numero) {
            contador++;
            printf("%02d - Carta [%d%c]: %s\n", contador, carta[i].num, carta[i].letra, carta[i].nome);
        }
    }

    if (contador == 0) {
        printf("\n---> Nenhuma carta encontrada com o numero %d!\n", numero);
    }
}

/**
 * @brief Fun��o para exportar as cartas para um arquivo CSV
 * 
 * @param cartas Vetor de cartas
 * @param quantd_cartas Quantidade de cartas dispon�veis
 */
void exportar_csv(Cartas* cartas, int quantd_cartas){
    FILE* arq_exportar = fopen("assets/data/cartas_exportadas.csv", "w+");
    if(!arq_exportar){
        perror("\nErro ao abrir/criar arquivo");
        return;
    }

    // escrevendo cabe�alho do CSV
    fprintf(arq_exportar, "NOME,LETRA,NUMERO,SUPER-TRUNFO,FORCA,HABILIDADE,VELOCIDADE,PODERES,PODER CURA\n");

    for (int i = 0; i < quantd_cartas; i++) {
        fprintf(arq_exportar, "%s,%c,%i,%i,%i,%i,%i,%i,%i\n",
            cartas[i].nome, cartas[i].letra, cartas[i].num, cartas[i].super_trunfo,
            cartas[i].forca, cartas[i].habilidade, cartas[i].velocidade,
            cartas[i].poderes, cartas[i].poder_cura);
    }
    fclose(arq_exportar);
    printf("\n\033[3;92mCartas exportadas com sucesso!\033[m\n");
    return;
}

/**
 * @brief Embaralha um vetor de cartas usando o algoritmo de Fisher-Yates.
 *
 * @param cartas Ponteiro para o vetor de cartas a ser embaralhado.
 * @param quantd_cartas n�mero total de cartas no vetor.
 * 
 */
void embaralhar(Cartas* cartas, int quantd_cartas) {
    srand(time(NULL));
    Cartas temp;
    for (int i = quantd_cartas - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        temp = cartas[i];
        cartas[i] = cartas[j];
        cartas[j] = temp;
    }
}

/**
 * @brief Compara duas cartas para ordena��o.
 *
 * @param a Ponteiro para a primeira carta.
 * @param b Ponteiro para a segunda carta.
 * @return int 
 */
int compare_cartas(const void* a, const void* b) {
    Cartas* cartaA = (Cartas*)a;
    Cartas* cartaB = (Cartas*)b;

    // Compara a letra da classe
    if (cartaA->letra < cartaB->letra) return -1;
    if (cartaA->letra > cartaB->letra) return 1;

    /* Se a letra da classe � igual, compara o nome
    strcmp -> compara duas strings, retorna 0 se forem iguais,
    < 0 se a primeira for menor que a segunda,
    > 0 se a primeira for maior que a segunda
    */
    return strcasecmp(cartaA->nome, cartaB->nome);
}

/**
 * @brief Desembaralha um vetor de cartas com base na letra de classe (A/B/C/D) e organiza por ordem alfab�tica.
 *
 * @param cartas Ponteiro para o vetor de cartas a ser desembaralhado.
 * @param quantd_cartas n�mero total de cartas no vetor.
 */
void desembaralhar(Cartas** cartas, int quantd_cartas) {
    /**
     * qsort (quicksort) � uma fun��o/algoritmo de ordena��o r�pida, e vem por padr�o na stdlib.h
    Ela ordena um array de elementos com base em uma fun��o de compara��o fornecida pelo usu�rio.
    A fun��o de compara��o deve retornar:
    Um valor negativo se o primeiro elemento for menor que o segundo.
    Zero se os elementos forem iguais.
    Um valor positivo se o primeiro elemento for maior que o segundo
    */
    qsort((*cartas), quantd_cartas, sizeof(Cartas), compare_cartas);
}

/**
 * @brief Ordena os ��ndices em ordem decrescente para evitar problemas ao remover
 * @note Auxiliar da remover_carta
 * 
 * @param posi_excluir >>> posi��es a serem exclu�das 
 * @param num_excluir >>> quantidade de posi��es a serem exclu��das
 */
void sort_decrease(int* posi_excluir, int num_excluir) {
    int aux;
    for (int i = 0; i < num_excluir - 1; i++) {
        for (int j = i + 1; j < num_excluir; j++) {
            if (posi_excluir[i] < posi_excluir[j]) {
                aux = posi_excluir[i];
                posi_excluir[i] = posi_excluir[j];
                posi_excluir[j] = aux;
            }
        }
    }
    return;
}

/**
 * @brief Remove as cartas do vetor de acordo com as posi��es a serem exclu��das
 * 
 * @param vetor >>> vetor de cartas
 * @param quantd_cartas_vetor >>> quantidade de cartas no vetor
 * @param posicoes_excluir >>> posi��es a serem exclu��das
 * @param quantd_excluir >>> quantidade de posi��es a serem exclu��das
 */
void remover_cartas_vetor(Cartas** vetor, int* quantd_cartas_vetor, int* posicoes_excluir, int quantd_excluir){
    // Excluindo as cartas do vetor
    for (int i = 0; i < quantd_excluir; i++) {
        for (int j = posicoes_excluir[i]; j < *quantd_cartas_vetor - 1; j++) {
            (*vetor)[j] = (*vetor)[j + 1];
        }
        (*quantd_cartas_vetor)--;
    }

    // Realocando o vetor para o tamanho das cartas remanescentes
    Cartas* temp = realloc(*vetor, (*quantd_cartas_vetor) * sizeof(Cartas));
    if (!temp) {
        perror("\nErro ao alocar vetor Cartas");
        return;
    }

    *vetor = temp;
    temp = NULL;
    return;
}