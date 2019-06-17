#include "struct.h"
#ifndef __JOGO__
#define __JOGO__

unsigned int * inicializar_mapa(mapa *tabuleiro, FILE *input);
unsigned int * ler_matriz (FILE *input, mapa *tabuleiro);
personagem * inicializar_jean(mapa *tabuleiro, personagem *jean);
unsigned int randomize_semente(mapa *tabuleiro);
void imprimir_matriz(mapa *tabuleiro);
void inicia_turno(tempo *t_jogo);
void mover_personagem(mapa *tabuleiro, personagem *jean, int direcao, unsigned int * matriz_original, tempo *t_jogo);

void atualiza_turno(tempo *t_jogo, personagem *p, terreno *plantio, mapa *tabuleiro);
void inicia_terreno(terreno *plantio);
semente criaSemente();
void soltaSemente(terreno *plantio, int pos, semente sementeSolta);
int plantar (terreno *plantio, personagem *jean, mapa *tabuleiro, unsigned int * matriz_original );
semente * removeLista(semente *soltas, int tam, int pos);
semente * addSemente(semente *plantio, int tam, semente plantacao);
int buscaSemente (int posicao, terreno * plantio, int * retorno);
int observa (semente *plantacao, int tam, personagem *jean, mapa *tabuleiro, unsigned int * matriz_original);
void atualizaSementesMapa(terreno *plantio, mapa *tabuleiro);
semente *colherSemente(terreno *plantio, personagem *jean, mapa *tabuleiro);

//Conteudo acima precisa ser altera no jogo.c
void atualiza_terreno (semente *plantado, int tamSementesPlantadas);
semente *colher(terreno *plantio, personagem *jean, mapa *tabuleiro, unsigned int *matriz_original);
semente * removeFila(semente *plantio, int tam, semente *fruta);
semente * removePilha(semente *pilha, int tam);
semente copiaConteudo(semente plantio);


#define ESTRAGA 15 //somar madura + estraga = fruta estragada

extern char desenhoPlantado[];
extern int satisfacaoVetor[];
extern char nomeSemente[];
extern int tempoParaFicarMadura[];
extern int frutoOuPlanta[];
extern char desenhoSemente[];

#endif