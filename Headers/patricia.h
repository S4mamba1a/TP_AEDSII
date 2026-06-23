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

// Diferencia se o no e interno (guia a busca) ou externo (guarda a palavra)
typedef enum {
    Interno,
    Externo
} TipoNo;

typedef struct PATNo *Apontador;

struct PATNo {
    TipoNo tipo;
    union {
        // No Interno: nao armazena dado, so guia a descida
        struct {
            int indice;      // Posicao da string sendo comparada
            char charCmp;    // Caracter pivo de comparacao
            Apontador esq, dir;
        } NInterno;

        // No Externo (folha): armazena a palavra e a lista invertida
        struct {
            char *palavra;
            Ocorrencia *listaDocs;
        } NExterno;
    } NO;
};

// Prototipos das funcoes
void InicializaArvore(Apontador *raiz);
Apontador PesquisaPatricia(char *palavra, Apontador p, int *comparacoes);
Apontador InserePatricia(char *palavra, int idDoc, Apontador *raiz, InfoDoc docs[], int *comparacoes);
void ImprimeIndiceInvertidoPatricia(Apontador raiz);
void LiberaArvorePatricia(Apontador p);

#endif