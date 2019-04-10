/**
 * Productor por fichero
 *
 * Pablo Sanchez(pablo.sanchezredondo@estudiante.uam.es) y Arturo Morcillo (arturo.morcillo@estudiante.uam.es)
 *
 */

 
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <semaphore.h>
#include <sys/wait.h>
#include "queue.h"

#define SEM_P "/sem_p"
#define SEM_C "/sem_c"
#define SEM_Q "/sem_q"
#define SEM_SHM "/sem_shm"
#define QUEUE "queue.txt"

int main(void) {

	int fd_shm;
	int error;
	char caracter = 'a';
	sem_t *sem_p = NULL;
	sem_t *sem_c = NULL;
	sem_t *sem_q = NULL;
	sem_t *sem_shm = NULL;
	queue *example_queue;

    /* Creamos l memoria compartida */
	fd_shm = open(QUEUE,
		O_CREAT | O_RDWR | O_EXCL, /* Create it and open for reading and writing */ 
		S_IRUSR | S_IWUSR); /* The current user can read and write */

	/*Control de errores*/
	if(fd_shm == -1) {
		fprintf (stderr, "Error creating the shared memory segment \n");
		return EXIT_FAILURE;
	}
    
	error = ftruncate(fd_shm, sizeof(queue));
	/*Control de errores*/
	if(error == -1) {
		fprintf (stderr, "Error resizing the shared memory segment \n");
		unlink(QUEUE);
		return EXIT_FAILURE;
	}

	/* Mapeamos la cola */
	example_queue = (queue *)mmap(NULL, sizeof(*example_queue), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);

	/*Control de errores*/
	if(example_queue == MAP_FAILED) {
		fprintf (stderr, "Error mapping the shared memory segment \n");
		return EXIT_FAILURE;
	}

	/*Creamos el semaforo principal*/
	if((sem_p = sem_open(SEM_P, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1))== SEM_FAILED){
		perror("sem_open");
		unlink(QUEUE);
		return(EXIT_FAILURE);
	}

	/*Creamos el semaforo principal*/
	if((sem_c = sem_open(SEM_C, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0))== SEM_FAILED){
		perror("sem_open");
		unlink(QUEUE);
		sem_close(sem_p);
		return(EXIT_FAILURE);
	}

	/*Creamos el semaforo para la cola*/
	if((sem_q = sem_open(SEM_Q, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1))== SEM_FAILED){
		perror("sem_open");
		unlink(QUEUE);
		sem_close(sem_c);
		sem_close(sem_p);
		sem_unlink(SEM_P);
		sem_unlink(SEM_C);
		return(EXIT_FAILURE);
	}

	/*Creamos el semaforo para la memoria compartida*/
	if((sem_shm = sem_open(SEM_SHM, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0))== SEM_FAILED){
		perror("sem_open");
		unlink(QUEUE);
		sem_close(sem_c);
		sem_close(sem_q);
		sem_close(sem_p);
		sem_unlink(SEM_P);
		sem_unlink(SEM_Q);
		sem_unlink(SEM_C);
		return(EXIT_FAILURE);
	}

	/*Una vez hecho todo vamos a introducir la cadena*/
	while(1){
		/*Solo introducimos caracteres si la cola no esta llena*/
		printf("Esperando a que se puedan introducir elementos...\n");
		sem_wait(sem_p);
		printf("Introduce un caracter que introducir a la cola: \n");	
		caracter = getchar();
		printf("Esperando para aceder a la cola...\n");
		/*Esperamos para acceder a la cola*/
		sem_wait(sem_q);

		if(caracter == EOF){
			queue_push(example_queue,'\0');
			printf("Se ha recibido el caracter de fin de cadena...\n");
			/*Hay algo nuevo que consumir*/
			sem_post(sem_c);
			/*ya he terminado con la cola*/
			sem_post(sem_q);
			sem_wait(sem_shm);

			/* Liberamos la memoria compartida */
			munmap(example_queue, sizeof(*example_queue));
			unlink(QUEUE);
			sem_close(sem_p);
			sem_unlink(SEM_P);
			sem_close(sem_c);
			sem_unlink(SEM_C);
			sem_close(sem_q);
			sem_unlink(SEM_Q);
			sem_close(sem_shm);
			sem_unlink(SEM_SHM);

			return EXIT_SUCCESS;
		}
		queue_push(example_queue,caracter);
		printf("Se ha hecho push del caracter: %c\n",caracter);
		/*Hay algo nuevo que consumir*/
		sem_post(sem_c);
		/*ya he terminado con la cola*/
		sem_post(sem_q);
		/*Se puede volver a producir que la cola no esta llena*/
		if(queue_isfull(example_queue) == 0)
			sem_post(sem_p);
		

	}
	
	/*Esperamos a que el consumidor coja la cadena entera o pille end of file para liberar la memoria compartida*/

	/* Liberamos la memoria compartida */
	munmap(example_queue, sizeof(*example_queue));
	close(fd_shm);
	unlink(QUEUE);
	sem_close(sem_p);
	sem_unlink(SEM_P);
	sem_close(sem_c);
	sem_unlink(SEM_C);
	sem_close(sem_q);
	sem_unlink(SEM_Q);
	sem_close(sem_shm);
	sem_unlink(SEM_SHM);

	return EXIT_SUCCESS;
}
