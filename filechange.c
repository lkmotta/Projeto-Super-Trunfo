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

    int cartas_add;
    int contaN = quant_cartas(arq_cartaCOPIA);
    rewind(arq_cartaCOPIA);

    printf("\nInsira quantas cartas desejar inserir (temos %i cartas): ", contaN);
    do {
        scanf("%2d", &cartas_add);
        if (cartas_add < 1) {
            printf("\n\033[1mNÃºmero invalido. Digite um numero positivo > 0:\033[m ");
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

        int existenciaDoSuper=0;
        int escolhaTrunfo;

        for (int i = 0; i < contaN; i++)
        {
            if ((*carta)[i].super_trunfo==1)
            {
                existenciaDoSuper=1;
            }
        }

        if (existenciaDoSuper==1)
        {
            printf("já existe um super-Trunfo\n\n");
        }else{
            do {
                printf("Super Trunfo?\n1 - sim\n2 - nao\n: ");
                scanf("%i", &escolhaTrunfo);
                (*carta)[contaN + i].super_trunfo = (escolhaTrunfo == 1) ? 1 : 0;
            } while ((*carta)[contaN + i].super_trunfo != 1 && (*carta)[contaN + i].super_trunfo != 0);
        }

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

    printf("\nNumero de cartas: %d\n", num_cartas);

    printf("\n%-21s | %-5s | %-6s | %-12s | %-6s | %-12s | %-12s | %-9s | %-12s\n", "NOME", "LETRA", "NUMERO", "SUPER-TRUNFO", "FORCA", "HABILIDADE", "VELOCIDADE", "PODERES", "PODER CURA");

    for (int i = 0; i < num_cartas; i++) {
        printf("%-21s | %-5c | %-6d | %-12d | %-6d | %-12d | %-12d | %-9d | %-12d\n",
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


    printf("\n\033[1m\033[3mcCom defeito !!!!!!!!!!!!!!!!!\n\033[m\n");
    return;


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

    // Ãºnica maneira de realocar sem erros: criando outro vetor
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

/**
 * @brief Função para editar uma carta
 * 
 * @param arq_carta 
 * @param carta 
 * @param num_cartas 
 */
void alterar_carta(FILE* arq_cartas,Cartas** cartas, int quantd_cartas){
    char nome_alterar[TAM_NOME_CARTA];
    int atributo_alterar;
    int novo_valor;
    int posicao;
    int achou=0;
    setbuf(stdin,NULL);
    printf("qual o nome da carta que deseja alterar: ");
    burocracia(nome_alterar,TAM_NOME_CARTA);
    for (int i = 0; i < quantd_cartas; i++)
    {
        if (strcasecmp(nome_alterar,(*cartas)[i].nome)==0)
        {
            posicao=i;
            do
            {
                printf("\n1 - forca: %i\n2 - Habilidade: %i\n3 - Velocidade: %i\n4 - Poderes: %i\n5 - Poder cura: %i\nqual deseja alterar: ",(*cartas)[posicao].atributo_1, (*cartas)[posicao].atributo_2, (*cartas)[posicao].atributo_3, (*cartas)[posicao].atributo_4, (*cartas)[posicao].atributo_5);
                atributo_alterar=get_int();
                if (atributo_alterar>5 || atributo_alterar==0)
                {
                    printf("número inválido\n");
                }
                
            } while (atributo_alterar>5 || atributo_alterar==0);
            printf("qual o novo valor do atributo: ");
            novo_valor=get_int();
            
            switch (atributo_alterar)
            {
            case 1:
                (*cartas)[i].atributo_1=novo_valor;
                break;
            case 2:
                (*cartas)[i].atributo_2=novo_valor;
                break;
            case 3:
                (*cartas)[i].atributo_3=novo_valor;
                break;
            case 4:
                (*cartas)[i].atributo_4=novo_valor;
                break;
            case 5:
                (*cartas)[i].atributo_5=novo_valor;
                break;
            }
            achou=1;
            printf("\n\ncarta atualizada:\n");
            printf("\n\nnome: %-21s \nforca: %-6d \nhabilidade: %-12d\nvelocidade:  %-12d\nPoderes: %-9d \npode cura:  %-12d\n",
            (*cartas)[posicao].nome,(*cartas)[posicao].atributo_1, (*cartas)[posicao].atributo_2, (*cartas)[posicao].atributo_3, (*cartas)[posicao].atributo_4, (*cartas)[posicao].atributo_5);        
            printf("\n\033[3;92mCarta alterada com sucesso!\033[m\n");
            break;
        }
        
    }
    if (achou==0)
    {
        printf("não foi achado\n");
    }
    
}

/**
 * @brief Função principal para gerenciamento de busca de cartas
 * 
 * @param carta Vetor de cartas
 * @param qnt_cartas Quantidade de cartas disponíveis
 */
void buscar_carta(Cartas carta[], int qnt_cartas) {
    setlocale(LC_ALL, "Portuguese");

    int sair = 0;
    do {
        printf("\nOpções de pesquisa:\n");
        printf("1 - Pesquisar por Atributo\n");
        printf("2 - Pesquisar por Letra\n");
        printf("3 - Pesquisar por Número\n");
        printf("4 - Sair\n");

        int escolha = validar_entrada(1, 4, "Escolha uma opção: ");

        switch (escolha) {
            case PESQ_ATRIBUTO: {
                printf("\n=== Pesquisar por Atributo ===\n");
                int escolha_atributo = validar_entrada(1, 6, "Escolha um atributo (1 a 5, ou 6 para sair): ");
                if (escolha_atributo == 6) break;

                int valor_comparado = validar_entrada(0, 100, "Digite o valor para comparação: ");
                int comparacao = validar_entrada(1, 2, "1 - Maior que, 2 - Menor que: ");
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
                int numero = validar_entrada(1, 100, "Digite o número: ");
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
    