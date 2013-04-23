#include <stdio.h>
#include "matrizes.h"

int main(int argc, char *argv[]){

	int numArgumentos = 0;
	numArgumentos = argc;

	if ( numArgumentos < 2){
		printf("\n --- Você precisa informar o numero de processos/threads! --- \n \n");
		printf("       Ex: ./teste 4 \n \n");
		exit(0);
	}
	else
	{
		int n = atoi(argv[1]); //numero de processos ou threads
		printf("\n --- O programa será executado com %d processos/threads --- \n \n", n);
		leMatriz();
		//escreveMatriz();
	}
	
	/*leMatriz("in2.txt");
	Mmn = multiplicaMatriz (Mmx,Mxn);*/

	return;
}