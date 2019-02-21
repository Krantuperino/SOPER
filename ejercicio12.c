#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#define N 11

struct BOI{
  int input;
  int output;
};

void *elevate (void *x) {

  ((struct BOI *)x)->output = (int) pow(2, ((struct BOI *)x)->input);
  pthread_exit(NULL);
}

int main(int argc , char *argv[]) {
  pthread_t h1[N];
  struct BOI boi[N];
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