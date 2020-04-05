#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/** 运用Posix信号量使用”写者优先”策略解决”读者写者问题”
解题思路:
如果新写者到：
　　　①无写者、读者，新写者可以写；
　　　②有其它写者正在写，则新写者等待其他写者写完；
　　　③有读者读或等待，无其他写者，新写者直接写。
如果新读者到：
　　　①无写者或者有其它读者，新读者可以读；
　　　②有写者，新读者等待；
**/
// 需要用两个互斥量实现
pthread_mutex_t rmutex;
pthread_mutex_t wmutex;

const unsigned READERCOUNT = 6; //读者数
const unsigned WRITERCONUT = 3; //写者数

const int PAPERSIZE = 32;       //文件长度
char paper[PAPERSIZE+1];        //文件

unsigned short int write_index = 0; //写者需要写入的位置
char ch = 'A';  //写者需要写入的字母

pthread_t thread[READERCOUNT+WRITERCONUT];  //读者+写者线程

int nReader = 0;
int nWriter = 0;
//读者线程
void *reader(void *args)
{
    int number = *(int *)args;
    delete (int *)args;

    while (true)
    {
        pthread_mutex_lock(&rmutex);

        //开始读
        printf("-----%d号读者正在读...\n", number);
        printf("%d号读者读到内容: %s\n", number, paper);
        printf("-----%d号读者读完毕.\n\n", number);

        pthread_mutex_unlock(&rmutex);

        sleep(1);
    }
    pthread_exit(NULL);
}

//写者线程
void *writer(void *args)
{
    int number = *(int *)args;
    delete (int *)args;
    while (true)
    {
        //获取写锁
        pthread_mutex_lock(&wmutex);
        //获取读锁
        pthread_mutex_lock(&rmutex);
        //开始写

        printf("-----%d号写者正在写...\n", number);
        printf("%d号写者写入的数据是: %c\n", number, ch);
        paper[write_index] = ch;
        write_index = (write_index+1)%PAPERSIZE;
        ch = ch+1;
        if (ch > 'Z')
            ch = 'A';
        printf("-----%d号写者写完毕...\n\n", number);

        //释放读锁
        pthread_mutex_unlock(&rmutex);
        //释放写锁
        pthread_mutex_unlock(&wmutex);

        sleep(1);
    }

    pthread_exit(NULL);
}

int main()
{
    memset(paper, 0, sizeof(paper));
    pthread_mutex_init(&rmutex, NULL);
    pthread_mutex_init(&wmutex, NULL);

    for (unsigned int i = 0; i < READERCOUNT; ++i)
        pthread_create(&thread[i], NULL, reader, new int(i));
    for (unsigned int i = 0; i < WRITERCONUT; ++i)
        pthread_create(&thread[READERCOUNT+i], NULL, writer, new int(i));
    for (unsigned int i = 0; i < READERCOUNT+WRITERCONUT; ++i)
        pthread_join(thread[i], NULL);

    pthread_mutex_destroy(&rmutex);
    pthread_mutex_destroy(&wmutex);
}
