/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Victor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#ifndef BUSCA_H
#define BUSCA_H

#include "hash.h"

// Estrutura auxiliar para guardar a relevância de cada documento
// e facilitar a ordenação (qsort) no final
typedef struct {
    int idDoc;
    float relevancia;
} ResultadoBusca;

/*
 * Função principal de busca.
 * Recebe:
 * - termosConsulta: Matriz de strings com as palavras digitadas pelo usuário
 * - numTermosConsulta: Quantidade de palavras (o 'q' da fórmula)
 * - T: A Tabela Hash preenchida
 * - N: Número total de documentos na coleção
 * - termosDistintosPorDoc: Vetor onde o índice é o idDoc e o valor é o 'n_i' (termos distintos daquele doc)
 * - nomesDocumentos: Vetor de strings para imprimir o nome original do arquivo (ex: "arquivo1.txt")
 */
void BuscarFabulhas(char termosConsulta[][N], int numTermosConsulta, TipoDicionario T, 
                    int N_TotalDocs, int *termosDistintosPorDoc, char nomesDocumentos[][100]);

#endif