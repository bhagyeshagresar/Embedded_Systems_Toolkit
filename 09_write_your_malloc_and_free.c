#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


typedef struct
{
    size_t size;
    block_meta *next;
    int free;
    int magic; //only used for debugging
}block_meta;

#define META_SIZE sizeof(block_meta)

void* global_base = NULL;




// With the linked list structure  checking if we have a free block and returning it is straightforward. 
// When we get a request of some size, we iterate through our linked list to see if there's a free block that's large enough
block_meta *find_free_block(block_meta **last, size_t size)
{
    block_meta *current = global_base;
    while(current && !(current->free && current->size >= size))
    {
        *last = current;
        current = current->next;
    }
    return current;
}

// If we don't find a free block we request space from the OS using sbrk and add our new block to the end of the linked list
block_meta *request_space(block_meta *last, size_t size)
{
    block_meta *block;
    block = sbrk(0);
    void* request = sbrk(size + META_SIZE);
    assert((void*)block == request);
    if(request == (void*)-1)
    {
        return NULL; //sbrk failed
    }

    //if find_free_block() function does not find a free space then the function stores the last block of memory used in the variable pointer last
    //last will be NULL when allocating on the heap for the first time
    if(last)
    {
        last->next = block;
    }
    block->size = size;
    block->next = NULL;
    block->free = 0 // this current block is not free anymore
    block->magic = 0x12345678;
    return block;
}


void *new_malloc(size_t size)
{
    block_meta* block;
    //TODO: align size?

    //return null if size is invalid
    if(size <= 0){
        return NULL;
    }

    //First call
    if(!global_base)
    {
        block = request_space(NULL, size);
        if(!block)
        {
            return NULL;
        }
        global_base = block;
    }
    else
    {
        block_meta *last = global_base; //get the last used space
        block = find_free_block(&last, size);
        //failed to find the free block
        if(!block)
        {
            block = request_space(last, size);
            if(!block)
            {
                return NULL;
            }
        }
        //found free block
        else
        {
            block->free = 0; //mark it as not free anymore
            block->magic = 0x77777777;

        }

    }

    //block points to the header (the metadata) of the allocated block but malloc returns the pointer to start address of the block so we do block+1 (increment by sizeof(block_meta))
    return (block+1);
}

//  we'll need to get the address of our struct in multiple places in our code, let's define this function.
//this function returns the original block_meta, -1 moves the ptr from the user_data back to the struct block_meta. Since the ptr points to the original datatype the return type is block_meta
block_meta *get_block_ptr(void* ptr)
{
    return (block_meta*)ptr - 1;
}

void free(void* ptr)
{
    //when we calll free(NULL), this evaluates to !0 -> 1, this condition makes sure function does nothing
    if(!ptr)
    {
        return;
    }

    // TODO: consider merging blocks once splitting blocks is implemented.
    block_meta* block_ptr = get_block_ptr(ptr);
    assert(block_ptr->free == 0);
    assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
    block_ptr->free = 1;
    block_ptr->magic = 0x55555555;
}



//A simple malloc
//malloc asks sbrk to increment the heap size and returns a pointer to the start of the new region of heap.
//This does not consider the edge case of malloc(0). It runs fine, it should return a NULL pointer or something that can be passed to free
/*
    Note about sbrk return address (void*)-1 
    In most OSes, -1 is the maximum valid adddress ( 0xFFFFFFFF for 32 bit) so memory allocation cannot start at this address
    Also NULL (void*0) is invalid address because it is reserved for use to identify

*/
void *malloc(size_t size)
{
    void *p = sbrk(0); //sbrk(0) returns a pointer to the current top of the heap
    void *request = sbrk(size); //this pointer should point to the same address as *p
    if(request == (void*) - 1)  /*Note: -1 means  11111111 11111111 11111111 11111111   (for 32-bit) */
    {
        return NULL;
    }
    else
    {
        assert(p == request); //not thread safe(assume that no other operation happened on the heap after the sbrk(0) call)
        return p;
    }

}


