/**
 * * @brief Crea dos procesos hijo, el primero usa ls en consola para enseñar todos los
 *          archivos por pantalla y el segundo usa cat sobre uno o varios ficheros que le
 *          indiquen por teclado
 *
 * * @file   ejercicio12.c
 * * @author Pablo Sanchez(pablo.sanchezredondo@estudiante.uam.es) y Arturo Morcillo (arturo.morcillo@estudiante.uam.es)
 *
 */

/* wait and return process info */
#include <sys/types.h>
#include <sys/wait.h>
/* standard input/output */
#include <stdio.h>
/* malloc, free */
#include <stdlib.h> /* library for exec */
#include <unistd.h>
//for comparing strings
#include<string.h>

/*
* Pide al usuario una serie de ficheros separados por comas, los almacena en
* un vector de strings, crea un nuevo proceso y ejecuta el comando cat con el
* vector de strings como vector de argumentos
*/
void  processCat () {

    /* Variables que usa el metodo getline para leer la entrada del usuario */
    char *fileName = NULL;
    size_t fileLen = 0;
    ssize_t fileRead;
    pid_t pid;


    /* pide al usuario una linea de texto con todos los ficheros separados por comas */
    printf("Introduzca los ficheros que quiere mostrar separados por ',' \n");
    while((fileRead = getline(&fileName, &fileLen, stdin)) < 1)
    {
   	 printf("Por favor inserte al menos un fichero \n");
    }

    /* Cuenta el número de ficheros */
    size_t fileCount = 0;
    for(ssize_t i = 0; i < fileRead; i++)
    {
   	 if(fileName[i] == ',' || fileName[i] == '\n')
   	 {
   		 fileCount++;
   	 }
    }

    size_t nArgs = fileCount + 2;
    /* Reserva espacio para argumentos */
    char ** args = malloc(nArgs * sizeof(*args));
    if(args == NULL)
    {
   	 exit(EXIT_FAILURE);
    }

    args[0] = "cat";

    /*Lee por teclado todos los archivos, separados por ','*/
    char * filePtr = fileName;
    size_t argIndex = 1;
    for(ssize_t i = 0; i < fileRead; i++)
    {
   	 if(fileName[i] == ',' || fileName[i] == '\n')
   	 {
   		 fileName[i] = '\0';
   		 args[argIndex] = filePtr;
   		 argIndex++;
   		 filePtr = &fileName[i + 1];
   	 }
    }

    args[nArgs - 1] = NULL;

    if (nArgs >  1) {

     /*
   	 Creamos un nuevo proceso hijo y en el ejecutamos execv para ejecutar el
   	 comando cat con el vector de argumentos args. El padre debe esperar a que
   	 el hijo termine
     */

     pid = fork();
     /*Si el fork falla*/
     if(pid <  0)
 		{
 			printf("Error al emplear fork\n");
 			exit(EXIT_FAILURE);
 		}
    /*Si es el proceso hijo hacemos el cat*/
 		else if(pid ==  0)
 		{
      execv("/bin/cat", args);
 		}
    /*Si es el padre esperamos al hijo*/
 		else if(pid >  0)
 		{
 			wait(NULL);
 		}

    }

    /* Liberamos la memoria dinamica reservada por el proceso */
    free (args);
    /* liberamos la memoria reservada por getline */
    free (fileName);
}

void  showAllFiles () {
   /*
    * Creamos un nuevo proceso hijo usando la llamada execlp y en el ejecutamos el
    * comando ls -l. El proceso padre debe de esperar a que el hijo termine.
    */

    pid_t pid;

    pid = fork();
    /*Si el fork sale mal*/
		if(pid <  0)
		{
			printf("Error al emplear fork\n");
			exit(EXIT_FAILURE);
		}
    /*Si es el hijo usamos execlp y ejecutamos ls*/
		else if(pid ==  0)
		{
      /*El primer argumento es el path donde se encuentra lo que vamos a ejecutar*/
      /*El segundo y demás lo siguiente que vamos a ejecutar*/
      /*El ultimo debe ser NULL*/
      execlp("ls", "ls", "-l", (char *)NULL);
      exit(EXIT_SUCCESS);
		}
    /*Si es el padre esperamos al proceso hijo*/
		else if(pid >  0)
		{
      /*Esperamos a que termine el hijo anterior*/
			wait(NULL);
      }
}

int  main(void) {
    showAllFiles();
    processCat();
    exit (EXIT_SUCCESS);
}
