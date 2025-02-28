#include <stdlib.h>
#include <stdio.h>
#include "tabHash.h"

int main(){

    int tam, opcao, chave, func, mColisao, escolha;
    char arquivo[1024];
    Tab_hash *th = cria(tam);

    printf("Escolha a funcao de dispersao:\n");
    printf("0 - Divisao \n1 - Multiplicacao\n");
    scanf("%d", &func);

    if(func != 0 && func != 1){
        printf("Opcao invalida!\n");
        return 0;
    }

    printf("Informe o tratamento de colisao: \n");
    printf("0 - Enderecamento aberto \n1 - Encadeamento\n");
    scanf("%d", &mColisao);

    if(mColisao != 0 && mColisao != 1){
        printf("Opcao invalida!\n");
        return 0;
    }

    printf("Escolha a forma de leitura de dados\n");
    printf("0 - Manualmente \n1 - Arquivo\n");
    scanf("%d", &escolha);

    if(escolha == 0){
        printf("Digite o tamanho da tabela:\n");
        scanf("%d", &tam);

         printf("Digite a chave para insercao: \n");
         scanf("%d", &chave);
         
         if(mColisao == 0){
             insere_enderecamento_aberto(th, chave, func);
          }else if (mColisao == 1){
             insere_hash_encadeamento(th, func, chave);
          }
    }else if (escolha == 1){

        printf("Digite o nome do arquivo:\n");
        scanf("%s", arquivo);

        if(mColisao == 0){
            insere_hash_arquivo_enderecamento(arquivo,&th, func);
         }else if (mColisao == 1){
             insere_hash_arquivo_encadeamento(arquivo, &th, func);
         }

    }else{
        printf("Opção inválida!\n");
        return 0;
    }
    while(1){

        printf("1 - Remover Elemento\n");
        printf("2 - Buscar Elemento\n");
        printf("3 - Imprimir tabela (enderecamento aberto)\n");
        printf("4 - Imprimir tabela (encadeamento)\n");
        printf("5 - Sair\n");
        printf("\nEscolha uma das opcoes acima: \n");
        scanf("%d", &opcao);
    
    
        switch (opcao){

            case 1: 
            printf("Digite o valor para remocao: \n");
            scanf("%d", &chave);
    
            if(mColisao == 0){
                remove_hash_enderecamento(th, func, chave);
             }else if (mColisao == 1){
                remove_hash_encadeamento(th, func, chave);
             }
             printf("Fator de carga da tabela: %.2f\n",fatorCarga(th));
    
            break;
    
            case 2: 
            printf("Insira o valor da busca: \n");
            scanf("%d", &chave);
    
            if (mColisao == 0) {
                busca_hash_enderecamento(th, chave, func);
            } else if (mColisao == 1) {
                busca_hash_encadeamento(th, chave, func);
            }
            printf("Fator de carga da tabela: %.2f\n", fatorCarga(th));

            break;

            case 3: 
             if( th == NULL){
                    printf("Tabela nao criada ainda!\n");
                    return 0;
                }
            if(mColisao == 1){
                printf("A impressao deve ser realizada para o metodo encademento!\n");
                break;
            }
                printf("Imprimindo tabela: \n");
                imprime_enderecamento(th);

                printf("Fator de carga da tabela: %.2f\n",fatorCarga(th));
                break;
    
            break;
    
            case 4: 
            if( th == NULL){
                printf("Tabela nao criada ainda!\n");
                return 0;
            }
            if(mColisao == 0){
                printf("A impressao deve ser realizada para o metodo enderecamento!\n");
                break;
            }
            printf("Imprimindo tabela: \n");
            imprime_encadeamento(th);

            printf("Fator de carga da tabela: %.2f\n", fatorCarga(th));
            break;
    
            case 5:
            printf("Encerrando programa!\n");
            free(th);
            return 0;
    
            default: 
            printf("Opcao invalida!\n");
        }
    }

    return 0;
}