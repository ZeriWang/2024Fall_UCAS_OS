#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1000000  // 数组大小
#define NUM_THREADS 10      // 线程数量

int array[ARRAY_SIZE];

typedef struct {
    int start;
    int end;
} ThreadData;

void* fill_array(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    for (int i = data->start; i < data->end; i++) {
        array[i] = i + 1;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int segment_size = ARRAY_SIZE / NUM_THREADS;

    // 创建线程
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * segment_size;
        thread_data[i].end = (i + 1) * segment_size;
        pthread_create(&threads[i], NULL, fill_array, (void*)&thread_data[i]);
    }

    // 等待所有线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 主进程求和
    long long sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += array[i];
    }

    printf("Sum of array elements: %lld\n", sum);

    return 0;
}