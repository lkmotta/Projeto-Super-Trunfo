/**
 * @file interface.c
 * @authors @lkmotta, @Dirceu06, @boonfim
 * @brief Implementacao das funcoes de interface grafica do jogo
 *
 * LICENSE: MIT
 * @copyright Copyright (c) 2025
 * 
 */

#include "interface.h"
#include "funcaux.h"
#include "filechange.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // necessária para srand
#include "raylib.h"

/**
 * @brief Carrega a imagem da carta, redimensiona para o display da carta e carrega a textura.
 * Retorna true(1) se deu certo, false(0) caso contrario.
 * 
 * @param carta 
 * @param textura_carta 
 * @return bool
 */
bool carregando_imagem_carta(Cartas carta, Texture2D *textura_carta){
    Image carta_imagem = LoadImage(carta.textura);
    if(carta_imagem.data == NULL){
        printf("\n\033[1;91mErro ao carregar imagem da carta %s\033[m\n", carta.nome);
        UnloadImage(carta_imagem);
        return false;
    }

    ImageResize(&carta_imagem, 300, 400); // redimensionando imagem para o tamanho do display da carta
    *textura_carta = LoadTextureFromImage(carta_imagem);
    UnloadImage(carta_imagem);
    return true;
}

void interface(Cartas *cartas, int size_cartas, int quant_cartas_baralho)
{
    GameState estadoAtual = CARREGANDO_TELA_INICIAL; // carregando tela inicial primeiro
    Cartas *baralho_jogador = NULL, *baralho_cpu = NULL;
    bool player_joga = true;             // booleano -> 0 = cpu joga, 1 = player joga
    int quem_ganhou = -1;                // 0 = cpu, 1 = player, -1 = empate/ainda nao definido
    int qual_tela = -1;                  // -1 = tela inicial, 1 = tela playerwin, 0 = tela cpuwin
    bool tela_inicial = true;            // booleano -> 0 = tela inicial, 1 = continuar jogo
    const char *atributos[] = {"Força", "Habilidade", "Velocidade", "Poderes", "Poder Cura"};
    int atributo = 0;                    // atributo escolhido (indice)
    const char *atributo_nome = "Força"; // nome do atributo escolhido (forca por padrao)
    bool maior_menor = true;             // booleano -> 0 = menor, 1 = maior
    bool maior_menor_selecionado = true; // booleano -> 0 = menor selecionado, 1 = maior selecionado
    int quant_cartas_jogador = quant_cartas_baralho, quant_cartas_cpu = quant_cartas_baralho;
    Cartas carta_jogador, carta_cpu;

    int rodada = 1;
    Cartas *cartas_empate_jogador = NULL;
    Cartas *cartas_empate_cpu = NULL;
    int quant_cartas_empate = 0;

    // estatisticas
    int vitorias = 0, empates = 0, pontuacao = 0;
    Historico partidaHist;

    // variáveis das telas (para evitar redeclarar no loop)
    
    bool ganhou_a=0;
    bool veio_tela_inicial = false;
    static bool fundo_ranking_carregado = false;
    static bool fundo_regras_carregado = false;
    static bool texto_regras_carregado = false;
    char nickname[4] = "";
    char texto[10];
    int texto_largura, texto_altura, nome_largura;
    int tempo_contagem_regressiva, tempo_agora,tempo_inicial;
    int carta_x, carta_y, circulo_x, circulo_y, raio_circulo;
    int atributo_x = 0, atributo_y = 0;
    static float rgb = 0.0f;
    bool tentou_carregarPlayer = false, carregouPlayer = false;
    bool tentou_carregarCPU = false, carregouCPU = false;
    char informacao_rodada[30];
    Texture2D textura_fundo;
    Texture2D textura_carta_jogador, textura_carta_cpu;
    static Texture2D fundo_regras;
    static Texture2D texto_regras;
    static Texture2D fundo_ranking;
    Rectangle retangulo_fundo_botoes;
    bool voltar=false;
    
    Color cor, cor_destaque_telainicial = COR_DESTAQUE_TELAINICIAL;
    Image icon = LoadImage("assets/img/icons/windowicon.png");
    
    InitWindow(800, 600, "Projeto Super-Trunfo"); // iniciando a janela
    SetWindowIcon(icon);                                             // definindo icone da janela
    SetTargetFPS(FPS);                                               // definindo a taxa de quadros por segundo

    // carregando musica de fundo
    InitAudioDevice();
    Music musica_fundo = LoadMusicStream("assets/sounds/supermanepic.mp3");
    Music musica_fundo1 = LoadMusicStream("assets/sounds/shouldstayshouldgo.mp3");
    Music musica_vitoria = LoadMusicStream("assets/sounds/timmarionaoquerodinheiro.mp3");
    Music musica_derrota = LoadMusicStream("assets/sounds/sadmusicmario.mp3");

    Music musica_atual = musica_fundo;
    PlayMusicStream(musica_atual); 
    SetMusicVolume(musica_atual, 0.2);
    SetMasterVolume(0.7);

    // carregando audio de atributos e de confirmação/maior/menor
    Sound som_atributos = LoadSound("assets/sounds/atributos.mp3");
    Sound som_resto = LoadSound("assets/sounds/resto.mp3");
    SetSoundVolume(som_resto, 0.6);
    Sound som_tecla = LoadSound("assets/sounds/tecla.mp3");

    //colando opções de tela cheia e redimensionamento entre 1920x1080 e 800x600
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_ALWAYS_RUN);
    SetWindowMinSize(800, 600);
    SetWindowMaxSize(1920, 1080);
    //mudar de tamanho com f11
    // ToggleFullscreen();


    while (!WindowShouldClose()){
        UpdateMusicStream(musica_atual);
        BeginDrawing();
        ClearBackground(COR_FUNDO);

        switch (estadoAtual){
            case CARREGANDO_TELA_INICIAL: {
                if(qual_tela == 0){
                    textura_fundo = LoadTexture("assets/img/telas/tela_inicial_cpuwin(800x600).png");
                    cor_destaque_telainicial = COR_DESTAQUE_TELAINICIAL_CPUWIN;
                }else if(qual_tela == 1){
                    textura_fundo = LoadTexture("assets/img/telas/tela_inicial_playerwin(800x600).png");
                    cor_destaque_telainicial = COR_DESTAQUE_TELAINICIAL_PLAYERWIN;
                }else if(qual_tela == -1){
                    textura_fundo = LoadTexture("assets/img/telas/tela_inicial(800x600).png");
                    cor_destaque_telainicial = COR_DESTAQUE_TELAINICIAL;
                }

                retangulo_fundo_botoes.x = (SCREEN_WIDTH - retangulo_fundo_botoes.width) / 2, retangulo_fundo_botoes.y = SCREEN_HEIGHT - retangulo_fundo_botoes.height - 20;
                retangulo_fundo_botoes.width = 700, retangulo_fundo_botoes.height = 80;
                
                if(tela_inicial || voltar) estadoAtual = TELA_INICIAL;
                else estadoAtual = NOVO_BARALHO;
                break;
            }

            case TELA_INICIAL: {
                DrawTexture(textura_fundo, 0, 0, WHITE); // inserido imagem de fundo

                // inserindo retangulo de fundo dos botões com opacidade 80%
                DrawRectangleRounded(retangulo_fundo_botoes, 0.2, 0, Fade(BLACK, 0.8f));

                // inserindo botões dentro do retangulo de fundo
                Rectangle botoes[] = {
                    {retangulo_fundo_botoes.x + 20, retangulo_fundo_botoes.y + 20, (retangulo_fundo_botoes.width - 60) / 4, retangulo_fundo_botoes.height - 40},
                    {retangulo_fundo_botoes.x + 20 + (retangulo_fundo_botoes.width - 60) / 4 + 5, retangulo_fundo_botoes.y + 20, (retangulo_fundo_botoes.width - 60) / 4, retangulo_fundo_botoes.height - 40},
                    {retangulo_fundo_botoes.x + 20 + 2 * (retangulo_fundo_botoes.width - 60) / 4 + 10, retangulo_fundo_botoes.y + 20, (retangulo_fundo_botoes.width - 60) / 4, retangulo_fundo_botoes.height - 40},
                    {retangulo_fundo_botoes.x + 20 + 3 * (retangulo_fundo_botoes.width - 60) / 4 + 15, retangulo_fundo_botoes.y + 20, (retangulo_fundo_botoes.width - 60) / 4, retangulo_fundo_botoes.height - 40}
                };

                // Definindo as cores para cada botão
                Color cores_botoes[] = {Fade(GREEN, 0.5f), Fade(BLUE, 0.5f), Fade(GOLD, 0.5f), Fade(RED, 0.5f)};

                for (int i = 0; i < 4; i++) {
                    if (CheckCollisionPointRec(GetMousePosition(), botoes[i])) {
                        DrawRectangleRec(botoes[i], cores_botoes[i]);
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            if (i == 0) {
                                estadoAtual = NOVO_BARALHO;
                            } else if (i == 1) {
                                estadoAtual = RANKING;
                                veio_tela_inicial = true;
                            } else if (i == 2) {
                                estadoAtual = REGRAS;
                            } else if (i == 3) {
                                estadoAtual = FECHAR_JANELA;
                            }
                        }
                    } else {
                        DrawRectangleRec(botoes[i], Fade(DARKGRAY, 0.5f));
                    }
                    if (i == 0) {
                        DrawText("JOGAR", botoes[i].x + botoes[i].width / 2 - MeasureText("JOGAR", 20) / 2, botoes[i].y + botoes[i].height / 2 - 10, 20, RAYWHITE);
                    } else if (i == 1) {
                        DrawText("RANKING", botoes[i].x + botoes[i].width / 2 - MeasureText("RANKING", 20) / 2, botoes[i].y + botoes[i].height / 2 - 10, 20, RAYWHITE);
                    } else if (i == 2) {
                        DrawText("REGRAS", botoes[i].x + botoes[i].width / 2 - MeasureText("REGRAS", 20) / 2, botoes[i].y + botoes[i].height / 2 - 10, 20, RAYWHITE);
                    } else if (i == 3) {
                        DrawText("SAIR", botoes[i].x + botoes[i].width / 2 - MeasureText("SAIR", 20) / 2, botoes[i].y + botoes[i].height / 2 - 10, 20, RAYWHITE);
                    }
                }

                break;
            }
           
            case RESET: {
                // RESETANDO AS VARIAVEIS
                fundo_regras_carregado = false;
                texto_regras_carregado = false;
                fundo_ranking_carregado = false;
                veio_tela_inicial=false;
                player_joga = true;
                strcpy(nickname, "");
                tentou_carregarPlayer = false, carregouPlayer = false;
                tentou_carregarCPU = false, carregouCPU = false;
                quem_ganhou = -1, atributo = 0;
                atributo_nome = "Força";
                maior_menor = true, maior_menor_selecionado = true;
                quant_cartas_jogador = quant_cartas_baralho, quant_cartas_cpu = quant_cartas_baralho;
                rodada = 1, vitorias = 0, empates = 0, pontuacao = 0;
                
                quant_cartas_empate = 0;
                if (cartas_empate_jogador != NULL) {
                    free(cartas_empate_jogador);
                    cartas_empate_jogador = NULL;
                }
                if (cartas_empate_cpu != NULL) {
                    free(cartas_empate_cpu);
                    cartas_empate_cpu = NULL;
                }
                estadoAtual = CARREGANDO_TELA_INICIAL;
                break;
            }

            case NOVO_BARALHO: {
                int *cartas_usadas = NULL;
                int size_cartas_usadas = 0;

                baralho_jogador = gerar_baralho(cartas, size_cartas, quant_cartas_baralho, &size_cartas_usadas, &cartas_usadas);
                baralho_cpu = gerar_baralho(cartas, size_cartas, quant_cartas_baralho, &size_cartas_usadas, &cartas_usadas);
                
                /* printf("\nCartas jogador:\n");
                listar_cartas(baralho_jogador, quant_cartas_baralho);
                printf("\nCartas CPU:\n");
                listar_cartas(baralho_cpu, quant_cartas_baralho); */
                free(cartas_usadas);

                estadoAtual = TELA_CARREGAMENTO;
                break;
            }

            case TELA_CARREGAMENTO: {
                int largura_barra = 400;
                int altura_barra = 30;
                int pos_x_barra = (SCREEN_WIDTH - largura_barra) / 2;
                int pos_y_barra = SCREEN_HEIGHT - 100;
                int progresso = 0;
                int tempo_carregamento = GetRandomValue(2, 3) * FPS; // de 2 a 3 segundos
                Rectangle barra_progresso = {pos_x_barra, pos_y_barra, (largura_barra * progresso) / tempo_carregamento, altura_barra};

                while (progresso < tempo_carregamento && !WindowShouldClose())
                {
                    UpdateMusicStream(musica_atual);
                    progresso++;
                    BeginDrawing();

                    DrawTexture(textura_fundo, 0, 0, WHITE); // inserido imagem de fundo
                    DrawText("Embaralhando...", SCREEN_WIDTH / 2 - MeasureText("Embaralhando...", 20) / 2, pos_y_barra - 40, 20, WHITE);

                    DrawRectangleRounded(barra_progresso, 0.3, 0, cor_destaque_telainicial);  // barra de progresso
                    barra_progresso.width = (largura_barra * progresso) / tempo_carregamento; // incrementando barra de progresso

                    EndDrawing();
                }
                estadoAtual = TELA_PLAYER_ESCOLHENDO_ATRIBUTO;
                break;
            }

            case TELA_PLAYER_ESCOLHENDO_ATRIBUTO: {
                carta_jogador = baralho_jogador[0];
                carta_cpu = baralho_cpu[0];

                tempo_inicial = GetTime();

                maior_menor=maior_menor_selecionado;

                // PLACAR 
                DrawText(TextFormat("Jogador:%10d", quant_cartas_jogador), 10, 10, 20, BLUE);
                DrawText(TextFormat("CPU:%15d", quant_cartas_cpu), 10, 40, 20, RED);

                // ------------------------------------- CARTA JOGADOR -------------------------------------
                carta_x = (SCREEN_WIDTH - 300) / 2;
                carta_y = (SCREEN_HEIGHT - 400) / 2;

                
                // CARTA
                Rectangle rec= {carta_x-5, carta_y-5, 305, 420};
                if(!tentou_carregarPlayer){
                    tentou_carregarPlayer = true;
                    carregouPlayer = carregando_imagem_carta(carta_jogador, &textura_carta_jogador);
                }

                if(carregouPlayer){
                    DrawTexture(textura_carta_jogador, carta_x, carta_y + 10, WHITE);
                }else{
                    DrawRectangleRounded(rec, 0.1, 10, LIGHTGRAY);
                }

                if(carta_jogador.super_trunfo){ // se a carta for super-trunfo, terá efeito RGB na borda
                    rgb += 0.005f;
                    if (rgb > 1.0f) rgb = 0.0f;
                    cor = ColorFromHSV(rgb * 360.0f, 1.0f, 1.0f);
                    DrawRectangleRoundedLinesEx(rec, 0.1, 10, 3.0, cor);
                }else{
                    DrawRectangleRoundedLinesEx(rec, 0.1, 10, 3.0, cor_destaque_telainicial);
                }

                // ------------------------------------- ID CARTA -------------------------------------
                raio_circulo = 20;
                circulo_x = carta_x + raio_circulo + 5;
                circulo_y = carta_y + raio_circulo + 5;

                if (carta_jogador.super_trunfo){
                    DrawCircle(circulo_x, circulo_y, raio_circulo+3.5, BLACK);
                    DrawCircle(circulo_x, circulo_y, raio_circulo, LIGHTGRAY);
                }else{
                    DrawCircle(circulo_x, circulo_y, raio_circulo+3.5, RED);
                    DrawCircle(circulo_x, circulo_y, raio_circulo, LIGHTGRAY);
                }
                
                // texto da letra e número da carta
                snprintf(texto, sizeof(texto), "%c%d", carta_jogador.letra, carta_jogador.num);
                texto_largura = MeasureText(texto, 20);
                texto_altura = 20;
                DrawText(texto, circulo_x - texto_largura / 2, circulo_y - texto_altura / 2, 20, BLACK);

                // NOME DA CARTA
                nome_largura = MeasureText(carta_jogador.nome, 20);
                DrawRectangleRounded((Rectangle){carta_x + (300 - nome_largura) / 2 - 5, carta_y + 10 - 5, nome_largura + 10, 30}, 0.3, 10, Fade(COR_FUNDO, 0.5f));
                DrawText(carta_jogador.nome, carta_x + (300 - nome_largura) / 2, carta_y + 10, 20, WHITE);

                // ------------------------------------- ATRIBUTOS DA CARTA -------------------------------------
                int valores[] = {carta_jogador.forca, carta_jogador.habilidade, carta_jogador.velocidade, carta_jogador.poderes, carta_jogador.poder_cura};
                for (int i = 0; i < 5; i++)
                {
                    atributo_x = carta_x + 10;
                    atributo_y = carta_y + 190 + i * 45;
                    Rectangle retangulo_atributo = {atributo_x, atributo_y, 200, 30};

                    if (CheckCollisionPointRec(GetMousePosition(), retangulo_atributo) || IsKeyDown(KEY_ONE + i))
                    { // verificando se o mouse esta em cima do atributo
                        DrawRectangle(atributo_x-2.5, atributo_y-2.5, 205, 35, BLACK);
                        DrawRectangleRec(retangulo_atributo, Fade(GRAY, 0.5f));

                        if (i == atributo){
                            DrawRectangle(atributo_x-2.5, atributo_y-2.5, 205, 35, BLACK);
                            DrawRectangleRec(retangulo_atributo, Fade(GRAY, 0.5f));
                        }
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ONE + i))
                        {                                                                                   // se o botao do mouse for clicado
                            
                            //DrawRectangleLines(atributo_x, atributo_y_selecionado_anterior, 200, 30, GRAY); // apagando a borda
                            DrawRectangle(atributo_x-2.5, atributo_y-2.5, 205, 35, BLACK);                  // desenhando a borda no novo atributo selecionado
                            atributo = i;                                                                   // indice do atributo escolhido
                            atributo_nome = atributos[i];                                                   // nome do atributo escolhido
                            PlaySound(som_atributos);
                        }
                    }
                    else
                    {
                        DrawRectangleRec(retangulo_atributo, Fade(DARKGRAY, 0.5f));
                        if (i == atributo){
                            DrawRectangle(atributo_x-2.5, atributo_y-2.5, 205, 35, RED);
                            DrawRectangleRec(retangulo_atributo, Fade(GRAY, 0.5f));
                        }
                    }
                    DrawText(atributos[i], atributo_x + 5, atributo_y + 5, 20, RAYWHITE);
                    DrawText(TextFormat("%d", valores[i]), atributo_x + 160, atributo_y + 5, 20, RAYWHITE);                              
                }

                int maior_x = SCREEN_WIDTH - 150, maior_y = SCREEN_HEIGHT - 180;
                int menor_x = SCREEN_WIDTH - 150, menor_y = SCREEN_HEIGHT - 130;
                Rectangle maiorRect = {maior_x, maior_y, 100, 30};
                Rectangle menorRect = {menor_x, menor_y, 100, 30};

                // ------------------------------------- BOTAO MAIOR -------------------------------------
                if (CheckCollisionPointRec(GetMousePosition(), maiorRect) || IsKeyDown(KEY_UP))
                {
                    DrawRectangle(maior_x-2, maior_y-2, 105, 35, RED);
                    DrawRectangleRec(maiorRect, GRAY);

                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_UP))
                    {
                        PlaySound(som_resto);
                        if (!maior_menor_selecionado)
                        {
                            DrawRectangleLines(menor_x, menor_y, 100, 30, GRAY); // apagando a borda do outro
                            maior_menor_selecionado = true;
                        }
                        maior_menor = true;
                    }
                }
                else
                {
                    DrawRectangleRec(maiorRect, GRAY);
                    if (maior_menor_selecionado){
                        DrawRectangle(maior_x-2, maior_y-2, 105, 35, RED);
                        DrawRectangleRec(maiorRect, GRAY);
                    }
                }
                DrawText("Maior", SCREEN_WIDTH - 140, SCREEN_HEIGHT - 175, 20, RAYWHITE);

                // ------------------------------------- BOTAO MENOR -------------------------------------
                if (CheckCollisionPointRec(GetMousePosition(), menorRect) || IsKeyDown(KEY_DOWN))
                {
                    DrawRectangle(menor_x-2, menor_y-2, 105, 35, BLUE);
                    DrawRectangleRec(menorRect, GRAY);

                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_DOWN))
                    {
                        if (maior_menor_selecionado)
                        {
                            DrawRectangleLines(maior_x, maior_y, 100, 30, GRAY); // apagando a borda do outro
                            maior_menor_selecionado = false;
                        }
                        PlaySound(som_resto);
                        maior_menor = false;
                    }
                }else{
                    DrawRectangleRec(menorRect, GRAY);
                    if (!maior_menor_selecionado){
                        DrawRectangle(menor_x-2, menor_y-2, 105, 35, BLUE);
                        DrawRectangleRec(menorRect, GRAY);
                    }
                }
                DrawText("Menor", SCREEN_WIDTH - 140, SCREEN_HEIGHT - 125, 20, RAYWHITE);

                // ------------------------------------- BOTAO JOGAR -------------------------------------
                Rectangle retangulo_jogar = {SCREEN_WIDTH - 150, SCREEN_HEIGHT - 80, 100, 50};
                if (CheckCollisionPointRec(GetMousePosition(), retangulo_jogar) || IsKeyDown(KEY_ENTER) ){ // se o mouse tiver em cima do botao
                    DrawRectangleRec(retangulo_jogar, DARKGREEN);
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER) ){ // se o botao for clicado
                        PlaySound(som_resto);
                        atributo+=1; // o enum dos atributos começa com 1
                        estadoAtual = VERIFICANDO_GANHADOR;
                        tentou_carregarPlayer = false, carregouPlayer = false;
                    }
                }
                else{
                    DrawRectangleRec(retangulo_jogar, (Color){79, 79, 79, 255});
                }
                DrawText("Jogar", SCREEN_WIDTH - 130, SCREEN_HEIGHT - 65, 20, RAYWHITE);
                DrawText(TextFormat("Rodada: %d", rodada), 10, SCREEN_HEIGHT - 30, 20, RAYWHITE);

                // ------------------------------------- PLAYER DE MUSICA -------------------------------------
                Rectangle botao_serio = {10, SCREEN_HEIGHT / 2 - 20, 100, 30};
                Rectangle botao_legal = {120, SCREEN_HEIGHT / 2 - 20, 100, 30};
                static int botao_serio_pressionado = 1;
                static int botao_legal_pressionado = 0;

                //DrawText("Musica", botao_serio.x + 10, botao_serio.y-25, 20, WHITE);

                if (CheckCollisionPointRec(GetMousePosition(), botao_serio) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    PlaySound(som_resto);
                    StopMusicStream(musica_fundo1);
                    PlayMusicStream(musica_fundo);
                    botao_serio_pressionado = 1;
                    botao_legal_pressionado = 0;
                    musica_atual = musica_fundo;
                }
                if (CheckCollisionPointRec(GetMousePosition(), botao_legal) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    PlaySound(som_resto);
                    StopMusicStream(musica_fundo);
                    PlayMusicStream(musica_fundo1);
                    botao_legal_pressionado = 1;
                    botao_serio_pressionado = 0;
                    musica_atual = musica_fundo1;
                }

                // inserindo um retangulo arredondado atras dos botoes
                DrawRectangleRounded((Rectangle){5, SCREEN_HEIGHT / 2 - 35, 220, 90}, 0.3, 10, DARKGRAY);

                if (botao_serio_pressionado) {
                    DrawRectangleRounded((Rectangle){botao_serio.x - 2.5, botao_serio.y - 2.5, botao_serio.width + 5, botao_serio.height + 5}, 0.3, 10, BLUE);
                    DrawRectangleRounded(botao_serio, 0.3, 10, GRAY);
                    DrawText("Serio", botao_serio.x + 10, botao_serio.y + 5, 20, WHITE);

                    DrawRectangleRounded(botao_legal, 0.3, 10, GRAY);
                    DrawText("Legal", botao_legal.x + 10, botao_legal.y + 5, 20, WHITE);
                }
                if(botao_legal_pressionado) {
                    DrawRectangleRounded((Rectangle){botao_legal.x - 2.5, botao_legal.y - 2.5, botao_legal.width + 5, botao_legal.height + 5}, 0.3, 10, RED);
                    DrawRectangleRounded(botao_legal, 0.3, 10, GRAY);
                    DrawText("Legal", botao_legal.x + 10, botao_legal.y + 5, 20, WHITE);

                    DrawRectangleRounded(botao_serio, 0.3, 10, GRAY);
                    DrawText("Serio", botao_serio.x + 10, botao_serio.y + 5, 20, WHITE);
                }

                DrawRectangleRounded(botao_serio, 0.3, 10, GRAY);
                DrawText("Serio", botao_serio.x + 10, botao_serio.y + 5, 20, WHITE);

                DrawRectangleRounded(botao_legal, 0.3, 10, GRAY);
                DrawText("Legal", botao_legal.x + 10, botao_legal.y + 5, 20, WHITE);

                // ------------------------------------- CONTROLE DO VOLUME -------------------------------------
                int volume_x = 10;
                int volume_y = SCREEN_HEIGHT / 2 + 20;
                int volume_width = 210;
                int volume_height = 20;
                static float volume = 0.2;
                Rectangle volume_bar = {volume_x, volume_y, volume_width, volume_height};
                SetMusicVolume(musica_atual, volume);

                DrawRectangleRounded(volume_bar, 0.3, 10, LIGHTGRAY);

                int limitador = 2.4; //operando com um pouco menos da metade da capacidade valor padrao: 2.4
                DrawRectangle(volume_x, volume_y, volume_width * volume*limitador, volume_height, GREEN);

                int volume_circle_x = volume_x + volume_width * volume*limitador;
                int volume_circle_y = volume_y + volume_height / 2;
                int volume_circle_radius = 10;

                DrawCircle(volume_circle_x, volume_circle_y, volume_circle_radius, DARKGREEN);

                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), volume_bar)) {
                    volume = (GetMouseX() - volume_x) / (float)(volume_width*limitador);
                    if (volume < 0.0f) volume = 0.0f;
                    if (volume > 1.0f) volume = 0.5f;
                    SetMusicVolume(musica_fundo, volume);
                    SetMusicVolume(musica_fundo1, volume);
                }


                //----------------------------- BOTÃO VOLTAR --------------------------
                Rectangle retangulo_voltar = {690, 10, 100, 50};
                DrawRectangleRounded(retangulo_voltar, 0.3, 10, GRAY);
                DrawText("Voltar", 710, 25, 20, WHITE);
                if(CheckCollisionPointRec(GetMousePosition(), retangulo_voltar)){
                    DrawRectangleRounded(retangulo_voltar, 0.3, 10, DARKGRAY);
                    DrawText("Voltar", 710, 25, 20, WHITE);
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        PlaySound(som_tecla);
                        estadoAtual = TEM_CERTEZA;
                    }
                }

                break;
            }

            case TEM_CERTEZA: {
                Rectangle retangulo_sim = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 100, 50};
                Rectangle retangulo_nao = {SCREEN_WIDTH / 2 + 10, SCREEN_HEIGHT / 2 - 50, 100, 50};
                DrawText("Tem certeza que deseja voltar?", SCREEN_WIDTH / 2 - MeasureText("Tem certeza que deseja voltar?", 35) / 2, SCREEN_HEIGHT / 2 - 100, 35, RAYWHITE);
                DrawRectangleRounded(retangulo_sim, 0.3, 10, GREEN);
                DrawRectangleRounded(retangulo_nao, 0.3, 10, RED);
                DrawText("Sim", retangulo_sim.x + 35, retangulo_sim.y + 15, 20, WHITE);
                DrawText("Não", retangulo_nao.x + 30, retangulo_nao.y + 15, 20, WHITE);
                if(CheckCollisionPointRec(GetMousePosition(), retangulo_sim)){
                    DrawRectangleRounded(retangulo_sim, 0.3, 10, DARKGREEN);
                    DrawText("Sim", retangulo_sim.x + 35, retangulo_sim.y + 15, 20, WHITE);
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        PlaySound(som_tecla);
                        voltar = true;
                        estadoAtual = RESET;
                    }
                }
                Color DARKRED = (Color){139, 0, 0, 255};
                if (CheckCollisionPointRec(GetMousePosition(), retangulo_nao)){
                    DrawRectangleRounded(retangulo_nao, 0.3, 10, DARKRED);
                    DrawText("Não", retangulo_nao.x + 30, retangulo_nao.y + 15, 20, WHITE);
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        PlaySound(som_tecla);
                        estadoAtual = TELA_PLAYER_ESCOLHENDO_ATRIBUTO;
                    }
                }
                
                break;
            }

            case TELA_CPU_ESCOLHENDO_ATRIBUTO: {
                carta_jogador = baralho_jogador[0];
                carta_cpu = baralho_cpu[0];

                srand(time(NULL));
                atributo = rand() % 5 + 1; // +1 para nao pegar 0
                atributo_nome = atributos[atributo - 1];
                maior_menor = rand() % 2;

                // definindo variaveis
                int progresso_cpu = 0;
                int tempo_carregamento_cpu = GetRandomValue(1, 2) * FPS; // de 1 a 2 segundos
                int largura_barra_cpu = 400;
                int altura_barra_cpu = 20;
                int pos_x_barra_cpu = (SCREEN_WIDTH - largura_barra_cpu) / 2;
                int pos_y_barra_cpu = 550;
                Rectangle barra_progresso_cpu = { pos_x_barra_cpu, pos_y_barra_cpu, 0, altura_barra_cpu };

                while (progresso_cpu < tempo_carregamento_cpu && !WindowShouldClose()) {
                    UpdateMusicStream(musica_atual);
                    progresso_cpu++;

                    BeginDrawing();
                    ClearBackground((Color){30, 30, 30, 255}); // fundo da janela

                    DrawText("Vez da CPU", (SCREEN_WIDTH - 120) - MeasureText("Vez da CPU", 40) / 2, 20, 35, RED);

                    DrawText("CPU escolhendo atributo...", (SCREEN_WIDTH / 2) - MeasureText("CPU escolhendo atributo...", 20) / 2, pos_y_barra_cpu - 30, 20, RAYWHITE);

                    // BARRA DE PROGRESSO
                    barra_progresso_cpu.width = (largura_barra_cpu * progresso_cpu) / tempo_carregamento_cpu; // incrementando barra de progresso
                    DrawRectangleRounded(barra_progresso_cpu, 0.3, 0, (Color){251, 59, 0, 255});  // barra de progresso

                    // PLACAR
                    DrawText(TextFormat("Jogador:%10d", quant_cartas_jogador), 10, 10, 20, BLUE);
                    DrawText(TextFormat("CPU:%15d", quant_cartas_cpu), 10, 40, 20, RED);

                    // ------------------------------------- CARTA JOGADOR -------------------------------------
                    carta_x = (SCREEN_WIDTH - 300) / 2;
                    carta_y = (SCREEN_HEIGHT - 400) / 2;

                    Rectangle rec = {carta_x-5, carta_y-5, 305, 420};
                    
                    if(!tentou_carregarPlayer){
                        tentou_carregarPlayer = true;
                        carregouPlayer = carregando_imagem_carta(carta_jogador, &textura_carta_jogador);
                    }

                    if(carregouPlayer){
                        DrawTexture(textura_carta_jogador, carta_x, carta_y + 10, WHITE);
                    }else{
                        DrawRectangleRounded(rec, 0.1, 10, LIGHTGRAY);
                    }

                    if(carta_jogador.super_trunfo){ // se a carta for super-trunfo, terá efeito RGB na borda
                        rgb += 0.005f;
                        if (rgb > 1.0f) rgb = 0.0f;
                        cor = ColorFromHSV(rgb * 360.0f, 1.0f, 1.0f);
                        DrawRectangleRoundedLinesEx(rec, 0.1, 10, 3.0, cor);
                    }else{
                        DrawRectangleRoundedLinesEx(rec, 0.1, 10, 3.0, cor_destaque_telainicial);
                    }

                    // ------------------------------------- ID CARTA -------------------------------------
                    raio_circulo = 20;
                    circulo_x = carta_x + raio_circulo + 5;
                    circulo_y = carta_y + raio_circulo + 5;

                    // CIRCULO COM A BORDA CERTA
                    if (carta_jogador.super_trunfo){
                        DrawCircle(circulo_x, circulo_y, raio_circulo+3.5, BLACK);
                        DrawCircle(circulo_x, circulo_y, raio_circulo, LIGHTGRAY);
                    }else{
                        DrawCircle(circulo_x, circulo_y, raio_circulo+3.5, RED);
                        DrawCircle(circulo_x, circulo_y, raio_circulo, LIGHTGRAY);
                    }

                    // Texto da letra e número da carta
                    snprintf(texto, sizeof(texto), "%c%d", carta_jogador.letra, carta_jogador.num);
                    texto_largura = MeasureText(texto, 20);
                    texto_altura = 20;
                    DrawText(texto, circulo_x - texto_largura/2, circulo_y - texto_altura/2, 20, BLACK);

                    nome_largura = MeasureText(carta_jogador.nome, 20);
                    DrawRectangleRounded((Rectangle){carta_x + (300 - nome_largura) / 2 - 5, carta_y + 10 - 5, nome_largura + 10, 30}, 0.3, 10, Fade(COR_FUNDO, 0.5f));
                    DrawText(carta_jogador.nome, carta_x + (300 - nome_largura) / 2, carta_y + 10, 20, WHITE);

                    // ------------------------------------- ATRIBUTOS DA CARTA -------------------------------------
                    int valores[] = {carta_jogador.forca, carta_jogador.habilidade, carta_jogador.velocidade, carta_jogador.poderes, carta_jogador.poder_cura};

                    for (int i = 0; i < 5; i++) {
                        Rectangle retangulo_atributo = {carta_x + 10, carta_y + 190 + i * 45, 200, 30};
                        DrawRectangleRec(retangulo_atributo, Fade(DARKGRAY, 0.5f));
                        DrawText(atributos[i], carta_x + 20, carta_y + 195 + i * 45, 20, RAYWHITE);
                        DrawText(TextFormat("%d", valores[i]), carta_x + 180, carta_y + 195 + i * 45, 20, RAYWHITE);
                    }
                    //rodadas
                    DrawText(TextFormat("Rodada: %d", rodada), 10, SCREEN_HEIGHT - 30, 20, RAYWHITE);

                    EndDrawing();
                }

                tempo_inicial = GetTime();
                estadoAtual = VERIFICANDO_GANHADOR;
                tentou_carregarPlayer = false, carregouPlayer = false;
                break;
            }

            case VERIFICANDO_GANHADOR: {
                if(carta_jogador.super_trunfo || carta_cpu.super_trunfo){ // se uma das cartas for super-trunfo
                    if(carta_jogador.super_trunfo && carta_cpu.letra != 'A'){
                        quem_ganhou = 1; // jogador ganhou
                        vitorias++;
                        rodada++;
                    }else if(carta_cpu.super_trunfo && carta_jogador.letra == 'A'){
                        quem_ganhou = 1; // jogador ganhou
                        vitorias++;
                        rodada++;
                        ganhou_a = true;
                    }else if(carta_cpu.super_trunfo && carta_jogador.letra != 'A'){
                        quem_ganhou = 0; // CPU ganhou
                        rodada++;
                    }else if(carta_jogador.super_trunfo && carta_cpu.letra == 'A'){
                        quem_ganhou = 0; // CPU ganhou
                        rodada++;
                        ganhou_a = true;
                    }
                    
                }else{
                    if(maior_menor) quem_ganhou = verifica_maior(&carta_jogador, &carta_cpu, atributo);
                    else quem_ganhou = verifica_menor(&carta_jogador, &carta_cpu, atributo);
                }

                if(quem_ganhou == -1){ // empate
                    //printf("\nEMPATE\n");
                    empates++;
                    lidar_com_empate(&baralho_jogador, &quant_cartas_jogador, &baralho_cpu, &quant_cartas_cpu, &cartas_empate_jogador, &cartas_empate_cpu, &quant_cartas_empate);
                }else if(quem_ganhou){ // jogador ganhou
                    //printf("\nGANHOU\n");
                    vitorias++;
                    // adicionando as cartas de empate ao baralho do jogador
                    if(quant_cartas_empate > 0){
                        for (int i = 0; i < quant_cartas_empate; i++) {
                            adicionar_carta_vencedor(&baralho_jogador, &cartas_empate_jogador, &quant_cartas_jogador, &quant_cartas_empate);
                            adicionar_carta_vencedor(&baralho_jogador, &cartas_empate_cpu, &quant_cartas_jogador, &quant_cartas_empate);
                        }
                        quant_cartas_empate = 0;
                        if (cartas_empate_jogador != NULL) {
                            free(cartas_empate_jogador);
                            cartas_empate_jogador = NULL;
                        }
                        if (cartas_empate_cpu != NULL) {
                            free(cartas_empate_cpu);
                            cartas_empate_cpu = NULL;
                        }
                    }
                    adicionar_carta_vencedor(&baralho_jogador, &baralho_cpu, &quant_cartas_jogador, &quant_cartas_cpu);
                }else{                // CPU ganhou
                    //printf("\nPERDEU\n");
                    // adicionando as cartas de empate ao baralho do CPU
                    if(quant_cartas_empate > 0){
                        for (int i = 0; i < quant_cartas_empate; i++) {
                            adicionar_carta_vencedor(&baralho_cpu, &cartas_empate_jogador, &quant_cartas_cpu, &quant_cartas_empate);
                            adicionar_carta_vencedor(&baralho_cpu, &cartas_empate_cpu, &quant_cartas_cpu, &quant_cartas_empate);
                        }
                        quant_cartas_empate = 0;
                        if (cartas_empate_jogador != NULL) {
                            free(cartas_empate_jogador);
                            cartas_empate_jogador = NULL;
                        }
                        if (cartas_empate_cpu != NULL) {
                            free(cartas_empate_cpu);
                            cartas_empate_cpu = NULL;
                        }
                    }
                    adicionar_carta_vencedor(&baralho_cpu, &baralho_jogador, &quant_cartas_cpu, &quant_cartas_jogador);
                }
                
                atributo-=1; // voltando o valor do atributo para índice
                estadoAtual = TELA_RESULTADO;
                break;
            }

            case TELA_RESULTADO: {
                // ------------------------------------- CARTA DO JOGADOR -------------------------------------
                carta_x = (SCREEN_WIDTH - 700) / 2;
                carta_y = (SCREEN_HEIGHT - 500) / 2;

                DrawText("Sua carta", carta_x + 100, (SCREEN_HEIGHT - 550) / 2, 20, DARKGREEN);
                Rectangle rec = {carta_x-5, carta_y-5, 305, 420};
                
                if(!tentou_carregarPlayer){
                    tentou_carregarPlayer = true;
                    carregouPlayer = carregando_imagem_carta(carta_jogador, &textura_carta_jogador);
                }

                if(carregouPlayer){
                    DrawTexture(textura_carta_jogador, carta_x, carta_y + 10, WHITE);
                }else{
                    DrawRectangleRounded(rec, 0.1, 10, LIGHTGRAY);
                }

                if(carta_jogador.super_trunfo){ // se a carta for super-trunfo, terá efeito RGB na borda
                    rgb += 0.005f;
                    if (rgb > 1.0f) rgb = 0.0f;
                    cor = ColorFromHSV(rgb * 360.0f, 1.0f, 1.0f);
                    DrawRectangleRoundedLinesEx(rec, 0.1, 10, 3.0, cor);
                }else{
                    DrawRectangleRoundedLinesEx(rec, 0.1, 10, 3.0, cor_destaque_telainicial);
                }

                // ------------------------------------- ID CARTA JOGADOR -------------------------------------
                raio_circulo = 20;
                circulo_x = carta_x + raio_circulo + 5;
                circulo_y = carta_y + raio_circulo + 5;

                //CIRCULO COM A BORDA CERTA
                if (carta_jogador.super_trunfo){
                    DrawCircle(circulo_x, circulo_y, raio_circulo+3.5, BLACK);
                    DrawCircle(circulo_x, circulo_y, raio_circulo, LIGHTGRAY);
                }else{
                    DrawCircle(circulo_x, circulo_y, raio_circulo+3.5, RED);
                    DrawCircle(circulo_x, circulo_y, raio_circulo, LIGHTGRAY);
                }

                // texto da letra e numero da carta
                snprintf(texto, sizeof(texto), "%c%d", carta_jogador.letra, carta_jogador.num);
                texto_largura = MeasureText(texto, 20);
                texto_altura = 20;
                DrawText(texto, circulo_x - texto_largura/2, circulo_y - texto_altura/2, 20, BLACK);

                // NOME DA CARTA
                nome_largura = MeasureText(carta_jogador.nome, 20);
                DrawRectangleRounded((Rectangle){carta_x + (300 - nome_largura) / 2 - 5, carta_y + 10 - 5, nome_largura + 10, 30}, 0.3, 10, Fade(COR_FUNDO, 0.5f));
                DrawText(carta_jogador.nome, carta_x + (300 - nome_largura) / 2, carta_y + 10, 20, WHITE);

                // ------------------------------------- ATRIBUTOS DA CARTA JOGADOR -------------------------------------
                int valores[] = {carta_jogador.forca, carta_jogador.habilidade, carta_jogador.velocidade, carta_jogador.poderes, carta_jogador.poder_cura};

                for (int i = 0; i < 5; i++) {
                    UpdateMusicStream(musica_atual);

                    Rectangle retangulo_atributo = {carta_x + 10, carta_y + 190 + i * 45, 200, 30};

                    if (i == atributo) {
                        if (quem_ganhou == -1) {
                            DrawRectangle(carta_x + 10-2.5, carta_y + 190 + i * 45-2.5, 205, 35, YELLOW);
                            DrawRectangleRec(retangulo_atributo, DARKGRAY);
                        } else if (quem_ganhou == 0) {
                            DrawRectangleRec(retangulo_atributo, RED);
                        } else if (quem_ganhou == 1) {
                            DrawRectangleRec(retangulo_atributo, GREEN);
                        }
                    }else DrawRectangleRec(retangulo_atributo, Fade(DARKGRAY, 0.4f));

                    DrawText(atributos[i], carta_x + 20, carta_y + 195 + i * 45, 20, RAYWHITE);
                    DrawText(TextFormat("%d", valores[i]), carta_x + 180, carta_y + 195 + i * 45, 20, RAYWHITE);
                }

                // DESENHANDO UM X ENTRE AS DUAS CARTAS
                DrawText("x", (SCREEN_WIDTH - MeasureText("X", 64)) / 2, (SCREEN_HEIGHT - 100 - MeasureText("X", 64)) / 2, 64, RAYWHITE);

                // --------------------------------------- CARTA DA CPU ----------------------------------------
                carta_x = (SCREEN_WIDTH - 350);
                carta_y = (SCREEN_HEIGHT - 500) / 2;
                rec.x = carta_x-5, rec.y = carta_y-5, rec.width = 305, rec.height = 420;
                
                DrawText("Carta CPU", carta_x + 100, (SCREEN_HEIGHT - 550) / 2, 20, (Color){ 156, 0, 0, 255 }); // #9C0000

                if(!tentou_carregarCPU){
                    tentou_carregarCPU = true;
                    carregouCPU = carregando_imagem_carta(carta_cpu, &textura_carta_cpu);
                }

                if(carregouCPU){
                    DrawTexture(textura_carta_cpu, carta_x, carta_y + 10, WHITE);
                }else{
                    DrawRectangleRounded(rec, 0.1, 10, LIGHTGRAY);
                }

                if(carta_cpu.super_trunfo){ // se a carta for super-trunfo, terá efeito RGB na borda
                    rgb += 0.005f;
                    if (rgb > 1.0f) rgb = 0.0f;
                    cor = ColorFromHSV(rgb * 360.0f, 1.0f, 1.0f);
                    DrawRectangleRoundedLinesEx(rec, 0.1, 10, 3.0, cor);
                }else{
                    DrawRectangleRoundedLinesEx(rec, 0.1, 10, 3.0, cor_destaque_telainicial);
                }

                // ------------------------------------- ID CARTA CPU -------------------------------------
                raio_circulo = 20;
                circulo_x = carta_x + raio_circulo + 5;
                circulo_y = carta_y + raio_circulo + 5;

                // CIRCULO COM A BORDA CERTA
                if (carta_cpu.super_trunfo){
                    DrawCircle(circulo_x, circulo_y, raio_circulo+3.5, BLACK);
                    DrawCircle(circulo_x, circulo_y, raio_circulo, LIGHTGRAY);
                }else{
                    DrawCircle(circulo_x, circulo_y, raio_circulo+3.5, RED);
                    DrawCircle(circulo_x, circulo_y, raio_circulo, LIGHTGRAY);
                }
                // texto da letra e numero da carta
                snprintf(texto, sizeof(texto), "%c%d", carta_cpu.letra, carta_cpu.num);
                texto_largura = MeasureText(texto, 20);
                texto_altura = 20;
                DrawText(texto, circulo_x - texto_largura/2, circulo_y - texto_altura/2, 20, BLACK);

                // NOME DA CARTA
                nome_largura = MeasureText(carta_cpu.nome, 20);
                DrawRectangleRounded((Rectangle){carta_x + (300 - nome_largura) / 2 - 5, carta_y + 10 - 5, nome_largura + 10, 30}, 0.3, 10, Fade(COR_FUNDO, 0.5f));
                DrawText(carta_cpu.nome, carta_x + (300 - nome_largura) / 2, carta_y + 10, 20, WHITE);

                // ------------------------------------- ATRIBUTOS DA CARTA CPU -------------------------------------
                int valores_cpu[] = {carta_cpu.forca, carta_cpu.habilidade, carta_cpu.velocidade, carta_cpu.poderes, carta_cpu.poder_cura};

                for (int i = 0; i < 5; i++) {
                    UpdateMusicStream(musica_atual);

                    Rectangle retangulo_atributo = {carta_x + 10, carta_y + 190 + i * 45, 200, 30};
                    if (i == atributo) {
                        if (quem_ganhou == -1) {
                            DrawRectangle(carta_x + 10-2.5, carta_y + 190 + i * 45-2.5, 205, 35, YELLOW);
                            DrawRectangleRec(retangulo_atributo, DARKGRAY);
                        } else if (quem_ganhou == 0) {
                            DrawRectangleRec(retangulo_atributo, GREEN);
                        } else if (quem_ganhou == 1) {
                            DrawRectangleRec(retangulo_atributo, RED);
                        }
                    }else DrawRectangleRec(retangulo_atributo, Fade(DARKGRAY, 0.4f));

                    DrawText(atributos[i], carta_x + 20, carta_y + 195 + i * 45, 20, RAYWHITE);
                    DrawText(TextFormat("%d", valores_cpu[i]), carta_x + 180, carta_y + 195 + i * 45, 20, RAYWHITE);
                }

                // --------------------------------------- BARRAS DE VIDA ----------------------------------------
                float proporcao_jogador = (float)quant_cartas_jogador / (2 * quant_cartas_baralho);
                float proporcao_cpu = (float)quant_cartas_cpu / (2 * quant_cartas_baralho);

                int largura_barra_dentro = 300;  // largura da barra de dentro
                int largura_maxima_barra = 280; // largura máxima da barra de vida

                // LARGURA DAS BARRAS baseada nas proporções
                int largura_barra_jogador = largura_maxima_barra * proporcao_jogador;
                int largura_barra_cpu = largura_maxima_barra * proporcao_cpu;

                DrawRectangle(50, 470, largura_barra_dentro, 20, WHITE); // barra de dentro
                DrawRectangle(60, 475, largura_barra_jogador, 10, GREEN); // barra do jogador

                DrawRectangle(SCREEN_WIDTH-350, 470, largura_barra_dentro, 20, WHITE); // barra de dentro                
                DrawRectangle(SCREEN_WIDTH-340, 475, largura_barra_cpu, 10, RED); // barra da CPU
                
                // TEXTO DAS BARRAS
                char texto_quant_jogador[20];
                char texto_quant_cpu[20];

                sprintf(texto_quant_jogador, "%d cartas", quant_cartas_jogador);
                sprintf(texto_quant_cpu, "%d cartas", quant_cartas_cpu);

                int texto_jogador_largura = MeasureText(texto_quant_jogador, 16);
                int texto_cpu_largura = MeasureText(texto_quant_cpu, 16);

                DrawText(texto_quant_jogador, 50 + (300 - texto_jogador_largura) / 2, 500, 16, RAYWHITE); // Texto do jogador
                DrawText(texto_quant_cpu, SCREEN_WIDTH - 350 + (300 - texto_cpu_largura) / 2, 500, 16, RAYWHITE); // Texto da CPU

                // RODADAS
                DrawText(TextFormat("Rodada: %d", rodada), 10, SCREEN_HEIGHT - 30, 20, RAYWHITE);

                // INFORMACOES DA COMPARACAO
                if((carta_cpu.super_trunfo || carta_jogador.super_trunfo) && ganhou_a){
                    DrawText("Classe A sobre Trunfo", SCREEN_WIDTH - 10 - MeasureText("Classe A sobre Trunfo", 20), SCREEN_HEIGHT - 30, 20, RAYWHITE);
                }else if((carta_cpu.super_trunfo || carta_jogador.super_trunfo) && !ganhou_a){
                    DrawText("Super-Trunfo", SCREEN_WIDTH - 10 - MeasureText("Super-Trunfo", 20), SCREEN_HEIGHT - 30, 20, RAYWHITE);
                }else{
                    sprintf(informacao_rodada, "%s - %s", atributo_nome, maior_menor ? "MAIOR" : "MENOR");
                    DrawText(informacao_rodada, SCREEN_WIDTH - 10 - MeasureText(informacao_rodada, 20), SCREEN_HEIGHT - 30, 20, RAYWHITE); 
                }

                // BOTAO OK
                Rectangle botaook = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - 80, 100, 50};
                Color botaocor = CheckCollisionPointRec(GetMousePosition(), botaook) ? DARKGREEN : GREEN;
                DrawRectangleRec(botaook, botaocor);
                DrawRectangleLines(botaook.x, botaook.y, botaook.width, botaook.height, WHITE);
                DrawText("OK", botaook.x + botaook.width / 2 - MeasureText("OK", 20) / 2, botaook.y + botaook.height / 2 - 10, 20, WHITE);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER)){
                    PlaySound(som_resto);
                    if (CheckCollisionPointRec(GetMousePosition(), botaook) || IsKeyPressed(KEY_ENTER)){
                        if(quant_cartas_cpu == 0) {
                            qual_tela = 1;
                            estadoAtual = JOGADOR_VENCEU;
                            break;
                        }else if(quant_cartas_jogador == 0){
                            qual_tela = 0;
                            estadoAtual = CPU_VENCEU;
                            break;
                        } else {
                            player_joga = !player_joga;
                            rodada++; // NOVA RODADA
                            ganhou_a=0;
                            tentou_carregarPlayer = false, carregouPlayer = false;
                            tentou_carregarCPU = false, carregouCPU = false;
                            if(player_joga) estadoAtual = TELA_PLAYER_ESCOLHENDO_ATRIBUTO;
                            else estadoAtual = TELA_CPU_ESCOLHENDO_ATRIBUTO;
                        }
                    }
                }
                
                tempo_contagem_regressiva = tempo_inicial + 30; // 30 segundos
                tempo_agora = GetTime();
                
                if (tempo_agora >= tempo_contagem_regressiva){
                    if(quant_cartas_cpu == 0) {
                        qual_tela = 1;
                        estadoAtual = JOGADOR_VENCEU;
                    }else if(quant_cartas_jogador == 0){
                        qual_tela = 0;
                        estadoAtual = CPU_VENCEU;
                    } else {
                        player_joga = !player_joga;
                        rodada++; // NOVA RODADA 
                        ganhou_a=0;
                        tentou_carregarPlayer = false, carregouPlayer = false;
                        tentou_carregarCPU = false, carregouCPU = false;
                        if(player_joga) estadoAtual = TELA_PLAYER_ESCOLHENDO_ATRIBUTO;
                        else estadoAtual = TELA_CPU_ESCOLHENDO_ATRIBUTO;
                    }
                }

                DrawText(TextFormat("%d", (tempo_contagem_regressiva - tempo_agora)), 
                    SCREEN_WIDTH / 2 - MeasureText(TextFormat("%d",(tempo_contagem_regressiva - tempo_agora)), 20) / 2,
                    botaook.y + botaook.height + 10, 20, WHITE);
                
                break;
            }

            case JOGADOR_VENCEU: {
                static bool ja_pousou = false;
                if (!ja_pousou) {
                    StopMusicStream(musica_atual);
                    musica_atual = musica_vitoria;
                    PlayMusicStream(musica_atual);
                    ja_pousou = true;
                }
                SetMusicVolume(musica_atual, 0.2);
        
                DrawText("Fim de Jogo", SCREEN_WIDTH / 2 - MeasureText("Fim de Jogo", 50) / 2, SCREEN_HEIGHT / 2 - 250, 50, WHITE);
                DrawText("Voce ganhou", SCREEN_WIDTH / 2 - MeasureText("Voce ganhou", 40) / 2, SCREEN_HEIGHT / 2 - 130, 40, GREEN);
                pontuacao = (10 * ((100 * vitorias) / rodada))+10*empates;
                DrawText(TextFormat("Rodadas: %d", rodada), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Rodadas: %d", rodada), 30) / 2, SCREEN_HEIGHT / 2 - 30, 30, WHITE);
                DrawText(TextFormat("Vitórias: %d", vitorias), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Vit�rias: %d", vitorias), 30) / 2, SCREEN_HEIGHT / 2, 30, WHITE);
                DrawText(TextFormat("Empates: %d", empates), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Empates: %d", empates), 30) / 2, SCREEN_HEIGHT / 2 + 30, 30, WHITE);
                DrawText(TextFormat("Pontuação: %d", pontuacao), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Pontua��o: %d", pontuacao), 30) / 2, SCREEN_HEIGHT / 2 + 60, 30, WHITE);
                // botao
                Rectangle botaook = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 180, 100, 50};
                Color botaocor = CheckCollisionPointRec(GetMousePosition(), botaook) ? DARKGREEN : GREEN;
                DrawRectangleRec(botaook, botaocor);
                DrawText("OK", botaook.x + botaook.width / 2 - MeasureText("OK", 20) / 2, botaook.y + botaook.height / 2 - 10, 20, WHITE);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER)){
                    if (CheckCollisionPointRec(GetMousePosition(), botaook) || IsKeyPressed(KEY_ENTER)){
                        PlaySound(som_resto);
                        estadoAtual = NICKNAME;
                        break;
                    }
                }

                strcpy(partidaHist.vencedor, "jogador");
                partidaHist.rodadas = rodada;
                partidaHist.vitorias = vitorias;
                partidaHist.empates = empates;
                partidaHist.pontuacao = pontuacao;

                //pegando a data atual usando time.h
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                partidaHist.dia=tm.tm_mday;
                partidaHist.mes=tm.tm_mon+1;

                break;
            }

            case CPU_VENCEU: {
                static bool ja_pousou = false;
                if (!ja_pousou) {
                    StopMusicStream(musica_atual);
                    musica_atual = musica_derrota;
                    PlayMusicStream(musica_atual);
                    ja_pousou = true;
                }
                SetMusicVolume(musica_atual, 0.2);

                DrawText("Fim de Jogo", SCREEN_WIDTH / 2 - MeasureText("Fim de Jogo", 50) / 2, SCREEN_HEIGHT / 2 - 250, 50, WHITE);
                DrawText("CPU ganhou", SCREEN_WIDTH / 2 - MeasureText("Voce ganhou", 40) / 2, SCREEN_HEIGHT / 2 - 130, 40, RED);
                pontuacao = 10 * ((100 * vitorias) / rodada);
                DrawText(TextFormat("Rodadas: %d", rodada), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Rodadas: %d", rodada), 30) / 2, SCREEN_HEIGHT / 2 - 30, 30, WHITE);
                DrawText(TextFormat("Vitórias: %d", vitorias), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Vitórias: %d", vitorias), 30) / 2, SCREEN_HEIGHT / 2, 30, WHITE);
                DrawText(TextFormat("Empates: %d", empates), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Empates: %d", empates), 30) / 2, SCREEN_HEIGHT / 2 + 30, 30, WHITE);
                DrawText(TextFormat("Pontuação: %d", pontuacao), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Pontuação: %d", pontuacao), 30) / 2, SCREEN_HEIGHT / 2 + 60, 30, WHITE);
                // botao
                Rectangle botaook = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 180, 100, 50};
                Color DARKRED = (Color){139, 0, 0, 255};
                Color botaocor = CheckCollisionPointRec(GetMousePosition(), botaook) ? DARKRED : RED;
                DrawRectangleRec(botaook, botaocor);
                DrawText("OK", botaook.x + botaook.width / 2 - MeasureText("OK", 20) / 2, botaook.y + botaook.height / 2 - 10, 20, WHITE);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER)){
                    if (CheckCollisionPointRec(GetMousePosition(), botaook) || IsKeyPressed(KEY_ENTER)){
                        PlaySound(som_resto);
                        estadoAtual = JOGAR_NOVAMENTE;
                        break;
                    }
                }

                strcpy(partidaHist.vencedor, "CPU");
                partidaHist.rodadas = rodada;
                partidaHist.vitorias = vitorias;
                partidaHist.empates = empates;
                partidaHist.pontuacao = pontuacao;
                //pegando a data atual usando time.h
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                partidaHist.dia=tm.tm_mday;
                partidaHist.mes=tm.tm_mon+1;

                break;
            }

            case NICKNAME: {
                DrawText("Digite seu nickname", SCREEN_WIDTH / 2 - MeasureText("Digite seu nickname", 50) / 2, SCREEN_HEIGHT / 2 - 80, 50, WHITE);
                DrawText("Nickname (3 letras):", SCREEN_WIDTH / 2 - MeasureText("Nickname (3 letras):", 30) / 2, SCREEN_HEIGHT / 2 - 20, 30, WHITE);

                //caixas da letra
                int larguracaixa = 50;
                int alturacaixa = 50;
                int comecoX = (SCREEN_WIDTH - (3 * larguracaixa + 20)) / 2;
                for (int i = 0; i < 3; i++) {
                    DrawRectangle(comecoX + i * (larguracaixa + 10), SCREEN_HEIGHT / 2 + 20, larguracaixa, alturacaixa, WHITE);
                    if (i < (int)strlen(nickname)) {
                        DrawText(TextFormat("%c", nickname[i]), comecoX + i * (larguracaixa + 10) + larguracaixa / 2 - MeasureText(TextFormat("%c", nickname[i]), 40) / 2, SCREEN_HEIGHT / 2 + 25, 40, BLACK);
                    }
                }

                int key = GetKeyPressed();
                if (key >= 32 && key <= 125 && strlen(nickname) < 3) {
                    PlaySound(som_tecla);
                    int len = strlen(nickname);
                    nickname[len] = (char)key;
                    nickname[len + 1] = '\0';
                }
                if (IsKeyPressed(KEY_BACKSPACE) && strlen(nickname) > 0) {
                    nickname[strlen(nickname) - 1] = '\0';
                }

                // botao ok
                Rectangle botaook = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 120, 100, 50};
                Color botaocor = CheckCollisionPointRec(GetMousePosition(), botaook) ? DARKGREEN : GREEN;
                DrawRectangleRec(botaook, botaocor);
                DrawText("OK", botaook.x + botaook.width / 2 - MeasureText("OK", 30) / 2, botaook.y + botaook.height / 2 - 15, 30, WHITE);

                static bool nickinvalido = false;
                static double nickinvalidoTempo = 0;

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER)){
                    if (CheckCollisionPointRec(GetMousePosition(), botaook) || IsKeyPressed(KEY_ENTER)){
                        PlaySound(som_resto);
                        if (strlen(nickname) < 3) {
                            nickinvalido = true;
                            nickinvalidoTempo = GetTime();
                        } else {
                            strncpy(partidaHist.vencedor, nickname, sizeof(partidaHist.vencedor) - 1);
                            partidaHist.vencedor[sizeof(partidaHist.vencedor) - 1] = '\0';

                            FILE *historico = fopen("assets/data/historico.dat", "rb+");
                            if (historico == NULL){
                                historico = fopen("assets/data/historico.dat", "wb+");
                                if (historico == NULL)
                                {
                                    perror("\n\033[1;91mErro ao criar historico.dat\033[m");
                                    exit(1);
                                }
                                fseek(historico, 0, SEEK_END);
                                fwrite(&partidaHist, sizeof(Historico), 1, historico);
                                fclose(historico);
                            }
                            else{
                                fseek(historico, 0, SEEK_END);
                                fwrite(&partidaHist, sizeof(Historico), 1, historico);
                                fclose(historico);
                            }
                            estadoAtual = RANKING;
                        }
                    }
                }

                if (nickinvalido && GetTime() - nickinvalidoTempo < 4) {
                    DrawText("Nickname inválido! Deve ter exatamente 3 caracteres.", SCREEN_WIDTH / 2 - MeasureText("Nickname inválido! Deve ter exatamente 3 caracteres.", 20) / 2, SCREEN_HEIGHT / 2 + 90, 20, RED);
                } else {
                    nickinvalido = false;
                }

                break;
            }
            
            case REGRAS: {
                // Carregar as texturas apenas uma vez
                if (!fundo_regras_carregado) {
                    if (qual_tela == 0) {
                        fundo_regras = LoadTexture("assets/img/telas/fundo_regras_cpuwin.png");
                    } else if (qual_tela == 1) {
                        fundo_regras = LoadTexture("assets/img/telas/fundo_regras_playerwin.png");
                    } else {
                        fundo_regras = LoadTexture("assets/img/telas/fundo_regras.png");
                    }
                    fundo_regras_carregado = true;
                }
                if (!texto_regras_carregado) {
                    texto_regras = LoadTexture("assets/img/telas/texto_regras.png");
                    texto_regras_carregado = true;
                }

                // Definir o retângulo para o botão de sair
                Rectangle retangulo_sair = {345, 517, 110, 40};

                // Definir a área de visualização do texto
                Rectangle area_texto = {105,142,590,350};
                
                Rectangle retangulo_rolagem = {710, 142, 20, 350}; //retângulo fundo da barra de rolagem
               
                // Variável para armazenar o deslocamento da rolagem
                static float scrollOffset = 0;
                static bool arrastando = false;

                Rectangle barra_rolagem = {710, 142 + scrollOffset , 20, 30}; //barra rolagem

                // Desenha fundo
                DrawTexture(fundo_regras, 0, 0, WHITE);

                // Atualiza a posição da barra de rolagem com base no movimento do mouse
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                    if (arrastando || CheckCollisionPointRec(GetMousePosition(), retangulo_rolagem)) {
                        arrastando = true;
                        float mouseY = GetMouseY();
                        barra_rolagem.y = mouseY - barra_rolagem.height / 2;

                        // Limita a barra de rolagem dentro do retângulo de rolagem
                        if (barra_rolagem.y < retangulo_rolagem.y) barra_rolagem.y = retangulo_rolagem.y;
                        if (barra_rolagem.y > retangulo_rolagem.y + retangulo_rolagem.height - barra_rolagem.height) barra_rolagem.y = retangulo_rolagem.y + retangulo_rolagem.height - barra_rolagem.height;
                    }
                } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                    arrastando = false;
                }

                // Adiciona a funcionalidade de rolagem com a roda do mouse
                float wheelMove = GetMouseWheelMove();
                if (wheelMove != 0) {
                    scrollOffset -= wheelMove * 20; // Ajuste a velocidade de rolagem conforme necessário

                    // Limita o scrollOffset dentro dos limites do texto
                    if (scrollOffset < 0) scrollOffset = 0;
                    if (scrollOffset > texto_regras.height - area_texto.height) scrollOffset = texto_regras.height - area_texto.height;
                }


                if (arrastando) {
                    // Calcula o deslocamento da rolagem com base na posição da barra de rolagem
                    scrollOffset = (barra_rolagem.y - retangulo_rolagem.y) / (retangulo_rolagem.height - barra_rolagem.height) * (texto_regras.height - area_texto.height);
                } else {
                    // Atualiza a posição da barra de rolagem com base no scrollOffset
                    barra_rolagem.y = retangulo_rolagem.y + (scrollOffset / (texto_regras.height - area_texto.height)) * (retangulo_rolagem.height - barra_rolagem.height);
                    // Limita a barra de rolagem dentro do retângulo de rolagem
                    if (barra_rolagem.y < retangulo_rolagem.y) barra_rolagem.y = retangulo_rolagem.y;
                    if (barra_rolagem.y > retangulo_rolagem.y + retangulo_rolagem.height - barra_rolagem.height) barra_rolagem.y = retangulo_rolagem.y + retangulo_rolagem.height - barra_rolagem.height;
                                
                }

                // Desenha a barra de rolagem
                DrawRectangleRec(retangulo_rolagem, DARKGRAY);
                // Desenha a barra de rolagem com cor diferente se estiver sendo pressionada
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && arrastando) {
                    DrawRectangleRec(barra_rolagem, LIGHTGRAY);
                } else {
                    DrawRectangleRec(barra_rolagem, GRAY);
                }

                // Desenha a parte visível do texto
                DrawTextureRec(texto_regras, (Rectangle){0, scrollOffset, area_texto.width, area_texto.height}, (Vector2){area_texto.x, area_texto.y}, WHITE);


                // Botao SAIR
                if(CheckCollisionPointRec(GetMousePosition(), retangulo_sair)){
                    DrawRectangleRec(retangulo_sair, DARKGREEN);

                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        PlaySound(som_resto);
                        estadoAtual = TELA_INICIAL;
                    }

                }else{
                    DrawRectangleRec(retangulo_sair, LIME);
                }
                DrawText("SAIR", retangulo_sair.x + retangulo_sair.width / 2 - MeasureText("SAIR", 20) / 2, retangulo_sair.y + retangulo_sair.height / 2 - 10, 20, WHITE);

                // Atualizar a música copilot
                UpdateMusicStream(musica_atual);
            
                break;
            }

            case RANKING: {
                // Carrega textura fundo ranking apenas uma vez
                if (!fundo_ranking_carregado) {
                    if (qual_tela == 0) {
                        fundo_ranking = LoadTexture("assets/img/telas/fundo_ranking_cpuwin.png");
                    } else if (qual_tela == 1) {
                        fundo_ranking = LoadTexture("assets/img/telas/fundo_ranking_playerwin.png");
                    } else {
                        fundo_ranking = LoadTexture("assets/img/telas/fundo_ranking.png");
                    }
                    fundo_ranking_carregado = true;
                }

                // Desenha fundo
                DrawTexture(fundo_ranking, 0, 0, WHITE);

                // Definir o retângulo para o botão de sair
                Rectangle retangulo_sair = {345, 517, 110, 40};

                FILE *historico = fopen("assets/data/historico.dat", "rb");
                if (historico == NULL) {
                    DrawText("Não houveram partidas anteriores.",160, 480, 30, RED);//-----------------------
                } else {
                    Historico historicos[100];
                    int aux = 0;
                    while (fread(&historicos[aux], sizeof(Historico), 1, historico) && aux < 100) {
                        aux++;
                    }
                    fclose(historico);

                    //ordenar
                    for (int i = 0; i < aux - 1; i++) {
                        for (int j = i + 1; j < aux; j++) {
                            if (historicos[i].pontuacao < historicos[j].pontuacao) {
                                Historico temp = historicos[i];
                                historicos[i] = historicos[j];
                                historicos[j] = temp;
                            }
                        }
                    }

                    bool top5 = false;
                    for (int i = 0; i < aux && i < 5; i++) {
                        
                        if (i == 0) cor = GOLD;
                        else if (i == 1) cor = SILVER;
                        else if (i == 2) cor = BRONZE;
                        else cor = WHITE;

                        DrawText(historicos[i].vencedor, 168, 217 + i * 48, 30, cor);
                        DrawText(TextFormat("%d", historicos[i].pontuacao), 418, 217 + i * 48, 30, cor);
                        DrawText(TextFormat("%02d/%02d", historicos[i].dia, historicos[i].mes), 607, 217 + i * 48, 30, cor);

                        if (strcmp(historicos[i].vencedor, partidaHist.vencedor) == 0 && historicos[i].pontuacao == partidaHist.pontuacao) {
                            top5 = true;
                        }
                    }

                    if (!top5) {
                        for (int i = 5; i < aux; i++) {
                            if (strcmp(historicos[i].vencedor, partidaHist.vencedor) == 0 && historicos[i].pontuacao == partidaHist.pontuacao) {
                               
                                DrawText(TextFormat("Sua posição: %d°", i + 1), 290, 463, 36, YELLOW);                                break;
                                break;
                            }
                        }
                    }
                }

                // Botao SAIR
                if(CheckCollisionPointRec(GetMousePosition(), retangulo_sair)){
                    DrawRectangleRec(retangulo_sair, DARKGREEN);

                    if((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER)) && veio_tela_inicial){
                        PlaySound(som_resto);
                        estadoAtual = TELA_INICIAL;
                    } else if((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER)) && !veio_tela_inicial){
                        PlaySound(som_resto);
                        estadoAtual = JOGAR_NOVAMENTE;
                    }

                }else{
                    DrawRectangleRec(retangulo_sair, LIME);
                }
                DrawText("SAIR", retangulo_sair.x + retangulo_sair.width / 2 - MeasureText("SAIR", 20) / 2, retangulo_sair.y + retangulo_sair.height / 2 - 10, 20, WHITE);

                break;
            }
            
            case JOGAR_NOVAMENTE: {             
                DrawText("Jogar Novamente?", SCREEN_WIDTH / 2 - MeasureText("Jogar Novamente?", 40) / 2, SCREEN_HEIGHT / 2 - 60, 40, WHITE);

                // botao sim
                Rectangle botaoSim = {SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2, 100, 50};
                DrawRectangleRec(botaoSim, GREEN);
                DrawText("Sim", botaoSim.x + botaoSim.width / 2 - MeasureText("Sim", 20) / 2, botaoSim.y + botaoSim.height / 2 - 10, 20, RAYWHITE);

                // botao nao
                Rectangle botaoNao = {SCREEN_WIDTH / 2 + 10, SCREEN_HEIGHT / 2, 100, 50};
                DrawRectangleRec(botaoNao, RED);
                DrawText("Não", botaoNao.x + botaoNao.width / 2 - MeasureText("Não", 20) / 2, botaoNao.y + botaoNao.height / 2 - 10, 20, RAYWHITE);

                // verificando se o botão sim foi clicado
                if (CheckCollisionPointRec(GetMousePosition(), botaoSim) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    // RESETAR AS VARIAVEIS POR PADRAO AQUI, OU CRIAR UM CASE ANTES DE NOVO BARALHO PRA TRATA
                    PlaySound(som_resto);
                    tela_inicial = false;
                    estadoAtual = RESET;
                }

                // verificando se o botão nao foi clicado
                if (CheckCollisionPointRec(GetMousePosition(), botaoNao) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    PlaySound(som_resto);
                    tela_inicial = true;
                    estadoAtual = RESET;
                }
                break;
            }

            default:
                break;
            }
        if (estadoAtual == FECHAR_JANELA)
            break;
        EndDrawing();
    }

    // parando e descarregando a musica
    StopMusicStream(musica_atual);
    UnloadMusicStream(musica_atual);
    UnloadImage(icon);
    CloseAudioDevice(); 


    CloseWindow(); // fechando a janela
    return;
} 