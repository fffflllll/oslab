/*
 * Filename : ipc.h
 * copyright : (C) 2006 by zhonghonglie
 * Function : 声明 IPC 机制的函数原型和全局变量
 */
#ifndef IPC_H
#define IPC_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSZ 256

// 全局变量声明（使用 extern）
extern key_t buff_key;
extern int buff_num;
extern char* buff_ptr;

extern key_t pput_key;
extern int pput_num;
extern int* pput_ptr;

extern key_t cget_key;
extern int cget_num;
extern int* cget_ptr;

extern key_t prod_key;
extern key_t pmtx_key;
extern int prod_sem;
extern int pmtx_sem;

extern key_t cons_key;
extern key_t cmtx_key;
extern int cons_sem;
extern int cmtx_sem;

extern int sem_val;
extern int sem_flg;
extern int shm_flg;

// 函数声明
int set_sem(key_t sem_key, int sem_val, int sem_flag);
int down(int sem_id);
int up(int sem_id);
char* set_shm(key_t shm_key, int shm_num, int shm_flg);
int get_ipc_id(char* proc_file, key_t key);

union semun {
    int val;
    struct semid_ds* buf;
    unsigned short* array;
    struct seminfo* __buf;
};

#endif