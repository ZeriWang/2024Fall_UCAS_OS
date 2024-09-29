#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 5
#define NUM_ITERATIONS 5

int array[ARRAY_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;

void* write_random_numbers(void* arg) {
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        pthread_mutex_lock(&mutex);

        // 写入随机数
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = rand() % 20 + 1;
        }

        ready = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        // 等待线程2读取并求和
        pthread_mutex_lock(&mutex);
        while (ready == 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* read_and_sum(void* arg) {
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        pthread_mutex_lock(&mutex);
        while (ready == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        // 读取并求和
        int sum = 0;
        for (int i = 0; i < ARRAY_SIZE; i++) {
            sum += array[i];
        }
        printf("Iteration %d: Sum = %d\n", iter + 1, sum);

        ready = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, write_random_numbers, NULL);
    pthread_create(&thread2, NULL, read_and_sum, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}