
#include "jogo.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "struct.h"

/*--------------------------------------------------------------------MAPA/LARA--------------------------------------------------------------------------------*/

/*ALOCACAO*/ 
unsigned int* inicializar_mapa(mapa *tabuleiro, FILE *input) {
    
    /* leitura dos parametos */
	fscanf(input, "%u", &tabuleiro->altura);
	fscanf(input, "%u", &tabuleiro->largura);    

    /* Confere se é menor do que 30x50 conforme a especificacao */
    if ((tabuleiro->altura > 30) || (tabuleiro->largura >50)){
        printf("MAPA INVALIDO \n");
        exit(1);
    }
    
    else {

        //caso o mapa seja valido, aloca a matriz (metodo 1)
        tabuleiro->matriz = malloc (tabuleiro->altura * tabuleiro->largura * sizeof (unsigned int));
        
		for(int i=0; i<tabuleiro->altura; i++) {
			for(int j=0; j<tabuleiro->largura; j++) {
				tabuleiro->matriz[(i*tabuleiro->altura) + j] = 0; //calcula a posicao de cada elemento e zera
			}
    	}
	}

	return(tabuleiro->matriz);

}


/*LEITURA DO MAPA*/
unsigned int * ler_matriz (FILE *input, mapa *tabuleiro) {

    unsigned char *matriz= (unsigned char *) malloc (tabuleiro->altura * tabuleiro->largura * sizeof (unsigned char)); 	//alocação da matriz auxiliar seguindo as dimensões
	
	fread(matriz, sizeof(unsigned char), (tabuleiro->altura * tabuleiro->largura), input); 	//leitura do arquivo
	
	for(int i=0; i < (tabuleiro->altura); i++){
		for(int j=0; j < (tabuleiro->largura); j++){
			tabuleiro->matriz[(i*tabuleiro->largura) + j] = (unsigned int) matriz[(i*tabuleiro->largura) + j];	//matriz alteravel recebe a matriz original
		}
	}
	fclose(input);		//uma vez que o arquivo ja esta na matriz, pode ser fechado
    return tabuleiro->matriz;
}

//teste
void imprimir_matriz(mapa *tabuleiro) {

    unsigned char aux;

    for(int i= 0; i< tabuleiro->altura; i++){
        for (int j=0; j< tabuleiro->largura; j++){
            aux = tabuleiro->matriz[(i*tabuleiro->largura)+j];
            printf("%c", aux);		//imprime como char o conteudo alocado na matriz
        }
    }

    printf("\n");
}


/* MOVE JEAN */
void mover_personagem(mapa *tabuleiro, personagem *jean, int direcao, unsigned int * matriz_original, tempo *t_jogo){
    
    int aux = jean->posicao; //posicao do jean
    
    if (direcao == DIREITA){
        if (matriz_original [aux + 1] != 35) { //35 = #
            //se nao for muro
            tabuleiro->matriz[aux+1] = 56;
            tabuleiro->matriz[aux] = matriz_original[aux];   //matriz a ser impressa recebe o que havia na matriz original
            jean->posicao = aux + 1;
        }
    }
    if (direcao == ESQUERDA) {
        if(matriz_original[aux - 1] != 35) { //35 = #
        	//se nao for muro
            tabuleiro->matriz[aux-1] = 56;
            tabuleiro->matriz[aux] = matriz_original[aux];  //matriz a ser impressa recebe o que havia na matriz original
            jean->posicao = aux -1;
        }
    }
    if (direcao == BAIXO) {
        if(matriz_original[aux + tabuleiro->largura + 1] != 35) { //35 = #
        	//se nao for muro
            tabuleiro->matriz[aux + tabuleiro->largura + 1] = 56;
            tabuleiro->matriz[aux] = matriz_original[aux];     //matriz a ser impressa recebe o que havia na matriz original
            jean->posicao = aux + tabuleiro->largura + 1;
        }
    }
    if (direcao == CIMA) {
        if(matriz_original[aux - tabuleiro->largura - 1] != 35) { //35 = #
        	//se nao for muro
            tabuleiro->matriz[aux - tabuleiro->largura - 1] = 56;  //matriz a ser impressa recebe o que havia na matriz original
            jean->posicao = aux + tabuleiro->largura + 1;
            tabuleiro->matriz[aux] = matriz_original[aux];
            jean->posicao = aux - tabuleiro->largura - 1;
        }
    }

}

