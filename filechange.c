#include <ctype.h> // necessario pra toupper
#include <locale.h> // necessario pra setlocale
#include "filechange.h"

/**
 * @brief faz a insercao da quantidade de cartas que o usuario quiser no vetor de cartas e no arq csv
 * 
 * @param arq_cartaCOPIA 
 * @param carta 
 * @param ptr_posicoesA 
 * @param ptr_posicoesB 
 * @param ptr_posicoesC 
 * @param ptr_posicoesD 
 */
void inserir_cartas(FILE *arq_cartaCOPIA, Cartas **carta, int **ptr_posicoesA, int **ptr_posicoesB, int **ptr_posicoesC, int **ptr_posicoesD) {
    if (sistema()) setlocale(LC_ALL, "pt-BR.UTF-8");

    int cartas_add, conta_super_trunfo = 0;
    int contaN = quant_cartas(arq_cartaCOPIA);
    rewind(arq_cartaCOPIA);

    printf("\nInsira quantas cartas desejar inserir (temos %i cartas): ", contaN);
    do {
        scanf("%2d", &cartas_add);
        if (cartas_add < 1) {
            printf("\n\033[1mNúmero invalido. Digite um numero positivo > 0:\033[m ");
        } else break;
        setbuf(stdin, NULL);
    } while (1);

    setbuf(stdin, NULL);

    // Realloc para o vetor de cartas
    *carta = realloc(*carta, (contaN + cartas_add) * sizeof(Cartas));
    if (*carta == NULL) {
        perror("Erro ao alocar memoria para cartas");
        return;
    }

    for (int i = 0; i < cartas_add; i++) {
        setbuf(stdin, NULL);
        printf("\nNome da carta: ");
        burocracia((*carta)[contaN + i].nome, TAM_NOME_CARTA);

        char TentaLetra;
        int colunaLetra;
        int aux = 1;
        int posi_procurada = 0;
        int sair = 1;

        do {
            printf("\nLetra da carta: ");
            scanf("%c", &TentaLetra);
            while (getchar() != '\n'); // Limpa o buffer de entrada
            TentaLetra = toupper(TentaLetra);

            switch (TentaLetra) {
                case 'A':
                    for (int j = 0; j < aux; j++) {
                        if ((*ptr_posicoesA)[j] != aux) {
                            posi_procurada = j + 1;
                        } else {
                            aux++;
                        }
                    }
                    *ptr_posicoesA = realloc(*ptr_posicoesA, posi_procurada * sizeof(int));
                    (*ptr_posicoesA)[posi_procurada - 1] = posi_procurada;
                    sair = 0;
                    break;

                case 'B':
                    for (int j = 0; j < aux; j++) {
                        if ((*ptr_posicoesB)[j] != aux) {
                            posi_procurada = j + 1;
                        } else {
                            aux++;
                        }
                    }
                    *ptr_posicoesB = realloc(*ptr_posicoesB, posi_procurada * sizeof(int));
                    (*ptr_posicoesB)[posi_procurada - 1] = posi_procurada;
                    sair = 0;
                    break;

                case 'C':
                    for (int j = 0; j < aux; j++) {
                        if ((*ptr_posicoesC)[j] != aux) {
                            posi_procurada = j + 1;
                        } else {
                            aux++;
                        }
                    }
                    *ptr_posicoesC = realloc(*ptr_posicoesC, posi_procurada * sizeof(int));
                    (*ptr_posicoesC)[posi_procurada - 1] = posi_procurada;
                    sair = 0;
                    break;

                case 'D':
                    for (int j = 0; j < aux; j++) {
                        if ((*ptr_posicoesD)[j] != aux) {
                            posi_procurada = j + 1;
                        } else {
                            aux++;
                        }
                    }
                    *ptr_posicoesD = realloc(*ptr_posicoesD, posi_procurada * sizeof(int));
                    (*ptr_posicoesD)[posi_procurada - 1] = posi_procurada;
                    sair = 0;
                    break;

                default:
                    printf("\n\033[1mLetra invalida. Tente novamente:\033[m ");
                    break;
            }
        } while (sair);

        (*carta)[contaN + i].letra = TentaLetra;
        (*carta)[contaN + i].num = posi_procurada;

        printf("\ntemos a posicao %i disponivel em %c\n\n", posi_procurada, TentaLetra);

        int escolhaTrunfo;
        do {
            printf("Super Trunfo?\n1 - sim\n2 - nao\n: ");
            scanf("%i", &escolhaTrunfo);
            (*carta)[contaN + i].super_trunfo = (escolhaTrunfo == 1) ? 1 : 0;
        } while ((*carta)[contaN + i].super_trunfo != 1 && (*carta)[contaN + i].super_trunfo != 0);

        printf("\nForca: ");
        (*carta)[contaN + i].atributo_1 = get_int();
        printf("\nHabilidade: ");
        (*carta)[contaN + i].atributo_2 = get_int();
        printf("\nVelocidade: ");
        (*carta)[contaN + i].atributo_3 = get_int();
        printf("\nPoderes: ");
        (*carta)[contaN + i].atributo_4 = get_int();
        printf("\nPoder cura: ");
        (*carta)[contaN + i].atributo_5 = get_int();

        fseek(arq_cartaCOPIA, 0, SEEK_END);
        fprintf(arq_cartaCOPIA, "%s,%c,%i,%i,%i,%i,%i,%i,%i\n",
            (*carta)[contaN + i].nome, (*carta)[contaN + i].letra, (*carta)[contaN + i].num,
            (*carta)[contaN + i].super_trunfo, (*carta)[contaN + i].atributo_1, (*carta)[contaN + i].atributo_2,
            (*carta)[contaN + i].atributo_3, (*carta)[contaN + i].atributo_4, (*carta)[contaN + i].atributo_5);
    }

    rewind(arq_cartaCOPIA);
}

