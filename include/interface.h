/**
 * @file interface.h
 * @authors @lkmotta, @Dirceu06, @boonfim
 * @brief Arquivo de cabe�alho com as fun��es de interface gr�fica do jogo
 *
 * LICENSE: MIT
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>
#include <raylib.h>
#include "game.h"

// estados do jogo
typedef enum
{
    RESET,
    CARREGANDO_TELA_INICIAL,
    TELA_INICIAL,
    NOVO_BARALHO,
    TELA_CARREGAMENTO,
    TELA_PLAYER_ESCOLHENDO_ATRIBUTO,
    TELA_CPU_ESCOLHENDO_ATRIBUTO,
    VERIFICANDO_GANHADOR,
    TELA_RESULTADO,
    JOGADOR_VENCEU,
    CPU_VENCEU,
    JOGAR_NOVAMENTE,
    NICKNAME,
    RANKING,
    FECHAR_JANELA
} GameState;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 90
#define COR_FUNDO CLITERAL(Color){30, 30, 30, 255} // #1E1E1E (hex)
#define COR_BARRA CLITERAL(Color){251, 59, 0, 255} // #FB3B00 (hex)

void interface(Cartas *cartas, int size_cartas, int quant_cartas_baralho);

#endif