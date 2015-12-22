/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   reservatorio.h
 * Author: leonardo
 *
 * Created on 21 de Dezembro de 2015, 18:37
 */

#ifndef RESERVATORIO_H
#define RESERVATORIO_H

typedef struct Reservatorio {
    int tam_reservatorio, posicao;
    FILE *arquivo;
} Reservatorio;

/**
 * Cria reservatório. Lembrar de usar free(reservatorio).
 * @param tamanho tamanho do reservatório
 * @param arquivo arquivo onde o reservatório será salvo
 * @return o reservatório criado
 */
Reservatorio * reservatorio(int tamanho, FILE * arquivo);

/**
 * Insere os valores na posição do cursor, houver espaço suficiente no reservatório.
 * @param reservatorio
 * @param valores array de inteiros com os valores a serem colocados no reservatório
 * @param lenght tamanho do array valores
 * @return 1 caso a inserção tenha sido bem sucedida ou 0 caso contrário
 */
int insere(Reservatorio *reservatorio, int *valores, int lenght);

/**
 * Pega todo o conteúdo do reservatório e passa para a memória.
 * @param reservatorio 
 * @param resposta ponteiro para o array de inteiros onde será colocado o conteúdo do repositório
 * @return o número de elementos lidos de forma bem sucedida
 */
int conteudo(Reservatorio *reservatorio, int * resposta);

#endif /* RESERVATORIO_H */

