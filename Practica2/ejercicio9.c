#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


#define N_PROC 9
#define SEM "/race"
#define END 66
#define MAX_RAND 100000

void manejador_SIGUSR1(int sig) {
	printf("Im ready\n");
	fflush(stdout);
}


int main(){

	int i, a, flag, readi,sval;
	int count[N_PROC] = {0};
	unsigned int r;
	pid_t pid;
	sem_t *sem = NULL;
	int race;
	struct sigaction act;
	char print;
    char buf[4] = {0};

	FILE *file;

	sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;

    /* Se arma la señal SIGUSR1. */
    act.sa_handler = manejador_SIGUSR1;
    if (sigaction(SIGUSR1, &act, NULL) < 0) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

	

	/**
	 * Opening the semaphore
	 */
	if ((sem = sem_open(SEM, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
        perror("sem_open");
        return -1;
    }

    /*Todo esta bien inicializado :)*/

    /*Creamos todos los procesos hijos*/
	for(i = 0; i < N_PROC; i++){

		pid = fork();

		if(pid < 0){ //Error
			perror("fork");
			return -1;
		}

		else if(pid == 0){ //Caso de los hijos
			
			
			fprintf(file,"%d\n",i);
			pause();			

			while(1){
                sem_wait(sem);
				/**
			 	* Opening a file for writing
			 	*/
				if(!(file = fopen("race.txt","a"))){
					printf("Error al abrir el archivo para escribir\n");
					return -1;
				}

				fprintf(file,"%d\n",i);
				fflush(file);

				fclose(file);
				sem_post(sem);

				r = (__useconds_t) ((rand()%MAX_RAND) + 1);
				usleep(r);
                
			}

			sem_close(sem);
			return 0;
		}


	}
	/*El padre*/
	if(!(file = fopen("race.txt","r"))){
		printf("Error al abrir el archivo para el padre\n");
		return -1;
	}

	signal(SIGUSR1, SIG_IGN);
	printf("Ready\n");
	sleep(1);
	printf("Set\n");
	sleep(1);
	printf("Go!\n");
	kill(0, SIGUSR1);

	sleep(2);

	while(flag != END){
		sleep(1);

		sem_wait(sem);

		/*De uno en uno porque en una unidad de tiempo se escriben mogollon*/
		while(fscanf(file, "%d", &readi) && flag != END){
			count[readi]++;
			printf("Estado de la carrera:\n");
			fflush(stdout);
			for(i = 0; i < N_PROC; i++){
				if(count[i] >= 20){
					/*Yes, multiple people can win*/
					printf("Process %d won! Congratulations!\n", i); 
					fflush(stdout);
					signal(SIGTERM, SIG_IGN);
					kill(0, SIGTERM);
					flag = END;
				}
				printf("\tProc %d: %d escrituras\n", i, count[i]);
				fflush(stdout);
			}
		}

		sem_post(sem);
		
			
		
			
		
		


	}

	sem_close(sem);
	sem_unlink(SEM);

	fclose(file);

	file = fopen("race.txt","w");
	fclose(file); //reset

	for(i = 0; i < N_PROC; i++)
		wait(NULL);

	return 0;
}