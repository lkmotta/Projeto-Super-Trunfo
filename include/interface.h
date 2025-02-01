/**
 * @file interface.h
 * @authors @lkmotta, @Dirceu06, @boonfim
 * @brief Arquivo de cabeçalho com as funções de interface gráfica do jogo
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
    CARREGANDO_TELA_INICIAL,
    TELA_INICIAL,
    AJUSTES,
    REGRAS,
    RESET,
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
#define COR_DESTAQUE_TELAINICIAL CLITERAL(Color){109, 96, 58, 255} // #6D603A (hex)
#define COR_DESTAQUE_TELAINICIAL_CPUWIN CLITERAL(Color){223, 0, 0, 255} // #DF0000(hex)
#define COR_DESTAQUE_TELAINICIAL_PLAYERWIN CLITERAL(Color){34, 183, 252, 255} // #22B7FC(hex)
#define SILVER CLITERAL(Color){192, 192, 192, 255}
#define BRONZE CLITERAL(Color){205, 127, 50, 255}

bool carregando_imagem_carta(Cartas carta, Texture2D *textura_carta);
void interface(Cartas *cartas, int size_cartas, int quant_cartas_baralho);

#endif