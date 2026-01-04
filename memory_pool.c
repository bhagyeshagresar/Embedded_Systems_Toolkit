#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

//fixed block memory pool, it operates like a stack (LIFO)
typedef struct
{
    uint8_t* pool;
    size_t pool_size;
    size_t block_size;
    uint8_t* free_list; //head of the linked list
    size_t free_count;
}mem_pool_t;

//Initialize the memory pool
//block size - typically size of a struct
int mem_pool_init(mem_pool_t* pool, size_t block_size, size_t block_count)
{
    pool->block_size = block_size;
    pool->pool_size = block_size*block_count;
    pool->pool = malloc(pool->pool_size);

    if(pool->pool == NULL)
    {
        return -1;
    }

    //initialise the free list
    pool->free_list = pool->pool; 
    pool->free_count = block_count;


     // Link blocks in free list
     /*
     
        current + block_size: This calculates the memory address where the next block starts.
        (uint8_t**): We tell the compiler, "Treat the memory at current as a place where we store another pointer."
        *(...) = ...: We write the address of the next block into the current block.
     */

    uint8_t* current = pool->pool;
    for (size_t i = 0; i < block_count - 1; i++) {
        *(uint8_t**)current = current + block_size; //store the pointer to the next block in the current block(update the next pointer in linked list terminlogy)
        current += block_size;
    }
    *(uint8_t**)current = NULL; //set the next pointer to NULL
    
    return 0;

}

//Allocate from pool -O(1)
void* mem_pool_alloc(mem_pool_t* pool)
{
    if(pool->free_count == 0)
    {
        return NULL; //pool exhausted
    }
    uint8_t* block = pool->free_list;
    pool->free_list = *(uint8_t**)block;
    pool->free_count--;
    return block;
}

//Free to pool
void mem_pool_free(mem_pool_t* pool, void* ptr)
{
    if(ptr == NULL) return;

    *(uint8_t**)ptr = pool->free_list;   //(uint8_t**)ptr says "Go to address 0x2000_0000 and treat it as a n-byte pointer slot."
    pool->free_list = ptr;                   //update the head
    pool->free_count++;

}


typedef struct {
    uint32_t sensor_id;
    float temperature;
    uint32_t timestamp;
} SensorData_t;

int main() {
    mem_pool_t my_pool;
    
    // 1. Initialize: 10 blocks, each large enough for our struct
    // Note: block_size MUST be >= sizeof(void*)
    size_t size = sizeof(SensorData_t) > sizeof(void*) ? sizeof(SensorData_t) : sizeof(void*);
    mem_pool_init(&my_pool, size, 10);

    // 2. ALLOCATE: Get a raw block
    void* raw_ptr = mem_pool_alloc(&my_pool);
    
    if (raw_ptr != NULL) {
        // 3. STORE DATA: Cast the pointer to your struct type
        SensorData_t* data = (SensorData_t*)raw_ptr;
        
        data->sensor_id = 0xAF01;
        data->temperature = 25.5f;
        data->timestamp = 1000;

        printf("Stored: ID: %X, Temp: %.1f\n", data->sensor_id, data->temperature);
    }

    // 4. STORING A STRING (Example)
    void* str_ptr = mem_pool_alloc(&my_pool);
    if (str_ptr) {
        // Just treat it as a char array
        char* name = (char*)str_ptr;
        strncpy(name, "STM32_Node", my_pool.block_size);
        
        printf("Stored String: %s\n", name);
    }

    // 5. FREE: When done, return the pointer
    /*

        Current State (Both blocks are out): free_list -> [Block C] -> [Block D] ...
        free(raw_ptr): raw_ptr (Block A) becomes the new head. It points to Block C.

        free(str_ptr): str_ptr (Block B) becomes the new head. It points to Block A.

        Resulting Chain: free_list -> Block B -> Block A -> Block C
            
    */
    mem_pool_free(&my_pool, raw_ptr);
    mem_pool_free(&my_pool, str_ptr);

    return 0;
}