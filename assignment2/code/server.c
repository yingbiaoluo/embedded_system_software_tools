#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

int main() {
  int fd = open("./fifo1",O_RDONLY);
  int i = 0;
  char buf[24]={0};
  while(read(fd, buf, 24)>0){
    printf(buf);
  }
  close(fd);
  return 0;
}
