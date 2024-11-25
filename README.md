这是一个Qt学习的git仓库，我将在其中记录我的Qt学习以及一些Qt项目。
配套文件中，.pro文件解释.txt是我入门时对.pro文件的学习记录；qt学习.txt是我目前Qt学习的知识点总结。

其余文件基本上都是我创建的项目文件，其中包含各知识点的应用。
后面将会有一个我已完成的律师事务系统项目，其中包含了qt和数据库之间的协同运行。
还有目前我正在学习的opencv在Qt上的使用，在尝试做一个基本的人脸识别。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "memory_management.h"

int *array = NULL;  // 将数组声明为全局变量
long megabytes = 0;

void handle_sigint(int sig) {
    printf("Signal %d received, freeing memory\n", sig);
    for (int i = 0; i < num_blocks; i++) {
        custom_free(memory_blocks[i].ptr);
    }
    print_memory_blocks();
    print_memory_stats();
    printf("Memory freed\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <megabytes>\n", argv[0]);
        return 1;
    }

    long megabytes = atol(argv[1]);
    long bytes = megabytes * 1024 * 1024;

    void *array = custom_malloc(bytes);
    printf("Allocated %ld MB of memory\n", megabytes);
    print_memory_blocks();

    signal(SIGINT, handle_sigint);

    int *ptr = (int *)array;
    int i = 0;
    while (1) {
        ptr[i] = i;
        i++;
        if (i % (1024 * 1024) == 0) {
            printf("Writing to memory... %d MB\n", i / (1024 * 1024));
        }
        sleep(1);
    }

    // 交互部分
    while (1) {
        char input[100];
        printf("Do you want to allocate more memory? (yes/no): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = '\0'; // 移除换行符

        if (strcmp(input, "yes") == 0) {
            printf("Enter the number of megabytes to allocate: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }
            long more_megabytes = atol(input);
            if (more_megabytes <= 0) {
                printf("Invalid input. Please enter a positive number.\n");
                continue;
            }
            long more_bytes = more_megabytes * 1024 * 1024;
            void *more_array = custom_malloc(more_bytes);
            printf("Allocated %ld MB of memory\n", more_megabytes);
            print_memory_blocks();
        } else if (strcmp(input, "no") == 0) {
            break;
        } else {
            printf("Invalid input. Please enter 'yes' or 'no'.\n");
        }
    }

    handle_sigint(2); // 释放所有已分配的内存

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_management.h"

#define MAX_BLOCKS 100

typedef struct {
    void *ptr;
    size_t size;
} MemoryBlock;

MemoryBlock memory_blocks[MAX_BLOCKS];
int num_blocks = 0;
int num_allocated = 0;
int num_freed = 0;

void *custom_malloc(size_t size) {
    printf("custom_malloc called with size = %zu\n", size);
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Error: Out of memory\n");
        exit(1);
    }
    MemoryBlock block = {ptr, size};
    memory_blocks[num_blocks++] = block;
    num_allocated++;
    return ptr;
}

void custom_free(void *ptr) {
    printf("custom_free called with ptr = %p\n", ptr);
    for (int i = 0; i < num_blocks; i++) {
        if (memory_blocks[i].ptr == ptr) {
            free(ptr);
            memmove(&memory_blocks[i], &memory_blocks[i + 1], (num_blocks - i - 1) * sizeof(MemoryBlock));
            num_blocks--;
            num_freed++;
            break;
        }
    }
}

void print_memory_blocks() {
    printf("Memory blocks:\n");
    for (int i = 0; i < num_blocks; i++) {
        printf("Block %d: ptr = %p, size = %zu\n", i, memory_blocks[i].ptr, memory_blocks[i].size);
    }
}

void print_memory_stats() {
    printf("Total blocks: %d, Allocated: %d, Freed: %d\n", num_blocks, num_allocated, num_freed);
}
