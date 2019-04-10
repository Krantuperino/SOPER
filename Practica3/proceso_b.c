/**
 * Proceso B: Lee los mensajes de A, los cambia y los envia a C
 *
 * Pablo Sanchez(pablo.sanchezredondo@estudiante.uam.es) y Arturo Morcillo (arturo.morcillo@estudiante.uam.es)
 *
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

typedef struct { 
	char aviso[80];
} Mensaje;
int main(int argc, char **argv) {

	struct mq_attr attributes;
	mqd_t queue_recieve, queue_send;
	Mensaje msg;

	attributes.mq_flags = 0;
	attributes.mq_maxmsg = 10;
	attributes.mq_curmsgs = 0;
	attributes.mq_msgsize = sizeof(Mensaje);


	/*Comprobamos los argumentos*/
    if(argc < 2){
        printf("ERROR. Los argumentos a recibir deben ser el nombre del fichero y el nombre de la cola\n");
        return(EXIT_FAILURE);
    }

	queue_recieve = mq_open(argv[1],
		O_CREAT | O_RDONLY, /* This process is only going to send messages */
		S_IRUSR | S_IWUSR, /* The user can read and write */
		&attributes); 

	if(queue_recieve == (mqd_t)-1) {
		fprintf (stderr, "Error opening the queue B 1\n");
		return EXIT_FAILURE;
	}

	queue_send = mq_open(argv[2],
		O_CREAT | O_WRONLY, /* This process is only going to send messages */
		S_IRUSR | S_IWUSR, /* The user can read and write */
		&attributes);

	if(queue_send == (mqd_t)-1) {
		fprintf (stderr, "Error opening the queue B 2\n");
		return EXIT_FAILURE;
	}

	
	while((mq_receive(queue_recieve, (char *)&msg, sizeof(msg), NULL)) != -1 && strcmp(msg.aviso,"FIN") != 0){
		cambiar(msg.aviso);
		if(mq_send(queue_send, (char *)&msg, sizeof(msg), 1) == -1){
			printf("Error sending to the new queue\n");
		}
    }

	if(mq_send(queue_send, (char *)&msg, sizeof(msg), 1) == -1){
			printf("Error sending FIN\n");
		}
	getchar();
	mq_close(queue_recieve);
	mq_close(queue_send);
	mq_unlink(argv[2]);	
	return EXIT_SUCCESS;
}