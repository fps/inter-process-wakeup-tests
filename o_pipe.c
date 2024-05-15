#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: io output-file-name\n");
    return EXIT_FAILURE;
  }

  int out = open(argv[1], O_WRONLY);
  if (-1 == out)
  {
    printf("Failed to open file for writing: %s: %s\n", argv[1], strerror(errno));
    return EXIT_FAILURE;
  }

  struct timespec ts;
  for (int index = 0; index < 10; ++index)
  {
    clock_gettime(CLOCK_MONOTONIC, &ts);

    char *buf[1];
    int ret = write(out, buf, 1);
    if (-1 == ret)
    {
      printf("Failed to write to: %s: %s\n", argv[1], strerror(errno));
      return EXIT_FAILURE;
    }
    printf("secs: %ld, nsecs: %ld\n", ts.tv_sec, ts.tv_nsec);
    usleep(1000000);
  }
  return EXIT_SUCCESS;
}
