#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>

int main(int argc, char *argv[])
{
  sem_t *s = sem_open("/s", O_CREAT, S_IRUSR | S_IWUSR, 0);
  if (SEM_FAILED == s)
  {
    printf("Failed to open semaphore: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  struct timespec ts;
  while (1)
  {
    int ret = sem_wait(s);
    if (0 != ret)
    {
      printf("Failed to wait on semaphore: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("secs: %ld, nsecs: %ld\n", ts.tv_sec, ts.tv_nsec);
  }
  return EXIT_SUCCESS;
}
