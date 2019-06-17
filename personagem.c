#include <string.h>
#include "personagem.h"
#include "jogo.h"

//---/*DEPENDENCIAS*/-------------------------------------------------------------------------------------
// "struct.h" pronta
// mo = malloc (sizeof(mochila));
// p = malloc(sizeof(personagem));
// t = malloc(sizeof(tempo));



//------------/* Funções internas auxiliares */------------------------------------------------------------------------------------

tempo * inicializa_tempo(tempo *t) { 
	t->turnos = 0; // inicializa o tempo em 1 e o movimento em 0
    t->movimentos = 0; // inicializa o movimento em 0
return t;
}

personagem * inicializa_jean(mapa *tabuleiro, personagem *jean){
    jean->posicao = tabuleiro->altura + 3;  //posicao inicial do Jean
    tabuleiro->matriz[jean->posicao] = 56;   
    jean->fome = 0;  // inicializa fome em 0
    jean->saude = 100; // inicializa saude em 100
    jean->pontos = 0; // inicializa pontos em 0
    return jean;
}

mochila * inicializa_mochila(mochila *mo, personagem *jean, mapa *tabuleiro, terreno *plantio) {
	
	mo->frutas = NULL;
	mo->sementes = NULL;
	mo->topo_fruta = 0;
	semente *sementinha;
		/* INICIALIZO UMA SEMENTE NA MOCHILA */
	soltaSemente(plantio, jean->posicao, criaSemente()); //coloca uma semente na posicao inicial do jean
	sementinha = colherSemente(plantio, jean, tabuleiro); //coloca na var sementinha
	mo->sementes = addSemente(mo->sementes, mo->topo_semente, sementinha[0]);
	mo->topo_semente = 1;

return mo;
}
//--------/* Funções que serão chamadas em outras arquivos */----------------------------------------------------------------------------

void inicializa_Personagem(mapa *tabuleiro, personagem *jean, tempo *t, mochila *mo, terreno *plantio) {

	t = inicializa_tempo(t);

	jean = inicializa_jean(tabuleiro, jean);

	mo = inicializa_mochila(mo,jean,tabuleiro,plantio);

}


void imprime_estado(personagem *jean, tempo *t, mochila *mo) {
	printf("\n");
	printf("FOME: %i\n", jean->fome);
	printf("SAÚDE: %i\n", jean->saude);
	printf("PONTOS: %i\n", jean->pontos);
	printf("TURNO: %i\n", t->turnos);
	printf("MOVIMENTOS: %i\n", t->movimentos);
	printf("CONTEUDO MOCHILA (sementes): %i\n", mo->topo_semente);
	printf("CONTEUDO MOCHILA (frutas): %i", mo->topo_fruta);

}

