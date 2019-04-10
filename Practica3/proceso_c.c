/**
 * @file
 *
 * @brief Código de ejemplo de cola de mensajes, para un proceso receptor.
 */
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	struct mq_attr attributes;
	mqd_t queue_recieve;
    char x[2000];

	attributes.mq_flags = 0;
	attributes.mq_maxmsg = 10;
	attributes.mq_curmsgs = 0;
	attributes.mq_msgsize = 6;

	queue_recieve = mq_open(argv[1],
		O_CREAT | O_RDONLY, /* This process is only going to read messages */
		S_IRUSR | S_IWUSR, /* The user can read and write */
		&attributes); 

	if(queue_recieve == (mqd_t)-1) {
		fprintf (stderr, "Error opening the queue C\n");
		return EXIT_FAILURE;
	}


	while((mq_receive(queue_recieve, (char *)&x, sizeof(x), NULL)) != -1 && strcmp(x,"FIN") != 0){
        fflush(stdout);
		printf("%s", x);
    }
			
	printf("\n");
	mq_close(queue_recieve);
	return EXIT_SUCCESS;
}