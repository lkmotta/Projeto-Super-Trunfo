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
        perror("\n\033[1;91mNÃ£o foi possÃ­vel abrir o arquivo!\033[m");
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

    rewind(arquivo); // voltando o ponteiro para o início
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        count++;
    }

    rewind(arquivo);
    return count;
}

/**
 * @brief Retorna um inteiro >= 0 do stdin(teclado) tratado.
 * 
 * @return int 
 */
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
    return -1; // carta nÃ£o encontrada
}

/**
 * @brief Verifica se valor esta no vetor. Retorna 1 se encontrado, 0 se nÃ£o encontrado.
 * 
 * @param vetor 
 * @param valor 
 * @return int 
 */
int verify_int_in_vetor(int* vetor, int size_vetor, int valor){
    for (int i = 0; i < size_vetor; i++){
        if(vetor[i] == valor) return 1;
    }
    return 0; // valor nÃ£o encontrado
}
/**
 * @brief Função para validar entrada inteira
 * 
 * @param min Valor mínimo aceitável
 * @param max Valor máximo aceitável
 * @param mensagem Mensagem exibida ao usuário para entrada
 * 
 * @return int Valor inteiro validado dentro do intervalo especificado
 */
int validar_entrada(int min, int max, const char *mensagem) {
    int valor;
    do {
        printf("%s", mensagem);
        if (scanf("%d", &valor) != 1 || valor < min || valor > max) {
            setbuf(stdin, NULL);
            printf("\033[1;91mEntrada inválida! Insira um número entre %d e %d.\033[m\n", min, max);
        } else {
            break;
        }
    } while (1);
    return valor;
}
/**
 * @brief Função para buscar cartas com base em um atributo específico
 * 
 * @param carta Vetor de cartas
 * @param qnt_cartas Quantidade de cartas disponíveis
 * @param atributo Atributo escolhido para pesquisa (1 a 5)
 * @param comparacao Tipo de comparação (1 para maior que, 2 para menor que)
 * @param valor Valor base para comparação
 */
void buscar_por_atributo(Cartas carta[], int qnt_cartas, int atributo, int comparacao, int valor) {
    const char *atributos[] = {"Força", "Habilidade", "Velocidade", "Poderes", "Poder de Cura"};
    printf("--%s--\n", atributos[atributo - 1]);

    int contador = 0;
    for (int i = 0; i < qnt_cartas; i++) {
        int valor_atual = 0;
        switch (atributo) {
            case FORCA: valor_atual = carta[i].atributo_1; break;
            case HABILIDADE: valor_atual = carta[i].atributo_2; break;
            case VELOCIDADE: valor_atual = carta[i].atributo_3; break;
            case PODERES: valor_atual = carta[i].atributo_4; break;
            case CURA: valor_atual = carta[i].atributo_5; break;
        }

        if ((comparacao == 1 && valor_atual > valor) || (comparacao == 2 && valor_atual < valor)) {
            contador++;
            printf("%02d - Carta [%d%c]: %s\n", contador, carta[i].num, carta[i].letra, carta[i].nome);
        }
    }

    if (contador == 0) {
        printf("\n---> Nenhuma carta encontrada com esses critérios!\n");
    }
}



/**
 * @brief Função para buscar cartas por uma letra específica
 * 
 * @param carta Vetor de cartas
 * @param qnt_cartas Quantidade de cartas disponíveis
 * @param letra Letra utilizada como critério de busca
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
 * @brief Função para buscar cartas por número
 * 
 * @param carta Vetor de cartas
 * @param qnt_cartas Quantidade de cartas disponíveis
 * @param numero Número utilizado como critério de busca
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
        printf("\n---> Nenhuma carta encontrada com o número %d!\n", numero);
    }
}
