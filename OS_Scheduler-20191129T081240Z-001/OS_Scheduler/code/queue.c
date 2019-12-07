
#include "headers.h"

typedef struct node
{
struct node * next;
PCB data;
} node;

typedef struct queue
{
   int count;
   node *front,*rear;
}queue;
queue* createQueue()
{
    
    struct queue* q = (queue*)malloc(sizeof(queue)); 
    q->front = q->rear = NULL; 
    q->count=0;
    return q; 
};
void enqueue(queue* q, PCB newPCB)
{
  
    node* temp = (node*)malloc(sizeof(node));
    temp->data=newPCB;
    temp->next = NULL;
    if(q->front==NULL)
    {
       q->front=q->rear=temp;
    }
    else 
    {
        q->rear->next = temp;
        q->rear = temp;
    }
    q->count++;
    q->rear->next = q->front;
    
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
    {
        return NULL;
    }    
    
    PCB  temp;
    
    if(q->front == q->rear)
    {
        temp = q->front->data;
        free(q->front);
        q->front = NULL;
        q->rear = NULL;
    }
    else
    {
        node * nodetemp = q->front;
        temp = nodetemp->data;
        q->front =q->front->next;
        q->rear->next=q->front;
        free(nodetemp);
    }
    
    node * val;
    val->data = temp;
    q->count--; 
    return val; 
} 

node* findNode(queue * q, int nodeNumber) 
{
    int icount =-1 ;
    node* crntNode = q->front;
    while (icount!=nodeNumber) 
    {
        icount++;
        if(icount == nodeNumber)
        {

            return crntNode;
        }
        
        crntNode = crntNode->next;
        if(crntNode==NULL)
            break;
    }
    perror("byrg3 null ya ro7 omak");
    return NULL;
}