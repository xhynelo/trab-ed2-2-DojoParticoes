#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "geracao_particoes.h"
#include "nomes.h"
#include <stdio.h>

void classificacao_interna(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M) {
    //TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

void selecao_com_substituicao(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M) {
    //TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

void selecao_natural(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M, int n) {
    int memoria[M];
    int r;
    int reservatorio[n];
    FILE *fae, *fas;
    Nomes * proximoNome = nome_arquivos_saida;
    int posMemoria, menor = 999999, ultimo = 999999, posReservatorio = -1;
    int fimArquivo = 0;
    fae = fopen(nome_arquivo_entrada, "rb");
    fas = fopen(proximoNome->nome, "ab");
    for(r = 0; r < n; r++){
    	reservatorio[r] = 999999;
    }
    int tamanhoM;
    for (tamanhoM = 0; tamanhoM < M; tamanhoM++) {
    	int next,code;
        code = fread(&next, sizeof (int), 1, fae);
       // printf(" \n!%d! ",code);
       // getchar();
        if(code < 1){
        	tamanhoM--;
        	//printf("\nbreak!!");
        	//getchar();
        	break;
        }
        memoria[tamanhoM] = next;
        //printf("%d ",memoria[tamanhoM]);
    }

    if(tamanhoM >= M)
    	tamanhoM = M-1;
   // printf("\ntamanhoM: %d\n",tamanhoM);
    while(fimArquivo == 0 || posReservatorio >= -1){
    	int fimMemoria = 1,fimReservatorio = 1;
		int i;
		for(i = 0; i <= tamanhoM; i++){
			if(memoria[i] != 999999)
				fimMemoria = 0;
		}
		for(i = 0; i < n; i++){
			if(reservatorio[i] != 999999)
				fimReservatorio = 0;
		}

		if(fimArquivo == 1 && fimMemoria == 1 && fimReservatorio == 1){
			posReservatorio = -1;
		}
    	menor = 999999;
    	//printf("\n fM: %d - fR: %d",fimMemoria,fimReservatorio);
    	//getchar();
    	if(fimMemoria == 1 && fimReservatorio == 0){
    		//printf("\n fM: %d - fR: %d",fimMemoria,fimReservatorio);
    		for(i = 0; i < n; i++){
    			if(reservatorio[i] != 999999){
    				memoria[i] = reservatorio[i];
    				reservatorio[i] = 999999;
    			}
    		}
    		ultimo = 999999;
    		fclose(fas);
			proximoNome = proximoNome->prox;
			fas = fopen(proximoNome->nome, "ab");
    	}
    	if(posReservatorio < n-1){
            int j;
            //printf("\nMemoria: ");
            for (j = 0; j <= tamanhoM; j++) {
            	//printf(" %d",memoria[j]);
                if (menor > memoria[j]) {
                    menor = memoria[j];
                    posMemoria = j;
                }
            }
            if(menor == ultimo){
            	//printf("\nMENOR == ULTIMO!!\n");
            	//getchar();
            	break;
            }
           // printf("\ntamanhoM: %d fimArquivo: %d Ultimo: %d Menor %d \n",tamanhoM,fimArquivo, ultimo, menor);
           // getchar();
            // caso seja o primeiro da particao
            if (ultimo == 999999) {
            	int code;
                fwrite(&menor, sizeof (int), 1, fas);
                code = fread(&memoria[posMemoria], sizeof (int), 1, fae);
                if(code < 1){
                	memoria[posMemoria] = 999999;
                }
                ultimo = menor;
               // printf("Escreveu na particao!! \nULTIMO = 999999 \n");
				//getchar();
            }else{
                if (menor < ultimo) {
                	//printf("\n RESERVATORIO!!!!! \n");
                	//getchar();
                	posReservatorio++;
                    reservatorio[posReservatorio] = menor;

                    int code;
                    memoria[posMemoria] = 999999;
                    code = fread(&memoria[posMemoria], sizeof (int), 1, fae);
                    //printf("---- %d ----",memoria[posMemoria]);
                    //getchar();
                    if(code < 1){
                    	fimArquivo = 1;
                        memoria[posMemoria] = 999999;
                    }

                }else {
                	if(menor == 999999){
                		ultimo = menor;
                	}
                	else{
                		fwrite(&menor, sizeof (int), 1, fas);
						//printf("Escreveu na particao!!\n");
						ultimo = menor;
						int code = fread(&memoria[posMemoria], sizeof (int), 1, fae);
						if(code < 1){
							fimArquivo = 1;
							memoria[posMemoria] = 999999;
						}
					}
                }
             }
        }
        else{
        	//printf("\n!!! RESERVATORIO CHEIO!!!");
        	//getchar();
        	//reservatorio cheio!!
            int endMemoria = 0;
            while(endMemoria < M){
                int j;
                //printf("\n Reservatorio: ");
                for (j = 0; j < M; j++) {
                	//printf("%d ",reservatorio[j]);
                    if(memoria[j] == 999999){
                        endMemoria++;
                        continue;
                    }
                    if (menor > memoria[j]) {
                        menor = memoria[j];
                        posMemoria = j;
                    }
                }
                fwrite(&menor, sizeof (int), 1, fas);
                memoria[posMemoria] = 999999;
            }
            int j;
            for(j = 0; j < n; j++){
                memoria[j] = reservatorio[j];
                reservatorio[j] = 999999;
            }
            if(n < M){
                int i;
                for (i = n-1; i <= tamanhoM; i++) {
                    fread(&memoria[i], sizeof (int), 1, fae);
                }  
            }
            ultimo = 999999;
            fclose(fas);
            proximoNome = proximoNome->prox;
            fas = fopen(proximoNome->nome, "ab");
            posReservatorio = 0;
        }
    }
    fclose(fae);
    fclose(fas);
}


