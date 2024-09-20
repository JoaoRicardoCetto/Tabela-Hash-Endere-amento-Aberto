// FEITO POR: JOAO RICARDO MATTEDI CETTO

#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct TipoMatricula {
    char nome[50];
    long long int num_matricula;
    int flag; //indica se há uma matricula no espaço

}TMatricula;

typedef struct TabelaHash {
    TMatricula *matricula;
    long quantidade, tamanho; //quantidade de matriculas e o tamanho do vetor 
    clock_t t0, t1; //usados para calcular o tempo de execução
}THash;


TMatricula *inicializaMatricula(long tamanho); //Inicializa e retorna um vetor de matriculas para hash->matricula

int tamanhoTabelaHash(); //Lê a quantidade de matriculas do arquivo

int descobreNumeroPrimo(long long int num); 

int proximoPrimo(int tamanho_hash); //incrementa o tamanho da hash até que o tamanho seja um número primo, chama a função proximoPrimo

int tamanhoVetorHash(THash *hash); //calcula o tamanho do vetor e chama as funções proximoPrimo e descobreNumeroPrimo

THash *inicializaHash(); //inicialização e alocação de memória

int funcaoHash(THash *hash, long long int matricula); //calcula uma posição para uma matricula

int funcao_ReHash(THash *hash, long long int matricula, int indice); //Recalcula a posição, caso haja conflito na primeira função hash

int pesquisar(THash *hash, long long int pesq); //Recebe uma matricula e pesquisa se ela está no sistema

long long int ler_matricula(); //le uma matricula do teclado

void inserirMatricula(THash *hash, long long int nova_matricula, char *nome); //Pesquisar se a matricula já existe, se não existir, insere no sistema

void removerMatricula(THash *hash, long long int excluir); //Recebe um numero de matricula, pesquisa se ele está no sistema e se estiver, remove

void lerArquivo(THash *hash); //Lê matriculas do arquivo

void gravarNoArquivo(THash *hash, int tamanho); //grava as matriculas no arquivo

void imprimirMatriculas(THash *hash); //printa na tela todas as matriculas

void opcoesMenu(int *op); //Onde o usuário escolhe a opcao que deseja

void menu(THash *hash);


#endif