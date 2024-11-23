这是一个Qt学习的git仓库，我将在其中记录我的Qt学习以及一些Qt项目。
配套文件中，.pro文件解释.txt是我入门时对.pro文件的学习记录；qt学习.txt是我目前Qt学习的知识点总结。

其余文件基本上都是我创建的项目文件，其中包含各知识点的应用。
后面将会有一个我已完成的律师事务系统项目，其中包含了qt和数据库之间的协同运行。
还有目前我正在学习的opencv在Qt上的使用，在尝试做一个基本的人脸识别。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "memory_management.h"

#define MEMORY_SIZE 1024 * 1024  // 1MB memory pool
static char memory_pool[MEMORY_SIZE];
static char *free_list = memory_pool;

// Structure to keep track of allocated memory blocks
typedef struct Block {
    size_t size;
    struct Block *next;
    int is_free;
} Block;

static Block *head = NULL;

// my_malloc function
void *my_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    // Align the size to the nearest multiple of 16
    size_t aligned_size = (size + 15) & ~15;

    // Find a suitable free block
    Block *current = head;
    Block *prev = NULL;
    while (current != NULL) {
        if (current->is_free && current->size >= aligned_size) {
            // Found a suitable block
            if (current->size - aligned_size >= sizeof(Block) + 15) {
                // Split the block
                Block *new_block = (Block *)((char *)current + sizeof(Block) + aligned_size);
                new_block->size = current->size - aligned_size - sizeof(Block);
                new_block->is_free = 1;
                new_block->next = current->next;
                current->size = aligned_size;
                current->next = new_block;
            }
            current->is_free = 0;
            return (void *)((char *)current + sizeof(Block));
        }
        prev = current;
        current = current->next;
    }

    // No suitable block found, allocate a new block from the free list
    if ((char *)free_list + aligned_size + sizeof(Block) <= memory_pool + MEMORY_SIZE) {
        Block *new_block = (Block *)free_list;
        new_block->size = aligned_size;
        new_block->is_free = 0;
        new_block->next = NULL;
        if (head == NULL) {
            head = new_block;
        } else {
            prev->next = new_block;
        }
        free_list += aligned_size + sizeof(Block);
        return (void *)((char *)new_block + sizeof(Block));
    }

    // Out of memory
    return NULL;
}

// my_free function
void my_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    Block *block = (Block *)((char *)ptr - sizeof(Block));
    block->is_free = 1;

    // Merge with previous block if it is free
    Block *prev = head;
    while (prev != NULL && prev->next != block) {
        prev = prev->next;
    }
    if (prev != NULL && prev->is_free) {
        prev->size += block->size + sizeof(Block);
        prev->next = block->next;
        block = prev;
    }

    // Merge with next block if it is free
    if (block->next != NULL && block->next->is_free) {
        block->size += block->next->size + sizeof(Block);
        block->next = block->next->next;
    }
}

// Function to print memory status
void print_memory_status() {
    Block *current = head;
    while (current != NULL) {
        printf("Block at %p: size = %zu, is_free = %d\n", current, current->size, current->is_free);
        current = current->next;
    }
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "memory_management.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <MB>\n", argv[0]);
        return 1;
    }

    size_t mb = atoi(argv[1]);
    size_t size = mb * 1024 * 1024;

    // Allocate an int array
    int *int_array = (int *)my_malloc(size / 2);
    if (int_array != NULL) {
        printf("Allocated int array at %p with size %zu\n", int_array, size / 2);
        print_memory_status();
    } else {
        printf("Failed to allocate int array\n");
    }

    // Allocate a char array
    char *char_array = (char *)my_malloc(size / 4);
    if (char_array != NULL) {
        printf("Allocated char array at %p with size %zu\n", char_array, size / 4);
        print_memory_status();
    } else {
        printf("Failed to allocate char array\n");
    }

    // Free the int array
    my_free(int_array);
    printf("Freed int array at %p\n", int_array);
    print_memory_status();

    // Allocate another int array
    int *int_array2 = (int *)my_malloc(size / 2);
    if (int_array2 != NULL) {
        printf("Allocated another int array at %p with size %zu\n", int_array2, size / 2);
        print_memory_status();
    } else {
        printf("Failed to allocate another int array\n");
    }

    // Constantly stream through the array
    for (int i = 0; i < 100000000; i++) {
        if (int_array2 != NULL) {
            int_array2[i % (size / 2 / sizeof(int))] = i;
        }
        if (char_array != NULL) {
            char_array[i % (size / 4)] = 'A';
        }
    }

    return 0;
}
