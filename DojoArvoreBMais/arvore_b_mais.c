#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>

#include "arvore_b_mais.h"
#include "metadados.h"
#include "no_interno.h"
#include "no_folha.h"

int busca(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice,
        char *nome_arquivo_dados, int *pont_folha, int *encontrou) {
    Metadados * metadados = le_arq_metadados(nome_arquivo_metadados);

    FILE * indice = fopen(nome_arquivo_indice, "rb");
    FILE * dados = fopen(nome_arquivo_dados, "rb");
    if (metadados->raiz_folha) {
        fseek(dados, metadados->pont_raiz, SEEK_SET);
        NoFolha * folha = le_no_folha(dados);
        int i;
        for (i = 0; (i < folha->m && folha->clientes[i]->cod_cliente <= cod_cli); i++) {
            if (folha->clientes[i]->cod_cliente == cod_cli) {
                *encontrou = 1;
                *pont_folha = 0;
                fclose(indice);
                fclose(dados);
                libera_no_folha(folha);
                return i;
            }
        }
        *encontrou = 0;
        *pont_folha = 0;
        fclose(indice);
        fclose(dados);
        libera_no_folha(folha);
        return i;
    } else {

        fseek(indice, metadados->pont_raiz, SEEK_SET);
        NoInterno * raiz = le_no_interno(indice);
        int i;

        for (i = 0; (i < raiz->m/* && raiz->chaves[i] <= cod_cli*/); i++) {
            if (cod_cli < raiz->chaves[i]) {
                if (raiz->aponta_folha) {
                    fseek(dados, raiz->p[i], SEEK_SET);

                    NoFolha * folha = le_no_folha(dados);
                    imprime_no_folha(folha);
                    int j;
                    for (j = 0; (j < folha->m && folha->clientes[j]->cod_cliente <= cod_cli); j++) {
                        if (folha->clientes[j]->cod_cliente == cod_cli) {
                            *encontrou = 1;
                            *pont_folha = raiz->p[i];
                            fclose(indice);
                            fclose(dados);
                            libera_no_folha(folha);
                            return j;
                        }
                    }
                    *encontrou = 0;
                    *pont_folha = raiz->p[i];
                    fclose(indice);
                    fclose(dados);
                    libera_no_folha(folha);
                    return j;

                } else {
                    printf("ENTROU NO ELSE ERRADO DE DENTRO\n\0");
                }
            } /* raiz->chaves[i] <= cod_cli */else {
                printf("ENTROU NO ELSE ERRADO DE FORA\n\0");
            }
        }
    }

    *pont_folha = 0;
    *encontrou = 1;

    return 1;
}

int insere(int cod_cli, char *nome_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados) {
    //TODO: Inserir aqui o codigo do algoritmo de insercao
    return INT_MAX;
}

int exclui(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados) {
    //TODO: Inserir aqui o codigo do algoritmo de remocao
    return INT_MAX;
}
