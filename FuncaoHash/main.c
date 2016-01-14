/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: leonardo
 *
 * Created on 6 de Janeiro de 2016, 08:07
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int dobra(int chave) {
    int vetor[6];
    int i;
    for (i = 0; i < 6; i++) {
        //TODO gerar vetor
    }
    printf("%d %d %d %d %d %d\n", vetor[5], vetor[4], vetor[3], vetor[2], vetor[1], vetor[0]);
    vetor[2] = (vetor[2] + vetor[5]) % 10;
    vetor[3] = (vetor[3] + vetor[4]) % 10;
    printf("%d %d %d %d\n", vetor[3], vetor[2], vetor[1], vetor[0]);
    vetor[0] = (vetor[0] + vetor[3]) % 10;
    vetor[1] = (vetor[1] + vetor[2]) % 10;

    return vetor[0] + vetor[1]*10;
}

/*
 * 
 */
int main(int argc, char** argv) {
    printf("%d", dobra(813409));
    return (EXIT_SUCCESS);
}

