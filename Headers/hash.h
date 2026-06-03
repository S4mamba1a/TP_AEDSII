/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Victor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#ifndef hash_h
#define hash_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define M 1000
#define N 20     
#define TAMALFABETO 256

typedef unsigned int  TipoPesos[N];
typedef char TipoChave[N];
typedef unsigned TipoPesos[N][TAMALFABETO];
typedef struct TipoItem {
  /* outros componentes */
  TipoChave Chave;
} TipoItem;
typedef unsigned int TipoIndice;
typedef struct TipoCelula* TipoApontador;
typedef struct TipoCelula {
  TipoItem Item;
  TipoApontador Prox;
} TipoCelula;
typedef struct TipoLista {
  TipoCelula *Primeiro, *Ultimo;
} TipoLista;
typedef TipoLista TipoDicionario[M];
TipoDicionario Tabela;
TipoItem Elemento;
TipoPesos p;
TipoApontador i;

void FLVazia(TipoLista *Lista);

short Vazia(TipoLista Lista);

void Ins(TipoItem x, TipoLista *Lista);

void Ret(TipoApontador p, TipoLista *Lista, TipoItem *Item);


void GeraPesos(TipoPesos p);

TipoIndice h(TipoChave Chave, TipoPesos p);


void GeraPesos(TipoPesos p);

TipoIndice h(TipoChave Chave, TipoPesos p);

void Inicializa(TipoDicionario T);

TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoDicionario T);

void Insere(TipoItem x, TipoPesos p, TipoDicionario T);

void Retira(TipoItem x, TipoPesos p, TipoDicionario T);

void Imp(TipoLista Lista);

void Imprime(TipoDicionario Tabela);
 
void LerPalavra(char *p, int Tam);

#endif