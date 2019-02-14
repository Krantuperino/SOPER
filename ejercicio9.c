/*
 * Ejemplo de codigo que genera un numero aleatorio y lo muestra por pantalla
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char *argv[]) {

	pid_t pid, pid1;
	int *r, *hello, flag;
	int fd[2], fd1[2];

 /* Inicializa el generador con una semilla cualquiera, OJO! este metodo solo
	se llama una vez */
	printf("Wut");
	srand(time(NULL));
	/* Devuelve un numero aleatorio en 0 y MAX_RAND(un número alto que varia
	 segun el sistema) */

	flag = pipe(fd);
	if(flag == -1)
		printf("Well, shit");
	flag = pipe(fd1);
	if(flag == -1)
		printf("Well, shit again");

	pid = fork();


	if(pid < 0){
		printf("Oopsie");
		exit(EXIT_FAILURE);
	} 
	else if(pid == 0){
		
		close(fd[0]);
		*r = rand();

		printf("Imma send this: %d", *r);
		flag = write(fd[1], &r, sizeof(int));

		if(flag == 0)
			printf("Not written well 0");

		printf("Ive sent this: %d", *r);

		exit(EXIT_SUCCESS);
	} 
	else if (pid > 0){
		pid1 = fork();

		if(pid1 < 0){
			exit(EXIT_FAILURE);
		} 
		else if(pid1 == 0){
			close(fd1[1]);

			flag = read(fd1[0], &hello, sizeof(int));
			
			if(flag == 0)
				printf("Not read well 1");

			printf("My daddy gave me this: %d\n", *hello);

			exit(EXIT_SUCCESS);
		} 
		else if (pid1 > 0){
			close(fd[1]);

			flag = read(fd[0], &hello, sizeof(int));

			if(flag == 0)
				printf("Not read well 0");

			close(fd1[0]);

			printf("OwO %d\n", *hello);

			flag = write(fd1[1], &hello, sizeof(int));

			if(flag == 0)
				printf("Not written well 1");
		}
		

	}

	wait(NULL);
	exit(EXIT_SUCCESS);
}
