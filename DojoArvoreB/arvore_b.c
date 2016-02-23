#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>

#include "arvore_b.h"
#include "metadados.h"
#include "lista_nos.h"

int getPosicaoNaLista(int ponteiroDoNo) {
    return ponteiroDoNo / tamanho_no();
}

int busca(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_dados, int *pt, int *encontrou) {
    //printf("\nChamou o Busca para o cliente #%d\n", cod_cli);
    Metadados *metadados = le_arq_metadados(nome_arquivo_metadados);
    ListaNos *listaNos = le_nos(nome_arquivo_dados);
    //printf("Leu os arquivos\n");
    int pos;
    *pt = metadados->pont_raiz;
    No *p = listaNos->lista[metadados->pont_raiz];
    //printf("Encontrou a raiz\n");
    *encontrou = 0;
    while (p != NULL) {
        //imprime_no(p);
        //printf("Entrou no while de dentro\n");
        int i = 0;
        while (i < p->m) {
            //printf("Testando o cliente #%d na pos %d\n", p->clientes[i]->cod_cliente, i);
            if (cod_cli > p->clientes[i]->cod_cliente) {
                pos = ++i;
            } else {
                if (cod_cli == p->clientes[i]->cod_cliente) {
                    *encontrou = 1;
                    //printf("    Encontrou!\n");
                    p = NULL;
                    return pos;
                } else {
                    //printf("    p->p[i] = %d\n", p->p[i]);
                    if (p->p[i] > 0) {
                        //printf("        'listaNos->qtd': %d\n", listaNos->qtd);
                        //printf("        'tamanho_no()': %d\n", tamanho_no());
                        //printf("        'getPosicaoNaLista(p->p[i])': %d\n", getPosicaoNaLista(p->p[i]));
                        *pt = p->p[i];
                        p = listaNos->lista[getPosicaoNaLista(*pt)];
                        i = p->m + 1;
                    } else {
                        //printf("        else\n");
                        i = p->m + 1;
                        p = NULL;
                        break;
                    }
                }
            }
            //printf("FIM-Testando\n");
        }
        //printf("Saiu do while de dentro\n");

        if (p != NULL && i == p->m) {
            //printf("p != NULL && i == p->m\n");
            *pt = p->p[p->m];
            p = listaNos->lista[getPosicaoNaLista(*pt)];
        }
    }
    return 0;
}

int insere(int cod_cli, char *nome_cli, char *nome_arquivo_metadados, char *nome_arquivo_dados) {
    //TODO: Inserir aqui o codigo do algoritmo de insercao
    return INT_MAX;
}

int exclui(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_dados) {
    //TODO: Inserir aqui o codigo do algoritmo de remocao
    return INT_MAX;
}
