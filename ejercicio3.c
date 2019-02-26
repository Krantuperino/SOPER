/**
 * * @brief Crea NUM_PROC hijos e imprime sus pid y las de sus padres
 *
 * * @file   ejercicio12.c
 * * @author Pablo Sanchez() y Arturo Morcillo (arturo.morcillo@estudiante.uam.es)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROC 3

int main(void)
{
	pid_t pid;
	int i;

	for(i = 0; i < NUM_PROC; i++)
	{
		pid = fork();

		if(pid <  0)
		{
			/*Comprobamos que todo ha ido bien*/
			printf("Error al emplear fork\n");
			exit(EXIT_FAILURE);
		}
		else if(pid ==  0)
		{
			/*Imprimimos pid y pid del padre y salimos*/
			printf("HIJO %lld --> PADRE %lld\n", (long long int)getpid(), (long long int)getppid());
            exit(EXIT_SUCCESS);
		}
		else if(pid >  0)
		{
			/*Imprimimos pid del proceso padre*/
			printf("PADRE %lld\n", (long long int)getpid());
		}
	}

    /*Si queremos que espere a todos sus hijos deberia ejecutar 3 waits*/
	wait(NULL);
	exit(EXIT_SUCCESS);
}

