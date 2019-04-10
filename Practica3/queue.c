#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

#define MAX 10


queue *queue_create(){
	queue *queue_aux = (queue*)calloc(1, sizeof(queue)); 
	if (queue_aux == NULL)
		return NULL;

	queue_aux->rear = 0;
	queue_aux->front = 0;
	
	return queue_aux;
}

void queue_delete(queue *queue)
{
	if (queue == NULL)
		return;

	free(queue);
}

void queue_push(queue *queue, char c)
{
	if(queue_isfull(queue) == 1){
		printf("No se ha podido hacer push porque la cola esta llena\n");
		return;
	}

	queue->items[queue->rear] = c;
	queue->rear = (queue->rear + 1) % MAX;
}

char queue_pop(queue *q)
{
	char ret;

	if (queue_isempty(q)){
		printf("No se ha podido hacer pop porque la cola esta vacia");
		return -1;
	}

    ret = q->items[q->front];
	q->items[q->front] = 0;
	q->front = (q->front + 1) % MAX;
	
	return ret;
}

int queue_isempty(queue *q){
	if (q == NULL)
		return -1;
	
	if (q->front == q->rear)
		return 1;

	return 0;
}

int queue_isfull(queue *q){
	if (q == NULL)
		return -1;
	
	if(((q->rear+1) % MAX) == q->front)
		return 1;

	return 0;
}
