/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Victor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#ifndef BUSCA_H
#define BUSCA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "arquivo.h"
#include "hash.h"
#include "patricia.h"


// Estrutura auxiliar para guardar a relevância de cada documento
// e facilitar a ordenação (qsort) no final
typedef struct {                                   
    int   idDoc;
    float relevancia;
    char  nomeArquivo[MAX_NOME];
} ResultadoBusca;

double CalcularTempoEmMs(struct timeval inicio, struct timeval fim);

void BuscarNaHash(char termosConsulta[][MAX_PALAVRA], int numTermos, TipoDicionario T, TipoPesos pesosHash, int numDocs, InfoDoc docs[]);
         
void BuscarNaPatricia(char termosConsulta[][MAX_PALAVRA], int numTermos, Apontador raiz, int numDocs, InfoDoc docs[]);
                  
#endif