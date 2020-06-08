#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAXLIN 4096
 
int main(int argc,char **argv)
{
    int sockfd,rec_len;//句柄
    char sendline[4096];
    char buf[MAXLIN];
    struct sockaddr_in servaddr;//地址信息结构体
 
    if(argc!=2)
    {
        printf("usage:./client <ipaddress>\n");
        exit(0);
    }
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)//(协议族/域，协议类型，协议编号)
    {
        printf("create socket error:%s(error:%d)\n",strerror(errno),errno);
        exit(0);
    }
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;/* 该属性表示接收本机或其它机器传输 */
    servaddr.sin_port=htons(8000);/* 端口号 */
    if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<=0)
    {
        printf("inet_pton error for %s\n",argv[1]);
        exit(0);
    }
    if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
    {
        printf("connect error:%s(errno:%d)\n",strerror(errno),errno);
        exit(0);
    }
    printf("send msg to server:");
    fgets(sendline,4096,stdin);
    if(!fork())
    {
        if(send(sockfd,sendline,strlen(sendline),0)<0)
        {
            printf("send msg error :%s(errno:%d)\n",strerror(errno),errno);
            exit(0);
        }
    }
    if((rec_len=recv(sockfd,buf,MAXLIN,0))==-1)
    {
        perror("recv error\n");
        exit(1);
    }
    buf[rec_len]='\0';
    if(buf[0]!='\0') printf("Received from server:%s\n",buf);
    close(sockfd);
    return 0;
}

