#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10000
#define SEARCHES 1000
#define MAX_VALUE 100000

typedef struct Node
{
    int value;
    struct Node* next;
} Node;

typedef struct
{
    Node* node;
    int size;
} List;

List* createList()
{
    List* l = (List*)malloc(sizeof(List));
    if(!l)
    {
        printf("Error: Memory allocation problem!\n");
        exit(1);
    }
    l->node = NULL;
    l->size = 0;
    return l;
}

void insert(List* l, int value)
{
    Node *n = (Node*)malloc(sizeof(Node));
    if(!n)
    {
        printf("Error: Memory allocation problem!\n");
        exit(1);
    }
    n->value = value;

    if(l->node == NULL)
    {
        n->next = n;
        l->node = n;
    }
    else
    {
        n->next = l->node->next;
        l->node->next = n;
    }
    l->size++;
}

List* merge(List* l1, List* l2)
{
    if(!l1->node) return l2;
    if(!l2->node) return l1;

    Node* temp = l1->node->next;
    l1->node->next = l2->node->next;
    l2->node->next = temp;

    l1->size += l2->size;
    free(l2);
    return l1;
}

void printList(List* l)
{
    if(!l->node) return;
    Node* start = l->node->next;
    Node* curr = start;
    do
    {
        printf("%d ", curr->value);
        curr = curr->next;
    }while(curr != start);
    printf("\n");
}

int search(List* l, int value, int* compare)
{
    if(!l->node)
    {
        *compare = 0;
        return 0;
    }

    int comp = 0;
    Node* start = l->node->next;
    Node* curr = start;
    do
    {
        comp++;
        if(curr->value == value)
        {
            *compare = comp;
            return 1;
        }
        curr = curr->next;
    }while(curr != start);

    *compare = comp;
    return 0;
}

int main()
{
    srand(time(NULL));

    List* l1 = createList();
    List* l2 = createList();

    printf("List l1: ");
    for(int i=10;i<20;i++)
    {
        insert(l1, i+1);
    }
    printList(l1);

    printf("List l2: ");
    for(int i=20;i<30;i++)
    {
        insert(l2, i+1);
    }
    printList(l2);

    List* mergedList = merge(l1, l2);
    printf("Merged List: ");
    printList(mergedList);

    int T[N];
    List* l = createList();
    for(int i=0;i<N;i++)
    {
        T[i] = rand() % (MAX_VALUE + 1);
        insert(l, T[i]);
    }

    int totalCompInT = 0;
    int totalCompFromI = 0;

    for(int i=0;i<SEARCHES; i++)
    {
        int index = rand() % N;
        int comp;
        search(l, T[index], &comp);
        totalCompInT += comp;

        int randomValue = rand() % (MAX_VALUE + 1);
        search(l, randomValue, &comp);
        totalCompFromI += comp;
    }

    printf("\nAverage cost of searching a value from list: %.2f\n", totalCompInT / (float)SEARCHES);    
    printf("\nAverage cost of searching a random value: %.2f\n", totalCompFromI / (float)SEARCHES);
    return 0;
}