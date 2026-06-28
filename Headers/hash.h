/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Vitor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#ifndef hash_h
#define hash_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "arquivo.h"

#define M 1000
#define N 128

// --- ESTRUTURAS DO ÍNDICE INVERTIDO ---

// Usa a struct Ocorrencia que agora vem de arquivo.h
typedef struct Ocorrencia* ApontadorOcorrencia;

typedef char TipoChave[N];
typedef unsigned int TipoPesos[N];

// O Item guardado na Hash contém a palavra e a lista de documentos
typedef struct TipoItem {
    TipoChave Chave;
    ApontadorOcorrencia ListaDocs; // Cabeça da lista de ocorrências
} TipoItem;

// --- ESTRUTURAS DA TABELA HASH (ZIVIANI) ---
typedef struct TipoCelula* TipoApontador;
typedef struct TipoCelula {
    TipoItem Item;
    TipoApontador Prox;
} TipoCelula;

typedef struct TipoLista {
    TipoCelula *Primeiro, *Ultimo;
} TipoLista;

typedef TipoLista TipoDicionario[M];

// --- FUNÇÕES ---

void FLVazia(TipoLista *Lista);
short Vazia(TipoLista Lista);
void Ins(TipoItem x, TipoLista *Lista);

// Funções de Hashing
void GeraPesos(TipoPesos p);
unsigned int h(TipoChave Chave, TipoPesos p);
void Inicializa(TipoDicionario T);

// Funções principais modificadas para o Trabalho
TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoDicionario T, int *comparacoes);
void Insere(TipoChave Ch, int idDoc, TipoPesos p, TipoDicionario T, int *comparacoes);

// Funções de Impressão
void ImprimeIndiceInvertido(TipoDicionario Tabela);
void LerPalavra(char *p, int Tam);
// Medicao de memoria 
size_t MemoriaHash(TipoDicionario T);

void LiberaHash(TipoDicionario T);

#endif