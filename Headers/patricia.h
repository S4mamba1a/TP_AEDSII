/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Victor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#ifndef PATRICIA_H
#define PATRICIA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"

// Diferencia se o nó é interno (guarda indice/char) ou externo (guarda a palavra e a lista)
typedef enum {
    Interno,
    Externo
} TipoNo;

typedef struct PATNo *Apontador;

struct PATNo {
    TipoNo tipo;
    union {
        // Nó Interno: guia a busca
        struct {
            int indice;      // Qual posicao da string estamos comparando
            char charCmp;    // Caracter de comparacao
            Apontador esq, dir;
        } NInterno;
        
        // Nó Externo: folha que guarda a palavra e os documentos
        struct {
            char *palavra;
            Ocorrencia *listaDocs;
        } NExterno;
    } NO;
};

// Prototipos das funcoes
void InicializaArvore(Apontador *raiz);
Apontador PesquisaPatricia(char *palavra, Apontador p);
Apontador InserePatricia(char *palavra, int idDoc, Apontador *raiz);
void ImprimePatricia(Apontador p);

#endif