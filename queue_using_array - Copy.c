// Queue using fixed sized array
//Front pointer points to the first element of the queue
//Rear pointer points to the last element of the queue

#include <stdio.h>

#define MAX_SIZE 5


typedef struct
{
    int queue[MAX_SIZE];
    int front;
    int rear;
}Queue;


//Initialize the queue
void initializeQueue(Queue *q)
{
    q->front = -1;
    q->rear = -1;
}

//Check if queue is empty
int isEmpty(Queue *q)
{
    return (q->front == -1);
}

//Check if queue is full
int isFull(Queue *q)
{
    return (q->rear == MAX_SIZE-1);
}

//Function to insert an element in the queue
void enqueue(Queue *q, int data)
{
    if(isFull(q))
    {
        return;
    }

    if(isEmpty(q))
    {
        q->front = 0;

    }

    q->rear++;
    q->queue[q->rear] = data;

}


//Deqeue
int dequeue(Queue *q)
{
    if(isEmpty(q))
    {
        return -1;
    }

    int data = q->queue[q->front];
    //here if queue is empty then reset the pointers
    if(q->front == q->rear)
    {
        q->front = -1;
        q->rear = -1;
    }
    else
    {
        q->front++;
    }

    return data;
}

void display(Queue *q)
{
    if(isEmpty(q))
    {
        return;
    }

    for(int i = q->front; i <= q->rear; i++)
    {
        printf("%d", q->queue[i]);
    }
}

int main()
{
    Queue *q;
    initializeQueue(&q);

    enqueue(&q, 1);
    enqueue(&q, 2);
    enqueue(&q, 3);


}