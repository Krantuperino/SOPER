/**
 * @file
 *
 * @brief Código de ejemplo de cola de mensajes, para un proceso emisor.
 */
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>



typedef struct { 
	char aviso[80];
} Mensaje;
int main(int argc, char **argv) {

    int fd;
	mqd_t queue;
	struct mq_attr attributes;
    Mensaje msg;
    char x[1000];
    char *token;
    char* f;
	const char s[2] = " ";

	


    /*Comprobamos los argumentos*/
    if(argc < 2){
        printf("ERROR. Los argumentos a recibir deben ser el nombre del fichero y el nombre de la cola\n");
        return(EXIT_FAILURE);
    }

    /*Abrimos el file y comprobamos que se ha abierto bien*/
    fd = open (argv[1], O_RDONLY);
    if(fd == -1){
        printf("ERROR al abrir el map\n");
        return (EXIT_FAILURE);
    }

    
    attributes.mq_flags = 0;
	attributes.mq_maxmsg = 10;
	attributes.mq_curmsgs = 0;
	attributes.mq_msgsize = sizeof(Mensaje);

    /*Creamos la cola de mensajes*/
	queue = mq_open(argv[2],
				O_CREAT | O_WRONLY, /* This process is only going to send messages */
				S_IRUSR | S_IWUSR, /* The user can read and write */
				&attributes); 

	if(queue == (mqd_t)-1) {
		fprintf (stderr, "Error opening the queue A\n"); 
		return EXIT_FAILURE;
	}

    /*f contiene todo el contenido del fichero*/
    f = (char *) mmap (0, 2000, PROT_READ, MAP_PRIVATE, fd, 0);
    
    strcpy(x,f);
    token = strtok(x, s);

    
    while( token != NULL ) {
        strcpy(msg.aviso,token);
        if(mq_send(queue, (char *)&msg, sizeof(msg), 1) == -1) {
            fprintf (stderr, "Error sending message\n");
	        
	    }
        token = strtok(NULL, s);      
   }

      
     /* Wait for input to end the program */
	getchar(); 

	mq_close(queue);
	mq_unlink(argv[2]);
	return EXIT_SUCCESS;
}