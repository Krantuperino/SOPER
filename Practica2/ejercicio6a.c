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
    printf("Ha pasado el tiempo establecido hasta la alarma del proceso hijo\n");
    exit(EXIT_SUCCESS);
}


int main(void) {
    pid_t pid;
    int counter;
    struct sigaction act;
    sigset_t set1, set2, oset;

    /*aqui estaran las señales que quiero bloquear cuando se empiece a contar*/
    sigemptyset(&set1);
    sigaddset(&set1, SIGUSR1);
    sigaddset(&set1, SIGUSR2);
    sigaddset(&set1, SIGALRM);

    /*Aqui las señales que desbloquearé cuando termine de contar antes de la espera
    de 3 segundos*/
    sigemptyset(&set2);
    sigaddset(&set2, SIGUSR1);
    sigaddset(&set2, SIGALRM);

    /*Ahora set tiene las funciones que quiero bloquear*/
    pid = fork();

    if(pid < 0){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    else if(pid == 0){

        /*Hacemos todo lo relativo a la alarma*/
        sigemptyset(&(act.sa_mask));
        act.sa_flags = 0;

        /* Se arma la señal SIGALRM. */
        act.sa_handler = manejador_SIGALRM;
        if (sigaction(SIGALRM, &act, NULL) < 0) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }

        /*Montamos la alarma para el tiempo especificado.
          No debería ser capaz de salir mientras cuenta*/
        if (alarm(SECS))
            fprintf(stderr, "Existe una alarma previa establecida\n");

        while(1){
            /*Antes de que cuente bloqueamos las señales*/
            if(sigprocmask(SIG_BLOCK, &set1, &oset) < 0){
                printf("Error al bloquear las señales");
                exit(EXIT_FAILURE);
        }

        /*Comenzamos a contar de 0 a 4*/
        for(counter = 0; counter<N_ITER; counter++){
            printf("%d\n",counter);
            sleep(1);
        }

        /*desbloqueamos las señales antes de la
        espera de 3 segundos, pero sin SIGUSR1*/
        if(sigprocmask(SIG_UNBLOCK, &set2, &oset) < 0){
            printf("Error al desbloquear las señales");
            exit(EXIT_FAILURE);
        }

        sleep(3);
        }

    }

    else if(pid > 0){
        wait(0);
        printf("El proceso hijo <%lld> ha recibido la alarma y ha terminado con exito. Ahora termino yo <%lld>\n",(long long int)pid,(long long int)getpid());
        exit(EXIT_SUCCESS);

    }

}