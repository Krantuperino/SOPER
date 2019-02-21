#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROC 3

int main(void)
{
	pid_t pid;
	int i;

	for(i = 0; i < NUM_PROC; i++)
	{
		pid = fork();

		if(pid <  0)
		{
			printf("Error al emplear fork\n");
			exit(EXIT_FAILURE);
		}
		else if(pid ==  0)
		{
			printf("HIJO %lld --> PADRE %lld\n", (long long int)getpid(), (long long int)getppid());
            exit(EXIT_SUCCESS);
		}
		else if(pid >  0)
		{
			printf("PADRE %lld\n", (long long int)getpid());
		}
	}
    
	wait(NULL);
	exit(EXIT_SUCCESS);
}

