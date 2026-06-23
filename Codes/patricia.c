/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Victor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

#include "patricia.h"

// Retorna o caracter da string na posicao 'indice' evitando segmentation fault
char GetChar(const char *palavra, int indice) {
    if (indice < (int)strlen(palavra)) {
        return palavra[indice];
    }
    return '\0';
}

// Substitui o strdup para garantir compatibilidade com compiladores estritos (ANSI C)
char *DuplicaString(const char *src) {
    size_t tam = strlen(src) + 1;
    char *dst = (char *)malloc(tam);
    if (dst == NULL) {
        printf("Erro: sem memoria em DuplicaString\n");
        exit(1);
    }
    memcpy(dst, src, tam);
    return dst;
}

// Atualiza a lista de ocorrencias mantendo a ordem crescente de idDoc
// Retorna 1 se for a primeira vez da palavra no doc, ou 0 se ja existia
int AtualizaListaDoc(Ocorrencia **lista, int idDoc) {
    Ocorrencia *atual = *lista;
    Ocorrencia *ant = NULL;

    while (atual != NULL && atual->idDoc < idDoc) {
        ant = atual;
        atual = atual->Prox;
    }

    if (atual != NULL && atual->idDoc == idDoc) {
        atual->quantidade++;
        return 0; // Documento ja existia, so soma a quantidade
    }

    Ocorrencia *nova = (Ocorrencia *)malloc(sizeof(Ocorrencia));
    if (nova == NULL) {
        printf("Erro: sem memoria para Ocorrencia\n");
        exit(1);
    }
    nova->idDoc = idDoc;
    nova->quantidade = 1;
    nova->Prox = atual;

    if (ant == NULL) *lista = nova;
    else ant->Prox = nova;

    return 1; // Documento novo para essa palavra
}

// Cria um no externo (folha)
Apontador CriaNoExterno(char *palavra, int idDoc) {
    Apontador p = (Apontador)malloc(sizeof(struct PATNo));
    if (p == NULL) {
        printf("Erro: sem memoria para no externo\n");
        exit(1);
    }
    p->tipo = Externo;
    p->NO.NExterno.palavra = DuplicaString(palavra);
    p->NO.NExterno.listaDocs = NULL;
    AtualizaListaDoc(&(p->NO.NExterno.listaDocs), idDoc);
    return p;
}

// Cria um no interno (decisao)
Apontador CriaNoInterno(Apontador esq, Apontador dir, int indice, char charCmp) {
    Apontador p = (Apontador)malloc(sizeof(struct PATNo));
    if (p == NULL) {
        printf("Erro: sem memoria para no interno\n");
        exit(1);
    }
    p->tipo = Interno;
    p->NO.NInterno.indice = indice;
    p->NO.NInterno.charCmp = charCmp;
    p->NO.NInterno.esq = esq;
    p->NO.NInterno.dir = dir;
    return p;
}

void InicializaArvore(Apontador *raiz) {
    *raiz = NULL;
}

// Pesquisa a palavra descendo pelos nos internos
Apontador PesquisaPatricia(char *palavra, Apontador p, int *comparacoes) {
    if (p == NULL) return NULL;

    while (p->tipo == Interno) {
        (*comparacoes)++;
        char c = GetChar(palavra, p->NO.NInterno.indice);
        if (c < p->NO.NInterno.charCmp)
            p = p->NO.NInterno.esq;
        else
            p = p->NO.NInterno.dir;
    }

    (*comparacoes)++; 
    if (strcmp(palavra, p->NO.NExterno.palavra) == 0)
        return p;
        
    return NULL;
}

// Acha o ponto de insercao correto e reorganiza a arvore
Apontador InsereEntre(char *palavra, int idDoc, Apontador p, int idxDif, char charDif) {
    if (p->tipo == Externo || p->NO.NInterno.indice > idxDif) {
        Apontador novoExt = CriaNoExterno(palavra, idDoc);
        char c = GetChar(palavra, idxDif);
        if (c < charDif)
            return CriaNoInterno(novoExt, p, idxDif, charDif);
        else
            return CriaNoInterno(p, novoExt, idxDif, charDif);
    }

    char c = GetChar(palavra, p->NO.NInterno.indice);
    if (c < p->NO.NInterno.charCmp)
        p->NO.NInterno.esq = InsereEntre(palavra, idDoc, p->NO.NInterno.esq, idxDif, charDif);
    else
        p->NO.NInterno.dir = InsereEntre(palavra, idDoc, p->NO.NInterno.dir, idxDif, charDif);
        
    return p;
}

