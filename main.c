/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Victor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"
#include "hash.h"
#include "patricia.h"
#include "busca.h"

// Funcao auxiliar para limpar o buffer com seguranca e evitar loops infinitos
void LimpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    InfoDoc docs[MAX_DOCS];
    memset(docs, 0, sizeof(docs)); // Zera tudo logo no inicio pra evitar lixo de memoria no TF-IDF

    int numDocs = 0;
    int indicesConstruidos = 0;

    TipoDicionario TabelaHash;
    TipoPesos pesosHash;
    Apontador raizPatricia;

    // Inicializacao das estruturas
    Inicializa(TabelaHash);
    GeraPesos(pesosHash);
    InicializaArvore(&raizPatricia);

    if (carregarStopWords("Fabulas/stopwords.txt") < 0) {
        printf("Aviso: Nao foi possivel carregar 'stopwords.txt'. Verifique se o arquivo existe na pasta.\n");
    }

    int opcao;
    char nomeArqEntrada[MAX_NOME];

    do {
        printf("\n-------------------------------------------------------\n");
        printf("          MAQUINA DE BUSCA - BIBLIOTECA DE FABULAS       \n");
        printf("-------------------------------------------------------\n");
        printf("1. (a) Receber arquivo de entrada (ex: Fabulas/entrada.txt)\n");
        printf("2. (b) Construir indices invertidos (PATRICIA e HASH)\n");
        printf("3. (c) Imprimir indices invertidos em ordem alfabetica\n");
        printf("4. (d) Realizar buscas por termo(s)\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            LimpaBuffer(); 
            opcao = -1;
        }

        switch(opcao) {
            case 1:
                printf("\nDigite o nome do arquivo de entrada: ");
                scanf("%255s", nomeArqEntrada);
                
                // Se o usuario carregar um novo arquivo, limpamos as estruturas pra reconstruir do zero
                if (indicesConstruidos) {
                    printf("-> Limpando indices anteriores para nova leitura...\n");
                    LiberaArvorePatricia(raizPatricia);
                    
                    Inicializa(TabelaHash);
                    InicializaArvore(&raizPatricia);
                    indicesConstruidos = 0;
                }
                
                memset(docs, 0, sizeof(docs)); // Zera o array de docs novamente
                numDocs = LerEntrada(nomeArqEntrada, docs, MAX_DOCS);
                
                if (numDocs > 0) {
                    printf("-> Sucesso: %d documentos lidos da lista.\n", numDocs);
                } else {
                    printf("-> Erro ao ler o arquivo de entrada.\n");
                }
                break;

            case 2:
                if (numDocs <= 0) {
                    printf("\n-> Erro: Leia o arquivo de entrada primeiro (Opcao 1).\n");
                    break;
                }
                if (indicesConstruidos) {
                    printf("\n-> Aviso: Os indices ja estao construidos e atualizados!\n");
                    break;
                }

                printf("\nConstruindo indices... Processando arquivos...\n");
                int totalCompHash = 0;
                int totalCompPatricia = 0;

                for (int i = 0; i < numDocs; i++) {
                    char caminhoFabula[MAX_NOME + 20];
                    sprintf(caminhoFabula, "Fabulas/%s", docs[i].nomeArquivo);
                    FILE *f = arq_AbrirFabula(caminhoFabula);
                    if (f == NULL) {
                        printf("-> Erro: Nao foi possivel abrir a fabula '%s'.\n", docs[i].nomeArquivo);
                        continue;
                    }
                    char bufferTitulo[MAX_NOME];
                    docs[i].titulo[0] = '\0';           // Inicializa o titulo como vazio

                    while (fgets(bufferTitulo, MAX_NOME, f) != NULL) {
                        
                        bufferTitulo[strcspn(bufferTitulo, "\r\n")] = '\0';         //Limpa quebras de linha
                        
                        if (strlen(bufferTitulo) > 0) {      //Se a linha não for vazia, achou o título
                            strncpy(docs[i].titulo, bufferTitulo, MAX_NOME - 1);
                            docs[i].titulo[MAX_NOME - 1] = '\0';
                            break;    //Saí do while assim que fechar o titulo
                        }
                    }

                    if (strlen(docs[i].titulo) == 0) { 
                        strcpy(docs[i].titulo, "Sem Titulo");
                    }

                    rewind(f);           //Volta o cursor do arquivo para o início para ler as palavras

                    char palavra[MAX_PALAVRA];
                    while (arq_ProxPalavra(f, palavra)) {
                        
                        int compH = 0;
                        Insere(palavra, docs[i].idDoc, pesosHash, TabelaHash, &compH);
                        totalCompHash += compH;

                        int compP = 0;
                        InserePatricia(palavra, docs[i].idDoc, &raizPatricia, docs, &compP);
                        totalCompPatricia += compP;
                    }
                    arq_FecharFabula(f);
                }
                indicesConstruidos = 1;
                printf("-> Indices construidos com sucesso!\n");
                printf("   [Estatistica] Comparacoes totais na insercao HASH: %d\n", totalCompHash);
                printf("   [Estatistica] Comparacoes totais na insercao PATRICIA: %d\n", totalCompPatricia);
                break;

            case 3:
                if (!indicesConstruidos) {
                    printf("\n-> Erro: Construa os indices primeiro (Opcao 2).\n");
                    break;
                }
                printf("\n--- INDICE INVERTIDO: TABELA HASH ---\n");
                ImprimeIndiceInvertido(TabelaHash);
                
                printf("\n--- INDICE INVERTIDO: ARVORE PATRICIA ---\n");
                ImprimeIndiceInvertidoPatricia(raizPatricia);
                break;

            case 4:
                if (!indicesConstruidos) {
                    printf("\n-> Erro: Construa os indices primeiro (Opcao 2).\n");
                    break;
                }
                
                printf("\nDigite o(s) termo(s) de busca separados por espaco: ");
                LimpaBuffer();
                
                char linhaBusca[512];
                if (fgets(linhaBusca, sizeof(linhaBusca), stdin) == NULL) break;
                
                char termos[20][MAX_PALAVRA];
                int numTermos = 0;
                
                // Quebra a string digitada pegando cada pedaço separado por espaco
                char *pedaco = strtok(linhaBusca, " \n\r");
                while (pedaco != NULL && numTermos < 20) {
                    arq_LimparPalavra(pedaco); 
                    
                    if (strlen(pedaco) > 0 && !EhStopWord(pedaco)) {
                        // Uso de strncpy para nao estourar a memoria do vetor
                        strncpy(termos[numTermos], pedaco, MAX_PALAVRA - 1);
                        termos[numTermos][MAX_PALAVRA - 1] = '\0';
                        numTermos++;
                    }
                    pedaco = strtok(NULL, " \n\r");
                }
                
                if (numTermos == 0) {
                    printf("-> Nenhum termo valido para busca. (Verifique se digitou apenas stopwords).\n");
                } else {
                    BuscarNaHash(termos, numTermos, TabelaHash, pesosHash, numDocs, docs);
                    BuscarNaPatricia(termos, numTermos, raizPatricia, numDocs, docs);
                }
                break;

            case 0:
                printf("\nSaindo da maquina de busca. Ate logo!\n");
                break;

            default:
                printf("\n-> Opcao invalida!\n");
        }
    } while (opcao != 0);

    // Limpeza completa de memoria no fim da execucao
    LiberaArvorePatricia(raizPatricia);

    return 0;
}