/*
 * Ejemplo de codigo que genera un numero aleatorio
	y lo muestra por pantalla
 */
 #include <sys/types.h>
 #include <sys/wait.h>
 #include <unistd.h>


 #include <time.h>
 #include <stdlib.h>
 #include <stdio.h>

int main(int argc, char *argv[]) {
	/* Inicializa el generador con una semilla cualquiera, OJO! este metodo solo
	 se llama una vez */
	srand(time(NULL));
	/* Devuelve un numero aleatorio en 0 y MAX_RAND(un número alto que varia
	 segun el sistema) */
	int fd_1[2],fd_2[2];
	int nbytes, pipe_status;
	pid_t pid_1, pid_2;
	int r;
	int padre, hijo_final;

	/*Creamos la pipe*/
	pipe_status=pipe(fd_1);
	if(pipe_status == -1) {
		perror("Error creando la tuberia\n");
		exit(EXIT_FAILURE);
	}

	pipe_status=pipe(fd_2);
	if(pipe_status == -1) {
		perror("Error creando la tuberia\n");
		exit(EXIT_FAILURE);
	}

	/*Creamos el primer hijo*/
	pid_1 = fork();
	/*Control de errores*/
	if (pid_1 <  0  )
	{
		printf("Error al emplear fork\n");
		exit (EXIT_FAILURE);
	}

	/*Si es el hijo creamos el aleatorio y lo enviamos*/
	else  if(pid_1 ==  0)
	{
		r = rand();
		close(fd_1[0]);

		write(fd_1[1], (int*)&r, sizeof(int));
		exit(EXIT_SUCCESS);
	}
	/*Si es el padre: */
	/*1.- Recibimos el numero aleatorio*/
	/*2.- Creamos otro hijo*/
	/*3.- Le enviamos al hijo el numero aleatorio*/
	else
	{
		/*Esperamos al hijo*/
		
		/*1*/
		close(fd_1[1]);

		read(fd_1[0], &padre, sizeof(int));
		
		
		/*Ahora ya tenemos el numero almacenado donde el padre*/
		/*2*/

		pid_2 = fork();
		if (pid_2 <  0)
		{
			printf("Error al emplear fork para el segundo hijo\n");
			exit (EXIT_FAILURE);
		}
		else  if(pid_2 > 0)
		{
			 /*Enviamos el numero*/
			close(fd_2[0]);
			
			printf("Recibido %d, Enviando\n", padre);
			
			write(fd_2[1], (int*)&padre, sizeof(int));
			wait(NULL);
		}
		else
		{
			/*Recibimos el numero*/
			close(fd_2[1]);

			read(fd_2[0], &hijo_final, sizeof(int));

			
			printf("El valor del numero aleatorio es: %d\n",hijo_final);
			exit(EXIT_SUCCESS);
		}

	}

	exit(EXIT_SUCCESS);
}
