#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

typedef struct _queue{
	char items[MAX+1];
	int rear;
	int front;
}queue;

queue *queue_create();

void queue_delete(queue *queue);

void queue_push(queue *queue, char c);

char queue_pop(queue *q);

int queue_isempty(queue *q);

int queue_isfull(queue *q);
