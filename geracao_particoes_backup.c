#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "geracao_particoes.h"
#include "nomes.h"


void classificacao_interna(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
	//TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

void selecao_com_substituicao(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
	//TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

void selecao_natural(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M, int n)
{
    int memoria[M];
    int reservatorio[n];
    FILE *fae, *fas;
    int posMemoria, menor = 99999999, ultimo = -999999, posReservatorio = 0;

    fae = fopen(nome_arquivo_entrada, "rb");
    fas = fopen(nome_arquivos_saida, "ab");
    //TODO: checar arquivos de saida, nome das particoes;
    for(int i = 0; i < M; i++){
        fread(&memoria[i], sizeof(int), 1, fae); 
    }
    while(13){
        if(posReservatorio != n){
            for(int j = 0; j < M; j++){
                if(menor > memoria[j]){
                    menor = memoria[j];
                    posMemoria = j;
                }
            }
            if(ultimo == -999999){
                fwrite(&menor, sizeof(int), 1, fas);
                fread(&memoria[posMemoria], sizeof(int), 1, fae);
                ultimo = menor;
            }else{
                if(menor < ultimo){
                    reservatorio[posReservatorio] = menor;
                    posReservatorio++;
                    fread(&memoria[posMemoria], sizeof(int), 1, fae);
                }else{
                    fwrite(&menor, sizeof(int), 1, fas);
                    ultimo = menor;
                    fread(&memoria[posMemoria], sizeof(int), 1, fae);

                }
            }
        }
    }
    fclose(fae);
    fclose(fas);
    
	//TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

