这是一个Qt学习的git仓库，我将在其中记录我的Qt学习以及一些Qt项目。
配套文件中，.pro文件解释.txt是我入门时对.pro文件的学习记录；qt学习.txt是我目前Qt学习的知识点总结。

其余文件基本上都是我创建的项目文件，其中包含各知识点的应用。
后面将会有一个我已完成的律师事务系统项目，其中包含了qt和数据库之间的协同运行。
还有目前我正在学习的opencv在Qt上的使用，在尝试做一个基本的人脸识别。
// memory_consumption.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "memory_management.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <megabytes>\n", argv[0]);
        return 1;
    }

    long megabytes = atol(argv[1]);
    if (megabytes <= 0) {
        printf("Invalid megabyte value: %ld\n", megabytes);
        return 1;
    }

    long bytes = megabytes * 1024 * 1024;
    int *array = (int *)custom_malloc(bytes);
    if (array == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Allocated %ld MB of memory\n", megabytes);

    // 模拟内存消耗
    while (1) {
        for (long i = 0; i < bytes / sizeof(int); i++) {
            array[i] = i;
        }
        sleep(1);

        // 读取用户输入
        char input[100];
        if (fgets(input, sizeof(input), stdin) != NULL) {
            input[strcspn(input, "\n")] = 0;  // 去掉换行符

            if (strcmp(input, "free") == 0) {
                custom_free(array);
                array = NULL;
                printf("Memory freed\n");
            } else if (strcmp(input, "stats") == 0) {
                print_memory_stats();
            } else if (sscanf(input, "malloc %ld", &megabytes) == 1) {
                if (megabytes <= 0) {
                    printf("Invalid megabyte value: %ld\n", megabytes);
                    continue;
                }
                bytes = megabytes * 1024 * 1024;
                if (array != NULL) {
                    custom_free(array);
                }
                array = (int *)custom_malloc(bytes);
                if (array == NULL) {
                    printf("Memory allocation failed\n");
                } else {
                    printf("Allocated %ld MB of memory\n", megabytes);
                }
            } else {
                printf("Unknown command: %s\n", input);
            }
        }
    }

    return 0;
}


#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

void *custom_malloc(size_t size);
void custom_free(void *ptr);
void print_memory_stats();

#endif // MEMORY_MANAGEMENT_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_management.h"

typedef struct MemoryBlock {
    void *ptr;
    size_t size;
} MemoryBlock;

static MemoryBlock *memory_blocks = NULL;
static int num_blocks = 0;
static int num_allocated = 0;
static int num_freed = 0;

int find_memory_block(void *ptr) {
    for (int i = 0; i < num_blocks; i++) {
        if (memory_blocks[i].ptr == ptr) {
            return i;
        }
    }
    return -1;
}

void *custom_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        printf("Failed to allocate %zu bytes of memory\n", size);
        return NULL;
    }

    void *new_memory_blocks = realloc(memory_blocks, (num_blocks + 1) * sizeof(MemoryBlock));
    if (memory_blocks == NULL) {
        free(ptr);
        printf("Failed to realloc memory_blocks for %zu bytes\n", size);
        return NULL;
    }
    memory_blocks = new_memory_blocks;

    memory_blocks[num_blocks].ptr = ptr;
    memory_blocks[num_blocks].size = size;
    num_blocks++;
    num_allocated++;

    printf("Memory blocks after allocation:\n");
    for (int i = 0; i < num_blocks; i++) {
        printf("Block %d: ptr = %p, size = %zu\n", i, memory_blocks[i].ptr, memory_blocks[i].size);
    }
    return ptr;
}

void custom_free(void *ptr) {
    int index = find_memory_block(ptr);
    if (index == -1) {
        printf("Pointer not found in memory blocks\n");
        return;
    }

    free(ptr);
    memory_blocks[index].ptr = NULL;
    num_freed++;

    // 重新排列内存块数组
    for (int j = index; j < num_blocks - 1; j++) {
        memory_blocks[j] = memory_blocks[j + 1];
    }

    num_blocks--;
    void *new_memory_blocks = realloc(memory_blocks, num_blocks * sizeof(MemoryBlock));
    if (new_memory_blocks == NULL && num_blocks > 0) {
        printf("Failed to realloc memory_blocks\n");
        return;
    }
    memory_blocks = new_memory_blocks;

    printf("Memory blocks after free:\n");
    for (int i = 0; i < num_blocks; i++) {
        printf("Block %d: ptr = %p, size = %zu\n", i, memory_blocks[i].ptr, memory_blocks[i].size);
    }
}

void print_memory_stats() {
    printf("Total blocks: %d, Allocated: %d, Freed: %d\n", num_blocks, num_allocated, num_freed);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

void print_memory_status() {
    Block *current = head;
    while (current != NULL) {
        printf("Block at %p: size = %zu, is_free = %d\n", current, current->size, current->is_free);
        current = current->next;
    }
}

#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

void *my_malloc(size_t size);
void my_free(void *ptr);
void print_memory_status();

#endif

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

    return 0;
}

