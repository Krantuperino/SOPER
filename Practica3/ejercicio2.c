#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define SHM_NAME "/client_communications"
#define NAME_MAX 255

int flag = 0; 


void manejador_SIGUSR1(int sig) {
}

typedef struct{
	int previous_id; //!< Id of the previous client.
	int id; //!< Id of the current client.
	char name[NAME_MAX]; //!< Name of the client.
} ClientInfo;

int main(int argc, char const *argv[])
{
	int error, i, count = 0;
	int shm;
	int input;
	ClientInfo *ci = NULL;
	pid_t pid;
	int wait_time;
	struct sigaction act;

	if(argc < 2){
		printf("You need to provide a valid number as an input\n");
		return -1;
	}

	input = atoi(argv[1]);
	if(input < 0){
		printf("You need to provide a valid number as an input\n");
		return -1;
	}


	sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;

	act.sa_handler = manejador_SIGUSR1;
    if (sigaction(SIGUSR1, &act, NULL) < 0) {
        perror("sigaction");
        return EXIT_FAILURE;
	}

	// Open the memory segment
	shm = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if(shm == -1){
		printf("Error creating the shared memory segment\n");
		return -1;
	}

	// Resize
	error = ftruncate(shm, sizeof(ClientInfo));
	if(error == -1){
		printf("Error resizing the memory segment");
		return -1;
	}

	// Map the mem segment
	ci = mmap(NULL, sizeof(*ci), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
	if(ci == MAP_FAILED){
		printf("Error mapping the shared memory segment \n");
		shm_unlink(SHM_NAME);
		return -1;
	}
	shm_unlink(SHM_NAME);
	close(shm);

	ci->id = 0;
	ci->previous_id = -1;

	for(i = 0; i < input; i++){
		pid = fork();

		if(pid < 0){
			perror("Fork");
			return -1;
		}
		else if(pid == 0){
			// Wait from 1 to 10 seconds
			srand(getpid());
			wait_time = rand()%10 + 1;
			sleep(wait_time);
			// We change the previous id
			ci->previous_id++;
			// We ask for the client name
			fflush(stdout);
			printf("Please insert the client name: \n");
			fgets(ci->name, sizeof(char) * NAME_MAX, stdin);
			// We change the id
			ci->id++;
			// We send SIGUSR1
			kill(getppid(), SIGUSR1);
			// Unmap memory
			munmap(ci, sizeof(*ci));
			exit(EXIT_SUCCESS);
		}
	}

	while(count < input){
		
		pause();
		fflush(stdout);
		printf("Info del Cliente\n\tNombre: %s\n\tId Anterior:%d\n\tId Actual%d\n", 
			ci->name, ci->previous_id, ci->id);
		flag = 0;
		count++;
	}

	munmap(ci, sizeof(*ci));
	for(i = 0; i<input; i++)
		wait(NULL);

	return 0;
}
