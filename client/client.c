#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
 
#define BUFFER_LENGTH 256 
static char data[BUFFER_LENGTH];
 
int main(int argc, char* argv[]) {
  int fd;

  fd = open("/dev/server", O_RDWR);
  if (fd < 0) {
    perror("open failed");
    return errno;
  }

  return 0;
}
