/**
 * @file filechange.c
 * @brief Funções para manipulação de arquivos e cartas
 * 
 * @license MIT 
 * @copyright Copyright (c) 2024
 * 
 */

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
void inserir_cartas(Cartas **carta, int **ptr_posicoesA, int **ptr_posicoesB, int **ptr_posicoesC, int **ptr_posicoesD, int *size) {
    int cartas_add;

    printf("\nInsira quantas cartas desejar inserir (temos %i cartas): ", *size);
    do {
        scanf("%2d", &cartas_add);
        if (cartas_add < 1) {
            printf("\n\033[1;91mNúmero invalido.\033[1m Digite um numero positivo > 0:\033[m ");
        } else break;
        setbuf(stdin, NULL);
    } while (1);

    setbuf(stdin, NULL);

    // Realloc para o vetor de cartas
    *carta = realloc(*carta, ((*size) + cartas_add) * sizeof(Cartas));
    if (*carta == NULL) {
        perror("\033[1;31mErro ao alocar memoria para cartas\033[m");
        return;
    }

    for (int i = 0; i < cartas_add; i++) {
        setbuf(stdin, NULL);
        printf("\nNome da carta: ");
        burocracia((*carta)[(*size) + i].nome, TAM_NOME_CARTA);

        char TentaLetra;
        int aux = 1;
        int posi_procurada = 0;
        int sair = 1;

        printf("\nLetra da carta: ");
        do {
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
                    printf("\n\033[1;31mLetra invalida. Tente novamente:\033[m ");
                    break;
            }
        } while (sair);

        (*carta)[(*size) + i].letra = TentaLetra;
        (*carta)[(*size) + i].num = posi_procurada;

        printf("\nTemos a posicao %i disponivel em %c\n", posi_procurada, TentaLetra);

        int existenciaDoSuper=0;
        int escolhaTrunfo;

        for (int i = 0; i < (*size); i++){
            if ((*carta)[i].super_trunfo==1){
                existenciaDoSuper=1;
            }
        }

        if (existenciaDoSuper==1){
            printf("\033[1mJ� existe um Super-Trunfo\033[m\n");
            (*carta)[(*size) + i].super_trunfo = 0;
        }else{
            do {
                printf("Super Trunfo?\n1 - Sim\n2 - N�o\n: ");
                scanf("%i", &escolhaTrunfo);
                (*carta)[(*size) + i].super_trunfo = (escolhaTrunfo == 1) ? 1 : 0;
            } while ((*carta)[(*size) + i].super_trunfo != 1 && (*carta)[(*size) + i].super_trunfo != 0);
        }

        printf("\nForca[1 ~ 50]: ");
        (*carta)[(*size) + i].forca = get_int(1, 50, "\033[1;31mInsira um valor valido!\033[1;30m[1 ~ 50]:\033[m ");
        printf("\nHabilidade[1 ~ 100]: ");
        (*carta)[(*size) + i].habilidade = get_int(1, 100, "\033[1;31mInsira um valor valido!\033[1;30m[1 ~ 100]:\033[m ");
        printf("\nVelocidade[1 ~ 20]: ");
        (*carta)[(*size) + i].velocidade = get_int(1, 20, "\033[1;31mInsira um valor valido!\033[1;30m[1 ~ 20]:\033[m ");
        printf("\nPoderes[1 ~ 50]: ");
        (*carta)[(*size) + i].poderes = get_int(1, 50, "\033[1;31mInsira um valor valido!\033[1;30m[1 ~ 50]:\033[m ");
        printf("\nPoder cura[1 ~ 100]: ");
        (*carta)[(*size) + i].poder_cura = get_int(1, 100, "\033[1;31mInsira um valor valido!\033[1;30m[1 ~ 100]:\033[m ");
    }
    *size += cartas_add;
    
    printf("\n\033[3;92mCartas inseridas com sucesso!\033[m\n");
    return;
}

/**
 * @brief Lista todas as cartas disponíveis.
 * 
 * @param carta 
 * @param num_cartas 
 */
