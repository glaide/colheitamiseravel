#include "struct.h"
#ifndef __PERSONAGEM__
#define __PERSONAGEM__

tempo * inicializa_tempo(tempo *t);
personagem * inicializar_jean(mapa *tabuleiro, personagem *jean);
mochila * inicializa_mochila(mochila *mo, personagem *jean, mapa *tabuleiro, terreno *plantio);
int vazia_sementes(mochila *mo);
int vazia_frutas(mochila *mo);

//-------------------------------------------------------------------------------------------------------------

void inicializa_Personagem(mapa *tabuleiro, personagem *jean, tempo *t, mochila *mo, terreno *plantio);
void imprime_estado(personagem *jean, tempo *t, mochila *mo);
char* pop_sementes(mochila *mo);
char* pop_frutas(mochila *mo);
void push_sementes(mochila *mo, char *item); 
void push_frutas(mochila *mo, char *item);
char *verifica_planta (semente *fruta);

#endif