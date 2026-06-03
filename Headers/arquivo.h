/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Victor Costa[****]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#ifndef arquivo_h
#define arquivo_h

#define MAX_DOCS 100            //Máximo de documentos suportados
#define MAX_PALAVRA 128        //Tamanho máximo de uma palavra indexada
#define MAX_NOME 256            //Tamanho máximo do nome de um arquivo  
#define MAX_STOPWORDS 400         //Máximo de stopwords carregadas em memória

typedef struct Ocorrencia{             //Representa uma ocorrencia de uma palavra em um documento. A lista é mantida em ordem crescente de idDoc          
    int idDoc;
    int quantidade;                //Número de vezes que a palavra aparece no documento
    struct Ocorrencia* prox;          //Ponteiro para o próximo nó
}Ocorrencia;

typedef struct{       //Associa o idDoc ao nome do arquivo e ao numero de termos distintos              
    int idDoc;
    char nomeArquivo[MAX_NOME];       //Nome do arquivo da fábula 
    int quantidadeTermosdistintos;     //Termos distintos após stopwords, necessário para o cálculo do TF-IDF 
}InfoDoc;

int carregarStopWords(const char *arqStopWord);   //const char para não modificar a string passada

int EhStopWord(const char *palavra);

int LerEntrada(const char *arqEntrada, InfoDoc docs[], int maxDocs);   //Lê o arquivo de entrada (formato: N seguido de N nomes de arquivo). Preenche docs[] e retorna o número de documentos lidos, -1 se erro.

void *arq_AbrirFabula(const char *arqFabula);  //Abre o arquivo da fábula para leitura

int arq_ProxPalavra(void *handle, char *buf);   //Le a proxima palavra valida: aplica limpeza e filtra stopwords.

void  arq_FecharFabula(void *handle);

void arq_LimparPalavra(char *string);

Ocorrencia* arq_NovaOcorrencia(int idDoc);    //Aloca e inicializa um novo Posting com qtde=1 para idDoc.

int arq_InserirOcorrencia(Ocorrencia **lista, int idDoc);   //Insere ou atualiza idDoc na lista encadeada, mantendo ordem por idDoc. Retorna 1 se criou novo no, 0 se apenas incrementou qtde
  
int arq_LiberarOcorrencia(Ocorrencia *lista);   //Libera todos os nós

void ImprimeOcorrencia(Ocorrencia *lista);  //Imprime a lista no formato: <qtde,idDoc> <qtde,idDoc>




#endif