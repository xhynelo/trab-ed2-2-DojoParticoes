#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>
#include <stdio.h>

#include "encadeamento_exterior.h"
#include "lista_compartimentos.h"
#include "lista_clientes.h"

void cria_hash(char *nome_arquivo_hash, int tam) {
    ListaCompartimentos *compartimentos = cria_compartimentos(tam);
    int i;
    for (i = 0; i < tam; i++) {
        compartimentos->lista[i] = compartimento_hash(-1);
    }
    salva_compartimentos(nome_arquivo_hash, compartimentos);
    libera_compartimentos(compartimentos);
}

int busca(int cod_cli, char *nome_arquivo_hash, char *nome_arquivo_dados) {
    ListaCompartimentos *compartimentos = le_compartimentos(nome_arquivo_hash);
    int pos = compartimentos->lista[cod_cli % 7]->prox;
    if (pos == -1)
        return -1;
    ListaClientes *clientes = le_clientes(nome_arquivo_dados);
    while (clientes->lista[pos]->cod_cliente != cod_cli || !clientes->lista[pos]->flag) {
        if (clientes->lista[pos]->prox == -1) {
            return -1;
        } else {
            pos = clientes->lista[pos]->prox;
        }
    }
    return pos;
}

//TODO nada funciona além do busca
int insere(int cod_cli, char *nome_cli, char *nome_arquivo_hash, char *nome_arquivo_dados, int num_registros) {
    if (busca(cod_cli, nome_arquivo_hash, nome_arquivo_dados) != -1)
        return -1;
    printf("num_registros: %d", num_registros);
    Cliente * c = cliente(cod_cli, nome_cli, -1, 0);
    ListaClientes *clientes = le_clientes(nome_arquivo_dados);
    clientes->lista[clientes->qtd] = c;
    clientes->qtd++;
    ListaCompartimentos *compartimentos = le_compartimentos(nome_arquivo_hash);
    int pos = compartimentos->lista[cod_cli % 7]->prox;
    if (pos != -1) {
        while (clientes->lista[pos]->prox != -1) {
            pos = clientes->lista[pos]->prox;
        }
        clientes->lista[pos]->prox = clientes->qtd;
    } else {
        compartimentos->lista[cod_cli % 7]->prox = clientes->qtd-1;
    }
    salva_clientes(nome_arquivo_dados, clientes);
    salva_compartimentos(nome_arquivo_dados, compartimentos);
    return clientes->qtd-1;
}

int exclui(int cod_cli, char *nome_arquivo_hash, char *nome_arquivo_dados) {
    //TODO: Inserir aqui o codigo do algoritmo de remocao
    return INT_MAX;
}
