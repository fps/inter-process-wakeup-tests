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
  int s = shm_open("/s", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (-1 == s)
  {
    printf("Failed to create shared memory object: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  int ret = ftruncate(s, sizeof(sem_t));
  if (0 != ret)
  printf("ftruncate failed: %s\n", strerror(errno));

  void *m = mmap(0,  sizeof(int) , PROT_READ | PROT_WRITE, MAP_SHARED, s, 0);
  if (MAP_FAILED == m)
  {
    printf("mmap failed\n");
    return EXIT_FAILURE;
  }

  ret = sem_init((sem_t*)m, 1, 0);
  if (0 != ret)
  {
    printf("Failed to open semaphore: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  struct timespec ts;
  while (1)
  {
    ret = sem_wait((sem_t*)m);
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
