#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
int main() {
  char* msg = "Hello, I am client!\n";
  int fd=open("./fifo1", O_WRONLY);
  int i = 0;
  for(;i<10;i++){
    printf(msg);
    if(write(fd, msg, strlen(msg)+1)<0){
      _exit(-1);
    }
    sleep(2);
  }
  close(fd);
  return 0;
}
