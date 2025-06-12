#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *pNextNode;
} Node;

int main(void)
{
    void printLinkedList(struct Node * pNode);
    void printLinkedListUsingReccursion(struct Node * pNode);
    void printReversedLinkedListUsingReccursion(struct Node * pNode);
    void addNodeAfter(struct Node * head, struct Node * newNode, int data);
    void addNodeAtBegining(struct Node * *head, struct Node * newNode);
    void addNodeAtEnd(struct Node * head, struct Node * newNode);
    void deleteNode(struct Node * head, int data);
    void deleteNodeAfter(struct Node * head, int data);

    printf("Hello World\n");

    // struct Node *pHead;

    // struct Node node1 = (struct Node){1, NULL};
    // struct Node node2 = (struct Node){2, NULL};
    // struct Node node3 = (struct Node){3, NULL};

    Node *pHead = (Node *)malloc(sizeof(Node));
    Node *node1 = (Node *)malloc(sizeof(Node));
    Node *node2 = (Node *)malloc(sizeof(Node));
    Node *node3 = (Node *)malloc(sizeof(Node));

    
    node1->pNextNode = &node2;
    node2->pNextNode = &node3;

    pHead = &node1;

    printLinkedList(pHead);
    printf("\n");
    printLinkedListUsingReccursion(pHead);
    printf("\n");
    printReversedLinkedListUsingReccursion(pHead);
    printf("\n");

    struct Node node4 = (struct Node){4, NULL};
    addNodeAfter(pHead, &node4, 2);

    printf("Linked list after adding new node\n");
    printLinkedList(pHead);
    printf("\n");

    struct Node node5 = (struct Node){5, NULL};
    addNodeAtBegining(&pHead, &node5);
    printf("Linked list after adding new node at begining\n");
    printLinkedList(pHead);
    printf("\n");

    struct Node node6 = (struct Node){6, NULL};
    addNodeAtEnd(pHead, &node6);
    printf("Linked list after adding new node at end\n");
    printLinkedList(pHead);
    printf("\n");

    deleteNode(pHead, 3);
    printf("Linked list after deleting node with value 3\n");
    printLinkedList(pHead);
    printf("\n");

    deleteNodeAfter(pHead, 1);
    printf("Linked list after deleting node next to node with value 1\n");
    printLinkedList(pHead);
    printf("\n");
}

void printLinkedList(struct Node *pNode)
{
    struct Node *temp = pNode;
    while (temp != NULL)
    {
        printf("[%d] ", temp->data);
        temp = temp->pNextNode;
    }
}

void printReversedLinkedListUsingReccursion(struct Node *pNode)
{
    if (pNode == NULL)
    {
        return;
    }
    printLinkedListUsingReccursion(pNode->pNextNode);
    printf("[%d] ", pNode->data);
}

void printLinkedListUsingReccursion(struct Node *pNode)
{
    if (pNode == NULL)
    {
        return;
    }
    printf("[%d] ", pNode->data);
    printLinkedListUsingReccursion(pNode->pNextNode);
}

void addNodeAfter(struct Node *head, struct Node *newNode, int data)
{
    struct Node *temp = head;
    while (temp->data != data)
    {
        temp = temp->pNextNode;
    }

    newNode->pNextNode = temp->pNextNode;
    temp->pNextNode = newNode;
}

void addNodeAtBegining(struct Node **head, struct Node *newNode)
{
    newNode->pNextNode = *head;
    *head = newNode;
}

void addNodeAtEnd(struct Node *head, struct Node *newNode)
{
    struct Node *lastNode = head;
    while (lastNode->pNextNode != NULL)
    {
        lastNode = lastNode->pNextNode;
    }

    lastNode->pNextNode = newNode;
}

void deleteNodeAfter(struct Node *head, int data)
{
    struct Node *temp = head;
    while (temp->data != data)
    {
        temp = temp->pNextNode;
    }

    temp->pNextNode = temp->pNextNode->pNextNode;
}

void deleteNode(struct Node *head, int data)
{
    struct Node *temp = head;
    while (temp->pNextNode->data != data)
    {
        temp = temp->pNextNode;
    }

    temp->pNextNode = temp->pNextNode->pNextNode;
}
