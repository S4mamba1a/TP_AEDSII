/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Victor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // Necessário para a função log()
#include "busca.h"

// Função auxiliar para o qsort ordenar de forma DECRESCENTE por relevância
int ComparaRelevancia(const void *a, const void *b) {
    ResultadoBusca *resA = (ResultadoBusca *)a;
    ResultadoBusca *resB = (ResultadoBusca *)b;
    
    if (resA->relevancia < resB->relevancia) return 1;
    if (resA->relevancia > resB->relevancia) return -1;
    return 0; 
}

void BuscarNaHash(char termosConsulta[][MAX_PALAVRA], int numTermos, TipoDicionario T, TipoPesos pesosHash, int numDocs, InfoDoc docs[]){
    // Vetor para acumular a soma dos pesos de cada documento.
    float somaPesos[MAX_DOCS + 1];        // Como idDoc começa em 1, criamos com tamanho MAX_DOCS + 1.
    for (int i = 0; i <= MAX_DOCS; i++) {
        somaPesos[i] = 0.0;
    }

    for (int j = 0; j < numTermos; j++) {    // Processa cada termo digitado pelo usuário
        char *termo = termosConsulta[j];
        int comparacoes = 0;
        TipoApontador ap = Pesquisa(termosConsulta[j], pesosHash, T, &comparacoes);    // Pesquisa na Hash. O Pesquisa do Ziviani retorna a célula ANTERIOR à que contém o item.
        
        if (ap != NULL && ap->Prox != NULL) {
            Ocorrencia *auxDoc = ap->Prox->Item.ListaDocs;   // Encontrou a palavra! Pegamos a lista de ocorrências (documentos) dela
            
            int d_k = 0;               // Passo 1: Descobrir o d_k (quantos documentos possuem essa palavra)
            Ocorrencia *contar = auxDoc;
            while (contar != NULL) {
                d_k++;
                contar = contar->Prox;
            }
            
            // Passo 2: Se a palavra está em algum doc, calcula o peso w para cada um deles
            if (d_k > 0) {
                while (auxDoc != NULL) {
                    int idDoc = auxDoc->idDoc;
                    int f_ki = auxDoc->quantidade;  // Frequência do termo neste documento
                    
                    // Fórmula: w_ki = f_ki * log(N / d_k)
                    float peso_w = (float)f_ki * log2((float)numDocs / (float)d_k);
                    
                    somaPesos[idDoc] += peso_w;
                    auxDoc = auxDoc->Prox;
                }
            }
        }
    }

    // Passo 3: Calcular a relevância final r_i = (1 / n_i) * somatorio(w)
    ResultadoBusca ranking[MAX_DOCS];
    int qtdResultados = 0;

    for (int i = 1; i <= numDocs; i++) {
        if (somaPesos[i] > 0.0) {
            int n_i = docs[i - 1].quantidadeTermosdistintos;  // n_i do documento
            
            if (n_i > 0) {
                ranking[qtdResultados].idDoc = i;
                ranking[qtdResultados].relevancia = (1.0 / (float)n_i) * somaPesos[i];
                qtdResultados++;
            }
        }
    }

    // Passo 4: Ordenar e exibir o ranking obtido pela Hash
    printf("\n========================================");
    printf("\n   RANKING DE RELEVANCIA (TABELA HASH)   ");
    printf("\n========================================\n");
    
    if (qtdResultados > 0) {
        qsort(ranking, qtdResultados, sizeof(ResultadoBusca), ComparaRelevancia);
        
        for (int k = 0; k < qtdResultados; k++) {
            int id = ranking[k].idDoc;
            printf("%d. Fabula: %-20s | Relevancia: %.4f\n", k + 1, docs[id - 1].nomeArquivo, ranking[k].relevancia);
        }
    }else{
        printf("Nenhuma fabula correspondente encontrada.\n");
    }
}

void BuscarNaPatricia(char termosConsulta[][MAX_PALAVRA], int numTermos, Apontador raizPatricia, int numDocs, InfoDoc docs[]){
    
    float somaPesos[MAX_DOCS + 1];
    for (int i = 0; i <= MAX_DOCS; i++) {
        somaPesos[i] = 0.0;
    }

    // Processa cada termo digitado pelo usuário
    for (int j = 0; j < numTermos; j++) {
        char *termo = termosConsulta[j];
        // Cria a variável para contar as comparações dessa palavra específica
        int comparacoes = 0;
        // Pesquisa na Árvore PATRICIA
        Apontador no = PesquisaPatricia(termo, raizPatricia, &comparacoes);
        
        // Verifica se achou, se o nó é folha (Externo) e se a palavra bate exatamente
        if (no != NULL && no->tipo == Externo && strcmp(no->NO.NExterno.palavra, termo) == 0) {
            Ocorrencia *auxDoc = no->NO.NExterno.listaDocs;
            
            // Passo 1: Descobrir o d_k
            int d_k = 0;
            Ocorrencia *contar = auxDoc;
            while (contar != NULL) {
                d_k++;
                contar = contar->Prox;
            }
            
            // Passo 2: Calcular o peso w para cada documento
            if (d_k > 0) {
                while (auxDoc != NULL) {
                    int idDoc = auxDoc->idDoc;
                    int f_ki = auxDoc->quantidade;
                    
                    float peso_w = (float)f_ki * log2((float)numDocs / (float)d_k);
                    
                    somaPesos[idDoc] += peso_w;
                    auxDoc = auxDoc->Prox;
                }
            }
        }
    }

    // Passo 3: Calcular a relevância final
    ResultadoBusca ranking[MAX_DOCS];
    int qtdResultados = 0;

    for (int i = 1; i <= numDocs; i++) {
        if (somaPesos[i] > 0.0) {
            int n_i = docs[i - 1].quantidadeTermosdistintos;
            
            if (n_i > 0) {
                ranking[qtdResultados].idDoc = i;
                ranking[qtdResultados].relevancia = (1.0 / (float)n_i) * somaPesos[i];
                qtdResultados++;
            }
        }
    }

    // Passo 4: Ordenar e exibir o ranking obtido pela PATRICIA
    printf("\n========================================");
    printf("\n   RANKING DE RELEVANCIA (PATRICIA)     ");
    printf("\n========================================\n");
    
    if (qtdResultados > 0) {
        qsort(ranking, qtdResultados, sizeof(ResultadoBusca), ComparaRelevancia);
        
        for (int k = 0; k < qtdResultados; k++) {
            int id = ranking[k].idDoc;
            printf("%d. Fabula: %-20s | Relevancia: %.4f\n", k + 1, docs[id - 1].nomeArquivo, ranking[k].relevancia);
        }
    } else {
        printf("Nenhuma fabula correspondente encontrada.\n");
    }
}
