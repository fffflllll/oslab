#include "ipc.h"
int main() {
    int rate = 1;
    buff_h = 101;
    buff_number = 2;
    shm_flg = IPC_CREAT | 0644;
    buff_ptr = (char*)set_shm(buff_h, buff_number, shm_flg);

    prod_sem = set_sem(201, 1, IPC_CREAT | 0644);
    cmtx_sem = set_sem(202, 1, IPC_CREAT | 0644);

    // 初始化三个消费者信号量
    cons_sem[0] = set_sem(301, 0, IPC_CREAT | 0644);
    cons_sem[1] = set_sem(302, 0, IPC_CREAT | 0644);
    cons_sem[2] = set_sem(303, 0, IPC_CREAT | 0644);

    char *material_name[3] = {"tobacco", "paper", "glue"};
    char supply[3][2] = {
        {'B', 'C'}, // 缺A（tobacco），供应 paper 和 glue
        {'A', 'C'}, // 缺B（paper），供应 tobacco 和 glue
        {'A', 'B'}  // 缺C（glue），供应 tobacco 和 paper
    };
    int idx = 0;
    while (1) {
        down(prod_sem);
        down(cmtx_sem);

        buff_ptr[0] = supply[idx][0];
        buff_ptr[1] = supply[idx][1];
        printf("%d The producer gives %s and %s\n", getpid(),
               material_name[supply[idx][0] - 'A'],
               material_name[supply[idx][1] - 'A']);
        fflush(stdout);

        up(cmtx_sem);
        up(cons_sem[idx]); // 唤醒缺材料的那个消费者

        idx = (idx + 1) % 3;
        sleep(rate);
    }
    return 0;
}
