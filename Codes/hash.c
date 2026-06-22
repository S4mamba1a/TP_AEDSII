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

void AdicionaOuAtualizaOcorrencia(ApontadorOcorrencia *lista, int idDoc) {
    ApontadorOcorrencia aux = *lista;
    ApontadorOcorrencia ant = NULL;

    // Procura o documento na lista da palavra
    while (aux != NULL && aux->idDoc != idDoc) {
        ant = aux;
        aux = aux->Prox;
    }

    if (aux != NULL && aux->idDoc == idDoc) {
        // Documento já existe na lista: apenas incrementa a frequência (TF)
        aux->qtde++;
    } else {
        // Primeira vez que a palavra aparece neste documento
        ApontadorOcorrencia nova = (ApontadorOcorrencia)malloc(sizeof(CelulaOcorrencia));
        nova->idDoc = idDoc;
        nova->qtde = 1;
        nova->Prox = NULL;

        if (ant == NULL) {
            *lista = nova; // Insere no início (primeiro doc da palavra)
        } else {
            ant->Prox = nova; // Insere no final (mantém a ordem de chegada dos docs)
        }
    }
}

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
        
        // Cria a primeira ocorrência para o documento atual
        AdicionaOuAtualizaOcorrencia(&x.ListaDocs, idDoc);
        
        // Insere a nova palavra na Tabela Hash
        Ins(x, &T[h(Ch, p)]);
    } else {
        // CASO 2: A palavra já existe! Apenas atualizamos a lista de documentos dela
        AdicionaOuAtualizaOcorrencia(&(Ap->Item.ListaDocs), idDoc);
    }
} 

void ImprimeIndiceInvertido(TipoDicionario Tabela) { 
    int i;
    for (i = 0; i < M; i++) { 
        TipoApontador AuxPalavra = Tabela[i].Primeiro->Prox;
        
        while (AuxPalavra != NULL) {
            printf("%s ", AuxPalavra->Item.Chave);
            
            // Percorre a lista de documentos dessa palavra
            ApontadorOcorrencia AuxDoc = AuxPalavra->Item.ListaDocs;
            while (AuxDoc != NULL) {
                printf("<%d, %d> ", AuxDoc->qtde, AuxDoc->idDoc);
                AuxDoc = AuxDoc->Prox;
            }
            printf("\n");
            
            AuxPalavra = AuxPalavra->Prox;
        }
    }
} 
 
void LerPalavra(char *p, int Tam) { 
    char c; int i, j;
    fflush(stdin); j=0;
    while (((c=getchar())!='\n') && j<Tam-1) p[j++]= c;
    p[j]='\0';
    while(c != '\n') c=getchar();
    for(i=j-1;(i>=0 && p[i]==' ');i--) p[i]='\0';
}