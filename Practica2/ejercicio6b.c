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
void manejador_SIGTERM(int sig) {
    printf("Soy %lld he recibido la señal SIGTERM\n",(long long int)getpid());
    exit(EXIT_SUCCESS);
}


int main(void) {
  pid_t pid;
  int counter;
  struct sigaction act;

  /*aqui estaran las señales que quiero bloquear
    cuando se empiece a contar*/
  sigset_t set, oset;

  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  sigaddset(&set, SIGUSR2);
  sigaddset(&set, SIGALRM);

  /*Ahora set tiene las funciones que quiero bloquear*/

  pid = fork();

  if(pid < 0){
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if(pid == 0){

    /*Hacemos todo lo relativo a la alarma*/
    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;

    /* Se arma el manejador de señal de SIGALRM. */
    act.sa_handler = manejador_SIGTERM;
    if (sigaction(SIGTERM, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }


    while(1){
      /*Antes de que cuente bloqueamos las señales*/
      if(sigprocmask(SIG_BLOCK, &set, &oset) < 0){
        printf("Error al bloquear las señales");
        exit(EXIT_FAILURE);
      }

      /*Comenzamos a contar de 0 a 4*/
      for(counter = 0; counter<N_ITER; counter++){
        printf("%d\n",counter);
        sleep(1);
      }

      /*desbloqueamos las señales antes de la
        espera de 3 segundos*/
        if(sigprocmask(SIG_UNBLOCK, &set, &oset) < 0){
          printf("Error al desbloquear las señales");
          exit(EXIT_FAILURE);
        }

      sleep(3);
    }

  }

  if(pid > 0){ /*Padre*/
    /*Espera lo 40 segundos*/
    sleep(SECS);
    kill(pid, SIGTERM);
    wait(0);

    exit(EXIT_SUCCESS);

  }

  while (wait(NULL) > 0);


}