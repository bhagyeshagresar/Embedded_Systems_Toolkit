typedef struct {
    int *data;
    int newest_index;
    int oldest_index;
    int capacity;
} MyCircularQueue;


MyCircularQueue* myCircularQueueCreate(int k) {
    MyCircularQueue *circularQueue = malloc(sizeof(MyCircularQueue));
    if(!circularQueue)
    {
        return NULL;
    }

    //allocate the data buffer
    circularQueue->data = malloc((k+1)*sizeof(int));
    if(!circularQueue->data)
    {
        return NULL;
    }
    circularQueue->capacity = k+1;
    circularQueue->newest_index = 0;
    circularQueue->oldest_index = 0;

    return circularQueue;

}

bool myCircularQueueIsEmpty(MyCircularQueue* obj) {
    return (obj->oldest_index == obj->newest_index);
}

bool myCircularQueueIsFull(MyCircularQueue* obj) {
    int next_index = (obj->newest_index+1)%(obj->capacity);
    return next_index == obj->oldest_index;
}


bool myCircularQueueEnQueue(MyCircularQueue* obj, int value) {
    if (myCircularQueueIsFull(obj)) {
        return false;
    }

    obj->data[obj->newest_index] = value;
    obj->newest_index = (obj->newest_index+1)%(obj->capacity);
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

int myCircularQueueFront(MyCircularQueue* obj) {
    //Check if Queue is empty
      if (myCircularQueueIsEmpty(obj)) {
        return -1;
    }

    return obj->data[obj->oldest_index];
}

int myCircularQueueRear(MyCircularQueue* obj) {

    int result = 0;
    int last_index = ((obj->capacity + obj->newest_index -1)%(obj->capacity));

    //if queue is empty return -1
      if (myCircularQueueIsEmpty(obj)) {
        return -1;
    }


    result = obj->data[last_index];

    return result;
}


void myCircularQueueFree(MyCircularQueue* obj) {
    free(obj);
}