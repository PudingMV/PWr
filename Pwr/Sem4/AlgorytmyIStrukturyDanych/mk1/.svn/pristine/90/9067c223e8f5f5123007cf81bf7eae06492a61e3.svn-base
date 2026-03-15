#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 50

typedef struct
{
    int arr[MAX_SIZE];
    int top;
} Stack;

void Initialize(Stack *s)
{
    s->top = -1;
}

int isEmpty(Stack *s)
{
    return s->top == -1;
}

int isFull(Stack *s)
{
    return s->top == MAX_SIZE -1;
}

void push(Stack *s, int value)
{
    if(isFull(s))
    {
        printf("Error: Stack Overflow!!");
        return;
    }
    s->arr[++s->top] = value;
    printf("Pushed %d onto the stack\n", value);
}

int pop(Stack *s)
{
    if(isEmpty(s))
    {
        printf("Error: Stack Underflow!!");
        return -1;
    }
    int value = s->arr[s->top--];
    printf("Poped from the stack: %d\n", value);
    return value;
}

int main()
{
    Stack s;

    Initialize(&s);

    for(int i=0;i<MAX_SIZE;i++)
    {
        push(&s, i+1);
    }

    while(!isEmpty(&s))
    {
        pop(&s);
    }

    return 0;
}