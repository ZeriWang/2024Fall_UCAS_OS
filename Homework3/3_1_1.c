#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 1000000

int main() {
    // 创建一个大小为 100 万的整数数组
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL) {
        printf("内存分配失败\n");
        return 1;
    }

    // 初始化数组
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }

    // 记录开始时间
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    // 求和
    long long sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += array[i];
    }

    // 记录结束时间
    clock_gettime(CLOCK_REALTIME, &end);

    // 计算耗时
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    // 打印结果
    printf("求和结果: %lld\n", sum);
    printf("求和操作耗时: %f 秒\n", time_spent);

    // 释放内存
    free(array);

    return 0;
}