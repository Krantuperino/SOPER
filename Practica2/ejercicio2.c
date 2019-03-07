#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	int i;

	for(i=0; i<4; i++){

		pid = fork();

		if (pid < 0) {
			printf("Error when forking");
			return -1;
		}
		else if(pid == 0){
			printf("Soy el proceso hijo <%lld>\n", (long long int)getpid());
			sleep(30);
			printf("Soy el proceso hijo <%lld> y ya me toca terminar\n", (long long int)getpid());
			exit(EXIT_SUCCESS);
		}
		else if(pid > 0){
			sleep(5);
			kill(pid, SIGTERM);
		}
		
	}
	return 0;
}
