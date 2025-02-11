
/**
 * @file filechange.c
 * @brief Funções para manipulação de arquivos e cartas
 * 
 * @license MIT 
 * @copyright Copyright (c) 2024
 * 
 */

#include <ctype.h> // necessario pra toupper
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

    printf("\nInsira quantas cartas deseja inserir (temos %i cartas): ", *size);
    do {
        if (scanf("%2d", &cartas_add) != 1 || cartas_add < 1) {
            printf("\n\033[1;91mNúmero inválido.\033[1m Digite um número positivo > 0:\033[m ");
            while (getchar() != '\n'); // Limpa o buffer
        } else break;
    } while (1);

    while (getchar() != '\n'); // Limpa o buffer

    // Realloc para o vetor de cartas
    Cartas *novo_carta = realloc(*carta, ((*size) + cartas_add) * sizeof(Cartas));
    if (novo_carta == NULL) {
        perror("\033[1;31mErro ao alocar memória para cartas\033[m");
        return;
    }
    *carta = novo_carta;

    for (int i = 0; i < cartas_add; i++) {

        char tentaNome[TAM_NOME_CARTA];
        int nome_valido=0;
        do
        {
            printf("\nNome da carta: ");
            burocracia(tentaNome, TAM_NOME_CARTA);
            for (int i = 0; i < *size; i++){
                if(strcmp(tentaNome, (*carta)[i].nome)==0){
                    printf("\n\033[1;31mNome já existente, insira outro nome.\033[m\n");
                    break;
                }else{
                    nome_valido=1;
                    strcpy((*carta)[(*size) + i].nome, tentaNome);
                }
            }
            
        } while (nome_valido==0);
        

        char TentaLetra;
        int posi_procurada = 0;
        int sair = 1;

        printf("\nLetra da carta (A, B, C, D): ");
        do {
            if (scanf(" %c", &TentaLetra) != 1) {
                printf("\n\033[1;31mLetra inválida. Tente novamente:\033[m ");
                while (getchar() != '\n'); // Limpa o buffer
                continue;
            }
            while (getchar() != '\n'); // Limpa o buffer
            TentaLetra = toupper(TentaLetra);

            int **ptr_posicoes;
            switch (TentaLetra) {
                case 'A': ptr_posicoes = ptr_posicoesA; break;
                case 'B': ptr_posicoes = ptr_posicoesB; break;
                case 'C': ptr_posicoes = ptr_posicoesC; break;
                case 'D': ptr_posicoes = ptr_posicoesD; break;
                default:
                    printf("\n\033[1;31mLetra inválida. Tente novamente:\033[m ");
                    continue;
            }

            // Calcula a próxima posi��o dispon�vel
            int aux = (*ptr_posicoes == NULL) ? 0 : (*size);
            for (int j = 0; j < aux; j++) {
                if ((*ptr_posicoes)[j] != j + 1) {
                    posi_procurada = j + 1;
                    break;
                }
            }
            if (posi_procurada == 0) posi_procurada = aux + 1;

            // Realoca mem�ria para a nova posi��o
            int *novo_posicoes = realloc(*ptr_posicoes, posi_procurada * sizeof(int));
            if (novo_posicoes == NULL) {
                perror("\033[1;31mErro ao alocar memória para posições\033[m");
                return;
            }
            *ptr_posicoes = novo_posicoes;
            (*ptr_posicoes)[posi_procurada - 1] = posi_procurada;
            sair = 0;
        } while (sair);

        (*carta)[(*size) + i].letra = TentaLetra;
        (*carta)[(*size) + i].num = posi_procurada;

        printf("\nTemos a posição %i disponível em %c\n", posi_procurada, TentaLetra);

        int existenciaDoSuper = 0;
        for (int j = 0; j < (*size); j++) {
            if ((*carta)[j].super_trunfo == 1) {
                existenciaDoSuper = 1;
                break;
            }
        }

        if (existenciaDoSuper) {
            printf("\033[1mJ� existe um Super-Trunfo\033[m\n");
            (*carta)[(*size) + i].super_trunfo = 0;
        } else {
            int escolhaTrunfo;
            do {
                printf("Super Trunfo?\n1 - Sim\n2 - N�o\n: ");
                if (scanf("%d", &escolhaTrunfo) != 1) {
                    printf("\033[1;31mEntrada inv�lida! Tente novamente.\033[m\n");
                    while (getchar() != '\n'); // Limpa o buffer
                    continue;
                }
                (*carta)[(*size) + i].super_trunfo = (escolhaTrunfo == 1) ? 1 : 0;
            } while (escolhaTrunfo != 1 && escolhaTrunfo != 2);
        }

        printf("\nInsira os atributos da carta:\n");

        printf("Força [1 ~ 50]: ");
        (*carta)[(*size) + i].forca = get_int(1, 50, "\033[1;31mInsira um valor v�lido!\033[1;30m [1 ~ 50]:\033[m ");

        printf("Habilidade [1 ~ 100]: ");
        (*carta)[(*size) + i].habilidade = get_int(1, 100, "\033[1;31mInsira um valor v�lido!\033[1;30m [1 ~ 100]:\033[m ");

        printf("Velocidade [1 ~ 20]: ");
        (*carta)[(*size) + i].velocidade = get_int(1, 20, "\033[1;31mInsira um valor v�lido!\033[1;30m [1 ~ 20]:\033[m ");

        printf("Poderes [1 ~ 50]: ");
        (*carta)[(*size) + i].poderes = get_int(1, 50, "\033[1;31mInsira um valor v�lido!\033[1;30m [1 ~ 50]:\033[m ");

        printf("Poder de cura [1 ~ 100]: ");
        (*carta)[(*size) + i].poder_cura = get_int(1, 100, "\033[1;31mInsira um valor v�lido!\033[1;30m [1 ~ 100]:\033[m ");
        //strcpy((*carta)[(*size) + i].textura, '\0');
        printf("Deseja inserir uma textura para a carta? [1 - Sim, 2 - Não]: \n");
        int escolha_textura;
        do
        {
            scanf("%d", &escolha_textura);
            if (escolha_textura != 1 && escolha_textura != 2)
            {
                printf("\033[1;31mInsira um valor válido!\033[1;30m [1 ~ 2]:\033[m ");
            }
            
        } while (escolha_textura != 1 && escolha_textura != 2);
        
        setbuf(stdin, NULL);
        if(escolha_textura==1){
            printf("\nPrimeiro coloque na sua textura na pasta assets/img/cartas\n\npor fim nos diga o nome da textura\n");
            printf("Nome textura: ");
            burocracia((*carta)[(*size) + i].textura, TAM_NOME_CARTA);
            //concatenando o caminho da textura das pastas
            int escolha_formato;
            do
            {
                printf("1 - png\n2 - jpg\n");
                scanf("%d", &escolha_formato);
                if (escolha_formato==1)
                {
                    strcat((*carta)[(*size) + i].textura, ".png");
                }else if(escolha_formato==2){
                    strcat((*carta)[(*size) + i].textura, ".jpg");
                }else{
                    printf("\033[1;31mInsira um valor válido!\033[1;30m [1 ~ 2]:\033[m ");
                }
            } while (escolha_formato != 1 && escolha_formato != 2);
            
            char caminho[20] = "assets/img/cartas/";
            strcat(caminho, (*carta)[(*size) + i].textura);
            strcpy((*carta)[(*size) + i].textura, caminho);

        }
        setbuf(stdin, NULL);
    }

    *size += cartas_add;
    printf("\n\033[3;92mCartas inseridas com sucesso!\033[m\n");
}

