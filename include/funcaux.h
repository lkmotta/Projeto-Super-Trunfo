#ifndef FUNCAUX_H
#define FUNCAUX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define TAM_NOME_CARTA 21

typedef struct {
    char nome[TAM_NOME_CARTA];
    char letra;
    int num;
    int super_trunfo;
    int forca;
    int habilidade;
    int velocidade;
    int poderes;
    int poder_cura;
} Cartas;

typedef struct {
    char vencedor[10];
    int rodadas;
    int vitorias;
    int empates;
    int pontuacao; 
    int dia;
    int mes;
} Historico;

typedef enum {
    FORCA = 1, HABILIDADE, VELOCIDADE, PODERES, CURA
} Atributo;

enum MenuOpcoes { PESQ_ATRIBUTO = 1, PESQ_LETRA, PESQ_NUMERO, SAIR };

int sistema();
void burocracia(char nome[], int tamanho);
FILE* abrir_arquivo(const char nome_arq[], const char mode[]);
int quant_cartas(FILE* arquivo);
int get_int(int valor_min, int valor_max, const char mensagem_erro[]);
int get_pos_carta(Cartas** cartas, int quantid_cartas, const char nome_carta[]);
int verify_int_in_vetor(int* vetor, int size_vetor, int valor);

void sort_decrease(int* posi_excluir, int num_excluir);
void remover_cartas_vetor(Cartas** vetor, int* quantd_cartas_vetor, int* posicoes_excluir, int quantd_excluir);

// auxiliares buscar_carta
void buscar_por_atributo(Cartas carta[], int qnt_cartas, int atributo, int comparacao, int valor);
void buscar_por_letra(Cartas carta[], int qnt_cartas, char letra);
void buscar_por_numero(Cartas carta[], int qnt_cartas, int numero);

void exportar_csv(Cartas* cartas, int quantd_cartas);

void embaralhar(Cartas* cartas, int quantd_cartas);
int compare_cartas(const void* a, const void* b);
void desembaralhar(Cartas** cartas, int quantd_cartas);

#endif