void listar_cartas(Cartas* carta, int num_cartas) {
    printf("\n%-21s | %-5s | %-6s | %-12s | %-6s | %-12s | %-12s | %-9s | %-12s\n", "NOME", "LETRA", "NUMERO", "SUPER-TRUNFO", "FORCA", "HABILIDADE", "VELOCIDADE", "PODERES", "PODER CURA");

    for (int i = 0; i < num_cartas; i++) {
        printf("%-21s | %-5c | %-6d | %-12d | %-6d | %-12d | %-12d | %-9d | %-12d\n",
            carta[i].nome, carta[i].letra, carta[i].num, carta[i].super_trunfo,
            carta[i].forca, carta[i].habilidade, carta[i].velocidade, carta[i].poderes, carta[i].poder_cura);
    }
    
    return;
}

/**
 * @brief Exclui as cartas solicitadas pelo player.
 * 
 * @param cartas endereço na memória do vetor de cartas
 * @param quantd_cartas endereço na memória da variável contendo a quantidade de cartas
 */
void remover_carta(Cartas** cartas, int* quantd_cartas) {
    int quant_excluir = 0;
    char nome_excluir[TAM_NOME_CARTA];

    printf("\nInsira quantas cartas deseja excluir: ");
    quant_excluir = get_int(0, *quantd_cartas, "\n\033[1;31mInsira um valor válido!\033[1;30m[0-cancelar]:\033[m ");
    if (quant_excluir == 0 || quant_excluir >= *quantd_cartas-2) { // deve haver no m��nimo duas cartas no jogo
        printf("\n\033[1m\033[3mOperacao cancelada.\033[m\n");
        return;
    }

    int pos_excluir[quant_excluir], posicao;

    setbuf(stdin, NULL);

    // adicionando as posicoes das cartas a excluir corretamente
    for (int i = 0; i < quant_excluir; i++) {
        printf("\nInsira o nome da %da carta: ", i + 1);
        burocracia(nome_excluir, TAM_NOME_CARTA);
        posicao = get_pos_carta(cartas, *quantd_cartas, nome_excluir);
        if (posicao == -1) {
            printf("\n\033[1m\033[3mCarta nao encontrada.\033[m\n");
            i--;
            continue;
        } else if (verify_int_in_vetor(pos_excluir, quant_excluir, posicao)) {
            printf("\n\033[1m\033[3mCarta ja inserida.\033[m\n");
            i--;
            continue;
        } else pos_excluir[i] = posicao;
    }

    // Ordenar os índices em ordem decrescente para evitar problemas ao remover
    sort_decrease(pos_excluir, quant_excluir);
    
    // Excluindo as cartas do vetor
    remover_cartas_vetor(cartas, quantd_cartas, pos_excluir, quant_excluir);

    printf("\n\033[3;92mCarta(s) removida(s) com sucesso!\033[m\n");
}

/**
 * @brief Função para editar qualquer atributo de uma carta
 * 
 * @param carta 
 * @param num_cartas 
 */
void alterar_carta(Cartas** cartas, int quantd_cartas){
    char nome_alterar[TAM_NOME_CARTA];
    int atributo_alterar;
    int posicao;

    setbuf(stdin, NULL);
    printf("\nQual o nome da carta que deseja alterar: ");
    do
    {
        burocracia(nome_alterar,TAM_NOME_CARTA);
        posicao = get_pos_carta(cartas, quantd_cartas, nome_alterar);
        if (posicao == -1){
            printf("\n\033[1m\033[3mCarta não encontrada.\033[1m Insira um nome válido: \n");
        }else break;
    } while (1);
    
    printf("\n1 - forca: %i\n2 - Habilidade: %i\n3 - Velocidade: %i\n4 - Poderes: %i\n5 - Poder cura: %i\nQual deseja alterar: ",
        (*cartas)[posicao].forca, (*cartas)[posicao].habilidade, (*cartas)[posicao].velocidade,
        (*cartas)[posicao].poderes, (*cartas)[posicao].poder_cura);

    atributo_alterar = get_int(1, 5, "\033[1;91mEntrada inválida!\033[1m[1 ~ 5]:\033[m ");

    printf("\nInsira o novo valor do atributo: ");
    
    switch (atributo_alterar)
    {
    case 1:
        (*cartas)[posicao].forca = get_int(1, 50, "\033[1;31mInsira um valor válido!\033[1m[1 ~ 50]:\033[m ");
        break;
    case 2:
        (*cartas)[posicao].habilidade = get_int(1, 100, "\033[1;31mInsira um valor válido!\033[1m[1 ~ 100]:\033[m ");
        break;
    case 3:
        (*cartas)[posicao].velocidade = get_int(1, 20, "\033[1;31mInsira um valor válido!\033[1m[1 ~ 20]:\033[m ");
        break;
    case 4:
        (*cartas)[posicao].poderes = get_int(1, 50, "\033[1;31mInsira um valor válido!\033[1m[1 ~ 50]:\033[m ");
        break;
    case 5:
        (*cartas)[posicao].poder_cura = get_int(1, 100, "\033[1;31mInsira um valor válido!\033[1m[1 ~ 100]:\033[m ");
        break;
    }
    printf("\n\n\033[1;94mCarta atualizada:\033[m\n");
    printf("Nome: %-21s \nForça: %-6d \nHabilidade: %-12d\nVelocidade:  %-12d\nPoderes: %-9d \nPoder cura:  %-12d\n",
    (*cartas)[posicao].nome,(*cartas)[posicao].forca, (*cartas)[posicao].habilidade, (*cartas)[posicao].velocidade, (*cartas)[posicao].poderes, (*cartas)[posicao].poder_cura);        
    printf("\n\033[3;92mCarta alterada com sucesso!\033[m\n");
}

