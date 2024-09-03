// 通过syscall指令（64位系统）的内联汇编调用
#include <stdio.h>
#include <sys/syscall.h>
// getpid() 所用的头文件
#include <sys/types.h>
#include <unistd.h>
// clock_gettime() 所用的头文件
#include <time.h>

int main()
{
    // 系统调用起始时间
    struct timespec time1 = {0, 0};
    // 系统调用结束时间
    struct timespec time2 = {0, 0};
    // 系统调用时间开销
    float time_diff = 0.0;

    // clock_gettime() 用于获取当前时间
    clock_gettime(CLOCK_REALTIME, &time1);

    // getpid() 用于获取当前进程的进程号
    // syscall(SYS_getpid);
    // 通过syscall指令（64位系统）的内联汇编调用
    asm volatile(
        "mov $39, %%rax\n\t"
        "syscall\n\t"
        :
        :
        : "rax"
    );

    // clock_gettime() 用于获取当前时间
    clock_gettime(CLOCK_REALTIME, &time2);

    // 计算系统调用时间开销
    time_diff = (time2.tv_sec - time1.tv_sec) * 1000.0 + (time2.tv_nsec - time1.tv_nsec) / 1000000.0;

    // 输出系统调用时间开销
    printf("The system call time is %f ms\n", time_diff);

    return 0;
}