/* RANDOMIZACAO DA SEMENTE */ //merda aki
unsigned int randomize_semente(mapa *tabuleiro) {

    unsigned int semente_random; 
	unsigned int dimensoes = (tabuleiro->altura - 2) * (tabuleiro->largura - 2); // linha e coluna maxima (sem muro)
    int ok = 1; // booleana para saber se foi
    
    srand(time(NULL)); //time(null) recebe de compilacao pra adaptar a uma randomizacao atraves do srand, gerando uma posicao randomizada
    semente_random = rand() % (dimensoes); //faz com que as proximas randomizacoes sejam diferentes 
    
    while (ok == 1){
        if (tabuleiro->matriz[semente_random] == 45){
            ok = 0; //caiu em posicao valida
        }
        else
        {
            srand(time(NULL)); //caiu em uma posicao invalida
            semente_random = rand() % (dimensoes); 
        }
    }
    return semente_random;
}



/*--------------------------------------------------------------------TERRENO/GLAIDE--------------------------------------------------------------------------------*/

/*funcao que atualiza turno, fome e saude*/
void atualiza_turno(tempo *t_jogo, personagem *p, terreno *plantio, mapa *tabuleiro){
    int turno;

    if (t_jogo->movimentos % 10 == 0)
    {
        //se o numero de movimentos = 10 é pq deu um turno
        t_jogo->turnos++;   
        p->fome++;
          //atualiza turno
    }
    turno = 0; //variavel pra atualizar
    if  (p->fome>=100)  //quando a fome chega em 100 nova variavel conta de 2 em 2
        {
            turno++; //aumenta a auxiliar
            if (turno % 2 == 0) p->saude--; //quando auxiliar é mult de 2, decrescenta saude
        }
    if (t_jogo->movimentos % 10 == 0 )
    {
        int aux = randomize_semente(tabuleiro); //gera uma posicao aleatoria
        soltaSemente(plantio, aux, criaSemente()); //coloca uma semente nessa posicao
    }
} 


/* FUNCOES DO TERRENO */

int satisfacaoVetor[4] = {0, 2, 3, 5}; // nivel de satisfação de cada planta nao precisa mais pq ta no define
char nomeSemente[4] = {'J', 'L', 'A', 'T'}; // nome da semente
int tempoParaFicarMadura[4] = {5, 6, 8, 10}; //tempo que cada semente leva pra ficar madura
int frutoOuPlanta[4] = {0, 1, 1, 2}; //0 = sem fruto(apenas planta); 1 = fruto; 2 = fruto e planta;
char desenhoPlantado[4] = {'a', 'b', 'c', 'd'};
char desenhoSemente[4] = {'0', '1', '2', '3'}; //desenho de cada fruto no mapa*/

void atualizaSementesMapa(terreno *plantio, mapa *tabuleiro){
    //unsigned char aux;
    for(int i = 0; i < plantio->tamSementesSoltas; i++)
        {
            tabuleiro->matriz[plantio->soltas[i].posicao] = desenhoSemente[plantio->soltas[i].tipo];
  
        }
    for(int i = 0; i < plantio->tamSementesPlantadas; i++)
    {
        tabuleiro->matriz[plantio->plantado[i].posicao] = desenhoPlantado[plantio->plantado[i].tipo];
    }
}


//Conteudo acima precisa ser altera no jogo.c


/*inicia terreno de plantio */
void inicia_terreno(terreno *plantio){
    plantio->tamSementesSoltas = 0;  
    plantio->tamSementesPlantadas = 0;  //em 0 pra crescer
    plantio->plantado = NULL;     //guarda as posicoes plantadas
    plantio->soltas = NULL;
}

