#define _USE_GNU
#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ARRAY_SIZE 1000000
#define NUM_THREADS 10 // 线程数量，可以根据需要调整

int CPU_CORES = 0; // CPU 核心编号
int array[ARRAY_SIZE];
long long partial_sums[NUM_THREADS] = {0}; // 每个线程的部分和

void *sum_array(void *arg) {
    cpu_set_t cpuset;   //CPU 核的位图 
    CPU_ZERO(&cpuset);  //清空
    CPU_SET(CPU_CORES, &cpuset); //设置CPU 核心编号
    CPU_CORES++; //下一个线程绑定到下一个 CPU 核心
    sched_setaffinity(0, sizeof(cpuset), &cpuset); //设置 CPU 亲和力

    int thread_id = *(int *)arg;
    int start = thread_id * (ARRAY_SIZE / NUM_THREADS);
    int end = (thread_id + 1) * (ARRAY_SIZE / NUM_THREADS);

    for (int i = start; i < end; i++) {
        partial_sums[thread_id] += array[i];
    }

    pthread_exit(NULL);
}

int main() {
    // 初始化数组
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // 记录开始时间
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    // 创建线程
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        int rc = pthread_create(&threads[i], NULL, sum_array, (void *)&thread_ids[i]);
        if (rc) {
            printf("Error:无法创建线程 %d\n", rc);
            exit(-1);
        }
    }

    // 等待所有线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 计算总和
    long long total_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total_sum += partial_sums[i];
    }

    // 记录结束时间
    clock_gettime(CLOCK_REALTIME, &end);

    // 计算耗时
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    // 打印结果
    printf("求和结果: %lld\n", total_sum);
    printf("求和操作耗时: %f 秒\n", time_spent);

    return 0;
}