/**
 * Proceso C: Lee los mensajes de B y los imprime por pantalla
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
#include <unistd.h>

typedef struct { 
	char aviso[80];
} Mensaje;

int main(int argc, char **argv) {
	sleep(2);
	struct mq_attr attributes;
	mqd_t queue_recieve;
	Mensaje msg;

	attributes.mq_flags = 0;
	attributes.mq_maxmsg = 10;
	attributes.mq_curmsgs = 0;
	attributes.mq_msgsize = sizeof(Mensaje);

	queue_recieve = mq_open(argv[1],
		O_CREAT | O_RDONLY, /* This process is only going to read messages */
		S_IRUSR | S_IWUSR, /* The user can read and write */
		&attributes); 

	if(queue_recieve == (mqd_t)-1) {
		fprintf (stderr, "Error opening the queue C\n");
		return EXIT_FAILURE;
	}

	printf("COMIENZO\n");
	while((mq_receive(queue_recieve, (char *)&msg, sizeof(msg), NULL)) != -1 && strcmp(msg.aviso,"FIN") != 0){
        fflush(stdout);
		printf("PROCESO C:%s\n", msg.aviso);
    }
	
	printf("DONE\n");

	mq_close(queue_recieve);
	mq_unlink(argv[1]);
	return EXIT_SUCCESS;
}