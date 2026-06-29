# Máquina de Busca e Recuperação de Informação (TF-IDF)

Trabalho Prático desenvolvido para a disciplina CCF 212 - Algoritmos e Estruturas de Dados II.

Este projeto implementa um motor de busca em C capaz de indexar documentos de texto e processar consultas do usuário, retornando um ranking de relevância baseado no cálculo estatístico TF-IDF. O Índice Invertido é estruturado utilizando uma Tabela Hash e uma Árvore PATRICIA para fins de comparação de desempenho.

## Estrutura de Diretórios

A organização dos arquivos no repositório segue a estrutura abaixo:

* Codes/   : Arquivos de implementação e código-fonte (.c).
* Headers/ : Arquivos de cabeçalho (.h).
* Fabulas/ : Diretório contendo os textos a serem indexados, o arquivo base (entrada.txt) e as palavras ignoradas (stopwords.txt).
* main.c   : Arquivo principal que orquestra a execução do programa.
* Makefile : Script de automação para compilação do projeto.

## Compilação e Execução

O projeto inclui um "Makefile" configurado para compilar os códigos dos subdiretórios corretamente e realizar a linkagem com a biblioteca matemática.

No terminal, acesse a raiz do projeto e utilize os seguintes comandos:

1. Para compilar o projeto:
   make

2. Para executar o programa gerado:
   ./programa

3. Para remover o arquivo executável compilado:
   make clean

## Instruções de Uso

Ao iniciar o programa, siga a ordem lógica do menu interativo apresentado no terminal:

1. Receber arquivo de entrada: Insira o caminho do arquivo de configuração (exemplo: Fabulas/entrada.txt).
2. Construir índices invertidos: O sistema fará a leitura dos documentos, remoção de stopwords e construção da Hash e da PATRICIA.
3. Imprimir índices: Exibe no terminal as palavras indexadas e suas respectivas frequências.
4. Realizar buscas: Digite os termos desejados para gerar o ranking de relevância dos documentos.
5. Exibir uso de memória: Calcula o consumo dinâmico e fixo de RAM pelas estruturas de dados.
0. Sair: Encerra a execução e libera toda a memória alocada dinamicamente.



## Observação

Testes realizados em um Lenovo Loq-e(16gb RAM, i5-12450HX, RTX 3050).