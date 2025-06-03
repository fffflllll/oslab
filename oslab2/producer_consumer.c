#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define PRODUCT_NUM 10 //现在拥有的产品数量

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

pthread_mutex_t mutex;
pthread_cond_t full;
pthread_cond_t empty;

void* producer(void* arg) {
    (void)arg; // 显式忽略参数
    for (int i = 0; i < PRODUCT_NUM; i++) {
        usleep(rand() % 1000000);//随即延迟，模拟生产中的不确定性
        pthread_mutex_lock(&mutex);
        // 缓冲区满时等待
        while ((in + 1) % BUFFER_SIZE == out) {
            pthread_cond_wait(&empty, &mutex);
        }
        // 生产数据
        buffer[in] = rand() % 100;
        printf("Producer produced: %d\n", buffer[in]);
        in = (in + 1) % BUFFER_SIZE;
        // 通知消费者缓冲区非空
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* consumer(void* arg) {
    (void)arg; // 显式忽略参数
    for (int i = 0; i < PRODUCT_NUM; i++) {
        usleep(rand() % 1000000);
        pthread_mutex_lock(&mutex);
        // 缓冲区空时等待
        while (in == out) {
            pthread_cond_wait(&full, &mutex);
        }
        // 消费数据
        printf("Consumer consumed: %d\n", buffer[out]);
        out = (out + 1) % BUFFER_SIZE;
        // 通知生产者缓冲区非满
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    srand(time(NULL));

    // 初始化同步资源
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);

    // 创建线程
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // 等待线程结束
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // 销毁同步资源
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);

    return 0;
}