#include "jogo.h"
#include "personagem.h"
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#include <curses.h>

int main(int argc, char **argv) {

    mapa *tabuleiro = (mapa*) malloc(sizeof(mapa));

    personagem *jean = (personagem*) malloc(sizeof(personagem));

    mochila *mo = (mochila*) malloc(sizeof(mochila));
    tempo *t_jogo = (tempo*) malloc (sizeof(tempo));

    int acabou = 0;     // boolean que indica se o jogo encerrou
    int direcao;
    char ch;        //char a ser lido quando o usuario pressionar o teclado
    
    FILE *input = fopen(argv[1], "r+");
    tabuleiro->matriz = inicializar_mapa(tabuleiro, input);
    tabuleiro->matriz = ler_matriz(input, tabuleiro);
    unsigned int *matriz_original = (unsigned int *) malloc(tabuleiro->altura * tabuleiro->altura * sizeof (unsigned int));
     for (int i = 0; i < tabuleiro->altura; i ++){
         for (int j = 0; j < tabuleiro->largura; j++){
             matriz_original[i* tabuleiro->largura + j] = tabuleiro->matriz[i* tabuleiro->largura + j];
         }
     }

    terreno plantio;
    inicia_terreno(&plantio);
    inicializa_Personagem(tabuleiro, jean, t_jogo, mo,&plantio); // inicializa o jean (poe ele na posição inicial) mochila e tempo
    
    int aux = randomize_semente(tabuleiro); //gera uma posicao aleatoria
    soltaSemente(&plantio, aux, criaSemente()); //coloca uma semente nessa posicao
    printf("Bonjuor, bem-vindo a colheita miserável de Jean Valjean, aperte enter para começar o jogo");
    semente *fruta = NULL;
    do {
        
        atualizaSementesMapa(&plantio,tabuleiro);
        imprime_estado(jean,t_jogo,mo);
        imprimir_matriz(tabuleiro);
        ch = getchar();
        switch (ch)

        {
            case 'w':      //W 87
                direcao = CIMA;
                mover_personagem(tabuleiro, jean, direcao, matriz_original, t_jogo);
                t_jogo->movimentos++;
                atualiza_turno(t_jogo,jean,&plantio,tabuleiro);    
                break;
            case 'a':      //A 97
                direcao = ESQUERDA;
                mover_personagem(tabuleiro, jean, direcao, matriz_original, t_jogo);
                t_jogo->movimentos++;
                atualiza_turno(t_jogo,jean,&plantio,tabuleiro);    
                break;
            case 'd':      //D 68
                direcao = DIREITA;
                mover_personagem(tabuleiro, jean, direcao, matriz_original, t_jogo);
                t_jogo->movimentos++;
                atualiza_turno(t_jogo,jean,&plantio,tabuleiro);  
                break;
                case 's':      //S 83
                direcao = BAIXO;
                mover_personagem(tabuleiro, jean, direcao, matriz_original, t_jogo);
                t_jogo->movimentos++;
                atualiza_turno(t_jogo,jean,&plantio,tabuleiro);   

                break;
            case 'q':      //Q quit 81 
                acabou = 1;
                break;
            case 'c':     //colher
                fruta= colher(&plantio, jean, tabuleiro, matriz_original);
                
                if(fruta != NULL)
                {
                    //coloca a fruta na mochila
                    mo->frutas = addSemente(mo->frutas, mo->topo_fruta, fruta[0]);
                    mo->topo_fruta = mo->topo_fruta + 1;
                    jean->pontos++;
                    free(fruta);
                    fruta = NULL;
                } 
                else {
                    //pega a semente do terreno e coloca na mochila
                    fruta = colherSemente(&plantio, jean, tabuleiro);
                    if(fruta != NULL)
                    {
                        mo->sementes = addSemente(mo->sementes, mo->topo_semente, fruta[0]);
                        mo->topo_semente = mo->topo_semente + 1;
                        free(fruta);
                        fruta = NULL;
                    }
                }

                if (jean->pontos==10) 
                    acabou=1;
                t_jogo->movimentos++;
                atualiza_turno(t_jogo,jean,&plantio,tabuleiro);  
                break; 
           case 'p':     //plantar
	            if (plantar(&plantio, jean, tabuleiro, matriz_original)) 
		            printf("plantou com sucesso");
	            else
                {
                    //remove da pilha de sementes da mochila
	                fruta = removePilha(mo->sementes, mo->topo_semente);
                    if(fruta != NULL){
                        mo->topo_semente = mo->topo_semente - 1;
                        //joga no chao
	                    soltaSemente(&plantio, jean->posicao, fruta[0]);
                        free(fruta);
                        fruta = NULL;

                        jean->fome = plantio.tamSementesSoltas;
                        //planta
	                    if(plantar(&plantio, jean, tabuleiro, matriz_original))
	                        printf("plantou com sucesso");
                }
                t_jogo->movimentos++;
                atualiza_turno(t_jogo,jean,&plantio,tabuleiro);    
            //else nao conseguiu plantar
            break;
            
            case 'o':     //observar
                observa(plantio.plantado, plantio.tamSementesPlantadas, jean, tabuleiro, matriz_original);
                //else nao conseguiu observar
                t_jogo->movimentos++;
                atualiza_turno(t_jogo,jean,&plantio,tabuleiro);  
                break;
    
            case 'e': //comer=eat 
                if (mo->topo_fruta>0)
                    {   
                        mo->frutas = removePilha(mo->frutas, mo->topo_fruta); //remove a fruta comida da mochila
                        mo->topo_fruta=mo->topo_fruta-1;
                        jean->pontos++; //atualiza os pontos
                        jean->fome--;//atualiza fome
                    }       

        }
    

        system("clear || cls");
        atualiza_terreno (plantio.plantado, plantio.tamSementesPlantadas);
        if (jean->saude==0) acabou=1;
        // sleep(1);
        
        
    } 

    while (acabou == 0); 
    printf("Felicitation, você salvou jean de ter sua fazenda tomada por crossfiteiros!\n");
    free(tabuleiro);
    return 0;
}