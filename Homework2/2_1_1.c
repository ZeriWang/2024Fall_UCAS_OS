#include <stdio.h>
#include <stdlib.h>

// data段
int initialized_data = 42;

// bss段
int uninitialized_data;

int main()
{
    // 动态分配内存（堆）
    int *heap_data = (int *)malloc(sizeof(int));
    if (heap_data == NULL)
    {
        fprintf(stderr, "内存分配失败\n");
        return 1;
    }

    // 使用分配的内存
    *heap_data = 100;

    // 打印变量的值
    printf("initialized_data: %d\n", initialized_data);
    printf("uninitialized_data: %d\n", uninitialized_data);
    printf("heap_data: %d\n", *heap_data);

    // 释放分配的内存
    free(heap_data);

    return 0;
}