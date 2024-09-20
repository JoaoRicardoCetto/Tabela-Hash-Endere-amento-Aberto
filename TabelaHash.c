// FEITO POR: JOAO RICARDO MATTEDI CETTO

#include "TabelaHash.h"


TMatricula *inicializaMatricula(long tamanho){
    TMatricula *matricula = (TMatricula*)malloc(tamanho*(sizeof(TMatricula)));
    int i;

    for(i = 0; i < tamanho; i++) { //Percorre todo o vetor inicializando o numero das matriculas como -1
        matricula[i].num_matricula = -1;
        matricula[i].flag = 0;
    }

    return matricula;
}


int tamanhoTabelaHash(){
        long long int matricula;
        int cont;
        char nome[50];
        cont = 0;
        FILE *arquivo = NULL;

        arquivo = fopen("../Matriculas.txt", "r");
        if (arquivo != NULL) { 
            while (!feof(arquivo)) {   //passa por todo arquivo e aumenta o contador a cada vez que uma matricula é lida
                fscanf(arquivo,"%[^\n]s", nome);
                fscanf(arquivo,"%I64d\n", &matricula);
                cont++;
            }
        }

        fclose(arquivo);
        printf("\n\nQuantidade de matriculas: %d",cont);
        fflush(stdin); //limpa o buffer

        return cont;
}


int descobreNumeroPrimo(long long int num) {
    int i;

    if (num <= 1){
        return 0;
    }
    if (num <= 3){
        return 1;  
    }
    if (num % 2 == 0 || num % 3 == 0){
        return 0;
    }
    for (i = 5; i * i <= num; i += 6) {      // Verificação de números primos
        if (num % i == 0 || num % (i + 2) == 0) 
            return 1;
    }

    return 1;
}

int proximoPrimo(int tamanho_hash) {
    while (descobreNumeroPrimo(tamanho_hash) != 1) {      // Incrementa até encontrar o próximo número primo

        tamanho_hash++;
    }   
    return tamanho_hash;
}


int tamanhoVetorHash(THash *hash) {
    int opcao, tamanho;

    printf("\n\nEscolha quantos porcento maior que a quantidade de matriculas será a hash (como padrão, sera 150%%)\n");
    printf("\n120%% ");
    printf("\n150%% ");
    printf("\n180%% \n\n");
    scanf("%d",&opcao);

    hash->t0 = clock();

    switch (opcao) { //Dada a opcao, multiplica o valor escolhido pela quandidade matriculas que foi lida do arquivo
        case 120:
            tamanho = proximoPrimo(tamanhoTabelaHash()*1.2);
            break;
        case 150:
            tamanho = proximoPrimo((tamanhoTabelaHash()*1.5));
            break;
        default:
            tamanho = proximoPrimo((tamanhoTabelaHash()*1.8));
            break;
    }

    printf("\nO tamanho da tabela hash e: %d",tamanho);
    
    return tamanho;
}


THash *inicializaHash() {
    THash *hash = (THash*)malloc(sizeof(THash));
    hash->tamanho = tamanhoVetorHash(hash); //calcula o tamanho do vetor hash com base no numero de matriculas e com a escolha da porcentagem pelo usuário
    hash->matricula = inicializaMatricula(hash->tamanho); //Aloca memoria e inicializa as matriculas
    hash->quantidade = 0; //inicializa a quantidade de matriculas como zero
    
    return hash;
}


int funcaoHash(THash *hash, long long int matricula){
    int posicao;
    posicao = matricula % hash->tamanho;
    return posicao;
}


int funcao_ReHash(THash *hash, long long int matricula, int indice){
    int incremento = 1 + (matricula % (hash->tamanho -1));
    int calc = (indice + incremento) % hash->tamanho;

    return calc;
}


int pesquisar(THash *hash, long long int pesq) {
    int indice = funcaoHash(hash, pesq); //calcula a chave em que a matricula sera inserida

    while ((hash->matricula[indice].flag) == 1 || (hash->matricula[indice].num_matricula == pesq)){ //se houver colisão (flag == 1) ou se a pesquisa for igual a uma matricula, 
        if (hash->matricula[indice].num_matricula == pesq){
            return indice;
        }
        indice = funcao_ReHash(hash, pesq, indice); //Caso houver colisão, recalcula o indice com o Re-Hash
    }
     
    return -1;
}


long long int ler_matricula(){
    long long int matricula;
    printf("\ninsira o numero de matricula do aluno: ");
    scanf("%I64d",&matricula);
    fflush(stdin);
    return matricula;
}

void ler_nome(char *nome){
    printf("\nInsira o Nome do aluno: ");
    scanf("%[^\n]s",nome);
    fflush(stdin);
}


