//
//

#include <stdio.h>
#include <stdlib.h>


void leMatriz(void)
{
    char c;
    int linhas, colunas;
    FILE *arquivo1;
    arquivo1=fopen ("in1.txt","r"); 

    if (!arquivo1)
    {
        printf ("Erro na abertura do arquivo.");
        exit(0);       
    }
    else
    {
        /* while((c = getc(arquivo1) ) != EOF)      //Enquanto não chegar ao final do arquivo 
        printf("%c", c);                 //imprime o caracter lido 
        fclose(arquivo1);
        return; */
        linhas = leLinha(arquivo1);
        colunas = leColuna(arquivo1);
        //le a matriz...
        fclose ( arquivo1 );

    }
}
    
int leLinha(FILE *arquivo){

    char n[12];
    int linha;
    rewind(arquivo); // garante que a posicao corrente do arquivo é o inicio dele
    fgets(n, 11, arquivo);
    printf("Numero de linhas: %c\n", n[9]);
    linha =  n[9] - '0'; //converte o char para int
    return linha;
}

int leColuna(FILE *arquivo){
    
    int test;
    rewind(arquivo); // garante que a posicao corrente do arquivo é o inicio dele
    test = fseek (arquivo, 11, SEEK_CUR);
    if (test == 0)
    {
        char n[13];
        int coluna;
        fgets(n, 13, arquivo);
        printf("Numero de Colunas: %c \n", n[10]);
        coluna =  n[10] - '0'; //converte o char para int
        return coluna; 
    }
    else
    {
        printf("fseek falhou \n");
        exit(0);
    }
    
}

void escreveMatriz(void){

    char string[7] = "testando";
    int i;
    FILE *saida;     
    saida=fopen ("out.txt","w+"); //Cria um arquivo texto para leitura e gravação.  

    if (!saida){
        printf ("Erro na abertura do arquivo.");
        exit (1);
    }
    else
    {
        for(i=0; string[i]; i++) 
            putc(string[i], saida); /* Grava a string, caractere a caractere */
        fclose(saida);
    }

return;
}
