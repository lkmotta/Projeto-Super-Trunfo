#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // necessaria para srand
#include <unistd.h> // necessaria para sleep
#include "funcaux.h"
#include "filechange.h" // necessaria para listar_cartas

/**
 * @brief Retorna o modo de jogo escolhido pelo player.
 *
 * @return int
 */
int modo()
{
    printf("\n\033[5mEscolha o modo de jogo:\n\033[1;93m1 - NORMAL\n\033[1;31m2 - DIF�CIL\033[m\n: ");

    return get_int(1, 2, "\033[1;91mModo inv�lido!\033[1m Insira 1 ou 2:\033[m ");
}

/**
 * @brief Retorna um vetor de cartas(baralho) embaralhado.
 *
 * @param cartas // vetor das cartas do jogo
 * @param quantd_cartas // quantidade de cartas no vetor
 * @param quant_cartas_baralho // quantidade de cartas que deve ter no baralho
 * @param quant_cartas_usadas // quantidade de cartas usadas
 * @param cartas_usadas // vetor de posicoes das cartas usadas (posicoes em 'cartas')
 * @return Cartas*
 */
Cartas *get_baralho(Cartas *cartas, int quantd_cartas, int quant_cartas_baralho, int *quant_cartas_usadas, int **cartas_usadas)
{
    Cartas *baralho = (Cartas *)malloc(quant_cartas_baralho * sizeof(Cartas));
    if (baralho == NULL)
    {
        perror("\n\033[1;91mErro ao alocar mem�ria para o baralho\033[m");
        exit(1);
    }

    int usadas = 0;
    if (*quant_cartas_usadas > 0)
        usadas = 1;

    if (!usadas)
    {
        *cartas_usadas = (int *)malloc(quant_cartas_baralho * sizeof(int));
        if (*cartas_usadas == NULL)
        {
            perror("\n\033[1;91mErro ao alocar mem�ria para cartas_usadas\033[m");
            exit(1);
        }
        embaralhar(cartas, quantd_cartas);
        for (int i = 0; i < quant_cartas_baralho; i++)
        {
            baralho[i] = cartas[i];
            (*cartas_usadas)[i] = i;
        }
        *quant_cartas_usadas = quant_cartas_baralho;
    }
    else
    {
        int j = 0;
        for (int i = 0; i < quantd_cartas; i++)
        {
            if (!verify_int_in_vetor(*cartas_usadas, *quant_cartas_usadas, i))
            {
                baralho[j] = cartas[i];
                j++;
                if (j >= quant_cartas_baralho)
                    break;
            }
        }
        *quant_cartas_usadas += quant_cartas_baralho;
        *cartas_usadas = realloc(*cartas_usadas, *quant_cartas_usadas * sizeof(int));
        if (*cartas_usadas == NULL)
        {
            perror("\n\033[1;91mErro ao realocar mem�ria para cartas_usadas\033[m");
            exit(1);
        }
        for (int i = 0; i < quant_cartas_baralho; i++)
        {
            (*cartas_usadas)[*quant_cartas_usadas - quant_cartas_baralho + i] = i;
        }
        // futuramente caso aja modo multiplayer ou +de 2 jogadores
        /* *quant_cartas_usadas += quant_cartas_baralho;
        *cartas_usadas = realloc(*cartas_usadas, *quant_cartas_usadas * sizeof(int));
        if(*cartas_usadas == NULL){
            perror("\n\033[1;91mErro ao realocar mem�ria para cartas_usadas\033[m");
            exit(1);
        }
        for (int i = 0; i < quant_cartas_baralho; i++) {
            (*cartas_usadas)[*quant_cartas_usadas - quant_cartas_baralho + i] = i;
        } */
    }
    // free ser� chamado na main em main.c

    return baralho;
}

/**
 * @brief Retorna o primeiro player a jogar. Caso 0, cpu comeca, caso 1, player comeca.
 *
 * @return int
 */
int primeiro_player()
{
    srand(time(NULL));
    return rand() % 2;
}