/**
 * @brief Função principal para gerenciamento de busca de cartas
 * 
 * @param carta Vetor de cartas
 * @param qnt_cartas Quantidade de cartas disponíveis
 */
void buscar_carta(Cartas carta[], int qnt_cartas) {
    int sair = 0;
    do {
        printf("\nOpções de pesquisa:\n");
        printf("1 - Pesquisar por Atributo\n");
        printf("2 - Pesquisar por Letra\n");
        printf("3 - Pesquisar por Número\n");
        printf("4 - Sair\n");

        printf("\nEscolha uma opção: ");
        int escolha = get_int(1, 4, "\033[1;91mOpção inválida!\033[1;30m Insira um número de 1 a 4:\033[m ");

        switch (escolha) {
            case PESQ_ATRIBUTO: {
                printf("\n=== Pesquisar por Atributo ===\n");
                int escolha_atributo = get_int(1, 6, "Escolha um atributo (1 a 5, ou 6 para sair): ");
                if (escolha_atributo == 6) break;

                int valor_comparado = get_int(0, 100, "Digite o valor para comparação: ");
                int comparacao = get_int(1, 2, "1 - Maior que, 2 - Menor que: ");
                buscar_por_atributo(carta, qnt_cartas, escolha_atributo, comparacao, valor_comparado);
                break;
            }
            case PESQ_LETRA: {
                printf("\n=== Pesquisar por Letra ===\n");
                char letra;
                do {
                    printf("Digite uma letra: ");
                    scanf(" %c", &letra);
                    letra = toupper(letra);
                    if (letra < 'A' || letra > 'Z') {
                        printf("\033[1;91mEntrada inválida! Insira uma letra de A a Z.\033[m\n");
                    }
                } while (letra < 'A' || letra > 'Z');
                buscar_por_letra(carta, qnt_cartas, letra);
                break;
            }
            case PESQ_NUMERO: {
                printf("\n=== Pesquisar por Número ===\n");
                int numero = get_int(1, 100, "Digite o número: ");
                buscar_por_numero(carta, qnt_cartas, numero);
                break;
            }
            case SAIR:
                sair = 1;
                printf("\nSaindo...\n");
                break;
        }
    } while (!sair);
}

/**
 * @brief Funcao para listar o historico de partidas
 * 
 */
void historico() {
    FILE* arq = fopen("assets/data/historico.dat", "rb");
    if (arq == NULL) {
        printf("\nn�o houve outras partidas\n");
        return;
    }

    fseek(arq, 0, SEEK_END);
    int sizeHist=ftell(arq)/sizeof(Historico);
    fseek(arq, 0, SEEK_SET);
    Historico hist[sizeHist];

    fread(hist, sizeof(Historico), sizeHist, arq);


    printf("\nHistorico de partidas:\n");
    printf("| %10s | %9s | %9s | %9s | %11s |\n","Vencedor", "Rodadas", "Vitorias", "Empates", "Pontuacao");
    for (int i = 0; i < sizeHist; i++)
    {
        printf("| %10s | %9d | %9d | %9d | %11i |\n", hist[i].vencedor, hist[i].rodadas, hist[i].vitorias,
        hist[i].empates, hist[i].pontuacao);
    }
    
    fclose(arq);
    return;
}