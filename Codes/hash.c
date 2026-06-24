/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Victor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#include "hash.h"

void FLVazia(TipoLista *Lista) { 
    Lista->Primeiro = (TipoCelula *)malloc(sizeof(TipoCelula));
    Lista->Ultimo = Lista->Primeiro; 
    Lista->Primeiro->Prox = NULL;
}  

short Vazia(TipoLista Lista) { 
    return (Lista.Primeiro == Lista.Ultimo); 
}

void Ins(TipoItem x, TipoLista *Lista) { 
    Lista->Ultimo->Prox = (TipoCelula *)malloc(sizeof(TipoCelula));
    Lista->Ultimo = Lista->Ultimo->Prox; 
    Lista->Ultimo->Item = x;
    Lista->Ultimo->Prox = NULL;
}  

void GeraPesos(TipoPesos p) { 
    int i;
    struct timeval semente;
    gettimeofday(&semente, NULL); 
    srand((int)(semente.tv_sec + 1000000*semente.tv_usec));
    for (i = 0; i < N; i++)
        p[i] =  1 + (int) (10000.0 * rand() / (RAND_MAX + 1.0));
}

unsigned int h(TipoChave Chave, TipoPesos p) { 
    int i; 
    unsigned int Soma = 0; 
    int comp = strlen(Chave);
    
    // Trava de segurança para não ultrapassar o tamanho de pesos
    if (comp > N) comp = N; 

    for (i = 0; i < comp; i++) 
        Soma += (unsigned int)Chave[i] * p[i];
    return (Soma % M);
}

void Inicializa(TipoDicionario T) { 
    int i;
    for (i = 0; i < M; i++) FLVazia(&T[i]);
}

// ====================================================================
// FUNÇÕES ESPECÍFICAS PARA O TRABALHO PRÁTICO
// ====================================================================

TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoDicionario T, int *comparacoes) { 
    unsigned int i = h(Ch, p);
    TipoApontador Ap = T[i].Primeiro->Prox;
    
    while (Ap != NULL) {
        (*comparacoes)++; // Incrementa o contador de comparações!
        
        if (strcmp(Ch, Ap->Item.Chave) == 0) {
            return Ap; // Achou a palavra
        }
        Ap = Ap->Prox;
    }
    return NULL; /* Pesquisa sem sucesso */
}  

void Insere(TipoChave Ch, int idDoc, TipoPesos p, TipoDicionario T, int *comparacoes) { 
    TipoApontador Ap = Pesquisa(Ch, p, T, comparacoes);
    
    if (Ap == NULL) {
        // CASO 1: A palavra ainda não existe na Hash
        TipoItem x;
        strcpy(x.Chave, Ch);
        x.ListaDocs = NULL; // Inicializa a lista de documentos vazia
        
        // Usa a função do TAD arquivo
        arq_InserirOcorrencia(&x.ListaDocs, idDoc);
        
        // Insere a nova palavra na Tabela Hash
        Ins(x, &T[h(Ch, p)]);
    } else {
        // CASO 2: A palavra já existe, apenas atualiza chamando a função do TAD arquivo
        arq_InserirOcorrencia(&(Ap->Item.ListaDocs), idDoc);
    }
} 

// Função auxiliar para o qsort comparar duas chaves alfabeticamente na Hash
int ComparaPalavras(const void *a, const void *b) {
    TipoItem *itemA = *(TipoItem **)a;
    TipoItem *itemB = *(TipoItem **)b;
    return strcmp(itemA->Chave, itemB->Chave);
}

void ImprimeIndiceInvertido(TipoDicionario Tabela) { 
    // Conta e aloca para impressão ORDENADA (Requisito do trabalho)
    int totalPalavras = 0;
    for (int i = 0; i < M; i++) {
        TipoApontador Aux = Tabela[i].Primeiro->Prox;
        while (Aux != NULL) {
            totalPalavras++;
            Aux = Aux->Prox;
        }
    }

    if (totalPalavras == 0) return;

    TipoItem **vetorPalavras = (TipoItem **)malloc(totalPalavras * sizeof(TipoItem *));
    int idx = 0;
    
    for (int i = 0; i < M; i++) {
        TipoApontador Aux = Tabela[i].Primeiro->Prox;
        while (Aux != NULL) {
            vetorPalavras[idx++] = &(Aux->Item);
            Aux = Aux->Prox;
        }
    }

    // Ordena o vetor usando qsort
    qsort(vetorPalavras, totalPalavras, sizeof(TipoItem *), ComparaPalavras);

    // Imprime alfabeticamente e usa a função ImprimirOcorrencia do arquivo.h
    for (int i = 0; i < totalPalavras; i++) {
        printf("%s", vetorPalavras[i]->Chave);
        ImprimirOcorrencia(vetorPalavras[i]->ListaDocs);
        printf("\n");
    }

    free(vetorPalavras);
} 
 
void LerPalavra(char *p, int Tam) { 
    char c; int i, j;
    j = 0;
    while (((c = getchar()) != '\n') && c != EOF && j < Tam - 1) {
        if (c != '\r') p[j++] = c;
    }
    p[j] = '\0';
    if (c != '\n' && c != EOF) {
        while ((c = getchar()) != '\n' && c != EOF);
    }
    for (i = j - 1; (i >= 0 && p[i] == ' '); i--) p[i] = '\0';
}