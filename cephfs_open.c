#include <stdio.h>
#include <fcntl.h>

int main(int argc, const char **argv)
{
  int fd = open("/mnt/cephfs/test.txt", O_CREAT);
  printf("fd: %d\n", fd);
}