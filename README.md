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
        return NULL;
    }

    memory_blocks = realloc(memory_blocks, (num_blocks + 1) * sizeof(MemoryBlock));
    if (memory_blocks == NULL) {
        free(ptr);
        return NULL;
    }

    memory_blocks[num_blocks].ptr = ptr;
    memory_blocks[num_blocks].size = size;
    num_blocks++;
    num_allocated++;

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
    memory_blocks = realloc(memory_blocks, num_blocks * sizeof(MemoryBlock));
    if (memory_blocks == NULL) {
        printf("Failed to realloc memory_blocks\n");
    }
}

void print_memory_stats() {
    printf("Total blocks: %d, Allocated: %d, Freed: %d\n", num_blocks, num_allocated, num_freed);
}
