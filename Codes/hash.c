/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Vitor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#include "hash.h"

/*
 * Inicializa uma lista encadeada criando a celula-cabeca (no sentinela).
 */
void FLVazia(TipoLista *Lista) { 
    Lista->Primeiro = (TipoCelula *)malloc(sizeof(TipoCelula));
    Lista->Ultimo = Lista->Primeiro; 
    Lista->Primeiro->Prox = NULL;
}  

/*
 * Verifica se a lista de colisoes de uma posicao da Hash esta vazia.
 */
short Vazia(TipoLista Lista) { 
    return (Lista.Primeiro == Lista.Ultimo); 
}

/*
 * Insere um novo item (palavra) no final da lista encadeada de um bucket da Hash.
 */
void Ins(TipoItem x, TipoLista *Lista) { 
    Lista->Ultimo->Prox = (TipoCelula *)malloc(sizeof(TipoCelula));
    Lista->Ultimo = Lista->Ultimo->Prox; 
    Lista->Ultimo->Item = x;
    Lista->Ultimo->Prox = NULL;
}  

/*
 * Gera pesos aleatorios para melhorar a dispersao das chaves na Hash.
 * Utiliza o tempo atual do sistema em microssegundos como semente.
 */
void GeraPesos(TipoPesos p) { 
    int i;
    struct timeval semente;
    gettimeofday(&semente, NULL); 
    srand((int)(semente.tv_sec + 1000000*semente.tv_usec));
    for (i = 0; i < N; i++)
        p[i] =  1 + (int) (10000.0 * rand() / (RAND_MAX + 1.0));
}

/*
 * Funcao de Espalhamento (Hash).
 * Multiplica o valor ASCII de cada caractere da palavra pelo seu peso
 * correspondente e retorna o modulo do somatorio pelo tamanho da Tabela (M).
 */
unsigned int h(TipoChave Chave, TipoPesos p) { 
    int i; 
    unsigned int Soma = 0; 
    int comp = strlen(Chave);
    
    // Trava de seguranca para evitar acesso indevido a memoria caso 
    // a palavra ultrapasse o tamanho maximo do vetor de pesos (N)
    if (comp > N) comp = N; 

    for (i = 0; i < comp; i++) 
        Soma += (unsigned int)Chave[i] * p[i];
    return (Soma % M);
}

/*
 * Prepara a Tabela Hash para uso inicializando todos os seus M buckets.
 */
void Inicializa(TipoDicionario T) { 
    int i;
    for (i = 0; i < M; i++) FLVazia(&T[i]);
}

// ====================================================================
// FUNÇÕES ESPECÍFICAS PARA O TRABALHO PRÁTICO
// ====================================================================

/*
 * Pesquisa uma palavra na Tabela Hash.
 * Retorna o apontador para a celula exata onde a palavra esta, ou NULL se nao existir.
 * Incrementa o contador de comparacoes para analise de desempenho.
 */
TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoDicionario T, int *comparacoes) { 
    unsigned int i = h(Ch, p);
    TipoApontador Ap = T[i].Primeiro->Prox;
    
    // Percorre a lista de colisoes do respectivo bucket
    while (Ap != NULL) {
        (*comparacoes)++; // Incrementa o medidor de esforco computacional
        
        // Verifica se a chave atual eh igual a palavra buscada
        if (strcmp(Ch, Ap->Item.Chave) == 0) {
            return Ap; // Palavra encontrada
        }
        Ap = Ap->Prox;
    }
    return NULL; // Pesquisa sem sucesso (palavra nao existe)
}  

/*
 * Insere uma nova ocorrencia de palavra no Indice Invertido.
 * Trata dois cenarios: insercao de uma palavra inedita ou atualizacao da
 * frequencia/documento de uma palavra que ja esta na Hash.
 */
void Insere(TipoChave Ch, int idDoc, TipoPesos p, TipoDicionario T, int *comparacoes) { 
    TipoApontador Ap = Pesquisa(Ch, p, T, comparacoes);
    
    if (Ap == NULL) {
        // CASO 1: A palavra ainda nao existe na Hash
        TipoItem x;
        strcpy(x.Chave, Ch);
        x.ListaDocs = NULL; // Inicializa a lista secundaria de documentos vazia
        
        // Delega para o TAD arquivo a criacao do noh com o idDoc atual
        arq_InserirOcorrencia(&x.ListaDocs, idDoc);
        
        // Insere a nova palavra (com seu doc) na Tabela Hash
        Ins(x, &T[h(Ch, p)]);
    } else {
        // CASO 2: A palavra ja existe na Hash. 
        // Atualizamos apenas a sublista de documentos dela chamando o TAD arquivo.
        arq_InserirOcorrencia(&(Ap->Item.ListaDocs), idDoc);
    }
} 

