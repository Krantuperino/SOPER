#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define N_ITER 5
/*Contador del tiempo que espera el hijo*/
#define SECS 40

/* manejador_SIGALRM: saca un mensaje por pantalla y termina el proceso. */
void manejador_SIGALRM(int sig) {
    printf("Soy el proceso <%lld> y he recibido SIGALRM\n",(long long int)getpid());
}

/* manejador_SIGTERM: saca un mensaje por pantalla y termina el proceso. */
void manejador_SIGTERM(int sig) {
    printf("Soy el proceso %lld he recibido la señal SIGTERM\n",(long long int)getpid());
    exit(EXIT_SUCCESS);
}

int main(void) {
    pid_t pid;
    int counter;
    struct sigaction act;   

    /*aqui estaran las señales que quiero bloquear
      cuando se empiece a contar*/
    sigset_t set, set2 , oset; 

    /*set del hijo que bloquea SIGTERM mientras cuenta*/
    sigemptyset(&set);
    sigaddset(&set, SIGTERM);  

    /*set del padre con SIGALRM*/
    sigemptyset(&set2);
    sigaddset(&set2, SIGALRM);   

    pid = fork();

    if(pid < 0){ //caso error
      perror("fork");
      exit(EXIT_FAILURE);
    }

    if(pid == 0){ //Hijo

        /*Queremos montar el manejador de SIGTERM para cuando el hijo lo reciba*/
        sigemptyset(&(act.sa_mask));
        act.sa_flags = 0;   
        
        /* Se arma la señal SIGTERM. */
        act.sa_handler = manejador_SIGTERM;
        if (sigaction(SIGTERM, &act, NULL) < 0) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }

        /*Con el manejador montado podemos empezar a contar*/   while(1){      /*No queremos que el hijo reciba SIGTERM mientras cuenta*/
        if(sigprocmask(SIG_BLOCK, &set, &oset) < 0){
            printf("Error al bloquear las señales");
            exit(EXIT_FAILURE);
        }

        /*Comenzamos a contar*/
        for(counter = 0; counter<N_ITER; counter++){
            printf("%d\n",counter);
            sleep(1);
        }   
        
        /*desbloqueamos las señal SIGTERM antes de la espera de 3 segs*/
        if(sigprocmask(SIG_UNBLOCK, &set, &oset) < 0){
            printf("Error al desbloquear las señales");
            exit(EXIT_FAILURE);
        }
        sleep(3);
    }   

    if(pid > 0){ //Padre
    
    /*Hacemos todo lo relativo a la alarma, ya que hemos creado ya al proceso hijo*/
    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;

    /* Se arma la señal SIGALRM. */
    act.sa_handler = manejador_SIGALRM;
    if (sigaction(SIGALRM, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /*Montamos la alarma para el tiempo especificado*/
    if (alarm(SECS))
        fprintf(stderr, "Existe una alarma previa establecida\n");

    /*Esperamos a SIGALRM de forma especifica*/
    sigprocmask (SIG_BLOCK, &set2, &oset);
    sigsuspend(&oset);
    
    /*SIGTERM al hijo*/
    kill(pid,SIGTERM);
    printf("Le he enviado la señal SIGTERM a mi hijo con pid <%lld>\n",(long long int)pid);
    
    /*Esperamos al hijo*/
    wait(0);
    printf("El proceso hijo <%lld> ha recibido la señal sigterm y ha terminado. Ahora yo <%lld> terminare\n",(long long int)pid,(long long int)getpid());
    exit(EXIT_SUCCESS);
    }

}