semente criaSemente(){  //Cria uma semente
    int type;
    srand(time(NULL)); //para gerar sementes aleatorias
    type=rand () % 4;
    semente criada;
    criada.tipo = type; //deixar randomico entre 0 e 4
    criada.posicao = 0; 
    criada.crescimento = 0;
    criada.boolCrescimento = 0;
    criada.boolEstragou = 0;

    return criada;
}

//funcao que solta uma semente no terreno
void soltaSemente(terreno *plantio, int pos, semente sementeSolta){ //Quando a semente está no mapa, sem estar plantado
    sementeSolta.posicao = pos; //recebe a posicao que foi passada para ficar solta
    /*adiciona a semente no ponteiro das soltas */
    plantio->soltas = addSemente(plantio->soltas, plantio->tamSementesSoltas, sementeSolta);
    plantio->tamSementesSoltas = plantio->tamSementesSoltas + 1;
}


int plantar (terreno *plantio, personagem *jean, mapa *tabuleiro, unsigned int * matriz_original ){
    int aux = jean->posicao;
    int auxSemente;
    //Verifica se é terra fertil e se tem semente no local
    if(matriz_original[aux] == 43 && buscaSemente(aux, plantio, &auxSemente)){ 
        plantio->soltas[auxSemente].boolCrescimento = 1; //muda a semente para crescer
        plantio->plantado = addSemente(plantio->plantado, plantio->tamSementesPlantadas, plantio->soltas[auxSemente]);
        plantio->tamSementesPlantadas = plantio->tamSementesPlantadas + 1;
        plantio->soltas = removeLista(plantio->soltas, plantio->tamSementesSoltas, auxSemente); //remover plantio->soltas[auxSemente]
        plantio->tamSementesSoltas = plantio->tamSementesSoltas - 1;
        return 1; //se plantou com sucesso
    }
    

    return 0;    //aw nao plantou com sucesso
}

semente * removeLista(semente *soltas, int tam, int pos){
    tam = tam - 1; //diminui o tamanho da lista
    semente * auxSoltas = (semente*) malloc( tam * sizeof(semente));

    for(int i = 0; i < tam; i++)
        if(i < pos)
            auxSoltas[i] = copiaConteudo(soltas[i]);
        else
            auxSoltas[i] = copiaConteudo(soltas[i+1]);
    
    free(soltas);
    return auxSoltas;
}




semente * addSemente(semente *plantio, int tam, semente plantacao){
    semente * auxSemente = (semente*) malloc ((tam+1) * sizeof(semente));
    for(int i = 0; i < tam; i++)
        auxSemente[i] = copiaConteudo(plantio[i]);
    auxSemente[tam] = copiaConteudo(plantacao);
    free(plantio);
    return auxSemente;
}

//funcao que verifica se na posicao que o jean esta tem semente
int buscaSemente (int posicao, terreno * plantio, int * retorno){
    for(int i = 0; i < plantio->tamSementesSoltas; i++)
        if(posicao == plantio->soltas[i].posicao){ //Existe semente na posicao
            *retorno = i;
            return 1; //true
        }
    return 0; //false = nao tem semente na pos que pode ser plantado
}

int observa (semente *plantacao, int tam, personagem *jean, mapa *tabuleiro, unsigned int * matriz_original ){
    if(matriz_original[jean->posicao] == 43){ //se for + realiza insertSort
        semente *escolhido = (semente*) malloc(sizeof(semente));

        for(int i = 1, j = i-1; i < tam; i++, j = i-1){
            *escolhido = copiaConteudo(plantacao[i]);

            while((j >= 0) && (plantacao[j].crescimento > escolhido->crescimento)){
                plantacao[j+1] = copiaConteudo(plantacao[j]);
                j--;
            }
            plantacao[j + 1] = copiaConteudo(*escolhido);
        }
        return 1;
    }
    return 0;
}


