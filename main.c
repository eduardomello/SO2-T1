#include <stdio.h>
#include <sys/time.h>



#define ARRAY_SIZE 128

double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;

    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;

    diff = (double)y_ms - (double)x_ms;

    return diff/1000;

}

int main(int argc, char *argv[]){

	int arr[3][ARRAY_SIZE][ARRAY_SIZE];
	int arr_size[3][2];
	struct timeval time_begin, time_end;
	double total_time;

	int aux, i, j, k, t;

	char* buffer;
	char* buffer2;

	FILE *arquivo;

	// Verifica parametros 

	if (argc < 2){
		printf("\n --- Você precisa informar o numero de processos/threads! --- \n \n");
		printf("       Ex: ./teste 4 \n \n");
		exit(0);
	}
	
	int n = atoi(argv[1]); //numero de processos ou threads
	printf("\n --- O programa será executado com %d processos/threads --- \n \n", n);		
		
	
	
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


	// DEBUG: Imprime na tela
	// EXCLUIR ANTES DE ENTREGAR
	printf("\nArray 1 [%d][%d]\n", arr_size[0][0], arr_size[0][1]);
	for(i = 0; i < arr_size[0][0]; i++)
	{
		printf("\n");
		for(j = 0; j < arr_size[0][1]; j++)
			printf("%d ", arr[0][i][j]);
	}

	/************************************
			  SEGUNDO ARRAY
	 ************************************/
	// Le segunda matriz
	arquivo = fopen("in2.txt", "r"); 

	if (!arquivo)
	{
		printf ("Erro na abertura do arquivo in1.txt\n");
		exit(0);       
	}

    // Le tamanho do segundo array
	fscanf(arquivo, "%s = %d", &buffer, &arr_size[1][0]);
	fscanf(arquivo, "%s = %d", &buffer, &arr_size[1][1]);
	
	

	// Le valores do segundo array
	for(i = 0; i < arr_size[1][0]; i++)
		for(j = 0; j < arr_size[1][1]; j++)
			fscanf(arquivo, "%d", &arr[1][i][j]);


	// DEBUG: Imprime na tela
	// EXCLUIR ANTES DE ENTREGAR
	printf("\nArray 2 [%d][%d]\n", arr_size[1][0], arr_size[1][1]);
	for(i = 0; i < arr_size[1][0]; i++)
	{
		printf("\n");
		for(j = 0; j < arr_size[1][1]; j++)
			printf("%d ", arr[1][i][j]);
	}



  	/****************************************
			MULTIPLICA MATRIZES
	****************************************/

	// Verifica se é possivel multiplicar as matrizes
	if(arr_size[0][1] != arr_size[1][0])
	{
		printf("Matrizes inconpátiveis!\n");
		exit(0);
	}

	aux = 0; 

	// Multiplica matrizes
	for(t = 0; t < 10; t++)
	{
		printf("------Execução %d\n", t);

		gettimeofday(&time_begin, NULL);
		for(i = 0; i < arr_size[0][0]; i++){
			for(j = 0; j < arr_size[1][1]; j++){
				for(k = 0; k < arr_size[0][1]; k++){
					aux = aux + arr[0][i][k] * arr[1][k][j];
				}
				arr[2][i][j] = aux;
				aux = 0;
			}
		}
		gettimeofday(&time_end, NULL);
		total_time = time_diff(time_begin, time_end);

		printf("TEMPO %lfms\n", total_time);
	}
	// DEBUG: Imprime na tela
	// EXCLUIR ANTES DE ENTREGAR
	printf("\nMatriz Resultante\n");
	for(i = 0; i < arr_size[1][0]; i++)
	{
		printf("\n");
		for(j = 0; j < arr_size[1][1]; j++)
			printf("%d ", arr[2][i][j]);
	}
	

	return;
}
