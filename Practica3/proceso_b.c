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

void cambiar(char* s){
    int i;
    for(i=0;i<strlen(s);i++){
        if(s[i] == 'z')
            s[i] = 'a';
        else if(s[i] == 'Z')
            s[i] = 'A';
        else
            s[i]++;
    }
}


int main(int argc, char **argv) {

	struct mq_attr attributes;
	mqd_t queue_recieve, queue_send;
    char x[2000];

	attributes.mq_flags = 0;
	attributes.mq_maxmsg = 10;
	attributes.mq_curmsgs = 0;
	attributes.mq_msgsize = 6;

    printf("%s\n",argv[1]);

	queue_recieve = mq_open(argv[1],
		O_RDONLY, /* This process is only going to send messages */
		S_IRUSR | S_IWUSR, /* The user can read and write */
		&attributes); 

	if(queue_recieve == (mqd_t)-1) {
		fprintf (stderr, "Error opening the queue B 1\n");
		return EXIT_FAILURE;
	}

	attributes.mq_msgsize = sizeof(char) * 2000;
	queue_send = mq_open(argv[2],
		O_CREAT | O_WRONLY, /* This process is only going to send messages */
		S_IRUSR | S_IWUSR, /* The user can read and write */
		&attributes);

	if(queue_send == (mqd_t)-1) {
		fprintf (stderr, "Error opening the queue B 2\n");
		return EXIT_FAILURE;
	}


	while((mq_receive(queue_recieve, (char *)&x, sizeof(x), NULL)) != -1 && strcmp(x,"FIN") != 0){
        cambiar(x);
		if(mq_send(queue_send, (char *)&x, sizeof(x), 1) == -1){
			printf("Error sending to the new queue\n");
			return -1;
		}
    }

	getchar();
	mq_close(queue_send);
	mq_unlink(argv[2]);	
	return EXIT_SUCCESS;
}