void inserirMatricula(THash *hash, long long int nova_matricula, char *nome){
    int indice = funcaoHash(hash, nova_matricula); //calcula a chave em que a matricula sera inserida
    
    while (hash->matricula[indice].num_matricula != -1){ //enquanto tiver matriculas na posição, será executado o Re-Hash
        hash->matricula[indice].flag = 1; //Ocorrencia de colisão
        indice = funcao_ReHash(hash, nova_matricula, indice);
    }

    strcpy(hash->matricula[indice].nome, nome); 
    hash->matricula[indice].num_matricula = nova_matricula;
    hash->quantidade++;
      
}


void removerMatricula(THash *hash, long long int excluir){
    int indice;
    
    indice = pesquisar(hash, excluir); //Pesquisa se a matricula existe no sistema
    if(indice != -1) { //Se ela existir, seu numero de matricula é zerado e a qtde de matriculas diminui
        hash->matricula[indice].num_matricula = -1;
        hash->quantidade--;      
        printf("A matricula %I64d foi removida com sucesso!", excluir); 
    }
    else{
        printf("A matricula nao esta cadastrada no sistema");
    }

}


void lerArquivo(THash *hash){
        long long int matricula;
        char nome[50];
        FILE *arquivo = fopen("../Matriculas.txt","r");
        
        if (arquivo != NULL) {
            while (!feof(arquivo)) {
                fscanf(arquivo,"%[^\n]s", nome);
                fscanf(arquivo,"%I64d\n", &matricula);
                
                if((pesquisar(hash, matricula) == -1)) { //pesquisa se a matricula já existe antes de inserir
                    inserirMatricula(hash, matricula, nome);
                }
            }       
        }
    fclose(arquivo); 
}


void gravarNoArquivo(THash *hash, int tamanho){
    FILE *arquivo = fopen("../Matriculas.txt","w");
    int i;
    for(i = 0; i < tamanho; i++) { //percorre todo vetor e, se o numero de matricukla foi != -1, adiciona no arquivo
        if(hash->matricula[i].num_matricula != -1){
            fprintf(arquivo,"%s\n", hash->matricula[i].nome);
            fprintf(arquivo,"%I64d\n", hash->matricula[i].num_matricula);
        }
    }
    
    fclose(arquivo);
}


void imprimirMatriculas(THash *hash){
    int i;

    printf("\n -------------- MATRICULAS --------------\n");
        for(i = 0; i < hash->tamanho; i++) {
            if(hash->matricula[i].num_matricula == -1) {
                printf("\n [%d] - Espaco Vazio\n", i); //printa os espacos vazios
            }
            else{
                printf("\n\nMatricula: %I64d \nNome: %s\n", hash->matricula[i].num_matricula, hash->matricula[i].nome); //printa uma matricula e o nome relacionado a ela
            }
        }
    printf("\n----------------------------------------\n\n");
}


void opcoesMenu(int *op){
    printf("\n\nESCOLHA UMA OPCAO:\n");
    printf("\n1) Inserir matricula");
    printf("\n2) Imprimir matriculas");
    printf("\n3) Remover matricula");
    printf("\n4) Pesquisar matricula");
    printf("\n5) Quantidade de Matriculas.");
    printf("\n0) Finalizar programa");
    printf("\n\n");
    scanf("%d", op);
    fflush(stdin);

}


void menu(THash *hash) {
    double tempo;
    int opcao = -1;
    long long int matricula, pesq;
    char nome[50];

    tempo = (double)(hash->t1 - hash->t0) / CLOCKS_PER_SEC;
    printf("\nTempo de processamento (s): %f", tempo);
    
    do {
        opcoesMenu(&opcao);   
        switch (opcao) {
        case 1:
            matricula = ler_matricula();
            if(pesquisar(hash, matricula) == -1) { //A matricula ainda nao foi cadastrada
                ler_nome(nome);
                inserirMatricula(hash, matricula, nome);
                printf("\n\nMatricula inserida com sucesso!\n");
            }
            else {
                printf("\n\nERRO: A matricula já está cadastrada no sistema\n");
            }
            break;
        case 2:
            imprimirMatriculas(hash);
            break;
        case 3:
            matricula = ler_matricula();
            removerMatricula(hash, matricula);
            break;
        case 4:
            pesq = ler_matricula();
            if(pesquisar(hash, pesq) != -1){
                printf("A matricula %I64d esta cadastrada no sistema!", pesq);
            }
            else{
                printf("A matricula %I64d nao esta cadastrada no sistema", pesq);
            }
            break;
        case 5:
            printf("A quantidade de matriculas armazenadas no sistema e: %ld", hash->quantidade);
            break;
        case 0:
            printf("Programa Finalizado!");
            break;
        default:
            break;
        }
    } while (opcao != 0);

}

int main(){
    THash *tabela_hash = inicializaHash();

    tabela_hash->t1 = clock();
    lerArquivo(tabela_hash);
    menu(tabela_hash);
    
    free(tabela_hash);

    return 0;
}