int verifica_maior(Cartas *carta_jogador, Cartas *carta_cpu, int atributo)
{
    switch (atributo)
    {
    case FORCA:
        if (carta_jogador->forca > carta_cpu->forca)
            return 1;
        if (carta_jogador->forca < carta_cpu->forca)
            return 0;
        return -1;
    case HABILIDADE:
        if (carta_jogador->habilidade > carta_cpu->habilidade)
            return 1;
        if (carta_jogador->habilidade < carta_cpu->habilidade)
            return 0;
        return -1;
    case VELOCIDADE:
        if (carta_jogador->velocidade > carta_cpu->velocidade)
            return 1;
        if (carta_jogador->velocidade < carta_cpu->velocidade)
            return 0;
        return -1;
    case PODERES:
        if (carta_jogador->poderes > carta_cpu->poderes)
            return 1;
        if (carta_jogador->poderes < carta_cpu->poderes)
            return 0;
        return -1;
    case CURA:
        if (carta_jogador->poder_cura > carta_cpu->poder_cura)
            return 1;
        if (carta_jogador->poder_cura < carta_cpu->poder_cura)
            return 0;
        return -1;
    default:
        return -1;
    }
}

int verifica_menor(Cartas *carta_jogador, Cartas *carta_cpu, int atributo)
{
    switch (atributo)
    {
    case FORCA:
        if (carta_jogador->forca < carta_cpu->forca)
            return 1;
        if (carta_jogador->forca > carta_cpu->forca)
            return 0;
        return -1;
    case HABILIDADE:
        if (carta_jogador->habilidade < carta_cpu->habilidade)
            return 1;
        if (carta_jogador->habilidade > carta_cpu->habilidade)
            return 0;
        return -1;
    case VELOCIDADE:
        if (carta_jogador->velocidade < carta_cpu->velocidade)
            return 1;
        if (carta_jogador->velocidade > carta_cpu->velocidade)
            return 0;
        return -1;
    case PODERES:
        if (carta_jogador->poderes < carta_cpu->poderes)
            return 1;
        if (carta_jogador->poderes > carta_cpu->poderes)
            return 0;
        return -1;
    case CURA:
        if (carta_jogador->poder_cura < carta_cpu->poder_cura)
            return 1;
        if (carta_jogador->poder_cura > carta_cpu->poder_cura)
            return 0;
        return -1;
    default:
        return -1;
    }
}

/**
 * @brief Adiciona a carta do vencedor e do perdedor ao final do baralho do vencedor.
 *
 * @param baralho_vencedor
 * @param baralho_perdedor
 * @param quant_cartas_vencedor
 * @param quant_cartas_perdedor
 */
void adicionar_carta_vencedor(Cartas **baralho_vencedor, Cartas **baralho_perdedor, int *quant_cartas_vencedor, int *quant_cartas_perdedor)
{
    // realoca o baralho do vencedor para mais uma carta
    *baralho_vencedor = (Cartas *)realloc(*baralho_vencedor, (*quant_cartas_vencedor + 1) * sizeof(Cartas));
    if (*baralho_vencedor == NULL)
    {
        perror("\n\033[1;91mErro ao realocar mem�ria para baralho_vencedor\033[m");
        exit(1);
    }

    // adiciona a primeira carta do perdedor ao final do baralho do vencedor
    (*baralho_vencedor)[(*quant_cartas_vencedor)] = (*baralho_perdedor)[0];
    (*quant_cartas_vencedor)++;

    // remove a primeira carta do perdedor
    for (int i = 0; i < *quant_cartas_perdedor - 1; i++)
    {
        (*baralho_perdedor)[i] = (*baralho_perdedor)[i + 1];
    }
    (*quant_cartas_perdedor)--;

    // realoca o baralho do perdedor para o novo tamanho
    if (*quant_cartas_perdedor > 0)
    {
        Cartas *temp = realloc(*baralho_perdedor, (*quant_cartas_perdedor) * sizeof(Cartas));
        if (!temp)
        {
            perror("\n\033[1;91mErro ao realocar mem�ria para baralho_perdedor\033[m");
            exit(1);
        }
        *baralho_perdedor = temp;
    }
    else
    {
        free(*baralho_perdedor);
        *baralho_perdedor = NULL;
    }

    // move a primeira carta do vencedor para o final
    Cartas temp = (*baralho_vencedor)[0];
    for (int i = 0; i < *quant_cartas_vencedor - 1; i++)
    {
        (*baralho_vencedor)[i] = (*baralho_vencedor)[i + 1];
    }
    (*baralho_vencedor)[(*quant_cartas_vencedor) - 1] = temp;
    return;
}

