#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define N_PROC 3 

/* manejador_SIGUSR1: Avisa de que ha recibido la señal y termina. */
void manejador_SIGUSR1(int sig) {
	printf("Capturada la señal SIGUSR1 :)\n");
	fflush(stdout);
	exit(EXIT_SUCCESS);
}

void manejador_SIGUSR2(int sig) {
	printf("Recieved SIGUSR2\n");
	fflush(stdout);
}

int main(void) {
    struct sigaction act;
	pid_t pid, pidarr[N_PROC], pidfather1, pidfather2;
	int i;

    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;

    /* Se arma la señal SIGUSR1. */
    act.sa_handler = manejador_SIGUSR1;
    if (sigaction(SIGUSR1, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

	act.sa_handler = manejador_SIGUSR2;
    if (sigaction(SIGUSR2, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

	pidfather1 = getpid();
	pid = fork();
	if(pid < 0){
		printf("Error when forking\n");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0){
		for(i = 0; i < N_PROC; i++){

			pidfather2 = getpid();
			pidarr[i] = fork();

			if(pidarr[i] < 0){
				printf("Error at fork\n");
				fflush(stdout);
				exit(EXIT_FAILURE);
			}
			else if(pidarr[i] == 0){
				kill(pidfather2, SIGUSR2);
				pause();
			}
			else if(pidarr[i] > 0){
				pause();
			}
		}

		kill(pidfather1, SIGUSR2);
		pause();
	}
	else if(pid > 0){
		/*kill(0, ...) Envia a todos los procesos*/
		pause();
		signal(SIGUSR1, SIG_IGN);
		kill(0, SIGUSR1);
		wait(NULL);
	}

    return 0;
}