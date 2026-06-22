
/*
 * Grupo: Eduardo Braga[6568], Victor Laurindo[6580], Victor Costa[6589]
 * CCF212 - Algoritmos e Estruturas de Dados II
 * UFV Campus Florestal - 2026/1
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>          //isalpha(), tolower()
 #include "arquivo.h"


static char sw_banco[MAX_STOPWORDS][MAX_PALAVRA];    //Vetor interno de stopwords
static int  sw_count = 0;

int carregarStopWords(const char *arqStop) {       //Abre o arquivo e le uma palavra por linha, guardando em sw_banco[]
    FILE *f = fopen(arqStop, "r");
    if (f == NULL) {
        printf("Erro ao abrir stopwords: %s\n", arqStop);
        return -1;
    }

    sw_count = 0;
    char buf[MAX_PALAVRA];
    while (sw_count < MAX_STOPWORDS && fscanf(f, "%127s", buf) == 1) {
        //converte para minusculo antes de guardar 
        int i = 0;
        while (buf[i] != '\0') {
            buf[i] = (char)tolower((unsigned char)buf[i]);
            i++;
        }
        strncpy(sw_banco[sw_count], buf, MAX_PALAVRA - 1);
        sw_banco[sw_count][MAX_PALAVRA - 1] = '\0';
        sw_count++;
    }

    fclose(f);
    return sw_count;
}


int EhStopWord(const char *palavra) {    //Busca linear no vetor de stopwords. Retorna 1 se encontrou, 0 c.c.
    int i;
    for (i = 0; i < sw_count; i++) {
        if (strcmp(sw_banco[i], palavra) == 0) {
            return 1;
        }
    }
    return 0;
}

void arq_LimparPalavra(char *s) {         //Ex: "Hello, World!" ->  "hello" "world"
    int i, j;
    j = 0;
    for (i = 0; s[i] != '\0'; i++) {
        if (isalpha((unsigned char)s[i])) {
            s[j] = (char)tolower((unsigned char)s[i]);
            j++;
        }
    }
    s[j] = '\0';
}


int LerEntrada(const char *arqEntrada, InfoDoc docs[], int maxDocs) {
    FILE *f = fopen(arqEntrada, "r");
    if (f == NULL) {
        printf("Erro ao abrir arquivo de entrada: %s\n", arqEntrada);
        return -1;
    }

    int N;
    if (fscanf(f, "%d", &N) != 1) {
        fclose(f);
        return -1;
    }

    if (N > maxDocs) {
        N = maxDocs;
    }

    int i;
    char nome[MAX_NOME];
    for (i = 0; i < N; i++) {
        if (fscanf(f, "%255s", nome) != 1) break;

        // remove \r caso o arquivo tenha sido criado no Windows 
        int len = (int)strlen(nome);
        if (len > 0 && nome[len - 1] == '\r') {
            nome[len - 1] = '\0';
        }

        docs[i].idDoc              = i + 1;  // idDoc comeca em 1 
        docs[i].quantidadeTermosdistintos = 0;
        strncpy(docs[i].nomeArquivo, nome, MAX_NOME - 1);
        docs[i].nomeArquivo[MAX_NOME - 1] = '\0';
    }

    fclose(f);
    return i; // retorna quantos documentos foram lidos 
}


FILE *arq_AbrirFabula(const char *arqFabula) {     // Abre o arquivo da fabula e retorna o FILE* diretamente. 
    FILE *f = fopen(arqFabula, "r");
    if (f == NULL) {
        printf("Erro ao abrir fabula: %s\n", arqFabula);
    }
    return f;
}


int arq_ProxPalavra(FILE *arq, char *buf) {
    char tmp[MAX_PALAVRA];

    while (fscanf(arq, "%127s", tmp) == 1) {
        arq_LimparPalavra(tmp);

        // descarta se ficou vazio (era so pontuacao) 
        if (tmp[0] == '\0') {
            continue;
        }

        // descarta se e stopword 
        if (EhStopWord(tmp)) {
            continue;
        }

        // palavra valida: copia para buf e retorna 
        strncpy(buf, tmp, MAX_PALAVRA - 1);
        buf[MAX_PALAVRA - 1] = '\0';
        return 1;
    }

    return 0; //chegou no fim do arquivo 
}



void arq_FecharFabula(FILE *arq) {
    if (arq != NULL) {
        fclose(arq);
    }
}



Ocorrencia *arq_NovaOcorrencia(int idDoc) {
    Ocorrencia *p = (Ocorrencia *)malloc(sizeof(Ocorrencia));
    if (p == NULL) {
        printf("Erro: sem memoria para Ocorrencia\n");
        exit(1);
    }
    p->idDoc = idDoc;
    p->quantidade  = 1;
    p->Prox  = NULL;
    return p;
}


int arq_InserirOcorrencia(Ocorrencia **lista, int idDoc) {
    Ocorrencia *ant = NULL;
    Ocorrencia *cur = *lista;

    // avanca enquanto idDoc do no atual for menor que o idDoc buscado
    while (cur != NULL && cur->idDoc < idDoc) {
        ant = cur;
        cur = cur->Prox;
    }

    // documento ja existe na lista: so incrementa
    if (cur != NULL && cur->idDoc == idDoc) {
        cur->quantidade++;
        return 0;
    }

    // documento novo: cria no e encadeia 
    Ocorrencia *novo = arq_NovaOcorrencia(idDoc);
    novo->Prox = cur;
    if (ant == NULL) {
        *lista = novo; //insere no inicio
    } else {
        ant->Prox = novo;
    }
    return 1;
}
 
int arq_LiberarOcorrencia(Ocorrencia *lista) {
    Ocorrencia *tmp;
    while (lista != NULL) {
        tmp   = lista->Prox;
        free(lista);
        lista = tmp;
    }
    return 0;
}

void ImprimirOcorrencia(Ocorrencia *lista) {
    while (lista != NULL) {
        printf(" <%d,%d>", lista->quantidade, lista->idDoc);
        lista = lista->Prox;
    }
}








