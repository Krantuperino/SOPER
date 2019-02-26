/**
 * * @brief Se llama a una funcion que eleva 2 a un valor y se guarda en una variable.
 *			Todo esto lo hace un hilo y se ejecuta repetidas veces para comprobar su correcto funcionamiento.
 *
 * * @file   ejercicio12.c
 * * @author Pablo Sanchez() y Arturo Morcillo (arturo.morcillo@estudiante.uam.es)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#define N 11

/**
* @struct estructura
* @brief estructura que contiene dos enteros que se utilizaran como input y output
*/
struct ESTRUCTURA{
  int input;
  int output;
};

/**
* @name   elevate
* @author   Arturo Morcillo y Pablo Sanchez
* @brief   eleva 2 a un numero introducido y lo almacena en una variable
* @param   void x puntero a la estructura que contendrá el valor a el que lo elevo y donde lo guardo
* @return   void. Esta pensada para utilizar hilos
*/

void *elevate (void *x) {

  ((struct ESTRUCTURA *)x)->output = (int) pow(2, ((struct ESTRUCTURA *)x)->input);
  pthread_exit(NULL);
}

int main(int argc , char *argv[]) {
  pthread_t h1[N];
  struct ESTRUCTURA boi[N];
  int i;

  for(i = 0; i < N; i++){
      boi[i].input = i;
      pthread_create(&h1[i], NULL , elevate , (void *)&boi[i]);

  }



  for(i = 0; i < N; i++){
    pthread_join(h1[i],NULL);
    printf("%d ----> %d\n",boi[i].input,boi[i].output);
  }



  exit(EXIT_SUCCESS);
}
