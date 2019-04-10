#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>

#include "queue.h"


#define SHM_QUEUE "/shm_queue"
#define SEM_P "/sem_p"
#define SEM_C "/sem_c"
#define SEM_Q "/sem_q"
#define SEM_SHM "/sem_shm"



int main(void) {

	int fd_shm;
	sem_t *sem_p =NULL;
	sem_t *sem_c =NULL;
	sem_t *sem_q = NULL;
	sem_t *sem_shm = NULL;
	queue *example_queue;
	char aux = 'a';
	

	/* Abrimos la memoria compartida */
	fd_shm = shm_open(SHM_QUEUE,
		O_RDWR | O_EXCL,
		S_IRUSR | S_IWUSR); /* Unused */ 
	
	/*Control de errores*/
	if(fd_shm == -1) {
		fprintf (stderr, "Error opening the shared memory segment \n"); return EXIT_FAILURE;
	}
      
	/* Mapeamos la memoria ya creada */
	example_queue = (queue *)mmap(NULL, sizeof(*example_queue), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
	if(example_queue == MAP_FAILED) {
		fprintf (stderr, "Error mapping the shared memory segment \n");
		return EXIT_FAILURE;
	}

	/*Abrimos el semaforo ya creado del productor*/
	if((sem_p = sem_open(SEM_P, 0))== SEM_FAILED){
		perror("Error opening the semaphore");
		return(EXIT_FAILURE);
		}
	
	/*Abrimos el semaforo ya creado del consumidor*/
	if((sem_c = sem_open(SEM_C, 0))== SEM_FAILED){
		perror("Error opening the semaphore");
		return(EXIT_FAILURE);
		}
	
	/*Abrimos el semaforo ya creado para la cola*/
	if((sem_q = sem_open(SEM_Q, 0))== SEM_FAILED){
		perror("Error opening the semaphore");
		return(EXIT_FAILURE);
		}

	/*Abrimos el semaforo ya creado para la cola*/
	if((sem_shm = sem_open(SEM_SHM, 0))== SEM_FAILED){
		perror("Error opening the semaphore");
		return(EXIT_FAILURE);
		}
	
	/*En todo el contro de errores no tenemos que liberar memoria. De eso se encarga el otro proceso*/
	
	/*consumimos caracteres de la cola hasta que nos llegue la señal de fin de cadena*/
    while(aux != '\0'){
			/*Esperamos que haya algo que podamos consumir*/
			printf("Esperando que haya algo que consumir...\n");
			sem_wait(sem_c);
			/*esperamos que no este utilizando la cola*/
			printf("Esperando para acceder a la cola...\n");
			sem_wait(sem_q);
			/*Si estaba llena se podra volver a producir*/
			if(queue_isfull(example_queue))
				sem_post(sem_p);
			aux = queue_pop(example_queue);

			if(aux == '\0'){
				printf("Se ha recibido el caracter de fin de cadena...\n");
				munmap(example_queue, sizeof(*example_queue));
				sem_post(sem_shm);
				return EXIT_SUCCESS;
			}

			printf("Se ha hecho pop del caracter: %c\n",aux);
			/*Hemos terminado con la cola, se puede volver a utilizar*/
			sem_post(sem_q);
	}	

	/* desmapeamos la memoria compartida */
	munmap(example_queue, sizeof(*example_queue));
	
	return EXIT_SUCCESS;
}

