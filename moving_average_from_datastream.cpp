#include <queue>

class MovingAverage {
public:
    std::queue<int> q;   // queue to store elements
    int maxSize;         // maximum size of the window
    double sum;          // current sum of elements in the window

    // Constructor
    MovingAverage(int size) {
        maxSize = size;
        sum = 0;
    }

    // Add a new value and return the moving average
    // Time complexity- O(1) and Space complexity O(size)
    double next(int val) {
        // Add new value to queue and sum
        q.push(val);
        sum += val;

        // If queue size exceeds the maxSize, remove oldest value
        if (q.size() > maxSize) {
            sum -= q.front();
            q.pop();
        }

        // Return average
        return sum / q.size();
    }
};