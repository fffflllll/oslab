#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSZ 256

extern char* buff_ptr;
extern int buff_h, buff_number, shm_flg;
extern int prod_sem, cmtx_sem, sem_flg, sem_val;
extern int cons_sem[3]; // 三个消费者信号量

int get_ipc_id(char* proc_file, key_t key);
char* set_shm(key_t shm_key, int shm_num, int shm_flag);
int set_sem(key_t sem_key, int sem_val, int sem_flag);
int down(int sem_id);
int up(int sem_id);

// 在头文件中添加如下内容（如果没有）
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};