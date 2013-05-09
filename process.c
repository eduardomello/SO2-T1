#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h>

#define ARRAY_SIZE 128



struct region {
	int arr[4][ARRAY_SIZE][ARRAY_SIZE]; // shared->arr[0] = in1, shared->arr[1] = in2, shared->arr[2] = resultado, shared->arr[3] = separação de processos
	int arr_size[3][2];
};
struct region *shared;
int fd;






double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;

    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;

    diff = (double)y_ms - (double)x_ms;

    return diff/1000;

}

void print_result()
{
	int i, j;
	
	printf("Snapshot do Resultado \n");

	for(i = 0; i < shared->arr_size[2][0]; i++)
	{
		printf("\n");		
		for(j = 0; j < shared->arr_size[2][1]; j++)
		 	printf("%d ", shared->arr[2][i][j]);	
	}
	printf("\n\n");
}


void multiply(int i, int j)
{		
	int k, soma;
	soma = 0;
	for(k = 0; k < shared->arr_size[0][1]; k++){
		soma = soma + shared->arr[0][i][k] * shared->arr[1][k][j];	
	}
	shared->arr[2][i][j] = soma;	
	
}

void worker(int id)
{		
	
	int i, j, k, soma;
	for(i = 0; i < shared->arr_size[2][0]; i++)
	{		
		for(j = 0; j < shared->arr_size[2][0]; j++)
		{
			if(shared->arr[3][i][j] == id)
			{					
				multiply(i,j);
			}
		}
	}
	
	return 0;
}




int main(int argc, char *argv[]){

	
	int aux, i, j, k, exec, counter, status;	
	int total_proc,  current_proc;
	
	struct timeval  t1, t2;
	
	pid_t pid, exit_pid;

	char* buffer;
	char* buffer2;

	FILE *arquivo;	

	// Gera shared memory

	fd = shm_open("/myregion", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1) printf("Erro em shm_open!\n");

	if(ftruncate(fd, sizeof(struct region)) == -1) printf("truncou!\n");

	// Mapeia o objeto na memoria compartilhada

	shared = mmap(NULL, sizeof(struct region), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (shared == MAP_FAILED) printf("falhou mapeamento!\n");



	// Verifica parametros 

	if (argc < 2){
		printf("\n --- Você precisa informar o numero de processos/threads! --- \n \n");
		printf("       Ex: ./process 4 \n \n");
		exit(0);
	}
	
	total_proc = atoi(argv[1]); //numero de processos 
			
	
	/************************************
			  PRIMEIRO shared->arrAY
	 ************************************/
	// Le primeira matriz
	arquivo = fopen("in1.txt", "r"); 

	if (!arquivo)
	{
		printf ("Erro na abertura do arquivo in1.txt\n");
		exit(0);       
	}
   
    // Le tamanho do primeiro shared->array
	fscanf(arquivo, "%s = %d", &buffer, &shared->arr_size[0][0]);
	fscanf(arquivo, "%s = %d", &buffer, &shared->arr_size[0][1]);
	
	// Le valores do primeiro shared->array
	for(i = 0; i < shared->arr_size[0][0]; i++)
		for(j = 0; j < shared->arr_size[0][1]; j++)
			fscanf(arquivo, "%d", &shared->arr[0][i][j]);


	fclose(arquivo);



	/************************************
			  SEGUNDO shared->arrAY
	 ************************************/
	// Le segunda matriz
	arquivo = fopen("in2.txt", "r"); 

	if (!arquivo)
	{
		printf ("Erro na abertura do arquivo in2.txt\n");
		exit(0);       
	}

    // Le tamanho do segundo shared->array
	fscanf(arquivo, "%s = %d", &buffer, &shared->arr_size[1][0]);
	fscanf(arquivo, "%s = %d", &buffer, &shared->arr_size[1][1]);
	
	// Le valores do segundo shared->array
	for(i = 0; i < shared->arr_size[1][0]; i++)
		for(j = 0; j < shared->arr_size[1][1]; j++)
			fscanf(arquivo, "%d", &shared->arr[1][i][j]);




	fclose(arquivo);




  	/****************************************
			DEFINIÇÕES INICIAIS
	****************************************/

	// Verifica se é possivel multiplicar as matrizes
	if(shared->arr_size[0][1] != shared->arr_size[1][0])
	{
		printf("Matrizes inconpátiveis!\n");
		exit(0);
	}

	// define tamanho da matriz resultante
	shared->arr_size[2][0] = shared->arr_size[0][1];
	shared->arr_size[2][1] = shared->arr_size[0][1];
	
	// verificação de numero de processos
	// utiliza uma matriz que indica qual linha cada processo fica responsável

	current_proc = 0;
	for(i = 0; i < shared->arr_size[2][0]; i++)
	{	
		for(j = 0; j < shared->arr_size[2][1]; j++)
		{
			if(current_proc == total_proc) current_proc = 0;
			shared->arr[3][i][j] = current_proc;
			current_proc++;
		}
	}


		
	/****************************************
			EXECUTA MULTIPLICAÇÕES
	****************************************/	

	for(exec = 0; exec < 10; exec++)
	{
		
		gettimeofday(&t1, NULL);

		// Cria processos filhos
		for(aux = 0; aux < total_proc; aux++)
		{
			pid = fork();
			if(pid == 0)
			{		
				worker(aux);
				
				exit(0);
			}	
			
		}
		
		// Espera filhos terminarem
		counter = total_proc;

		while(counter > 0)
		{
			exit_pid = wait(&status);			
			--counter;
		}
	
		gettimeofday(&t2, NULL);

		

		double total_time = time_diff(t1, t2);
		printf("EXECUÇÃO %d: %lfms\n", (exec +1), total_time);
		
		
	}
	
	
	/****************************************
			GRAVA MATRIZ RESULTANTE
	****************************************/
	
	arquivo = fopen("out.txt", "w");

	fprintf(arquivo, "LINHAS = %d\n", shared->arr_size[2][0]);		
	fprintf(arquivo, "COLUNAS = %d", shared->arr_size[2][1]);

	for(i = 0; i < shared->arr_size[2][0]; i++)
	{
		fprintf(arquivo, "\n");		
		for(j = 0; j < shared->arr_size[2][1]; j++)
			fprintf(arquivo, "%d ", shared->arr[2][i][j]);	
	}

	shm_unlink("/myregion");

	fclose(arquivo);

	return;
}
