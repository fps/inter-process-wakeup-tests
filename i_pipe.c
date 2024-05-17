#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
  mlockall(MCL_CURRENT | MCL_FUTURE);
  if (argc < 2)
  {
    printf("Usage: io input-file-name\n");
    return EXIT_FAILURE;
  }

  int inp = open(argv[1], O_RDONLY);
  if (-1 == inp)
  {
    printf("Failed to open input-file: %s: %s\n", argv[1], strerror(errno));
    return EXIT_FAILURE;
  }


  struct timespec ts;
  for (int index = 0; index < 10; ++index)
  {
    char buf[1];

    int ret = read(inp, buf, 1);
    if (-1 == ret)
    {
      printf("Failed to read from: %s: %s\n", argv[1], strerror(errno));
      return EXIT_FAILURE;
    }

    if (0 == ret)
    {
      printf("Done.\n");
      return EXIT_SUCCESS;
    }

    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("secs: %ld, nsecs: %ld\n", ts.tv_sec, ts.tv_nsec);
  }
  return EXIT_SUCCESS;
}