// Insere a palavra e atualiza a quantidade de termos distintos do documento pro TF-IDF
Apontador InserePatricia(char *palavra, int idDoc, Apontador *raiz, InfoDoc docs[], int *comparacoes) {
    if (*raiz == NULL) {
        *raiz = CriaNoExterno(palavra, idDoc);
        docs[idDoc - 1].quantidadeTermosdistintos++;
        return *raiz;
    }

    int compAux = 0;
    Apontador noExistente = PesquisaPatricia(palavra, *raiz, &compAux);
    *comparacoes += compAux;

    if (noExistente != NULL) {
        // Se for doc novo, aumenta termos distintos
        int docNovo = AtualizaListaDoc(&(noExistente->NO.NExterno.listaDocs), idDoc);
        if (docNovo) {
            docs[idDoc - 1].quantidadeTermosdistintos++;
        }
        return *raiz;
    }

    // Desce ate a folha candidata
    Apontador folha = *raiz;
    while (folha->tipo == Interno) {
        (*comparacoes)++;
        char c = GetChar(palavra, folha->NO.NInterno.indice);
        if (c < folha->NO.NInterno.charCmp)
            folha = folha->NO.NInterno.esq;
        else
            folha = folha->NO.NInterno.dir;
    }
    (*comparacoes)++;

    // Acha o indice do primeiro caracter diferente
    int i = 0;
    while (GetChar(palavra, i) == GetChar(folha->NO.NExterno.palavra, i) && GetChar(palavra, i) != '\0') {
        i++;
    }

    // Pega o maior caracter pra ser o pivo
    char charPalavra = GetChar(palavra, i);
    char charFolha = GetChar(folha->NO.NExterno.palavra, i);
    char charCmp = (charPalavra > charFolha) ? charPalavra : charFolha;

    docs[idDoc - 1].quantidadeTermosdistintos++;

    *raiz = InsereEntre(palavra, idDoc, *raiz, i, charCmp);
    return *raiz;
}



// Funcoes de impressao ordenadas

// Conta os nos externos pra alocar o vetor no tamanho exato
int ContarFolhas(Apontador p) {
    if (p == NULL) return 0;
    if (p->tipo == Externo) return 1;
    return ContarFolhas(p->NO.NInterno.esq) + ContarFolhas(p->NO.NInterno.dir);
}

// Preenche o vetor com os ponteiros pras folhas
void ColetarFolhas(Apontador p, Apontador *vetorFolhas, int *posicao) {
    if (p == NULL) return;
    if (p->tipo == Interno) {
        ColetarFolhas(p->NO.NInterno.esq, vetorFolhas, posicao);
        ColetarFolhas(p->NO.NInterno.dir, vetorFolhas, posicao);
    } else {
        vetorFolhas[(*posicao)++] = p;
    }
}

// Compara strings pro qsort
int ComparaPalavras(const void *a, const void *b) {
    const Apontador *na = (const Apontador *)a;
    const Apontador *nb = (const Apontador *)b;
    return strcmp((*na)->NO.NExterno.palavra, (*nb)->NO.NExterno.palavra);
}

// Imprime formato <qtde, idDoc>
void ImprimeListaDocsPatricia(Ocorrencia *lista) {
    while (lista != NULL) {
        printf("<%d,%d> ", lista->quantidade, lista->idDoc);
        lista = lista->Prox;
    }
    printf("\n");
}

// Imprime todo o indice invertido em ordem alfabetica
void ImprimeIndiceInvertidoPatricia(Apontador raiz) {
    int total = ContarFolhas(raiz);
    if (total == 0) return;

    Apontador *vetorFolhas = (Apontador *)malloc(total * sizeof(Apontador));
    if (vetorFolhas == NULL) {
        printf("Erro: sem memoria para imprimir indice\n");
        return;
    }

    int posicao = 0;
    ColetarFolhas(raiz, vetorFolhas, &posicao);
    
    qsort(vetorFolhas, total, sizeof(Apontador), ComparaPalavras);

    for (int i = 0; i < total; i++) {
        printf("%s ", vetorFolhas[i]->NO.NExterno.palavra);
        ImprimeListaDocsPatricia(vetorFolhas[i]->NO.NExterno.listaDocs);
    }

    free(vetorFolhas);
}

void LiberaArvorePatricia(Apontador p) {
    if (p == NULL) return;
    if (p->tipo == Interno) {
        LiberaArvorePatricia(p->NO.NInterno.esq);
        LiberaArvorePatricia(p->NO.NInterno.dir);
    } else {
        free(p->NO.NExterno.palavra);
        arq_LiberarOcorrencia(p->NO.NExterno.listaDocs);
    }
    free(p);
}