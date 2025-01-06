#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "funcaux.h"
#include "filechange.h"

int modo();
Cartas* get_baralho(Cartas* cartas, int quantd_cartas, int quant_cartas_baralho, int* quant_cartas_usadas, int** cartas_usadas);
int primeiro_player();
int verifica_maior(Cartas* carta_jogador, Cartas* carta_cpu, int atributo);
int verifica_menor(Cartas* carta_jogador, Cartas* carta_cpu, int atributo);
void adicionar_carta_vencedor(Cartas** baralho_vencedor, Cartas** baralho_perdedor, int* quant_cartas_vencedor, int* quant_cartas_perdedor);
void partida(Cartas* baralho_jogador, Cartas* baralho_cpu, int quant_cartas_baralho);

#endif