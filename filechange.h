#ifndef FILECHANGE_H
#define FILECHANGE_H

#include "funcaux.h"
#include <stdio.h>    // funcoes basicas
#include <stdlib.h>  // para alocacao dinamica

#define MAX_INSERIR_CARTAS 32

void inserir_cartas(FILE *arq_cartaCOPIA, Cartas **carta, int **ptr_posicoesA, int **ptr_posicoesB, int **ptr_posicoesC, int **ptr_posicoesD);
void listar_cartas(Cartas* carta, int num_cartas);
void remover_carta(FILE* arq_cartas, Cartas** cartas, int quantd_cartas);
void editar_carta(FILE* arq_carta, Cartas carta[]);
void alterar_carta(FILE* arq_cartas,Cartas** cartas, int quantd_cartas);
void buscar_carta(Cartas carta[], int qnt_cartas);
#endif