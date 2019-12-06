
#include "headers.h"

typedef struct node {
struct node * next;
PCB data;
} node;
typedef struct queue
{
   node *front,*rear;
}queue;
queue* createQueue()
{
    struct queue* q = (queue*)malloc(sizeof(queue)); 
    q->front = q->rear = NULL; 
    return q; 
};
void enqueue(queue* q, PCB newPCB)
{
    node* temp = (node*)malloc(sizeof(node));
    temp->data=newPCB;
    temp->next = NULL;
    if(q->rear==NULL)
    {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next=temp;
    q->rear = temp;
};
void priorityEnqueue(queue* q, PCB newPCB)
{
    node* temp = (node*)malloc(sizeof(node));
    temp->data=newPCB;
    temp->next = NULL;
    //if empty queue
    if(q->rear==NULL)
    {
        q->front = q->rear = temp;
        return;
    }
    //else
    node* p = (node*)malloc(sizeof(node));
    //pointer to front
    p=q->front;
    //get to the position wherer last item with priority equal to mine
    while(p->data.priority>=temp->data.priority)
    {
        p=p->next;
    }
    temp->next=p->next;
    p->next=temp;
};
void remainingTimeEnqueue(queue* q, PCB newPCB)
{
    node* temp = (node*)malloc(sizeof(node));
    temp->data=newPCB;
    temp->next = NULL;
    //if empty queue
    if(q->rear==NULL)
    {
        q->front = q->rear = temp;
        return;
    }
    //else
    node* p = (node*)malloc(sizeof(node));
    //pointer to front
    p=q->front;
    //get to the position wherer last item with priority equal to mine
    while(p->data.runTime>=temp->data.runTime)
    {
        p=p->next;
    }
    temp->next=p->next;
    p->next=temp;
};
node* dequeue( queue* q) 
{  
    if (q->front == NULL) 
        return NULL; 
  
    struct node* temp = q->front; 
    q->front = q->front->next; 
    if (q->front == NULL) 
        q->rear = NULL; 
    return temp; 
} 

node* findNode(queue * q, int pid) {
    node* crntNode = q->front;
    while (crntNode != NULL) {
        if(crntNode->data.processID == pid) {
            return crntNode;
        }
        crntNode = crntNode->next;
    }
    return NULL;
}