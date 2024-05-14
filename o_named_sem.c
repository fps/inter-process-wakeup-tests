#include <semaphore.h>
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

  sem_t *s = sem_open("/s", O_CREAT, S_IRUSR | S_IWUSR, 0);
  if (SEM_FAILED == s)
  {
    printf("Failed to open semaphore: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
      
  struct timespec ts;
  while (1)
  {
    clock_gettime(CLOCK_MONOTONIC, &ts);

    int ret = sem_post(s);
    if (0 != ret)
    {
      printf("Failed to write to: %s: %s\n", argv[1], strerror(errno));
      return EXIT_FAILURE;
    }
    printf("secs: %ld, nsecs: %ld\n", ts.tv_sec, ts.tv_nsec);
    usleep(1000000);
  }
  return EXIT_SUCCESS;
}
