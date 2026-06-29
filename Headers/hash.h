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

// Tamanho da Tabela Hash (Número primo escolhido para manter o fator de carga em ~0.78)
#define M 503 
// Tamanho máximo permitido para uma palavra (chave)
#define N 128 

// ====================================================================
// ESTRUTURAS DO ÍNDICE INVERTIDO
// ====================================================================

// Ponteiro para a estrutura Ocorrencia (definida no TAD arquivo.h)
// Responsável por guardar a lista encadeada de documentos e frequências (TF)
typedef struct Ocorrencia* ApontadorOcorrencia;

typedef char TipoChave[N];
typedef unsigned int TipoPesos[N];

/*
 * Estrutura do Item guardado na Tabela Hash.
 * Contém a palavra indexada e o ponteiro para a sua respectiva 
 * lista de documentos (ocorrências).
 */
typedef struct TipoItem {
    TipoChave Chave;               // A palavra (ex: "raposa")
    ApontadorOcorrencia ListaDocs; // Cabeça da lista de ocorrências (<qtde, idDoc>)
} TipoItem;

// ====================================================================
// ESTRUTURAS DA TABELA HASH (Baseado em Nívio Ziviani - Endereçamento Fechado)
// ====================================================================

typedef struct TipoCelula* TipoApontador;

// Célula da lista encadeada para tratamento de colisões
typedef struct TipoCelula {
    TipoItem Item;
    TipoApontador Prox;
} TipoCelula;

// Estrutura que controla o início e o fim da lista de colisão de um índice
typedef struct TipoLista {
    TipoCelula *Primeiro, *Ultimo;
} TipoLista;

// A Tabela Hash é um vetor de M posições de TipoLista
typedef TipoLista TipoDicionario[M];

// ====================================================================
// PROTÓTIPOS DAS FUNÇÕES
// ====================================================================

// Funções básicas de manipulação de lista
void FLVazia(TipoLista *Lista);
short Vazia(TipoLista Lista);
void Ins(TipoItem x, TipoLista *Lista);

// Funções do motor da Tabela Hash
void GeraPesos(TipoPesos p);
unsigned int h(TipoChave Chave, TipoPesos p);
void Inicializa(TipoDicionario T);

// Funções principais do Trabalho Prático (Busca, Inserção e Impressão)
TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoDicionario T, int *comparacoes);
void Insere(TipoChave Ch, int idDoc, TipoPesos p, TipoDicionario T, int *comparacoes);
void ImprimeIndiceInvertido(TipoDicionario Tabela);

// Funções auxiliares
void LerPalavra(char *p, int Tam);
size_t MemoriaHash(TipoDicionario T);
void LiberaHash(TipoDicionario T);

#endif