/**
 * @brief Lista todas as cartas dispon��veis.
 * 
 * @param carta 
 * @param num_cartas 
 */
void listar_cartas(Cartas* carta, int num_cartas) {
    printf("\n%-21s | %-5s | %-6s | %-12s | %-6s | %-12s | %-12s | %-9s | %-12s\n", "NOME", "LETRA", "NUMERO", "SUPER-TRUNFO", "FORÇA", "HABILIDADE", "VELOCIDADE", "PODERES", "PODER CURA");

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
 * @param cartas endere�o na mem�ria do vetor de cartas
 * @param quantd_cartas endere�o na mem�ria da vari�vel contendo a quantidade de cartas
 */
void remover_carta(Cartas** cartas, int* quantd_cartas) {
    int quant_excluir = 0;
    char nome_excluir[TAM_NOME_CARTA];

    printf("\nInsira quantas cartas deseja excluir: ");
    quant_excluir = get_int(0, *quantd_cartas, "\n\033[1;31mInsira um valor valido!\033[1;30m[0-cancelar]:\033[m ");
    if (quant_excluir == 0 || quant_excluir >= *quantd_cartas-2) { // deve haver no m�ninimo duas cartas no jogo
        printf("\n\033[1m\033[3mOperacao cancelada.\033[m\n");
        return;
    }

    int pos_excluir[quant_excluir], posicao;
    for (int i = 0; i < quant_excluir; i++)
    {
        pos_excluir[i]=-1;
    }
    

    setbuf(stdin, NULL);

    // adicionando as posicoes das cartas a excluir corretamente
    for (int i = 0; i < quant_excluir; i++) {
        printf("\nInsira o nome da %da carta: ", i + 1);
        burocracia(nome_excluir, TAM_NOME_CARTA);
        posicao = get_pos_carta(cartas, *quantd_cartas, nome_excluir);
        if (posicao == -1) {
            printf("\n\033[1m\033[3mCarta n�o encontrada.\033[m\n");
            i--;
            continue;
        } else if (verify_int_in_vetor(pos_excluir, quant_excluir, posicao)) {
            printf("\n\033[1m\033[3mCarta j� inserida.\033[m\n");
            i--;
            continue;
        } else pos_excluir[i] = posicao;
    }

    // Ordenar os indices em ordem decrescente para evitar problemas ao remover
    sort_decrease(pos_excluir, quant_excluir);
    
    // Excluindo as cartas do vetor
    remover_cartas_vetor(cartas, quantd_cartas, pos_excluir, quant_excluir);

    printf("\n\033[3;92mCarta(s) removida(s) com sucesso!\033[m\n");
}

/**
 * @brief Fun��o para editar qualquer atributo de uma carta
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
            printf("\n\033[1m\033[3mCarta n�o encontrada.\033[1m Insira um nome v�lido: \n");
        }else break;
    } while (1);
    
    printf("\n1 - forca: %i\n2 - Habilidade: %i\n3 - Velocidade: %i\n4 - Poderes: %i\n5 - Poder cura: %i\nQual deseja alterar: ",
        (*cartas)[posicao].forca, (*cartas)[posicao].habilidade, (*cartas)[posicao].velocidade,
        (*cartas)[posicao].poderes, (*cartas)[posicao].poder_cura);

    atributo_alterar = get_int(1, 5, "\033[1;91mEntrada inv�lida!\033[1m[1 ~ 5]:\033[m ");

    printf("\nInsira o novo valor do atributo: ");
    
    switch (atributo_alterar)
    {
    case 1:
        (*cartas)[posicao].forca = get_int(1, 50, "\033[1;31mInsira um valor v�lido!\033[1m[1 ~ 50]:\033[m ");
        break;
    case 2:
        (*cartas)[posicao].habilidade = get_int(1, 100, "\033[1;31mInsira um valor v�lido!\033[1m[1 ~ 100]:\033[m ");
        break;
    case 3:
        (*cartas)[posicao].velocidade = get_int(1, 20, "\033[1;31mInsira um valor v�lido!\033[1m[1 ~ 20]:\033[m ");
        break;
    case 4:
        (*cartas)[posicao].poderes = get_int(1, 50, "\033[1;31mInsira um valor v�lido!\033[1m[1 ~ 50]:\033[m ");
        break;
    case 5:
        (*cartas)[posicao].poder_cura = get_int(1, 100, "\033[1;31mInsira um valor v�lido!\033[1m[1 ~ 100]:\033[m ");
        break;
    }
    printf("\n\n\033[1;94mCarta atualizada:\033[m\n");
    printf("Nome: %-21s \nFor�a: %-6d \nHabilidade: %-12d\nVelocidade:  %-12d\nPoderes: %-9d \nPoder cura:  %-12d\n",
    (*cartas)[posicao].nome,(*cartas)[posicao].forca, (*cartas)[posicao].habilidade, (*cartas)[posicao].velocidade, (*cartas)[posicao].poderes, (*cartas)[posicao].poder_cura);        
    printf("\n\033[3;92mCarta alterada com sucesso!\033[m\n");
}

/**
 * @brief Função principal para gerenciamento de busca de cartas
 * 
 * @param carta Vetor de cartas
 * @param qnt_cartas Quantidade de cartas disponiveis
 */
void buscar_carta(Cartas *carta, int qnt_cartas) {
    int sair = 0;
    do {
        printf("\nOpções de pesquisa:\n");
        printf("1 - Pesquisar por Atributo\n");
        printf("2 - Pesquisar por Letra\n");
        printf("3 - Pesquisar por Numero\n");
        printf("4 - Sair\n");

        printf("\nEscolha uma opção: ");
        int escolha = get_int(1, 4, "\033[1;91mOpcao invalida!\033[1;30m Insira um numero de 1 a 4:\033[m ");

        switch (escolha) {
            case PESQ_ATRIBUTO: {
                printf("\n=== Pesquisar por Atributo ===\n");
                printf("\n1 - Força\n2 - Habilidade\n3 - Velocidade\n4 - Poderes\n5 - Poder de Cura\n6 - Sair\n");
                printf("\nEscolha um atributo (1 a 5, ou 6 para sair): ");
                int escolha_atributo = get_int(1, 6, "\033[1;31mEscolha inválida!\033[1m (1 a 5, ou 6 para sair):\033[m ");
                if (escolha_atributo == 6) break;
                
                int valor_max;
                switch (escolha_atributo){
                case FORCA:
                    valor_max = FORCA_MAX;
                    break;
                case HABILIDADE:
                    valor_max = HABILIDADE_MAX;
                    break;
                case VELOCIDADE:
                    valor_max = VELOCIDADE_MAX;
                    break;
                case PODERES:
                    valor_max = PODERES_MAX;
                    break;
                case CURA:
                    valor_max = CURA_MAX;
                    break;
                }
                printf("\nDigite o valor para comparação[1~%d]: ", valor_max);
                int valor_comparado = get_int(1, valor_max, "\033[1;31mEscolha inválida!\033[1m Insira um valor válido:\033[m ");
                printf("\n1 - Maior que, 2 - Menor que: ");
                int comparacao = get_int(1, 2, "\033[1;31mEscolha inválida!\033[1m Insira um valor válido:\033[m ");
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
                        printf("\033[1;91mEntrada invalida! Insira uma letra de A a Z.\033[m\n");
                    }
                } while (letra < 'A' || letra > 'Z');
                buscar_por_letra(carta, qnt_cartas, letra);
                break;
            }
            case PESQ_NUMERO: {
                printf("\n=== Pesquisar por Numero ===\n");
                printf("\nDigite o numero: ");
                int numero = get_int(1, 100, "\n\033[1;31mEscolha inválida!\033[1m Insira um valor válido:\033[m ");
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
        printf("\nnão houve outras partidas ou somente o CPU ganhou\n");
        return;
    }

    fseek(arq, 0, SEEK_END);
    int sizeHist=ftell(arq)/sizeof(Historico);
    fseek(arq, 0, SEEK_SET);
    Historico hist[sizeHist];

    fread(hist, sizeof(Historico), sizeHist, arq);


    printf("\nHist�rico de partidas:\n");
    printf("| %10s | %9s | %17s | %9s | %18s | %5s |\n","Vencedor", "Rodadas", "Vitórias player", "Empates", "Pontuação player","Data");
    for (int i = 0; i < sizeHist; i++)
    {
        printf("| %10s | %9d | %17d | %9d | %18i | %02d/%02d |\n", hist[i].vencedor, hist[i].rodadas, hist[i].vitorias,
        hist[i].empates, hist[i].pontuacao, hist[i].dia, hist[i].mes);
    }
    
    fclose(arq);
    return;
}

/**
 * @brief Função que mostra as maiores pontuações do historico
 * 
 */
void ranking(){
    FILE* arq = fopen("assets/data/historico.dat", "rb");
    if (arq == NULL) {
        printf("\nnão houve outras partidas ou somente o CPU ganhou\n");
        return;
    }

    fseek(arq, 0, SEEK_END);
    int sizeHist=ftell(arq)/sizeof(Historico);
    fseek(arq, 0, SEEK_SET);
    Historico hist[sizeHist];

    fread(hist, sizeof(Historico), sizeHist, arq);

    int pontuacao[sizeHist];
    char nomes[sizeHist][4];

    for (int i = 0; i < sizeHist; i++)
    {
        pontuacao[i]=hist[i].pontuacao;
        strcpy(nomes[i],hist[i].vencedor);
    }

    sort_decrease(pontuacao, sizeHist);

    printf("\nRanking:\n");
    printf("| %10s | %18s | %5s |\n","Jogador", "Pontuação player","Data");
    int processed[sizeHist];
    for (int i = 0; i < sizeHist; i++) {
        processed[i] = 0;
    }

    for (int i = 0; i < sizeHist; i++) {
        if (processed[i]) continue;
        for (int j = 0; j < sizeHist; j++) {
            if (pontuacao[i] == hist[j].pontuacao && !processed[j]) {
                printf("| %10s | %18i | %02d/%02d |\n", nomes[j], pontuacao[i], hist[j].dia, hist[j].mes);
                processed[j] = 1;
            }
        }
    }
    fclose(arq);
    return;
}
