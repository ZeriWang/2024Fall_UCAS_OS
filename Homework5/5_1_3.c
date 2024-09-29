#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 10000000
#define CHUNK_SIZE 200

int data[MAX];
int current_index = 0;

void* write_even(void* arg) {
    for (int i = 0; i < MAX; i += 400) {

// 临界区
/////////////////////////////////////////////////////////////////////////////////////////////////////////

        for (int j = 0; j < CHUNK_SIZE && __atomic_add_fetch(&current_index, 0, __ATOMIC_SEQ_CST) < MAX; j++) {
            int idx = __atomic_add_fetch(&current_index, 1, __ATOMIC_SEQ_CST) - 1;
            data[idx] = i + j * 2;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

    }
    return NULL;
}

void* write_odd(void* arg) {
    for (int i = 0; i < MAX; i += 400) {

// 临界区
/////////////////////////////////////////////////////////////////////////////////////////////////////////

        for (int j = 0; j < CHUNK_SIZE && __atomic_add_fetch(&current_index, 0, __ATOMIC_SEQ_CST) < MAX; j++) {
            int idx = __atomic_add_fetch(&current_index, 1, __ATOMIC_SEQ_CST) - 1;
            data[idx] = i + j * 2 + 1;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, write_even, NULL);
    pthread_create(&thread2, NULL, write_odd, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    int max_diff = 0;
    for (int i = 1; i < MAX; i++) {
        int diff = abs(data[i] - data[i - 1]);
        if (diff > max_diff) {
            max_diff = diff;
        }
    }

    printf("最大绝对差值: %d\n", max_diff);
    return 0;
}