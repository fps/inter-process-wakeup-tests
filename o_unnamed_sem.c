#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
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

  int ret = ftruncate(s, sizeof(sem_t));
  if (0 != ret)
  printf("ftruncate failed: %s\n", strerror(errno));

  void *m = mmap(0,  sizeof(sem_t) , PROT_READ | PROT_WRITE, MAP_SHARED, s, 0);
  if (MAP_FAILED == m)
  {
    printf("mmap failed\n");
    return EXIT_FAILURE;
  }

  struct timespec ts;
  for (int index = 0; index < 10; ++index)
  {
    clock_gettime(CLOCK_MONOTONIC, &ts);

    int ret = sem_post((sem_t*)m);
    if (0 != ret)
    {
      printf("Failed to write to: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }
    printf("secs: %ld, nsecs: %ld\n", ts.tv_sec, ts.tv_nsec);
    usleep(1000);
  }
  return EXIT_SUCCESS;
}
