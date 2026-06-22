/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Victor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#include "busca.h"
#include <math.h>

// Função auxiliar para o qsort ordenar os documentos da maior para a menor relevância
int ComparaRelevancia(const void *a, const void *b) {
    ResultadoBusca *resA = (ResultadoBusca *)a;
    ResultadoBusca *resB = (ResultadoBusca *)b;
    
    // Ordem decrescente
    if (resA->relevancia < resB->relevancia) return 1;
    if (resA->relevancia > resB->relevancia) return -1;
    return 0;
}

void BuscarFabulas(char termosConsulta[][N], int numTermosConsulta, TipoDicionario T, 
                    int N_TotalDocs, int *termosDistintosPorDoc, char nomesDocumentos[][100]) {
    
    // Vetor para acumular a soma dos pesos (w) de cada documento
    // O tamanho pode ser alocado dinamicamente baseado em N_TotalDocs
    float somaPesos[1000] = {0.0}; 
    int comparacoesInuteis = 0; // Usado apenas para satisfazer a assinatura da função Pesquisa

    // 1. Processa cada termo da consulta (t1, t2, ..., tq)
    for (int j = 0; j < numTermosConsulta; j++) {
        
        // Pesquisa o termo na Hash
        TipoApontador palavraNaHash = Pesquisa(termosConsulta[j], NULL, T, &comparacoesInuteis);
        
        if (palavraNaHash != NULL) {
            // O termo existe na coleção! 
            // Passo A: Calcular o d_j (número de documentos que contém o termo j)
            int d_j = 0;
            ApontadorOcorrencia auxDoc = palavraNaHash->Item.ListaDocs;
            while (auxDoc != NULL) {
                d_j++;
                auxDoc = auxDoc->Prox;
            }
            
            // Passo B: Calcular o IDF = log2(N) / d_j (Conforme exemplo do PDF)
            // Cuidado: usar log2 e fazer cast para float
            float idf = (float)log2(N_TotalDocs) / (float)d_j;
            
            // Passo C: Percorrer a lista de documentos de novo e calcular o peso W para cada um
            auxDoc = palavraNaHash->Item.ListaDocs;
            while (auxDoc != NULL) {
                int f_ji = auxDoc->quantidade;     // Frequência do termo no documento i
                int idDoc = auxDoc->idDoc;   // O documento i
                
                // Calcula w = f * (log2(N) / dj)
                float peso_w = (float)f_ji * idf;
                
                // Acumula o somatório de pesos para este documento
                somaPesos[idDoc] += peso_w;
                
                auxDoc = auxDoc->Prox;
            }
        }
    }
    
    // 2. Calcula a relevância final r(i) para todos os documentos e prepara para ordenação
    ResultadoBusca resultados[1000];
    int qtdResultados = 0;
    
    for (int i = 1; i <= N_TotalDocs; i++) { // Assumindo que idDoc começa em 1
        if (somaPesos[i] > 0.0) { // Só nos importamos com documentos que tem relevância > 0
            
            float n_i = (float)termosDistintosPorDoc[i];
            float r_i = (1.0 / n_i) * somaPesos[i];
            
            resultados[qtdResultados].idDoc = i;
            resultados[qtdResultados].relevancia = r_i;
            qtdResultados++;
        }
    }
    
    // 3. Ordena os resultados usando qsort
    if (qtdResultados > 0) {
        qsort(resultados, qtdResultados, sizeof(ResultadoBusca), ComparaRelevancia);
        
        // 4. Imprime o ranking
        printf("\n=== RESULTADO DA BUSCA ===\n");
        for (int k = 0; k < qtdResultados; k++) {
            int id = resultados[k].idDoc;
            printf("%dº Lugar: %s | Relevancia: %.4f\n", 
                   k+1, nomesDocumentos[id], resultados[k].relevancia);
        }
        printf("==========================\n");
    } else {
        printf("\nNenhum documento encontrado para a sua busca.\n");
    }
}