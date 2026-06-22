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
#define N 50

// --- ESTRUTURAS DO ÍNDICE INVERTIDO ---

// 1. Célula da Lista de Documentos (Ocorrências)
typedef struct CelulaOcorrencia* ApontadorOcorrencia;
typedef struct CelulaOcorrencia {
    int idDoc;
    int qtde;
    ApontadorOcorrencia Prox;
} CelulaOcorrencia;

typedef char TipoChave[N];
typedef unsigned int TipoPesos[N];

// 2. O Item guardado na Hash contém a palavra e a lista de documentos
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
void Insere(TipoItem x, TipoLista *Lista);

// Funções de Hashing
void GeraPesos(TipoPesos p);
unsigned int h(TipoChave Chave, TipoPesos p);
void Inicializa(TipoDicionario T);

// Função auxiliar para o Índice Invertido
void AdicionaOuAtualizaOcorrencia(ApontadorOcorrencia *lista, int idDoc);

// Funções principais modificadas para o Trabalho
TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoDicionario T, int *comparacoes);
void Insere(TipoChave Ch, int idDoc, TipoPesos p, TipoDicionario T, int *comparacoes);

// Funções de Impressão
void ImprimeIndiceInvertido(TipoDicionario Tabela);
void LerPalavra(char *p, int Tam);

#endif