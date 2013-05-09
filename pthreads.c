#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

#define ARRAY_SIZE 128

int arr[4][ARRAY_SIZE][ARRAY_SIZE]; // arr[0] = in1, arr[1] = in2, arr[2] = resultado, arr[3] = separação de processos
int arr_size[3][2];

double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;

    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;

    diff = (double)y_ms - (double)x_ms;

    return diff/1000;

}


void multiply(int i, int j)
{		
	int k, soma;
	soma = 0;
	for(k = 0; k < arr_size[0][1]; k++){
		soma = soma + arr[0][i][k] * arr[1][k][j];	
	}
	arr[2][i][j] = soma;	
}

void *worker(void *arg)
{
	int id = *((int *)arg);
	
	
	int i, j, k, soma;
	for(i = 0; i < arr_size[2][0]; i++)
	{		
		for(j = 0; j < arr_size[2][0]; j++)
		{
			if(arr[3][i][j] == id)
			{	
				multiply(i, j);										
			}
		}
	}
	
	return 0;
}




int main(int argc, char *argv[]){
	
	int aux, i, j, k, exec;	
	int total_proc,  current_proc;

	struct timeval  t1, t2;

	pthread_t *threads;

	char* buffer;
	char* buffer2;

	FILE *arquivo;	

	// Verifica parametros 

	if (argc < 2){
		printf("\n --- Você precisa informar o numero de processos/threads! --- \n \n");
		printf("       Ex: ./main_unix 4 \n \n");
		exit(0);
	}
	
	total_proc = atoi(argv[1]); //numero de processos 	
	
	threads = (pthread_t *) malloc(total_proc * sizeof(pthread_t));
	


	/************************************
			  PRIMEIRO ARRAY
	 ************************************/
	// Le primeira matriz
	arquivo = fopen("in1.txt", "r"); 

	if (!arquivo)
	{
		printf ("Erro na abertura do arquivo in1.txt\n");
		exit(0);       
	}
   
    // Le tamanho do primeiro array
	fscanf(arquivo, "%s = %d", &buffer, &arr_size[0][0]);
	fscanf(arquivo, "%s = %d", &buffer, &arr_size[0][1]);
		

	// Le valores do primeiro array
	for(i = 0; i < arr_size[0][0]; i++)
		for(j = 0; j < arr_size[0][1]; j++)
			fscanf(arquivo, "%d", &arr[0][i][j]);

	fclose(arquivo);




	/************************************
			  SEGUNDO ARRAY
	 ************************************/
	// Le segunda matriz
	arquivo = fopen("in2.txt", "r"); 

	if (!arquivo)
	{
		printf ("Erro na abertura do arquivo in2.txt\n");
		exit(0);       
	}

    // Le tamanho do segundo array
	fscanf(arquivo, "%s = %d", &buffer, &arr_size[1][0]);
	fscanf(arquivo, "%s = %d", &buffer, &arr_size[1][1]);
	
	// Le valores do segundo array
	for(i = 0; i < arr_size[1][0]; i++)
		for(j = 0; j < arr_size[1][1]; j++)
			fscanf(arquivo, "%d", &arr[1][i][j]);

	fclose(arquivo);



  	/****************************************
			DEFINIÇÕES INICIAIS
	****************************************/

	// Verifica se é possivel multiplicar as matrizes
	if(arr_size[0][1] != arr_size[1][0])
	{
		printf("Matrizes inconpátiveis!\n");
		exit(0);
	}

	// tamanho da matriz resultante
	arr_size[2][0] = arr_size[0][1];
	arr_size[2][1] = arr_size[0][1];
	
	// verificação de numero de processos	
	current_proc = 0;
	 
	for(i = 0; i < arr_size[2][0]; i++)
	{	
		for(j = 0; j < arr_size[2][1]; j++)
		{
			if(current_proc == total_proc) current_proc = 0;
			arr[3][i][j] = current_proc;
			current_proc++;
		}
	}

	
	/****************************************
			EXECUTA MULTIPLICAÇÕES
	****************************************/	
	
	for(exec = 0; exec < 10; exec++)
	{

		gettimeofday(&t1, NULL);

		for(aux = 0; aux < total_proc; aux++)
		{
			int *p = (int *) malloc(sizeof(int));  
			*p = aux;
		
			pthread_create(&threads[aux], NULL, worker, (void *)(p));
	
		}
	
		
		for (aux = 0; aux < total_proc; aux++) {
			pthread_join(threads[aux], NULL);
	  	}
		

		gettimeofday(&t2, NULL);
		double total_time = time_diff(t1, t2);

		printf("EXECUÇÃO %d: %lfms\n", (exec +1), total_time);	

	}
	
	/****************************************
			GRAVA MATRIZ RESULTANTE
	****************************************/
	
	arquivo = fopen("out.txt", "w");

	fprintf(arquivo, "LINHAS = %d\n", arr_size[2][0]);		
	fprintf(arquivo, "COLUNAS = %d", arr_size[2][1]);

	for(i = 0; i < arr_size[2][0]; i++)
	{
		fprintf(arquivo, "\n");		
		for(j = 0; j < arr_size[2][1]; j++)
			fprintf(arquivo, "%d ", arr[2][i][j]);	
	}

	fclose(arquivo);

	return;
}
