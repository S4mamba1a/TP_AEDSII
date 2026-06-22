/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Victor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#include "patricia.h"
// Retorna o caracter de uma string em um indice seguro (evita segmentation fault)
char GetChar(char *palavra, int indice) {
    if (indice < strlen(palavra)) {
        return palavra[indice];
    }
    return '\0'; // Se a palavra for menor que o indice, retorna fim de string
}

void InicializaArvore(Apontador *raiz) {
    *raiz = NULL;
}

// Funcao auxiliar para atualizar ou inserir um novo documento na lista
void AtualizaListaDoc(Ocorrencia **lista, int idDoc) {
    Ocorrencia *atual = *lista;
    Ocorrencia *ant = NULL;

    // Procura se o doc ja existe (lista deve ser ordenada por idDoc)
    while (atual != NULL && atual->idDoc < idDoc) {
        ant = atual;
        atual = atual->Prox;
    }

    if (atual != NULL && atual->idDoc == idDoc) {
        atual->quantidade++; // Palavra apareceu de novo no mesmo arquivo
    } else {
        // Nao achou, cria nova celula
        Ocorrencia *nova = (Ocorrencia *)malloc(sizeof(Ocorrencia));
        nova->idDoc = idDoc;
        nova->quantidade = 1;
        nova->Prox = atual;

        if (ant == NULL) {
            *lista = nova; // Insere no inicio
        } else {
            ant->Prox = nova; // Insere no meio/fim
        }
    }
}

// Cria um no externo (folha)
Apontador CriaNoExterno(char *palavra, int idDoc) {
    Apontador p = (Apontador)malloc(sizeof(struct PATNo));
    p->tipo = Externo;
    p->NO.NExterno.palavra = strdup(palavra); // aloca e copia a string
    p->NO.NExterno.listaDocs = NULL;
    AtualizaListaDoc(&(p->NO.NExterno.listaDocs), idDoc);
    return p;
}

// Cria um no interno (decisao)
Apontador CriaNoInterno(Apontador esq, Apontador dir, int indice, char charCmp) {
    Apontador p = (Apontador)malloc(sizeof(struct PATNo));
    p->tipo = Interno;
    p->NO.NInterno.indice = indice;
    p->NO.NInterno.charCmp = charCmp;
    p->NO.NInterno.esq = esq;
    p->NO.NInterno.dir = dir;
    return p;
}

Apontador PesquisaPatricia(char *palavra, Apontador p) {
    if (p == NULL) return NULL;

    if (p->tipo == Externo) {
        if (strcmp(palavra, p->NO.NExterno.palavra) == 0) {
            return p;
        } else {
            return NULL;
        }
    }

    // Se e interno, decide o caminho
    int idx = p->NO.NInterno.indice;
    char c = GetChar(palavra, idx);

    if (c < p->NO.NInterno.charCmp) {
        return PesquisaPatricia(palavra, p->NO.NInterno.esq);
    } else {
        return PesquisaPatricia(palavra, p->NO.NInterno.dir);
    }
}

// Funcao auxiliar que faz a insercao recursiva real
Apontador InsereEntre(char *palavra, int idDoc, Apontador p, int idxDif, char charDif) {
    if (p->tipo == Externo || p->NO.NInterno.indice >= idxDif) {
        // Achamos o ponto de insercao do novo no interno
        Apontador novoExt = CriaNoExterno(palavra, idDoc);
        char c = GetChar(palavra, idxDif);
        
        if (c < charDif) {
            return CriaNoInterno(novoExt, p, idxDif, charDif);
        } else {
            return CriaNoInterno(p, novoExt, idxDif, charDif);
        }
    }

    // Continua descendo na arvore
    int idx = p->NO.NInterno.indice;
    char c = GetChar(palavra, idx);

    if (c < p->NO.NInterno.charCmp) {
        p->NO.NInterno.esq = InsereEntre(palavra, idDoc, p->NO.NInterno.esq, idxDif, charDif);
    } else {
        p->NO.NInterno.dir = InsereEntre(palavra, idDoc, p->NO.NInterno.dir, idxDif, charDif);
    }

    return p;
}

Apontador InserePatricia(char *palavra, int idDoc, Apontador *raiz) {
    if (*raiz == NULL) {
        *raiz = CriaNoExterno(palavra, idDoc);
        return *raiz;
    }

    Apontador p = *raiz;
    // Desce ate achar um no externo para comparar
    while (p->tipo == Interno) {
        int idx = p->NO.NInterno.indice;
        char c = GetChar(palavra, idx);
        if (c < p->NO.NInterno.charCmp) {
            p = p->NO.NInterno.esq;
        } else {
            p = p->NO.NInterno.dir;
        }
    }

    // Chegou no no externo, compara a palavra inteira
    if (strcmp(palavra, p->NO.NExterno.palavra) == 0) {
        // Palavra ja existe na arvore, so adiciona o documento na lista
        AtualizaListaDoc(&(p->NO.NExterno.listaDocs), idDoc);
        return *raiz;
    }

    // Se nao eh a mesma palavra, acha o primeiro caracter diferente
    int i = 0;
    while (palavra[i] == p->NO.NExterno.palavra[i] && palavra[i] != '\0') {
        i++;
    }

    char charCmp;
    // Define qual char vai ficar no no interno como pivô
    if (palavra[i] > p->NO.NExterno.palavra[i]) {
        charCmp = palavra[i];
    } else {
        charCmp = p->NO.NExterno.palavra[i];
    }

    *raiz = InsereEntre(palavra, idDoc, *raiz, i, charCmp);
    return *raiz;
}

void ImprimeListaDocs(Ocorrencia *lista) {
    Ocorrencia *aux = lista;
    while (aux != NULL) {
        printf("<%d, %d> ", aux->idDoc, aux->quantidade);
        aux = aux->Prox;
    }
    printf("\n");
}

void ImprimePatricia(Apontador p) {
    if (p == NULL) return;

    if (p->tipo == Interno) {
        ImprimePatricia(p->NO.NInterno.esq);
        ImprimePatricia(p->NO.NInterno.dir);
    } else {
        // Imprime a palavra seguida da lista de paret
        printf("%s ", p->NO.NExterno.palavra);
        ImprimeListaDocs(p->NO.NExterno.listaDocs);
    }
}