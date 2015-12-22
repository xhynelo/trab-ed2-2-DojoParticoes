/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   reservatorio.c
 * Author: leonardo
 *
 * Created on 21 de Dezembro de 2015, 18:54
 */

#include <stdio.h>
#include <stdlib.h>

#include "reservatorio.h"

Reservatorio * reservatorio(int tamanho, FILE * arquivo) {
    Reservatorio *reservatorio = (Reservatorio *) malloc(sizeof (Reservatorio));
    if (reservatorio)
        memset(reservatorio, 0, sizeof (Reservatorio));
    reservatorio->tam_reservatorio = tamanho;
    reservatorio->arquivo = freopen("reservatorio.dat", "r+b", arquivo);
    fseek(reservatorio->arquivo, 0, SEEK_SET);
    reservatorio->posicao = 0;
    return reservatorio;
}

int insere(Reservatorio * reservatorio, int * valores, int lenght) {
    int newlenght = reservatorio->posicao + lenght;
    printf("lenght = %d\n", lenght);
    printf("newlenght = %d\n", newlenght);
    if (newlenght <= reservatorio->tam_reservatorio) {
        reservatorio->posicao = newlenght;
        fwrite(valores, sizeof (int), lenght, reservatorio->arquivo);
        fflush(reservatorio->arquivo);
        return 1;
    }
    return 0;
}

int conteudo(Reservatorio *reservatorio, int * valores) {
    int lenght = reservatorio->posicao;
    fseek(reservatorio->arquivo, 0, SEEK_SET);
    return fread(valores, sizeof (int), lenght, reservatorio->arquivo);
}
