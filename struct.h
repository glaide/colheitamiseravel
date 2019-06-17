#ifndef __STRUCT__
#define __STRUCT__

#include <stdio.h>
#include <stdlib.h>

#define DIREITA 1
#define CIMA 2
#define ESQUERDA 3
#define BAIXO 4

#define Laranja 2
#define Amora 3
#define Melao 5

#define MAX_COMPARTIMENTO 5
#define MAX_NOME 25

typedef struct mapa {
    unsigned int altura;     //linhas
    unsigned int largura;    //colunas
    unsigned int *matriz;
    unsigned int *matriz_original; //matriz que guarda os dados originais do mapa e nao pode ser alterada
}   mapa;

/*typedef struct {
    //Pilha Semente
    int topo_semente; // topo da pilha da semente
    char sementes [MAX_COMPARTIMENTO][MAX_NOME]; // parte 1 da mochila que terá apenas sementes
    
    int tam; // tamanho de ambas as pilhas

    //Pilha Fruta
    int topo_fruta; // todo da pilha de fruta
    char frutas [MAX_COMPARTIMENTO][MAX_NOME]; // parte 2 da mochila, terá apenas frutas

} mochila; */


typedef struct personagem {
    int fome;
    int saude;
    unsigned int posicao;
    int pontos;
}   personagem;

typedef struct semente {    //semente em geral
    int tipo;
    int posicao;            //pos no mapa
    int crescimento;     //tempo de vida da semente,  < X = semente; > X = madura; > X+ESTRAGA = morta;
    int boolCrescimento;  //se for 1 = plantado e crescendo. Se for 0 = nao esta plantado
    int boolEstragou; //se for 1 estragou
}   semente;

typedef struct mochila {
    /* Pilha Semente*/
    int topo_semente; // topo da pilha da semente
    semente *sementes;// [MAX_COMPARTIMENTO];
    
    int tam; // tamanho de ambas as pilhas
    /*Pilha fruta */
    int topo_fruta; // todo da pilha de fruta
    semente *frutas; // parte 2 da mochila, terá apenas frutas

} mochila;


typedef struct tempo {
    int turnos;
    int movimentos;     //de 0 ate 10, em 10 passa turno
}   tempo;

typedef struct terreno {
    int tamSementesSoltas;
    int tamSementesPlantadas;
    semente *plantado;     //guarda as posicoes plantadas
    semente *soltas;
}   terreno;

#endif