/**
 * @brief Lida com o empate entre as cartas dos jogadores.
 *
 * @param baralho_jogador
 * @param quant_cartas_jogador
 * @param baralho_cpu
 * @param quant_cartas_cpu
 * @param cartas_empate_jogador
 * @param cartas_empate_cpu
 * @param quant_cartas_empate
 */
void lidar_com_empate(Cartas **baralho_jogador, int *quant_cartas_jogador, Cartas **baralho_cpu, int *quant_cartas_cpu, Cartas **cartas_empate_jogador, Cartas **cartas_empate_cpu, int *quant_cartas_empate)
{
    // realoca vetores de empate para adicionar as cartas atuais
    (*quant_cartas_empate)++;
    *cartas_empate_jogador = (Cartas *)realloc(*cartas_empate_jogador, (*quant_cartas_empate) * sizeof(Cartas));
    *cartas_empate_cpu = (Cartas *)realloc(*cartas_empate_cpu, (*quant_cartas_empate) * sizeof(Cartas));
    if (*cartas_empate_jogador == NULL || *cartas_empate_cpu == NULL)
    {
        perror("\n\033[1;91mErro ao realocar mem�ria para cartas de empate\033[m");
        exit(1);
    }

    // adiciona as cartas atuais aos vetores de empate
    (*cartas_empate_jogador)[(*quant_cartas_empate) - 1] = (*baralho_jogador)[0];
    (*cartas_empate_cpu)[(*quant_cartas_empate) - 1] = (*baralho_cpu)[0];

    // remove a primeira carta de cada baralho
    for (int i = 0; i < (*quant_cartas_jogador) - 1; i++)
    {
        (*baralho_jogador)[i] = (*baralho_jogador)[i + 1];
    }
    for (int i = 0; i < (*quant_cartas_cpu) - 1; i++)
    {
        (*baralho_cpu)[i] = (*baralho_cpu)[i + 1];
    }
    (*quant_cartas_jogador)--;
    (*quant_cartas_cpu)--;

    // realoca os baralhos
    if (*quant_cartas_jogador > 0)
    {
        Cartas *temp_jogador = realloc(*baralho_jogador, (*quant_cartas_jogador) * sizeof(Cartas));
        if (!temp_jogador)
        {
            perror("\n\033[1;91mErro ao realocar mem�ria para baralho_jogador\033[m");
            exit(1);
        }
        *baralho_jogador = temp_jogador;
    }
    else
    {
        free(*baralho_jogador);
        *baralho_jogador = NULL;
    }

    if (*quant_cartas_cpu > 0)
    {
        Cartas *temp_cpu = realloc(*baralho_cpu, (*quant_cartas_cpu) * sizeof(Cartas));
        if (!temp_cpu)
        {
            perror("\n\033[1;91mErro ao realocar mem�ria para baralho_cpu\033[m");
            exit(1);
        }
        *baralho_cpu = temp_cpu;
    }
    else
    {
        free(*baralho_cpu);
        *baralho_cpu = NULL;
    }

    return;
}

/**
 * @brief Executa uma partida de Super-Trunfo entre o jogador e a CPU.
 *
 * Esta funcao gerencia o fluxo de uma partida completa, incluindo a selecao de atributos,
 * comparacao de cartas, tratamento de empates e transferencia de cartas entre os baralhos.
 * A partida continua ate que um dos jogadores fique sem cartas.
 *
 * @param baralho_jogador Ponteiro para o vetor de cartas do jogador.
 * @param baralho_cpu Ponteiro para o vetor de cartas da CPU.
 * @param quant_cartas_baralho Quantidade inicial de cartas em cada baralho.
 *
 */
