#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
    int arr[10];
    for (int i = 0; i < 10; i++) 
    {
        arr[i] = i + 1;
    }

    pid_t pid = fork();

    if (pid < 0) 
    {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } 
    else if (pid == 0) 
    {
        // 子进程
        int sum = 0;
        for (int i = 0; i < 10; i++) 
        {
            sum += arr[i];
        }
        printf("Sum of array elements: %d\n", sum);
        exit(0);
    } 
    else 
    {
        // 父进程
        wait(NULL); // 等待子进程完成
        printf("parent process finishes\n");
    }

    return 0;
}