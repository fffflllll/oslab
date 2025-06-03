#include "ipc.h"
int main(int argc, char* argv[]) {
    int rate = 1;
    int consumerid = atoi(argv[1]); // 0:有烟草, 1:有纸, 2:有胶水
    buff_h = 101;
    buff_number = 2;
    shm_flg = IPC_CREAT | 0644;
    buff_ptr = (char*)set_shm(buff_h, buff_number, shm_flg);

    cmtx_sem = set_sem(202, 1, IPC_CREAT | 0644);
    cons_sem[0] = set_sem(301, 0, IPC_CREAT | 0644);
    cons_sem[1] = set_sem(302, 0, IPC_CREAT | 0644);
    cons_sem[2] = set_sem(303, 0, IPC_CREAT | 0644);
    prod_sem = set_sem(201, 1, IPC_CREAT | 0644);

    char *material_name[3] = {"tobacco", "paper", "glue"};
    while (1) {
        down(cons_sem[consumerid]); //消费者等待自己的信号量，只有当生产者提供了该消费者需要的材料时，信号量才会被释放。

        down(cmtx_sem); 

        printf("%d The consumer has %s.\nThe consumer gets %s and %s\n",
               getpid(),
               material_name[consumerid],
               material_name[(consumerid + 1) % 3],
               material_name[(consumerid + 2) % 3]);
        fflush(stdout);
        sleep(rate);

        // 抽完烟后清空桌面
        buff_ptr[0] = buff_ptr[1] = 'X';

        up(cmtx_sem);
        up(prod_sem); // 通知生产者可以继续
    }
    return 0;
}
