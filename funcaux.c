#include <string.h>
#include "funcaux.h"

/**
 * @brief Retorna 1(true) se o S.O. ser Windows, retorna 0(false) caso contrario.
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
        perror("\n\033[1;91mNão foi possível abrir o arquivo!\033[m");
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
    char c;

    rewind(arquivo); // voltando o ponteiro para o início
    while ((c = fgetc(arquivo)) != EOF) {
        if(c == '\n') count++;
    }

    rewind(arquivo);
    return count;
}

int get_int(){
    int num;
    do{
        if(!(scanf("%d", &num)) || num < 0){
            setbuf(stdin, NULL);
            printf("\n\033[1;91mInsira um valor inteiro >= 0:\033[m ");
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
    return -1; // carta não encontrada
}

/**
 * @brief Verifica se valor esta no vetor. Retorna 1 se encontrado, 0 se não encontrado.
 * 
 * @param vetor 
 * @param valor 
 * @return int 
 */
int verify_int_in_vetor(int* vetor, int size_vetor, int valor){
    for (int i = 0; i < size_vetor; i++){
        if(vetor[i] == valor) return 1;
    }
    return 0; // valor não encontrado
}