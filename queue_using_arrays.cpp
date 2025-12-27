#include <iostream>


class myQueue
{
    private:
        int *arr;           //Array to store queue elements
        int capacity;       //max capacity of the queue
        int size;           //current size of the queue
    
    public:

        myQueue(int c)
        {
            capacity = c;
            arr = new int[capacity];
            size = 0;
        }


        //if size is zero that means the queue is empty Time complexity O(1)
        bool isEmpty()
        {
            return (size == 0);
        }

        //Time complexity O(1)
        bool isFull()
        {
            return (size == capacity);
        }

        //Add an element to the rear of the queue Time complexity O(1)
        void enQueue(int x)
        {
            if(isFull())
            {
                std::cout << "The queue is full!\n";
                return;
            }

            arr[size] = x;
            size++;
        }

        //remove an element from the front of the queue Time complexity O(n)
        void deQueue()
        {
            if(isEmpty())
            {
                std::cout << "The queue is empty!\n";
                return;
            }

            for(int i = 1; i < size; i++)
            {
                arr[i-1] = arr[i];
            }
            size--;


        }


        //get the front element of the queue. Time complexity O(1)
        int front()
        {
             if(isEmpty())
            {
                std::cout << "The queue is empty!\n";
                return -1;
            }

            return arr[0];

        }

        //return the last added element of the queue Time complexity O(1)
        int rear()
        {
             if(isEmpty())
            {
                std::cout << "The queue is empty!\n";
                return -1;
            }

            return arr[size-1];
        }
    
};


int main()
{
    myQueue q(4);

    q.enQueue(10);
    q.enQueue(20);
    q.enQueue(30);
    q.enQueue(40);
    std::cout << "Front: " << q.front() << std::endl;
    q.deQueue();
    std::cout << "Front: " << q.front() << std::endl;
    std::cout << "Rear: " << q.rear() << std::endl;
  



}