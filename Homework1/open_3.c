// 通过syscall指令（64位系统）的内联汇编调用
#include <stdio.h>
#include <sys/syscall.h>
 #include <unistd.h>
// open() 所用到的头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// clock_gettime() 所用的头文件
#include <time.h>

int main()
{
    const char *filename = "test.txt";
    int fd;

    // 系统调用起始时间
    struct timespec time1 = {0, 0};
    // 系统调用结束时间
    struct timespec time2 = {0, 0};
    // 系统调用时间开销
    float time_diff = 0.0;

    // clock_gettime() 用于获取当前时间
    clock_gettime(CLOCK_REALTIME, &time1);

    // open() 用于打开文件
    asm volatile(
        "mov $2, %%rax\n\t"
        "mov $0, %%rdi\n\t"
        "mov $0x40000000, %%rsi\n\t"
        "mov $0, %%rdx\n\t"
        "syscall\n\t"
        : "=r" (fd)
        : "r" (filename)
        : "rax", "rdi", "rsi", "rdx"
    );

    // clock_gettime() 用于获取当前时间
    clock_gettime(CLOCK_REALTIME, &time2);

    // 计算系统调用时间开销
    time_diff = (time2.tv_sec - time1.tv_sec) * 1000.0 + (time2.tv_nsec - time1.tv_nsec) / 1000000.0;

    // 输出系统调用时间开销
    printf("The system call time is %f ms\n", time_diff);

    return 0;
}