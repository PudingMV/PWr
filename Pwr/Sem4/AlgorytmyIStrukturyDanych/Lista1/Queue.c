#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 50

typedef struct
{
    int arr[MAX_SIZE];
    int front, rear, size;    
} Queue;

void Initilize(Queue *q)
{
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

int isEmpty(Queue *q)
{
    return q->size == 0;
}

int isFull(Queue *q)
{
    return q->size == MAX_SIZE;
}

void push(Queue *q, int value)
{
    if(isFull(q))
    {
        printf("Error: Queue Overflow!!");
        return;
    }
    q->rear = (q->rear + 1) % MAX_SIZE;
    q->arr[q->rear] = value;
    q->size++;
    printf("Pushed: %d onto the queue\n", value);
}

int pop(Queue *q)
{
    if(isEmpty(q))
    {
        printf("Error: Queue Underflow!!");
        return -1;
    }

    int value = q->arr[q->front];
    q->front = (q->front + 1) % MAX_SIZE;
    q->size--;
    printf("Popped: %d from the queue\n", value);
    return value;
}

int main()
{
    Queue q;
    Initilize(&q);

    for(int i=0;i<MAX_SIZE;i++)
    {
        push(&q, i+1);
    }

    while(!isEmpty(&q))
    {
        pop(&q);
    }

    return 0;
}