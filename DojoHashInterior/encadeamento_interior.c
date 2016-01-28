#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>

#include "lista_clientes.h"
#include "cliente.h"
#include "encadeamento_interior.h"

#define LIBERADO 0
#define OCUPADO 1

void cria_hash(char *nome_arquivo_hash, int tam) {
    ListaClientes *tab_cliente = cria_clientes(tam);
    int i;
    for (i = 0; i < tam; i++) {
        tab_cliente->lista[i] = cliente(-1, "", i, LIBERADO);
    }
    salva_clientes(nome_arquivo_hash, tab_cliente);
}

int busca(int cod_cli, char *nome_arquivo_hash, int tam, int *encontrou) {
    ListaClientes *tab_cliente = le_clientes(nome_arquivo_hash);
    int a = 0, end = cod_cli % 7, j = -1;
    *encontrou = 0;
    while (a == 0) {
        if (tab_cliente->lista[end]->flag == LIBERADO) {
            j = end;
        }
        if (tab_cliente->lista[end]->cod_cliente == cod_cli &&
                tab_cliente->lista[end]->flag == OCUPADO) {
            a = 1;
            *encontrou = 1;
        } else {
            if (end == tab_cliente->lista[end]->prox) {
                a = 2;
                end = j;
            } else {
                end = tab_cliente->lista[end]->prox;
            }
        }
    }
    return end;
}

int insere(int cod_cli, char *nome_cli, char *nome_arquivo_hash, int tam) {
    //TODO: Inserir aqui o codigo do algoritmo de insercao
    return INT_MAX;
}

int exclui(int cod_cli, char *nome_arquivo_hash, int tam) {
    //TODO: Inserir aqui o codigo do algoritmo de remocao
    return INT_MAX;
}
