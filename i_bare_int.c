#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
  mlockall(MCL_FUTURE);

  int s = shm_open("/s", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (-1 == s)
  {
    printf("Failed to create shared memory object: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  int ret = ftruncate(s, sizeof(int));
  if (0 != ret)
  printf("ftruncate failed: %s\n", strerror(errno));

  void * volatile m = mmap(0,  sizeof(int) , PROT_READ | PROT_WRITE, MAP_SHARED, s, 0);
  if (MAP_FAILED == m)
  {
    printf("mmap failed\n");
    return EXIT_FAILURE;
  }

  *((int*)m) = 0;
  volatile int old_value = 0;

  struct timespec ts;
  for (int index = 0; index < 10; ++index)
  {
    while (*((int*)m) <= old_value)
    {
        // printf("%d", *((int*)m));
    }
    old_value = *((int*)m);

    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("secs: %ld, nsecs: %ld\n", ts.tv_sec, ts.tv_nsec);
  }
  return EXIT_SUCCESS;
}
