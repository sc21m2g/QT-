这是一个Qt学习的git仓库，我将在其中记录我的Qt学习以及一些Qt项目。
配套文件中，.pro文件解释.txt是我入门时对.pro文件的学习记录；qt学习.txt是我目前Qt学习的知识点总结。

其余文件基本上都是我创建的项目文件，其中包含各知识点的应用。
后面将会有一个我已完成的律师事务系统项目，其中包含了qt和数据库之间的协同运行。
还有目前我正在学习的opencv在Qt上的使用，在尝试做一个基本的人脸识别。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void consume_memory(size_t mb) {
    size_t size = mb * 1024 * 1024; // 计算需要分配的字节数
    char *data = (char *)malloc(size);
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // 初始化内存以确保它被物理分配
    memset(data, 0, size);

    // 无限循环，持续使用内存
    while (1) {
        for (size_t i = 0; i < size; i++) {
            data[i] = (char)(i % 256); // 用一些数据填充内存
        }
        sleep(1); // 暂停 1 秒
    }

    // 释放内存
    free(data);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <MB>\n", argv[0]);
        return 1;
    }

    size_t mb = (size_t)atol(argv[1]);
    if (mb == 0) {
        fprintf(stderr, "Invalid memory size\n");
        return 1;
    }

    consume_memory(mb);

    return 0;
}