void partida(Cartas *baralho_jogador, Cartas *baralho_cpu, int quant_cartas_baralho)
{
    if (sistema())
        system("cls");
    else
        system("clear");

    int player_joga = 0; // booleano -> 0 = cpu joga, 1 = player joga
    int win = 0;         // booleano -> 0 = ninguem ganhou, 1 = alguem ganhou -> fim de jogo
    int quem_ganhou = 0; // 0 = cpu, 1 = player
    int atributo = 0;    // atributo escolhido
    const char *atributo_nome;
    int maior_menor = 0; // booleando -> 0 = menor, 1 = maior
    int quant_cartas_jogador = quant_cartas_baralho, quant_cartas_cpu = quant_cartas_baralho;
    Cartas carta_jogador, carta_cpu;
    int rodada = 1;

    // vetores que armazenam as cartas empatadas
    Cartas *cartas_empate_jogador = NULL;
    Cartas *cartas_empate_cpu = NULL;
    int quant_cartas_empate = 0;

    // variaveis para estatisticas:
    int rodadas = 0;
    int vitorias = 0;
    int empates = 0;

    printf("\n\033[1;32mSEU BARALHO:\033[m");
    listar_cartas(baralho_jogador, quant_cartas_baralho);

    if (primeiro_player())
    {
        printf("\n\033[1;32mVocê comeca!\033[m");
        player_joga = 1;
    }

    while (!win)
    { // começando a partida
        printf("\n\n\033[1;34mRodada %d\033[m - %s", rodada++, player_joga ? "\033[32mSua vez\033[m" : "\033[31mVez do CPU\033[m");
        printf("\n\033[1;32mCartas do Jogador: %d\033[m", quant_cartas_jogador);
        printf("\n\033[1;31mCartas do CPU: %d\033[m\n", quant_cartas_cpu);

        carta_jogador = baralho_jogador[0];
        carta_cpu = baralho_cpu[0];
        if (player_joga)
        {
            printf("\nCarta atual:\n");
            listar_cartas(&carta_jogador, 1);
            printf("\nEscolha um atributo:\n1 - For�a\n2 - Habilidade\n3 - Velocidade\n4 - Poderes\n5 - Cura\n: ");
            atributo = get_int(1, 5, "\033[1;91mAtributo invalido!\033[m Insira um numero de 1 a 5:\033[m ");
            printf("\nAtributo maior ou menor?\n1 - Maior\n0 - Menor\n: ");
            maior_menor = get_int(0, 1, "\033[1;91mEntrada inv�lida!\033[m Insira 0 ou 1:\033[m ");
        }
        else
        {
            srand(time(NULL));
            atributo = rand() % 5 + 1; // +1 para não pegar 0
            maior_menor = rand() % 2;
        }

        switch (atributo)
        {
        case FORCA:
            atributo_nome = "For�a";
            break;
        case HABILIDADE:
            atributo_nome = "Habilidade";
            break;
        case VELOCIDADE:
            atributo_nome = "Velocidade";
            break;
        case PODERES:
            atributo_nome = "Poderes";
            break;
        case CURA:
            atributo_nome = "Cura";
            break;
        default:
            atributo_nome = "Desconhecido";
            break;
        }
        printf("\n\033[31m%s\033[m X \033[32m%s\033[m (%s)", carta_cpu.nome, carta_jogador.nome, atributo_nome);
        sleep(0.5);

        if (maior_menor)
        {
            quem_ganhou = verifica_maior(&carta_jogador, &carta_cpu, atributo);
        }
        else
        {
            quem_ganhou = verifica_menor(&carta_jogador, &carta_cpu, atributo);
        }

        if (quem_ganhou == -1)
        {
            // Empate
            printf("\n\033[1;33mEmpate! A batalha continua...\033[m");

            empates++;
            rodadas++;

            lidar_com_empate(&baralho_jogador, &quant_cartas_jogador, &baralho_cpu, &quant_cartas_cpu, &cartas_empate_jogador, &cartas_empate_cpu, &quant_cartas_empate);
        }
        else if (quem_ganhou)
        {
            printf("\n\033[1;32mVoc� venceu a rodada!\033[m");

            vitorias++;
            rodadas++;

            // Adiciona as cartas de empate ao baralho do jogador
            if (quant_cartas_empate > 0)
            {
                for (int i = 0; i < quant_cartas_empate; i++)
                {
                    adicionar_carta_vencedor(&baralho_jogador, &cartas_empate_jogador, &quant_cartas_jogador, &quant_cartas_empate);
                    adicionar_carta_vencedor(&baralho_jogador, &cartas_empate_cpu, &quant_cartas_jogador, &quant_cartas_empate);
                }
                quant_cartas_empate = 0;
                if (cartas_empate_jogador != NULL)
                {
                    free(cartas_empate_jogador);
                    cartas_empate_jogador = NULL;
                }
                if (cartas_empate_cpu != NULL)
                {
                    free(cartas_empate_cpu);
                    cartas_empate_cpu = NULL;
                }
            }
            adicionar_carta_vencedor(&baralho_jogador, &baralho_cpu, &quant_cartas_jogador, &quant_cartas_cpu);
        }
        else
        {
            printf("\n\033[1;31mCPU venceu a rodada!\033[m");

            rodadas++;

            // Adiciona as cartas de empate ao baralho do CPU
            if (quant_cartas_empate > 0)
            {
                for (int i = 0; i < quant_cartas_empate; i++)
                {
                    adicionar_carta_vencedor(&baralho_cpu, &cartas_empate_jogador, &quant_cartas_cpu, &quant_cartas_empate);
                    adicionar_carta_vencedor(&baralho_cpu, &cartas_empate_cpu, &quant_cartas_cpu, &quant_cartas_empate);
                }
                quant_cartas_empate = 0;
                if (cartas_empate_jogador != NULL)
                {
                    free(cartas_empate_jogador);
                    cartas_empate_jogador = NULL;
                }
                if (cartas_empate_cpu != NULL)
                {
                    free(cartas_empate_cpu);
                    cartas_empate_cpu = NULL;
                }
            }
            adicionar_carta_vencedor(&baralho_cpu, &baralho_jogador, &quant_cartas_cpu, &quant_cartas_jogador);
        }

        if (quant_cartas_cpu == 0 || quant_cartas_jogador == 0)
            win = 1;
        player_joga = !player_joga;
        sleep(1);
    }

    char vencedor[10];

    if (quant_cartas_cpu == 0)
    {
        printf("\n\n\033[7;32mVocê venceu o jogo!\033[m\n\n");

        int valid = 0;
        char playerName[4];
        while (!valid)
        {
            printf("Digite seu apelido para o ranking (exatamente 3 letras): ");
            scanf("%s", playerName);

            // Verifica se o comprimento � exatamente 3
            if (strlen(playerName) == 3)
            {
                for (int i = 0; i < 3; i++)
                {
                    playerName[i] = toupper(playerName[i]);
                }
                valid = 1;
            }
            else
            {
                printf("Nome inválido. Deve conter exatamente 3 letras.\n");
            }
        }

        strcpy(vencedor, playerName);
    }
    else
    {
        printf("\n\n\033[7;91mCPU venceu o jogo!\033[m\n\n");
        strcpy(vencedor, "CPU");
    }

    int pontuacao = (10 * ((100 * vitorias) / rodadas))+50*empates;
    printf("\n\n\033[7;32mPontua��o: %d\n\033[m", pontuacao);

    Historico partidaHist;
    strcpy(partidaHist.vencedor, vencedor);
    partidaHist.rodadas = rodadas;
    partidaHist.vitorias = vitorias;
    partidaHist.empates = empates;
    partidaHist.pontuacao = pontuacao;
    //adicionando dia e mes no historico

    //pegando a data atual usando time.h
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    partidaHist.dia=tm.tm_mday;
    partidaHist.mes=tm.tm_mon+1;

    printf("\n\033[1;32mEstatisticas da partida:\nRodadas: %i\nVit�rias: %i\nEmpates: %i\nSua pontua��o: %i pontos\n%02d/%02d\033[m",
           partidaHist.rodadas, partidaHist.vitorias, partidaHist.empates, partidaHist.pontuacao, partidaHist.dia, partidaHist.mes);

    FILE *historico = fopen("assets/data/historico.dat", "rb+");
    if (historico == NULL) {
        FILE *arq = fopen("assets/data/historico.dat", "wb+");
        printf("Salvando no historico...\n");
        if (arq == NULL) {
            printf("Erro ao salvar no historico cod:01\n");
            return;
        }
        fseek(arq, 0, SEEK_END);
        fwrite(&partidaHist, sizeof(Historico), 1, arq);    
        fclose(arq);
    }else{
        fseek(historico, 0, SEEK_END);
        fwrite(&partidaHist, sizeof(Historico), 1, historico);    
        fclose(historico);
    }

    
    if (cartas_empate_jogador != NULL)
    {
        free(cartas_empate_jogador);
    }
    if (cartas_empate_cpu != NULL)
    {
        free(cartas_empate_cpu);
    }

    return;
}