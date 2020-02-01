#include <stdbool.h> // bool
#include <stdio.h>   // printf()
#include <stdlib.h>  // malloc()

struct node
{
    int data;
    struct node *next;
    struct node *prev;
};

struct node *create(int elem)
{
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->data = elem;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

int main()
{
    struct node *pointer1 = create(4);
    struct node *pointer2 = create(5);

    pointer1->next = pointer2;

    struct node *temp = pointer1->next;

    pointer2->data = 6;

    pointer1->next = NULL;

    printf("%d\n", temp->data);
    if (pointer1->next)
    {
        printf("p1.next is not empty");
    }
}