/**
 * @brief Lista todas as cartas no arquivo csv 'arq_carta'
 * 
 * @param arq_carta // ponteiro do arquivo das cartas
 */
void listar_cartas(Cartas* carta, int num_cartas) {
    if(sistema()) setlocale(LC_ALL, "pt-BR.UTF-8");

    printf("\nNumero de cartas: %ld\n", sizeof(carta));

    printf("\n%-21s | %-6s | %-9s | %-12s | %-6s | %-12s | %-12s | %-9s | %-12s\n", "NOME", "LETRA", "NUMERO", "SUPER-TRUNFO", "FORCA", "HABILIDADE", "VELOCIDADE", "PODERES", "PODER CURA");

    for (int i = 0; i < num_cartas; i++) {
        printf("%-21s | %-6c | %-9d | %-12d | %-6d | %-12d | %-12d | %-9d | %-12d\n",
            carta[i].nome, carta[i].letra, carta[i].num, carta[i].super_trunfo,
            carta[i].atributo_1, carta[i].atributo_2, carta[i].atributo_3, carta[i].atributo_4, carta[i].atributo_5);
    }
}

/**
 * @brief Exclui as cartas solicitadas pelo player.
 * 
 * @param arq_cartas
 * @param cartas
 * @param quantd_cartas
 */
void remover_carta(FILE* arq_cartas, Cartas** cartas, int quantd_cartas){
    int quant_excluir = 0;
    char nome_excluir[TAM_NOME_CARTA];

    printf("\nInsira quantas cartas deseja excluir: ");
    quant_excluir = get_int();
    if(quant_excluir == 0 || quant_excluir >= quantd_cartas){
        printf("\n\033[1m\033[3mOperacao cancelada.\033[m\n");
        return;
    }

    int pos_excluir[quant_excluir], posicao;

    setbuf(stdin, NULL);

    // adicionando as posicoes das cartas a excluir corretamente
    for (int i = 0; i < quant_excluir; i++){
        printf("\nInsira o nome da %da carta: ", i+1);
        burocracia(nome_excluir, TAM_NOME_CARTA);
        posicao = get_pos_carta(cartas, quantd_cartas, nome_excluir);
        if(posicao == -1){
            printf("\n\033[1m\033[3mCarta nao encontrada.\033[m\n");
            i--;
            continue;
        }else if(verify_int_in_vetor(pos_excluir, quant_excluir, posicao)){
            printf("\n\033[1m\033[3mCarta ja inserida.\033[m\n");
            i--;
            continue;
        }else pos_excluir[i] = posicao;
    }

    int continuar = 0, aux = 0, new_quant_cartas = quantd_cartas-quant_excluir;
    // alocando vetor para as cartas remanescentes
    Cartas* cartas_aux = (Cartas*) malloc(new_quant_cartas* sizeof(Cartas));
    if(!cartas_aux){
        perror("\nErro ao alocar");
        exit(1);
    }

    fclose(arq_cartas);
    // abrindo outro arquivo para reescrever as cartas remanescentes
    arq_cartas = fopen("assets/data/temp_file.csv", "w+"); // reaproveitando o mesmo ponteiro

    for (int i = 0; i < quantd_cartas; i++){
        for (int j = 0; j < quant_excluir; j++){
            if(i == pos_excluir[j]){
                continuar = 1;
                break;
            }
        }

        if(continuar){
            continuar = 0;
            continue;
        }

        cartas_aux[aux] = (*cartas)[i];

        fprintf(arq_cartas, "%s,%c,%i,%i,%i,%i,%i,%i,%i\n",
            cartas_aux[aux].nome, cartas_aux[aux].letra, cartas_aux[aux].num, cartas_aux[aux].super_trunfo,
            cartas_aux[aux].atributo_1, cartas_aux[aux].atributo_2, cartas_aux[aux].atributo_3,
            cartas_aux[aux].atributo_4, cartas_aux[aux].atributo_5);
        aux++;
    }

    // única maneira de realocar sem erros: criando outro vetor
    Cartas *temp = realloc(*cartas, new_quant_cartas * sizeof(Cartas));
    if(!temp){
        perror("\nErro ao alocar vetor Cartas");
        free(cartas_aux); // liberando o vetor auxiliar caso erro
        return;
    }

    *cartas = temp;

    for (int i = 0; i < new_quant_cartas; i++) {
        (*cartas)[i] = cartas_aux[i]; // passando as cartas remanescentes para o vetor original
    }
    free(cartas_aux);

    // removendo o arquivo antigo e renomeando o arquivo temporario
    remove("assets/data/cartas_copia.csv");
    rename("assets/data/temp_file.csv", "assets/data/cartas_copia.csv");

    rewind(arq_cartas);
    printf("\n\033[3;92mCarta(s) removida(s) com sucesso!\033[m\n");
    return;
}