/*  Atualiza o crescimento da planta e ativa o bool de estragou  */
void atualiza_terreno (semente *plantado, int tamSementesPlantadas){
    for(int i = 0; i < tamSementesPlantadas; i++){
        plantado[i].crescimento++;
        if(plantado[i].crescimento >= (ESTRAGA+tempoParaFicarMadura[plantado[i].tipo]))
            plantado[i].boolEstragou = 1;   //Caso tenha estrago deixa ativado
    }
}
semente *colherSemente(terreno *plantio, personagem *jean, mapa *tabuleiro){
    int aux = jean->posicao;
    int auxSemente;

    semente *sementeSolta = NULL;

    if(plantio->tamSementesSoltas > 0 && buscaSemente(aux, plantio, &auxSemente)){
        sementeSolta = (semente*) malloc(sizeof(semente)); //aloca com o tipo semente
        sementeSolta[0] = copiaConteudo(plantio->soltas[auxSemente]);
        plantio->soltas = removeLista(plantio->soltas, plantio->tamSementesSoltas, auxSemente); //remover plantio->soltas[auxSemente]
        plantio->tamSementesSoltas = plantio->tamSementesSoltas - 1;
        return sementeSolta; //se pegou semente com sucesso
    }
    return sementeSolta; //Se for NULL não pegou semente
}


/*  Personagem pega o fruto e atualiza o mapa, contudo nao coloca no inventario.
  Ele retorna o objeto fruto, que eh possivel ver se esta podre ou nao.  */
semente *colher(terreno *plantio, personagem *jean, mapa *tabuleiro, unsigned int * matriz_original ){
    int aux = jean->posicao;
    semente *fruta = NULL;
    //se o tamanho da semente for > 0 e a posicao do jean for = a da planta
    if(plantio->tamSementesPlantadas > 0 && aux == plantio->plantado[0].posicao)
        if(plantio->plantado[0].crescimento >= tempoParaFicarMadura[plantio->plantado[0].tipo]){
            fruta = (semente*) malloc(sizeof(semente)); //aloca com o tipo semente
            //remove a fruta da fila
            plantio->plantado = removeFila(plantio->plantado, plantio->tamSementesPlantadas, fruta);
            plantio->tamSementesPlantadas = plantio->tamSementesPlantadas - 1;
            //se o tipo da semente é o 2 então vira planta
            //if(frutoOuPlanta[fruta->tipo] == 2)
            //    {
            //        tabuleiro->matriz[aux] = 'k'; //virou planta
            //    }
            //    
            //else
                tabuleiro->matriz[aux] = '+'; //voltou a ser terreno de plantio
            fruta->boolCrescimento = 0; //parou de crescer pois foi colhida*/
        }
    return fruta; //Se for NULL ou fruta->boolEstragou nao pontua
}


semente * removeFila(semente *plantio, int tam, semente *fruta){
    tam = tam-1; //diminui o tamanho da fila
    semente * auxPlantacao = NULL;
    if(tam != 0)
    {
        //define aux do tipo semente
        auxPlantacao = (semente*) malloc (tam * sizeof(semente));
        for(int i = 0; i < tam; i++)
            //passa todo o conteudo do plantio p aux menos o primeiro
            auxPlantacao[i] = copiaConteudo(plantio[i+1]);
    }
    //copia o conteudo do primeiro
    fruta = (semente*) malloc (sizeof(semente));
    fruta[0] = copiaConteudo(plantio[0]);
    free(plantio);
    return auxPlantacao; //Remove a Fruta da fila
}



semente * removePilha(semente *pilha, int tam){
    if(tam > 0){
        tam = tam-1; //diminui o tamanho da pilha
        /*if(tam != 0)
        {
            NovaPilha=(semente*)malloc(tam * sizeof(semente));
            for(int i = 0; i < tam; i++)
                NovaPilha[i] = copiaConteudo(pilha[i]);
        }*/
        //copia o conteudo do primeiro
        semente *sementeNova = (semente*) malloc (sizeof(semente));
        sementeNova[0] = copiaConteudo(pilha[tam]);
        return sementeNova;
    }
    return NULL; //Remove a Fruta da fila
}
 

//retorna o conteudo de plantio
semente copiaConteudo(semente plantio){
    return plantio;
}