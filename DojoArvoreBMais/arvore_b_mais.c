#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define D 2

#include <limits.h>

#include "arvore_b_mais.h"
#include "metadados.h"
#include "no_interno.h"
#include "no_folha.h"

int interno(int cod_cli, FILE* dados, FILE* indice, int* encontrou, int* pont_folha) {
    fseek(indice, *pont_folha, SEEK_SET);
    NoInterno * pagina = le_no_interno(indice);
    int i;
    for (i = 0; i < pagina->m; i++) {
        *pont_folha = pagina->p[i];
        if (cod_cli < pagina->chaves[i]) {
            // desce pra esquerda
            if (pagina->aponta_folha) {
                return folha(cod_cli, dados, encontrou, pont_folha);
            } else {
                return interno(cod_cli, dados, indice, encontrou, pont_folha);
            }
        }
    }
    *pont_folha = pagina->p[i];
    // desce pra direita
    if (pagina->aponta_folha) {
        return folha(cod_cli, dados, encontrou, pont_folha);
    } else {
        return interno(cod_cli, dados, indice, encontrou, pont_folha);
    }
}

int folha(int cod_cli, FILE* dados, int* encontrou, int* pont_folha) {
    fseek(dados, *pont_folha, SEEK_SET);

    NoFolha * folha = le_no_folha(dados);
    int i;
    for (i = 0; (i < folha->m && folha->clientes[i]->cod_cliente <= cod_cli); i++) {
        if (folha->clientes[i]->cod_cliente == cod_cli) {
            *encontrou = 1;
            libera_no_folha(folha);
            return i;
        }
    }
    *encontrou = 0;
    libera_no_folha(folha);
    return i;
}

int busca(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice,
        char *nome_arquivo_dados, int *pont_folha, int *encontrou) {
    Metadados * metadados = le_arq_metadados(nome_arquivo_metadados);

    FILE * indice = fopen(nome_arquivo_indice, "rb");
    FILE * dados = fopen(nome_arquivo_dados, "rb");
    int i;
    *pont_folha = metadados->pont_raiz;
    if (metadados->raiz_folha) {
        i = folha(cod_cli, dados, encontrou, pont_folha);
    } else {
        i = interno(cod_cli, dados, indice, encontrou, pont_folha);
    }
    fclose(indice);
    fclose(dados);
    return i;
}

/**
 * Após a inserção de um novo nó folha ou novo nó interno é necessario propagar para os nós acima
 * @param arquivo_indice arquivo de indice devidamente aberto
 * @param chave do nó que deve ser inserido
 * @param pont_pai ponteiro para o nó pai
 * @param pont ponteiro para o nó que foi inserido
 * @param aponta_folha flag se aponta folha ou não
 */
void propagarParticionamento(FILE * arquivo_indice, FILE * metadados, int chave_nova, int pont_pagina, int pont_nova, int aponta_folha) {
    //TODO: falta testar se a página existe ou se é a raiz que está propagando para cima
    fseek(arquivo_indice, pont_pagina, SEEK_SET);
    NoInterno * pagina = le_no_interno(arquivo_indice);

    // estrutura de dados auxiliar para ordenar o nó
    int * chaves = (int *) malloc(sizeof (int) * (2 * D + 1));
    int * ponts = (int *) malloc(sizeof (int) * (2 * D + 2));

    ponts[0] = pagina->p[0];
    int i;
    for (i = 0; (i < pagina->m)&&(pagina->chaves[i] < chave_nova); i++) {
        chaves[i] = pagina->chaves[i];
        ponts[i + 1] = pagina->p[i + 1];
    }
    chaves[i] = chave_nova;
    ponts[i + 1] = pont_nova;
    for (; i < pagina->m; i++) {
        chaves[i + 1] = pagina->chaves[i];
        ponts[i + 2] = pagina->p[i + 1];
    }
    for (; i < (2 * D); i++) {
        chaves[i + 1] = -1;
        ponts[i + 2] = -1;
    }

    pagina->m++;
    free(&(pagina->chaves));
    free(&(pagina->p));
    pagina->chaves = chaves;
    pagina->p = ponts;

    if (pagina->m > (2 * D)) {
        // cria um novo nó interno
        NoInterno * novo = no_interno(D, pagina->pont_pai, pagina->aponta_folha);
        for (i = 0; i < D; i++) {
            novo->chaves[i] = chaves[D + i + 1];
            novo->p[i] = ponts[D + i + 1];
        }
        novo->p[i] = ponts[D + i + 1];
        pagina->m = D;

        // le os metadados
        Metadados * meta = le_metadados(metadados);
        int pont_novo = meta->pont_prox_no_interno_livre;

        // salva o novo nó
        fseek(arquivo_indice, pont_novo, SEEK_SET);
        salva_no_interno(novo, arquivo_indice);

        // atualiza o nó que foi particionado
        fseek(arquivo_indice, pont_pagina, SEEK_SET);
        salva_no_interno(pagina, arquivo_indice);

        // atualiza os ponteiros dos filhos
        if (!(novo->aponta_folha)) {
            for (i = 0; i <= novo->m; i++) {
                fseek(arquivo_indice, novo->p[i], SEEK_SET);
                NoInterno * n = le_no_interno(arquivo_indice);
                n->pont_pai = pont_novo;
                fseek(arquivo_indice, novo->p[i], SEEK_SET);
                salva_no_interno(n, arquivo_indice);
            }
        }

        // atualiza os metadados
        fseek(arquivo_indice, 0, SEEK_END);
        meta->pont_prox_no_interno_livre = ftell(arquivo_indice);
        fseek(metadados, 0, SEEK_SET);
        salva_metadados(meta, metadados);
        free(meta);

        // propaga o particionamento recursivamente
        propagarParticionamento(arquivo_indice, metadados, chaves[D], novo->pont_pai, pont_novo, 1);
    } else {
        fseek(arquivo_indice, pont_pagina, SEEK_SET);
        salva_no_interno(pagina, arquivo_indice);
    }

}

