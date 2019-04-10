/**
 * Manejador de procesos A B y C
 *
 * Pablo Sanchez(pablo.sanchezredondo@estudiante.uam.es) y Arturo Morcillo (arturo.morcillo@estudiante.uam.es)
 *
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(){
	pid_t pid;
	int i;

	pid = fork();

	if(pid < 0){
		printf("Error creating process A\n");
		return -1;
	}
	else if(pid == 0){
		execl("proceso_a", "proceso_a", "fichero.txt", "/mq_queue1", NULL);
		return 0;
	}

	pid = fork();
	if(pid < 0){
		printf("Error creating process B\n");
		return -1;
	}
	else if(pid == 0){
		execl("proceso_b", "proceso_b", "/mq_queue1", "/mq_queue2", NULL);
		return 0;
	}

	pid = fork();
	if(pid < 0){
		printf("Error creating process C\n");
		return -1;
	}
	else if(pid == 0){
		execl("proceso_c", "proceso_c", "/mq_queue2", NULL);
		return 0;
	}

	i = 0;

	while(i<2){
		wait(NULL);
		i++;
	}

	return 0;
}