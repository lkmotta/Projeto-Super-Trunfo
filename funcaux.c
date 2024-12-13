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
    rewind(arquivo);
    int count = 0;
    char c;

    while ((c = fgetc(arquivo)) != EOF) {
        if(c == '\n') count++;
    }

    rewind(arquivo);
    return count;
}
