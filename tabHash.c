#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabHash.h"
#define REMOVIDO -1


struct insere{
    int chave;
    int valor;
    struct insere* prox;
};

struct tab_hash{
    int quantidade, tam;
    struct insere** itens;
};

Tab_hash * cria(int tam){

    Tab_hash* th = (Tab_hash*) malloc(sizeof(Tab_hash));

    if(th == NULL){
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }

    th -> tam = tam;
    th -> quantidade = 0;
    th -> itens = (struct insere**) malloc (tam *sizeof(struct insere*));

    if(th -> itens == NULL){
        free(th);
        return NULL;
    }
    int i;
    for(i = 0; i < th -> tam; i++){
        th -> itens[i] = NULL;
    }

    return th;
}

int mult(int chave, int tam){

    const double A = 0.6180339887;

    double valor = chave * A;
    valor = valor - (int) valor;
    return (int) (tam * valor);
}

int divisao(int chave, int tam){

    return (chave & 0x7FFFFFFF)%tam;
}

int duploHash (int chave, int i, int tam){
    int h1 = mult(chave, tam);
    int h2 = divisao(chave, tam);

    if(h2 == 0){
        h2 = 1;
    }

    return((h1+ i*h2 ) & 0x7FFFFFFF) % tam;
}

float fatorCarga(Tab_hash* th){

    if (th == NULL || th -> quantidade == 0){
        printf("Tabela invalida ou vazia!\n");
        return 0;
    }
    return (float) th -> quantidade / th -> tam;
}

int insere_enderecamento_aberto(Tab_hash* th, int chave, int func){
    if( th == NULL){
        printf("Tabela invalida!\n");
        return 0;
    }

    int i, novaPosicao;

    for(i = 0; i < th -> tam; i++){
        novaPosicao = duploHash(chave, i, th -> tam);

        if(th->itens[novaPosicao] == NULL || th->itens[novaPosicao] == (struct insere*)REMOVIDO) {
            struct insere* novo = (struct insere*) malloc(sizeof(struct insere));
            if(novo == NULL){
                printf("Erro ao alocar memoria!\n");
                return 0;
            }
            novo -> valor = -1;
            novo -> chave = chave;
            th ->itens[novaPosicao] = novo;
            novo->prox = NULL;
            th->quantidade++;
             return 1;
        }
    }
    return 0;
}

int insere_hash_encadeamento(Tab_hash* th, int func, int chave){
    if (th == NULL){
        printf("Tabela invalida!\n");
        return 0;
    }

    int posicao = (func == 0) ? mult(chave, th -> tam) : divisao(chave, th -> tam);

    struct insere* novo = (struct insere*) malloc(sizeof(struct insere));

    if(novo == NULL){
        printf("Erro ao alocar memoria!\n");
        return 0;
    }
    novo -> valor = -1;
    novo -> chave = chave;
    novo -> prox = th ->itens[posicao];
    th -> itens[posicao] = novo;
    th->quantidade++;
    return 1;

}

int insere_hash_arquivo_encadeamento( const char* nArquivo, Tab_hash** th, int func){

    FILE *arquivo = fopen(nArquivo, "r");

    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }

    int size; 
    fscanf(arquivo, "%d", &size);
    *th = cria(size);
    char linha[1024];

      while(fgets(linha, sizeof(linha), arquivo) != NULL){
        char *token = strtok(linha, ";.");
        while(token != NULL){
            if(token != NULL && token[0] != '\0' && strspn(token, "0123456789") == strlen(token)) {
                 int chave = atoi(token);
            insere_hash_encadeamento(*th, func, chave);
            }
            token = strtok(NULL, ";.");
        }
      }
    fclose(arquivo);
    return 1;
}

int insere_hash_arquivo_enderecamento( const char* nArquivo, Tab_hash** th, int func){
    FILE *arquivo = fopen(nArquivo, "r");

    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }
    int size; 
    fscanf(arquivo, "%d", &size);
    *th = cria(size);
    char linha[1024];

      while(fgets(linha, sizeof(linha), arquivo) != NULL){
        char *token = strtok(linha, ";.");

        while(token != NULL){
            if(token != NULL && token[0] != '\0' && strspn(token, "0123456789") == strlen(token)) {
            int chave = atoi(token);
            insere_enderecamento_aberto(*th, chave, func);
            }
            token = strtok(NULL, ";.");
        }
      }
    fclose(arquivo);
    return 1;

}

