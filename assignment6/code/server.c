#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define DEFAULT_PORT 8000
#define MAXLIN 4096

void get_data(char *data, char *buf){
    FILE *fp1;
    fp1=fopen("/mnt/ext3/temperature.txt","r");
    if(fp1==NULL){
        perror("open file1 error");
        exit(1);
    }
    FILE *fp2;
    fp2=fopen("/mnt/ext3/pm25.txt","r");
    if(fp2==NULL){
        perror("open file2 error");
        exit(1);
    }
    FILE *fp3;
    fp3=fopen("/mnt/ext3/humidity.txt","r");
    if(fp3==NULL){
        perror("open file3 error");
        exit(1);
    }

    if(strcmp(buf,"temperature\n")==0){
        fgets(data,sizeof(data),fp1);
    }
    else if(strcmp(buf,"pm2.5\n")==0){
        fgets(data,sizeof(data),fp2);
    }
    else if(strcmp(buf,"humidity\n")==0){
        fgets(data,sizeof(data),fp3);
    }
    else{
	char wrong_msg[]="错误指令\n";
        strcpy(data,wrong_msg);
    }
    fclose(fp1);
    return;
}


int main(int argc,char **argv)
{
    int socket_fd,connect_fd;
    struct sockaddr_in servaddr;
    char buff[4096];
    int n;
    //初始化
    if((socket_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        printf("create socket error:%s(errno :%d)\n",strerror(errno),errno);
        exit(0);
    }
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址
    servaddr.sin_port=htons(DEFAULT_PORT);
    //设置的端口为DEFAULT_PORT
    //将本地地址绑定到所创建的套接字上
    if(bind(socket_fd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
    {
        printf("bind socket error:%s(errno:%d)\n",strerror(errno),errno);
        exit(0);
    }
    //开始监听是否有客户端连接，第二个参数是最大监听数
    if(listen(socket_fd,10)==-1)
    {
        printf("listen socket error:%s(errno:%d)\n",strerror(errno),errno);
        exit(0);
    }
    printf("======waiting for client's request=====\n");
    while(1)
    {
        if((connect_fd=accept(socket_fd,(struct sockaddr*)NULL,NULL))==-1){
            printf("accept socket error :%s(errno:%d)\n",strerror(errno),errno);
            continue;
        }
        n=recv(connect_fd,buff,MAXLIN,0);
	buff[n]='\0';
	printf("收到客户端的请求: %s\n",buff);

	char data[1024];
	get_data(data,buff);

        if(!fork()){
            if(send(connect_fd,data,26,0)==-1)
            perror("send error");
            close(connect_fd);
            exit(0);
        }
    close(connect_fd);
    }
    close(socket_fd);
}

