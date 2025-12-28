#include <stdbool.h>
#include <stdint.h>

//Time Complexity of all the functions is O(1)

//Leaving one extra space to avoid the edge case of when is it empty vs full
typedef struct {
    int *dataBuffer;
    int newest_index; //newest index points to the position where data is gonna get filled in when Enqueue is called
    int oldest_index;
    int capacity;
} MyCircularQueue;


MyCircularQueue* myCircularQueueCreate(int k) {
    //allocate heap for the struct
    MyCircularQueue *circularQueue = malloc(sizeof(MyCircularQueue));
    if(circularQueue == NULL)
    {
        return NULL;
    }

    //allocate heap for the data buffer
    circularQueue->dataBuffer = malloc((k+1)*sizeof(int));
    if(circularQueue->dataBuffer == NULL)
    {
        return NULL;
    }

    circularQueue->capacity = k+1;
    circularQueue->newest_index = 0;
    circularQueue->oldest_index = 0;
    return circularQueue;
    
}

bool myCircularQueueIsEmpty(MyCircularQueue* obj) {
    return(obj->newest_index == obj->oldest_index);
}

bool myCircularQueueIsFull(MyCircularQueue* obj) {
    int next_index = ((obj->newest_index + 1)%(obj->capacity));
    return (next_index == obj->oldest_index);
}


bool myCircularQueueEnQueue(MyCircularQueue* obj, int value) {
    //check if it is full. if it is full then at this point
    //newest_index is pointing at the last element which is free but cannot be used to fill in because of our 1 space logic for edge cases
    if (myCircularQueueIsFull(obj)) {
        return false;
    }
    obj->dataBuffer[obj->newest_index] = value;
    obj->newest_index = (obj->newest_index + 1)%(obj->capacity);
    return true;
}

bool myCircularQueueDeQueue(MyCircularQueue* obj) {
    //Check if Queue is empty
     if (myCircularQueueIsEmpty(obj)) {
        return false;
    }

    obj->oldest_index = (obj->oldest_index+1)%(obj->capacity);
    return true;
}

//return the oldest element in the queue
int myCircularQueueFront(MyCircularQueue* obj) {
    if(myCircularQueueIsEmpty(obj))
    {
        return -1;
    }
    return obj->dataBuffer[obj->oldest_index];
}

//return the last added element(peek)
int myCircularQueueRear(MyCircularQueue* obj) {

    //(obj->capacity + obj->newest_index - 1) <- this part will always be negative
    //consider the case when the newest index has just wrapped around, the newest index can be zero and then newest_index - 1 will 
    //yield a negative index adding capacity to newest_index - 1 prevents that.

    /*
        Without modulo:

        last_index = 5 + 3 - 1 = 7   // INVALID index!

        But with modulo:

        7 % 5 = 2   // VALID index
    */
    int result = 0;
    int last_index = (obj->capacity + obj->newest_index-1)%(obj->capacity);



    //if queue is empty return -1
      if (myCircularQueueIsEmpty(obj)) {
        return -1;
    }

     result = obj->dataBuffer[last_index];

    return result;
    
}


void myCircularQueueFree(MyCircularQueue* obj) {
    free(obj->dataBuffer);
    free(obj);
}


int main()
{
    //TODO: add test code

    return 0;
}