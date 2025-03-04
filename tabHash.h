#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct insere Insere;
typedef struct tab_hash Tab_hash;

Tab_hash * cria(int tam);

int mult(int chave, int tam);
int divisao(int chave, int tam);

int duploHash (int chave, int i, int tam);

float fatorCarga(Tab_hash* th);

int insere_hash_encadeamento(Tab_hash* th, int func, int chave);
int insere_enderecamento_aberto(Tab_hash* th, int chave, int func);

int insere_hash_arquivo_enderecamento( const char* nArquivo, Tab_hash** th, int func);
int insere_hash_arquivo_encadeamento( const char* nArquivo, Tab_hash** th, int func);

int remove_hash_enderecamento(Tab_hash* th, int func, int chave);
int remove_hash_encadeamento(Tab_hash* th, int func, int chave);

int busca_hash_enderecamento(Tab_hash* th, int chave, int func);
int busca_hash_encadeamento(Tab_hash* th, int chave, int func);

void imprime_enderecamento (Tab_hash* th);
void imprime_encadeamento (Tab_hash* th);