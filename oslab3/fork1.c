#include <sys/types.h>
#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>

int value=5;     //where?
// value 是一个全局变量，定义在 main 函数外部，作用域为整个程序。它在父进程和子进程中共享初始值
// 但在 fork() 后，父进程和子进程会各自维护 value 的副本，修改 value 不会互相影响。

int main(){
	
	int i;     //where?
	// i 是一个局部变量，定义在 main 函数内部，作用域仅限于 main 函数。它在父进程和子进程中也会各自维护一份副本。
	pid_t pid;
	
	for(i=0;i<2;i++){    // How many new processes and printfs£¿
	/*
	fork() 每次调用都会创建一个新进程。在循环中，fork() 被调用两次：
	第一次调用时，父进程创建一个子进程。
	第二次调用时，父进程和第一个子进程都会执行 fork()，因此会再创建两个新进程。
	总共会有 4 个进程（1 个父进程 + 3 个子进程）。
	*/
	
		pid=fork();
	
		if(pid==0){
			value += 15;
			printf("Child: value = %d\n",value);
		}
		else if(pid>0){
			wait(NULL);
			printf("PARNET: value = %d\n",value);
			exit(0);   //Notice£ºWhat will happen with or without this line?
			/* 
			有 exit(0)：
				父进程在执行完 printf("PARNET: value = %d\n", value); 后会立即退出，不会继续执行后续代码。
				这样可以避免父进程在循环中再次调用 fork()，从而限制进程的数量。
			没有 exit(0)：
				父进程不会退出，会继续执行循环中的下一次 fork()，这会导致进程数量进一步增加，可能导致进程爆炸（进程数呈指数增长）。
			*/
		}
	}
}
