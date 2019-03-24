#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SEM1 "/sem1"
#define SEM2 "/sem2"
#define SEM3 "/sem3"


#define NREAD 10

#define SECS 1



/* SIGINT: Avisa de que ha recibido la señal y termina. */
void manejador_SIGINT(int sig) {
    int i;

    kill(0, SIGTERM);

    for(i = 0; i < NREAD; i++)
        wait(0);    

	exit(EXIT_SUCCESS);
}


int main() {

    
    
    struct sigaction act;

    int i, sval;
    pid_t pid;

    sem_t *semaforo_lectura = NULL;
    sem_t *semaforo_escritura = NULL;
    sem_t *semaforo_lectores = NULL; //Este simulara ser una variable compartida



    /*******************************************************/
    /***************Creamos todo lo necesario***************/
    /*******************************************************/

    /*creamos el semaforo de lectura*/
    if ((semaforo_lectura = sem_open(SEM1, O_CREAT, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
        perror("sem_open lectura");
        exit(EXIT_FAILURE);
    }

    
    /*creamos el semaforo de escritura*/
    if ((semaforo_escritura = sem_open(SEM2, O_CREAT, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
        perror("sem_open escritura");
        exit(EXIT_FAILURE);
    }

    /*creamos el semaforo de lectores (simula ser una variable compartida)*/
    if ((semaforo_lectores = sem_open(SEM3, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
        perror("sem_open lectores");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;

    /* Se arma la señal SIGINT.*/
    act.sa_handler = manejador_SIGINT;
    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        return EXIT_FAILURE;
}

    
    
    
    /*Creamos os hijos que son los lectores*/

    for (i = 0; i < NREAD; i++){
        pid = fork();

        if (pid < 0){
            printf("Error al crear los procesos\n");
            return 0;
        }
        
        else if (pid == 0){
            break;
        }
            
    }
        
    
    /*******************************************************/
    /***********Ya hemos creado todo lo necesario***********/
    /*******************************************************/

   /*Caso de los hijos (lectores)*/
   if (pid == 0){
       
       while(1){
        
        sem_wait(semaforo_lectura); //down

        sem_post(semaforo_lectores); //up aka lectores++;

        sem_getvalue(semaforo_lectores, &sval);


        if(sval == 1) //sval = lectores
            sem_wait(semaforo_escritura); //down No puede escribir

        sem_post(semaforo_lectura); //up


        printf("R-INI %lld\n",(long long int)getpid());
        sleep(1);
        printf("R-FIN %lld\n",(long long int)getpid());

        sem_wait(semaforo_lectura);

        sem_wait(semaforo_lectores); //aka lectores--;

        sem_getvalue(semaforo_lectores, &sval);
        
        if(sval == 0){ //sval = lectores
            sem_post(semaforo_escritura); //up
        }

        sem_post(semaforo_lectura); //up


        sleep(SECS);

       }
   }
    /*Caso del padre (escritor)*/
    else if (pid > 0){

       while(1){
           
            sem_wait(semaforo_escritura); //down
            printf("W-INI %lld\n",(long long int)getpid());
            sleep(1);
            printf("W-FIN %lld\n",(long long int)getpid());
            sem_post(semaforo_escritura); //up

            sleep(SECS);
       }

       }

       return 0;
}