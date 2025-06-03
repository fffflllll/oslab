#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int fx(int x) {
    if (x == 1)
        return 1;
    return x * fx(x - 1);
}

int fy(int y) {
    if (y == 1 || y == 2)
        return 1;
    return fy(y - 1) + fy(y - 2);
}

int main() {
    pid_t pid_fx, pid_fy;
    int x, y;
    int pipe_fx[2], pipe_fy[2]; // 管道用于父子进程通信
    int result_fx, result_fy;

    printf("Input x and y:\n");
    scanf("%d %d", &x, &y);

    // 创建管道
    if (pipe(pipe_fx) == -1 || pipe(pipe_fy) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // 创建第一个子进程计算 f(x)
    pid_fx = fork();
    if (pid_fx == 0) {
        close(pipe_fx[0]); // 关闭读端
        result_fx = fx(x);
        write(pipe_fx[1], &result_fx, sizeof(result_fx)); // 将结果写入管道
        close(pipe_fx[1]); // 关闭写端
        exit(0);
    }

    // 创建第二个子进程计算 f(y)
    pid_fy = fork();
    if (pid_fy == 0) {
        close(pipe_fy[0]); // 关闭读端
        result_fy = fy(y);
        write(pipe_fy[1], &result_fy, sizeof(result_fy)); // 将结果写入管道
        close(pipe_fy[1]); // 关闭写端
        exit(0);
    }

    // 父进程等待子进程完成并读取结果
    close(pipe_fx[1]); // 关闭写端
    close(pipe_fy[1]); // 关闭写端
    read(pipe_fx[0], &result_fx, sizeof(result_fx)); // 从管道读取 f(x) 的结果
    read(pipe_fy[0], &result_fy, sizeof(result_fy)); // 从管道读取 f(y) 的结果
    close(pipe_fx[0]); // 关闭读端
    close(pipe_fy[0]); // 关闭读端

    wait(NULL);
    wait(NULL);

    printf("f(x) = %d\n", result_fx);
    printf("f(y) = %d\n", result_fy);
    printf("f(x, y) = %d\n", result_fx + result_fy);

    return 0;
}