#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(){
	pid_t pid;

	pid = fork();

	if(pid < 0){
		printf("Error creating process A\n");
		return -1;
	}
	else if(pid == 0){
		execl("proceso_a", "proceso_a", "fichero.txt", "queue1", NULL);
		return 0;
	}

	if(pid < 0){
		printf("Error creating process B\n");
		return -1;
	}
	else if(pid == 0){
		execl("proceso_b", "proceso_b", "queue1", "queue2", NULL);
		return 0;
	}

	if(pid < 0){
		printf("Error creating process C\n");
		return -1;
	}
	else if(pid == 0){
		execl("proceso_c", "proceso_c", "queue2", NULL);
		return 0;
	}

	return 0;
}