int remove_hash_enderecamento(Tab_hash* th, int func, int chave){
    if (th == NULL || th -> quantidade == 0){
        printf("Tabela invalida ou vazia!\n");
        return 0;
    }
    int novaPosicao, i;

    for(i = 0; i < th -> tam; i++){
        novaPosicao = duploHash(chave, i, th -> tam);

        if(th -> itens[novaPosicao] != NULL && th -> itens[novaPosicao] -> chave == chave){
            free(th-> itens[novaPosicao]);
            th -> itens[novaPosicao] = (struct insere*)REMOVIDO;
            th -> quantidade--;
            printf("Remocao realizada!\n");
            return 1;
        }
        if(th -> itens[novaPosicao] == NULL){
            break;
        }
    }
    printf("Elemento nao encontrado!\n");

    return 0;
}

int remove_hash_encadeamento(Tab_hash* th, int func, int chave){
    if (th == NULL || th -> quantidade == 0){
        printf("Tabela invalida ou vazia!\n");
        return 0;
    }

    int posicao, novaPosicao, i;

    posicao = (func == 0) ? mult(chave, th -> tam) : divisao(chave, th -> tam);

    struct insere* atual = th -> itens[posicao];
    struct insere* anterior = NULL;

    while(atual != NULL){
        if(atual -> chave == chave){
            if(anterior == NULL){
                th -> itens[posicao] = atual -> prox;
            }else{
                anterior -> prox = atual -> prox;
            }

            free(atual);
            th -> quantidade--;
            printf("Remocao realizada!\n");
            return 1;
        }
        anterior = atual;
        atual = atual -> prox;
    }
    return 0;
}

int busca_hash_enderecamento(Tab_hash* th, int chave, int func) {
    if (th == NULL) {
        printf("Operacao invalida!\n");
        return 0;
    }
    int colisoes = 0;
    int i;

    for (i = 0; i < th->tam; i++) {

        int novaPosicao = duploHash(chave, i, th->tam);

        if (th->itens[novaPosicao] != NULL && th->itens[novaPosicao]->chave == chave && th->itens[novaPosicao]->chave != REMOVIDO) {
            printf("Elemento encontrado na posicao %d com %d colisao(s).\n", novaPosicao, colisoes);
            return 1;
        }
        if (th->itens[novaPosicao] == NULL) {
            break;
        }

        colisoes++;
    }

    printf("Elemento nao encontrado\n");
    return 0;
}

int busca_hash_encadeamento(Tab_hash* th, int chave, int func){

    if(th == NULL){
        printf("Operacao invalida!");
        return 0;
    }

    int posicao = (func == 0) ? mult(chave, th -> tam) : divisao(chave, th -> tam);

    struct insere* atual = th->itens[posicao];
    int colisoes = 0;

    while(atual != NULL){

        if(atual -> chave == chave){
            printf("Elemento encontrado na posicao %d e com %d colisao(s).\n", posicao, colisoes);
            return 1;
        }
        colisoes++;
        atual = atual -> prox;
    }
    printf("Elemento nao encontrado\n");
    return 0;
}

void imprime_enderecamento (Tab_hash* th){

    if (th == NULL || th -> quantidade == 0){
        printf("Tabela invalida ou vazia!\n");
    }

    printf("Impressao da tabela hash: \n");

    for(int i = 0; i < th -> tam; i++){
        printf("Indice %d: ", i);

        if(th -> itens[i] == NULL){
            printf("NULL\n");
        } else if ((struct insere*)th -> itens[i] == (struct insere*)REMOVIDO) { 
            printf("NULL\n");
        } else {
            printf("[%d]\n", th -> itens[i] -> chave);
        }
    }
}

void imprime_encadeamento (Tab_hash* th){
    if (th == NULL || th -> quantidade == 0){
        printf("Tabela invalida ou vazia!\n");
    }

    printf("Impressao da tabela hash: \n");

    for(int i = 0; i < th -> tam; i++){
        printf("Indice %d: ", i);

        struct insere* atual = th -> itens[i];
        if(atual == NULL){
            printf("NULL\n");
        }else{
            while(atual != NULL){
                printf("[%d] ->", atual -> chave);
                atual = atual -> prox;
            }
            printf("NULL\n");
        }
    }
}