/*
 * Funcao auxiliar utilizada pelo qsort para comparar duas chaves alfabeticamente.
 */
int ComparaPalavrasHash(const void *a, const void *b) {
    TipoItem *itemA = *(TipoItem **)a;
    TipoItem *itemB = *(TipoItem **)b;
    return strcmp(itemA->Chave, itemB->Chave);
}

/*
 * Imprime todo o Indice Invertido em ordem alfabetica e com suas 
 * respectivas listas de ocorrencias (<qtde, idDoc>).
 */
void ImprimeIndiceInvertido(TipoDicionario Tabela) { 
    // Passo 1: Conta o total de palavras cadastradas em toda a Hash
    int totalPalavras = 0;
    for (int i = 0; i < M; i++) {
        TipoApontador Aux = Tabela[i].Primeiro->Prox;
        while (Aux != NULL) {
            totalPalavras++;
            Aux = Aux->Prox;
        }
    }

    if (totalPalavras == 0) return;

    // Passo 2: Aloca dinamicamente um vetor de ponteiros para as palavras
    TipoItem **vetorPalavras = (TipoItem **)malloc(totalPalavras * sizeof(TipoItem *));
    int idx = 0;
    
    // Preenche o vetor com as referencias dos itens que estao na Hash
    for (int i = 0; i < M; i++) {
        TipoApontador Aux = Tabela[i].Primeiro->Prox;
        while (Aux != NULL) {
            vetorPalavras[idx++] = &(Aux->Item);
            Aux = Aux->Prox;
        }
    }

    // Passo 3: Ordena o vetor temporario alfabeticamente
    qsort(vetorPalavras, totalPalavras, sizeof(TipoItem *), ComparaPalavrasHash);

    // Passo 4: Imprime o indice perfeitamente ordenado
    for (int i = 0; i < totalPalavras; i++) {
        printf("%s", vetorPalavras[i]->Chave);
        // Usa a funcao do TAD arquivo para imprimir o formato <qtde, idDoc>
        ImprimirOcorrencia(vetorPalavras[i]->ListaDocs);
        printf("\n");
    }

    // Libera a memoria temporaria usada para ordenacao
    free(vetorPalavras);
} 
 
/*
 * Funcao auxiliar de entrada de dados para contornar problemas de 
 * formatacao de texto (como o \\r de arquivos criados no Windows).
 */
void LerPalavra(char *p, int Tam) { 
    char c; int i, j;
    j = 0;
    while (((c = getchar()) != '\n') && c != EOF && j < Tam - 1) {
        if (c != '\r') p[j++] = c;
    }
    p[j] = '\0';
    
    // Limpa o buffer residual se a palavra for maior que o tamanho maximo
    if (c != '\n' && c != EOF) {
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    // Desconsidera eventuais espacos em branco inseridos no final da cadeia
    for (i = j - 1; (i >= 0 && p[i] == ' '); i--) p[i] = '\0';
}

/*
 * Varre a Tabela Hash para desalocar toda a memoria dinamica utilizada,
 * prevenindo vazamento de memoria ao finalizar o programa.
 */
void LiberaHash(TipoDicionario T) {
    int i;
    for (i = 0; i < M; i++) {
        TipoApontador ap = T[i].Primeiro->Prox;
        while (ap != NULL) {
            TipoApontador prox = ap->Prox;
            // Desaloca a sublista de ocorrencias pertencente a palavra atual
            arq_LiberarOcorrencia(ap->Item.ListaDocs);
            free(ap);
            ap = prox;
        }
        // Desaloca a celula-cabeca instanciada na funcao FLVazia 
        free(T[i].Primeiro);
        T[i].Primeiro = NULL;
        T[i].Ultimo   = NULL;
    }
}

/*
 * Calcula com exatidao o uso de memoria em bytes da Tabela Hash em tempo de execucao.
 * Utilizado para os testes de analise comparativa do trabalho.
 */
size_t MemoriaHash(TipoDicionario T) {
    size_t mem = 0;
    int i;
    for (i = 0; i < M; i++) {
        mem += sizeof(TipoCelula); // Custo da celula-cabeca fixa de cada bucket
        TipoApontador ap = T[i].Primeiro->Prox;
        
        while (ap != NULL) {
            mem += sizeof(TipoCelula); // Custo do noh que aloca a palavra
            
            Ocorrencia *oc = ap->Item.ListaDocs;
            while (oc != NULL) {
                mem += sizeof(Ocorrencia); // Custo de cada no da lista de documentos
                oc = oc->Prox;
            }
            ap = ap->Prox;
        }
    }
    return mem;
}