int insere(int cod_cli, char *nome_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados) {
    int pont_folha, encontrou, pos = busca(cod_cli, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, &pont_folha, &encontrou);
    if (encontrou)
        return -1;

    Cliente * c = cliente(cod_cli, nome_cli);

    FILE * dados = fopen(nome_arquivo_dados, "r+b");
    fseek(dados, pont_folha, SEEK_SET);
    NoFolha * f = le_no_folha(dados);

    if (f->m < 2 * D) {
        int i;
        for (i = f->m; i >= pos; i--) {
            f->clientes[i + 1] = f->clientes[i];
        }
        f->clientes[pos] = c;
        f->m++;
        fseek(dados, pont_folha, SEEK_SET);
        printf("pont_folha: %d\n", pont_folha);
        salva_no_folha(f, dados);
        fclose(dados);
        imprime_no_folha(f);
        return pont_folha;
    } else {
        // Particionamento:
        // nova folha para o particionamento
        printf("Entrou no ELSE\n");
        NoFolha * nova = no_folha(D + 1, f->pont_pai, f->pont_prox);

        // estrutura de dados auxiliar com os registros existentes e o novo ordenados
        Cliente ** array = (Cliente **) malloc(sizeof (Cliente *) * (2 * D + 1));
        printf("Cliente ** array = (Cliente **) malloc(sizeof (Cliente *) * (2 * D + 1));\n");

        int i;
        for (i = 0; i < pos; i++) {
            array[i] = f->clientes[i];
        }
        array[pos] = c;
        for (i = pos; i < f->m; i++) {
            array[i + 1] = f->clientes[i];
        }
        printf("insere os registros na nova folha\n");

        // insere os registros na nova folha
        for (i = D; i <= 2 * D; i++) {
            nova->clientes[i - D] = array[i];
        }
        printf("atualiza os registros da folha velha\n");

        // atualiza os registros da folha velha
        f->m = D;
        for (i = 0; i < D; i++) {
            f->clientes[i] = array[i];
        }
        for (; i < 2 * D; i++) {
            f->clientes[i] = NULL;
        }

        printf("libera o array\n");

        // libera o array
        for (i = 0; i < 2 * D; i++) {
            free(array[i]);
        }
        free(array);

        printf("lê os metadados\n");

        // le os metadados
        FILE * metadados = fopen(nome_arquivo_metadados, "r+b");
        Metadados * meta = le_metadados(metadados);
        int pont_nova = meta->pont_prox_no_folha_livre;

        printf("salva nova folha\n");

        // salva nova folha
        fseek(dados, pont_nova, SEEK_SET);
        salva_no_folha(nova, dados);

        printf("atualiza folha\n");

        // atualiza folha
        f->pont_prox = pont_nova;
        fseek(dados, pont_folha, SEEK_SET);
        salva_no_folha(f, dados);

        printf("atualiza os metadados\n");

        // atualiza os metadados
        fseek(dados, 0, SEEK_END);
        meta->pont_prox_no_folha_livre = ftell(dados);
        fseek(metadados, 0, SEEK_SET);
        salva_metadados(meta, metadados);
        free(meta);


        fclose(dados);


        printf("propaga o particionamento\n");

        // propaga o particionamento
        FILE * arquivo_indice = fopen(nome_arquivo_indice, "r+b");
        propagarParticionamento(arquivo_indice, metadados,
                nova->clientes[0]->cod_cliente, f->pont_pai, pont_nova, 1);
        fclose(metadados);
        fclose(arquivo_indice);

        imprime_no_folha(f);

        libera_no_folha(f);
        libera_no_folha(nova);
        if (pos < D)
            return pont_folha;
        return pont_nova;
    }

}

int exclui(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados) {
    //TODO: Inserir aqui o codigo do algoritmo de remocao
    return INT_MAX;
}
