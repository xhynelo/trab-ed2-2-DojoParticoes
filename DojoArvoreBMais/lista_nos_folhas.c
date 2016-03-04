#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdarg.h>

#include "lista_nos_folhas.h"

void imprime_nos_folhas(ListaNosFolhas *lc) {
    int i;
    for (i = 0; i < lc->qtd; i++) {
        imprime_no_folha(lc->lista[i]);
    }
}

ListaNosFolhas *cria_nos_folhas(int qtd, ...) {
    va_list ap;
    ListaNosFolhas *lc = (ListaNosFolhas *) malloc(sizeof (ListaNosFolhas));
    lc->qtd = qtd;
    lc->lista = (NoFolha **) malloc(sizeof (NoFolha *) * (qtd));
    int i;
    va_start(ap, qtd);
    for (i = 0; i < qtd; i++) {
        lc->lista[i] = va_arg(ap, NoFolha *);
    }
    va_end(ap);
    return lc;
}

void salva_nos_folhas(char *nome_arquivo, ListaNosFolhas *lc) {
    FILE *out = fopen(nome_arquivo, "wb");
    int i;
    for (i = 0; i < lc->qtd; i++) {
        salva_no_folha(lc->lista[i], out);
    }
    fclose(out);
}

ListaNosFolhas *le_nos_folhas(char *nome_arquivo) {
    int qtd = 0;
    ListaNosFolhas *lc = (ListaNosFolhas *) malloc(sizeof (ListaNosFolhas));
    FILE *in = fopen(nome_arquivo, "rb");
    if (in != NULL) {
        NoFolha *no = NULL;
        while ((no = le_no_folha(in)) != NULL) {
            qtd += 1;
            libera_no_folha(no);
        }
        fseek(in, 0, SEEK_SET);
        lc->qtd = qtd;
        lc->lista = (NoFolha **) malloc(sizeof (NoFolha *) * (qtd));
        qtd = 0;
        while ((no = le_no_folha(in)) != NULL) {
            lc->lista[qtd++] = no;
        }
        fclose(in);
    } else {
        lc->qtd = 0;
        lc->lista = NULL;
    }
    return lc;
}

int cmp_nos_folhas(ListaNosFolhas *c1, ListaNosFolhas *c2) {
    if (c1->qtd != c2->qtd) {
        return 0;
    }
    int i;
    for (i = 0; i < c1->qtd; i++) {
        if (!cmp_no_folha(c1->lista[i], c2->lista[i])) {
            return 0;
        }
    }
    return 1;
}

void libera_nos_folhas(ListaNosFolhas *lc) {
    int i;
    for (i = 0; i < lc->qtd; i++) {
        libera_no_folha(lc->lista[i]);
    }
    free(lc->lista);
    free(lc);
}
