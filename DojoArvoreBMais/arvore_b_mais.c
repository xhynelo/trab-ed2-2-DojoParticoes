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
        NoFolha * nova = no_folha(D + 1, f->pont_pai, f->pont_prox);

        // estrutura de dados auxiliar com os registros existentes e o novo ordenados
        Cliente ** array = (Cliente **) malloc(sizeof (Cliente *) * (2 * D + 1));
        int i;
        for (i = 0; i < pos; i++) {
            array[i] = f->clientes[i];
        }
        array[pos] = c;
        for (i = pos; i < f->m; i++) {
            array[i + 1] = f->clientes[i];
        }

        // insere os registros na nova folha
        for (i = D; i <= 2 * D; i++) {
            nova->clientes[i - D] = array[i];
        }

        // atualiza os registros da folha velha
        f->m = D;
        for (i = 0; i < D; i++) {
            f->clientes[i] = array[i];
        }
        for (; i < 2 * D; i++) {
            f->clientes[i] = NULL;
        }

        // libera o array
        for (i = 0; i < 2 * D; i++) {
            free(array[i]);
        }
        free(array);

        FILE * metadados = fopen(nome_arquivo_metadados, "r+b");
        Metadados * meta = le_metadados(metadados);
        int pont_nova = meta->pont_prox_no_folha_livre;
        //TODO atualizar meta->pont_prox_no_folha_livre
        salva_metadados(meta, metadados);
        fclose(metadados);

        free(meta);

        fseek(dados, pont_nova, SEEK_SET);
        salva_no_folha(nova, dados);

        f->pont_prox = pont_nova;
        fseek(dados, pont_folha, SEEK_SET);
        salva_no_folha(f, dados);
        fclose(dados);

        propagarParticionamento(nome_arquivo_indice,
                nova->clientes[0]->cod_cliente, f->pont_pai, pont_nova, 1);
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

/**
 * Após a inserção de um novo nó folha ou novo nó interno é necessario propagar para os nós acima
 * @param arquivo_indice arquivo de indice devidamente aberto
 * @param chave do nó que deve ser inserido
 * @param pont_pai ponteiro para o nó pai
 * @param pont ponteiro para o nó que foi inserido
 * @param aponta_folha flag se aponta folha ou não
 */
void propagarParticionamento(FILE * arquivo_indice, int chave, int pont_pai, int pont, int aponta_folha) {
    //TODO: escrever função recursiva de propagação
}