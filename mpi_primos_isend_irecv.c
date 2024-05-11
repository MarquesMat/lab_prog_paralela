#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>

int primo (long int n) { /*  primos.c  */
	int i;
       
	for (i = 3; i < (int)(sqrt(n) + 1); i+=2) {
			if(n%i == 0) return 0;
	}
	return 1;
}

int main(int argc, char *argv[]) {
	double t_inicial, t_final;
	int cont = 0, total = 0, destino = 0;
	long int i, n;
	int meu_ranque, num_procs, inicio, salto;
	int etiq = 3;
	MPI_Status estado;
	MPI_Request pedido_envia;
	MPI_Request pedido_recebe;

	if (argc < 2) {
        	printf("Valor inválido! Entre com um valor do maior inteiro\n");
       	 	return 0;
    	} else {
        	n = strtol(argv[1], (char **) NULL, 10);
       	}

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);	
    t_inicial = MPI_Wtime();
	// Essas duas variáveis garantem que os processos vão pegar números diferentes
    inicio = 3 + meu_ranque*2;
    salto = num_procs*2;
	// Distribui os números entre os processos baseado nas variáveis acima
	for (i = inicio; i <= n; i += salto) if(primo(i) == 1) cont++;
	
	// O processo 0 soma a quantidade de números primos calculada por cada processo
    if (meu_ranque == 0) { 
        total = cont; 
        for (int origem = 1; origem < num_procs; origem++) { 
             MPI_Irecv(&cont, 1, MPI_INT, origem, etiq, MPI_COMM_WORLD, &pedido_recebe);
			 MPI_Wait(&pedido_recebe, &estado);
             total += cont; 
        }
    } 
     /* Os demais processos enviam as integrais parciais para o processo 0 */
    else {
        t_final = MPI_Wtime();
		MPI_Isend(&cont, 1, MPI_INT, destino, etiq, MPI_COMM_WORLD, &pedido_envia);
    }
	
	if (meu_ranque == 0) {
		t_final = MPI_Wtime();
        total += 1;    /* Acrescenta o dois, que também é primo */
		printf("Quant. de primos entre 1 e n: %d \n", total);
		printf("Tempo de execucao: %1.3f \n", t_final - t_inicial);	 
	}
	MPI_Finalize();
	return(0);
}