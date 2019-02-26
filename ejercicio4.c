/**
 * * @brief Crea un hijo, y a su vez este crea otro hijo, asi hasta NUM_PROC, 
 * 			imprimen sus pids con las de sus padres
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
		/*Creamos otro hijo*/
		pid = fork();

		if(pid <  0)
		{
			/*Comprobamos que todo ha ido bien*/
			printf("Error al emplear fork\n");
			exit(EXIT_FAILURE);
		}
		else if(pid ==  0)
		{
			/*Imprimimos pid y pid del padre*/
			printf("HIJO %lld --> PADRE %lld\n", (long long int)getpid(), (long long int)getppid());
		}
		else if(pid >  0)
		{
			/*Imprimimos pid del proceso que es padre de algun hijo*/
			printf("PADRE %lld\n", (long long int)getpid());
			/*Esperamos al unico hijo que tiene y salimos del programa*/
			wait(NULL);
			exit(EXIT_SUCCESS);
		}
	}

	exit(EXIT_SUCCESS);
}

