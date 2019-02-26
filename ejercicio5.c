/**
 * * @brief Crea un hijo que escribe hola en una palabra y lo imprime desde el proceso padre
 *			DISCLAIMER: No hace nada porque no usa pipes para enviar el cambio que ha realizado el hijo.
 *
 * * @file   ejercicio12.c
 * * @author Pablo Sanchez() y Arturo Morcillo (arturo.morcillo@estudiante.uam.es)
 *
 */

/* wait and return process info */
#include <sys/types.h>
#include <sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int  main ( void )
{
	pid_t pid;
	/*Reserva memoria para la frase y se apunta en sentece*/
	char * sentence = (char *)malloc(5 * sizeof (char));
	pid = fork();
	
	/*Comprobamos si ha habido error*/
	if (pid <  0  )
	{
		printf("Error al emplear fork\n");
		exit (EXIT_FAILURE);
	}
	/*Proceso hijo*/
	else  if(pid ==  0)
	{
		/*Guardamos hola en la frase*/
		strcpy(sentence, "hola");
		exit(EXIT_SUCCESS);
	}
	else
	{
		/*Esperamos al hijo e imprimimos la frase*/
		wait(NULL);
		printf("Padre: %s\n", sentence);
        free(sentence);
		exit(EXIT_SUCCESS);
	}
}