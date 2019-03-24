#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


#define N_PROC 1
#define SEM "/race"
#define END 66
#define MAX_RAND 100000

void manejador_SIGUSR1(int sig) {
	printf("Im ready\n");
	fflush(stdout);
}


int main(){

	int i, *a, flag, readi;
	int count[N_PROC] = {0};
	unsigned int r;
	pid_t pid;
	sem_t *sem = NULL;
	int race;
	struct sigaction act;
	char print;

	sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;

    /* Se arma la señal SIGUSR1. */
    act.sa_handler = manejador_SIGUSR1;
    if (sigaction(SIGUSR1, &act, NULL) < 0) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

	/**
	 * Opening a file
	 */
	if((race = open("race.txt", O_WRONLY | O_APPEND)) < 0){
		perror("file open");
		return -1;
	}

	/**
	 * Opening the semaphore
	 */
	if ((sem = sem_open(SEM, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
        perror("sem_open");
        return -1;
    }

	for(i = 0; i < N_PROC; i++){

		pid = fork();

		if(pid < 0){
			perror("fork");
			return -1;
		}
		else if(pid == 0){
			pause();			//Its not required but I'm making it fair

			while(1){
				sem_wait(sem);

				*a = i;
				
				
				write(race, a, sizeof(int));

				sem_post(sem);

				r = (__useconds_t) ((rand()%MAX_RAND) + 1);
				usleep(r);
			}

			sem_close(sem);
			return 0;
		}

	}
	signal(SIGUSR1, SIG_IGN);
	printf("Ready\n");
	sleep(1);
	printf("Set\n");
	sleep(1);
	printf("Go!\n");
	kill(0, SIGUSR1);

	sleep(1);

	while(flag != END){
		sleep(1);

		sem_wait(sem);

		ftruncate(race, 0);
		
		while(read(race, &readi, sizeof(int)) != 0)
			printf("%d\n", readi);
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

		ftruncate(race, 0);

		sem_post(sem);
	}

	sem_close(sem);
	sem_unlink(SEM);

	close(race);

	for(i = 0; i < N_PROC; i++)
		wait(NULL);

	return 0;
}