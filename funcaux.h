#ifndef FUNCAUX_H
#define FUNCAUX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_NOME_CARTA 21

typedef struct {
    char nome[TAM_NOME_CARTA];
    char letra;
    int num;
    int super_trunfo;
    int atributo_1;
    int atributo_2;
    int atributo_3;
    int atributo_4;
    int atributo_5;
} Cartas;

// Definição de enumeradores função pesquisa
enum MenuOpcoes { PESQ_ATRIBUTO = 1, PESQ_LETRA, PESQ_NUMERO, SAIR };
enum Atributos { FORCA = 1, HABILIDADE, VELOCIDADE, PODERES, CURA };

int sistema();
void burocracia(char nome[], int tamanho);
FILE* abrir_arquivo(const char nome_arq[], const char mode[]);
int quant_cartas(FILE* arquivo);
int get_int();
int get_pos_carta(Cartas** cartas, int quantid_cartas, const char nome_carta[]);
int verify_int_in_vetor(int* vetor, int size_vetor, int valor);

//head das funcoes para pesquisa
int validar_entrada(int min, int max, const char *mensagem);
void buscar_por_atributo(Cartas carta[], int qnt_cartas, int atributo, int comparacao, int valor);
void buscar_por_letra(Cartas carta[], int qnt_cartas, char letra);
void buscar_por_numero(Cartas carta[], int qnt_cartas, int numero);
#endif
